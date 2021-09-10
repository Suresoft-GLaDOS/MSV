#!/usr/bin/env python3
import os
import sys
import subprocess
import json
import time
import hashlib
import getopt
import afl_plot


class Fuzzer:
    def __init__(self, fuzzer: subprocess.Popen, fid: int, total: int, workdir: str) -> None:
        self.fuzzer = fuzzer
        self.fid = fid
        self.total = total
        self.result = False
        self.last_line = 0
        self.afl_result_file = os.path.join(workdir, "out", "fuzzer" + str(fid), "afl-result.csv")

    def set_result(self, result: bool) -> None:
        self.result = result

    def append_result(self, result_file: str) -> None:
        if not os.path.exists(self.afl_result_file):
            return
        with open(self.afl_result_file, "r") as arf:
            res = arf.readlines()
            lines = res[self.last_line:]
            self.last_line = len(res)
            with open(result_file, "a") as rf:
                rf.writelines(lines)

    def is_alive(self) -> bool:
        return self.fuzzer.poll() is None
    
    def __str__(self) -> str:
        return f"fuzzer{self.fid}: {self.fid}/{self.total}, file: {self.afl_result_file}, result: {self.result}"


class Config:
    def __init__(self, switch: int, case: int, is_condition: bool = False, line: int = -1, conf_type: str = "") -> None:
        self.switch = switch
        self.case = case
        self.is_condition = is_condition
        self.conf_type = conf_type
        self.line = line
        self.operator = ""
        self.variable = ""
        self.constant = 0

    def set_condition(self, operator: str, variable: str, constant: int) -> None:
        self.is_condition = True
        self.operator = operator
        self.variable = variable
        self.constant = constant
    
    def __str__(self) -> str:
        return f"{self.switch}, {self.case}({self.is_condition}): {self.line}, {self.conf_type}"

    def __hash__(self) -> int:
        hashstr = f"{self.switch}+{self.case}"
        a = hashlib.md5(hashstr.encode('utf-8'))
        return int(a.hexdigest(), 16)
    
    def __eq__(self, o: object) -> bool:
        equal = self.switch == o.switch
        equal = equal and self.case == o.case
        return equal


class ConfigGenerator:
    def __init__(self, arg_dict: dict, switch_num: int, switch_json: dict) -> None:
        self.switches = list(range(switch_num))
        self.switch_json = switch_json
        self.rules = dict()
        self.results = dict()
        self.result_dir = os.path.join(arg_dict["w"], "out", "afl-master")
        self.result_file = os.path.join(self.result_dir, "afl-result.csv")
        os.makedirs(self.result_dir, exist_ok=True)
        with open(os.path.join(self.result_dir, "is_main_node"), "w") as main_node:
            main_node.write("Sync from this!")
    
    def init_rules(self) -> None:
        root = self.switch_json["rules"][0]
        for rule in root:
            conf: Config = None
            if rule["is_condition"]:
                conf = Config(rule["switch"], rule["case"], rule["condition"], True, rule["line"], rule["type"])
                conf.set_condition(rule["condition"], rule["operator"], rule["variable"], rule["constant"])
            else:
                conf = Config(rule["switch"], rule["case"], 0, False, rule["line"], rule["type"])
            self.rules[conf] = conf
            self.results[conf] = False

    def fuzzer_id_generator(self, fid, total) -> str:
        if fid >= total:
            return ""
        confstr = f"fuzzer{fid}:{fid}/{total}" 
        return confstr

    # cycle, time, switch, case, result, [operator, variable, constant]
    def result_analyzer(self, fuzz: Fuzzer) -> bool:
        with open(fuzz.afl_result_file, "r") as arf:
            if arf is None:
                return False
            lines = arf.readlines()
            is_passed = False
            for line in lines:
                tokens = line.strip().split(",")
                conf = Config(int(tokens[2]), int(tokens[3]))
                self.results[conf] = (tokens[4] == "1")
                if self.results[conf]:
                    is_passed = True
        return is_passed


def run_afl(workdir: str, tools_dir: str, timeout: int, fuzzer_id: str, strategy: str, afl_master_dir: str, iteration_limit: int, previous_afl_result_file: str = "") -> subprocess.Popen:
    afl_cmd = ["afl-fuzz", "-o", workdir + "/out", "-m", "none", "-d", "-n", "-t",
               str(timeout*1000), "-w", workdir, "-S", fuzzer_id, "-y", strategy, "-k", afl_master_dir]
    if strategy == "positive":
        afl_cmd = afl_cmd + ["-u", previous_afl_result_file]
    if iteration_limit > 0:
        afl_cmd += ["-l", str(iteration_limit)]
    afl_cmd += ["--", os.path.join(tools_dir, "php-test.py"), os.path.join(workdir, "src"),
                os.path.join(workdir, "tests"), workdir]
    print("Run afl-fuzz " + fuzzer_id)
    print(afl_cmd)
    return subprocess.Popen(afl_cmd)


def main(argv):
    print("run-afl.py!!!")
    arg_dict = dict()
    parallel_count = 1
    iteration_limit = -1
    opts, args = getopt.getopt(argv[1:], "w:t:j:s:l:u:")
    arg_dict["s"] = "guided"
    for o, a in opts:
        if o == "-w":
            arg_dict["w"] = a
        elif o == "-t":
            arg_dict["t"] = a
        elif o == "-j":
            parallel_count = int(a)
        elif o == "-s":
            arg_dict["s"] = a
        elif o == "-l":
            iteration_limit = int(a)
        elif o == "-u":
            arg_dict["u"] = a

    conf_dict = dict()
    with open(os.path.join(arg_dict["w"], "repair.conf")) as conf_file:
        for line in conf_file.readlines():
            line = line.strip()
            key = line.split("=")[0]
            value = line.split("=")[1]
            conf_dict[key] = value
    switch_json = dict()
    with open(os.path.join(arg_dict['w'], 'switch-info.json')) as json_file:
        switch_json = json.load(json_file)

    # switches = switch_json["case_cluster"]
    priority = switch_json["priority"]
    switch_num = switch_json["switch_num"]
    print(switch_num, type(switch_num))

    neg_test = list()
    pos_test = list()
    with open(conf_dict["revision_file"]) as revision_file:
        line = revision_file.readline()
        line = revision_file.readline()
        line = revision_file.readline()
        line = revision_file.readline()
        for test in line.strip().split():
            neg_test.append(int(test))
        line = revision_file.readline()
        line = revision_file.readline()
        for test in line.strip().split():
            pos_test.append(int(test))

    os.environ["AFL_SKIP_CPUFREQ"] = "1"
    os.environ["AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES"] = "1"
    os.environ["AFL_NO_UI"] = "1"
    os.environ["AFL_FAST_CAL"] = "1"
    timeout = (int(arg_dict["t"]) // 1000) + 1
    # os.chdir(os.path.join(arg_dict["w"], "src"))
    #os.system("rm -rf " + arg_dict['w'] + "/out")

    fuzz_queue = []
    result_map = dict()
    fid = 0
    confgen = ConfigGenerator(arg_dict, switch_num, switch_json)
    #confgen.init_rules()
    flag = True
    # Test for negative case
    while True:
        if len(fuzz_queue) < parallel_count and flag:
            fuzzer_id = confgen.fuzzer_id_generator(fid, parallel_count)
            if len(fuzzer_id) == 0:
                flag = False
                continue
            previous_afl_result_file = ""
            if "u" in arg_dict:
                previous_afl_result_file = arg_dict["u"]
            running_fuzzer = run_afl(
                arg_dict['w'], conf_dict['tools_dir'], timeout, 
                fuzzer_id, arg_dict["s"], confgen.result_dir, iteration_limit, previous_afl_result_file)
            fuzzer = Fuzzer(running_fuzzer, fid, parallel_count, arg_dict['w'])
            fuzz_queue.append(fuzzer)
            time.sleep(1)
            print(f"append fuzzer{fid}")
            fid += 1
        elif flag == False and len(fuzz_queue) == 0:
            break
        else:
            fqstr = ""
            for fuzz in fuzz_queue:
                fqstr += f"fuzzer{fuzz.fid}/{fuzz.total}, "            
            print(f"fuzz_queue: [{fqstr}]")
            for i in range(len(fuzz_queue)):
                fuzz: Fuzzer = fuzz_queue[i]
                if fuzz.is_alive():
                    print(f"fuzzer{fuzz.fid} did not end...")
                    fuzz.append_result(confgen.result_file)
                    time.sleep(0.5)
                else:
                    print(f"fuzzer{fuzz.fid} finished!")
                    (out, err) = fuzz.fuzzer.communicate()
                    fuzz.append_result(confgen.result_file)
                    result = confgen.result_analyzer(fuzz)
                    print(f"result: {result}")
                    fuzz.set_result(result)
                    result_map[fuzz.fid] = fuzz
                    fuzz_queue.pop(i)
                    break
    succ_switches = []
    for i in range(switch_num):
        if i in result_map:
            fuzz = result_map[i]
            if fuzz.result:
                succ_switches.append(i)
    print(succ_switches)
    title = f"{arg_dict['s']} -j {parallel_count}"
    if iteration_limit > 0:
        title += " -l " + str(iteration_limit)
    afl_plot.afl_plot_one(confgen.result_file, title,  "", True)
    # Test for positive cases
    if len(succ_switches) > 0:
        exit(0)
    else:
        exit(1)


if __name__ == "__main__":
    main(sys.argv)
