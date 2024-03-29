# Copyright (C) 2016 Fan Long, Martin Rianrd and MIT CSAIL 
# Prophet
# 
# This file is part of Prophet.
# 
# Prophet is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Prophet is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Prophet.  If not, see <http://www.gnu.org/licenses/>.
from genericpath import isdir, isfile
import sys
import subprocess
from os import path, chdir, getcwd, environ, system, mkdir, walk,remove
import shutil
from tester_common import get_fix_revisions
import multiprocessing as mp

def is_due_to_autoconf_v(config_out):
    lines = config_out.rstrip("\n").split("\n")
    last_line = lines[len(lines) - 1]
    return last_line.find("Bad substitution") != -1

def is_due_to_bison_v(config_out):
    lines = config_out.rstrip("\n").split("\n")
    last_line = lines[len(lines) - 1]
    return last_line.find("bison is required") != -1

def switch_to(out_dir, revision, deps_dir = "php-deps", compile_only = False, config_only = False, paraj = 0):
    ori_dir = getcwd()
    if deps_dir[0] != "/":
        php_deps_dir = ori_dir + "/" + deps_dir
    else:
        php_deps_dir = deps_dir
    chdir(out_dir)
    my_env = environ
    my_env["PATH"] = php_deps_dir + "/bison-2.2-build/bin:" + my_env["PATH"]
    # my_env["PATH"] = php_deps_dir + "/autoconf-2.13:" + my_env["PATH"];
    my_env["PATH"] = php_deps_dir + "/flex-2.5.4-build/bin:" + my_env["PATH"]
    # switch to the revision
    if revision != "":
        assert(not compile_only);
        ret = subprocess.call(["git", "checkout", revision, "-f"], env = my_env);
        if ret != 0:
            print ("Failed to switch to the revision " + revision)
            chdir(ori_dir);
            return False;

    # it is tricky to find the right dependency version to make it
    # builds, we will try multiple times
    if not compile_only:
        cnt = 0;
        subprocess.call(["make","clean"],env=my_env)
        while (True):
            print ("Current path: ", my_env["PATH"])
            cnt = cnt + 1;
            if cnt > 3:
                print ("Failed to configure after " + str(cnt) + " times")
                chdir(ori_dir);
                return False;
            # clean up things
            subprocess.call(["git", "clean", "-f", "-d"], env = my_env)
            # create configure file
            ret = subprocess.call(["./buildconf --force"], env = my_env,shell=True);
            if ret != 0:
                print ("Failed to create config, check autoconf version!")
                chdir(ori_dir);
                return False;
            # do the configure
            # p = subprocess.Popen(["./configure", "-with-libxml-dir=" + php_deps_dir + "/libxml2-2.7.2-build/lib","-enable-debug","-enable-zip"], env = my_env, stderr = subprocess.PIPE);
            p = subprocess.Popen(["./configure", "-with-libxml-dir=" + php_deps_dir + "/libxml2-2.7.2-build","-enable-zip"], env = my_env, stderr = subprocess.PIPE)
            (out, err) = p.communicate();
            # print out
            # print p.returncode
            if p.returncode != 0:
                if is_due_to_autoconf_v(err.decode('utf-8')):
                    # This is possible caused by wrong autoconf version, we can use
                    # autoconf 2.13 to try again!
                    print ("Failed to configure, use autoconf 2.13 to try again")
                    my_env["PATH"] = php_deps_dir + "/autoconf-2.13:" + my_env["PATH"];
                elif is_due_to_bison_v(err.decode('utf-8')):
                    print ("Failed to configure, use bison 2.2 to try again")
                    my_env["PATH"] = php_deps_dir + "/bison-2.5.1-build/bin:" + my_env["PATH"];
                else:
                    print ("Failed to configure due to unknown reason")
                    chdir(ori_dir);
                    return False;
            else:
                break;
            subprocess.call(["make", "clean"], env = my_env);
            # subprocess.call(["git", "clean", "-f", "-d"], env = my_env);
        subprocess.call(["make", "clean"], env = my_env);

    if not config_only:
        ret = subprocess.call(["rm", "-rf", "ext/phar/phar.php"], env = my_env);
        assert( ret == 0);
        if paraj == 0:
            ret = subprocess.call(["make"], env = my_env)
        else:
            ret = subprocess.call(["make", "-j", str(paraj)], env = my_env);
        chdir(ori_dir);
        if ret != 0:
            print ("Failed to compile!")
            return False;
    return True;

def extract_test_cases(out_dir = "php-src-tests", repo_dir = ""):
    if path.exists(out_dir):
        s = raw_input(out_dir + " exist! continue and override (y/n)?")
        if s[0] != 'y':
            return True;
        else:
            shutil.rmtree(out_dir)
    mkdir(out_dir)
    addr = "https://git.php.net/repository/php-src.git"

    if (repo_dir != ""):
        tmp_repo = repo_dir;
        cleanup = False;
    else:
        tmp_repo = out_dir + "/php-src";
        # we git clone the repository
        ret = system("git clone " + addr + " " + out_dir+"/php-src")
        if ret != 0:
            print ("git-clone failed, check your network connection!")
            shutil.rmtree(out_dir)
            return False;
        cleanup = True;

    # walk in the cloned directory to find all test cases
    test_files = []
    for root, dirs, files in walk(tmp_repo):
        for file in files:
            tokens = file.split(".")
            ext = tokens[len(tokens) - 1].strip("\n").strip(" ")
            if ext == "phpt":
                tmpf = open(root + "/" + file, "r");
                s = tmpf.read();
                if (s.find("REDIRECTTEST") < 0):
                    print ("Found: " + root + "/" + file)
                    combined_name = root + "/" + file;
                    if (combined_name.find("ext/session/tests/003.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/session/tests/019.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/session/tests/023.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/standard/tests/file/file_get_contents_error001.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/standard/tests/mail/mail_variation2.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/standard/tests/streams/stream_get_meta_data_socket_variation2.phpt") != -1):
                        continue;
                    if (combined_name.find("Zend/tests/bug64821.1.phpt") != -1):
                        continue;
                    if (combined_name.find("Zend/tests/bug64821.3.phpt") != -1):
                        continue;
                    if (combined_name.find("Zend/tests/bug64821.2.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/standard/tests/file/mkdir-004.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/standard/tests/file/mkdir-005.phpt") != -1):
                        continue;
                    if (combined_name.find("ext/standard/tests/file/chroot_001.phpt") != -1):
                        continue;
                    print ("extract!")
                    shutil.copyfile(root+"/"+file, out_dir + "/" + str(len(test_files)).zfill(5) + ".phpt")
                    test_files.append(root+"/"+file)

    # write to a log
    f = open(out_dir + "/testfile.log", "w")
    print >>f, len(test_files);
    for i in range(0, len(test_files)):
        print >>f, i
        print >>f, test_files[i]
    f.close()

    # we only need testcases, so remove directory
    if cleanup:
        shutil.rmtree(out_dir + "/php-src")
    return True;

def _is_start(s):
    for c in s:
        if (c < 'A') or (c > 'Z'):
            return False;
    return True;

class php_initializer:
    # display repo information of an existing directory
    def display_info(self, repo_dir):
        ori_dir = getcwd();
        chdir(repo_dir);
        ret = subprocess.call(["git", "remote", "-v"]);
        chdir(ori_dir);
        return ret == 0;
    # reset a repo to the master head revision
    def reset(self, repo_dir):
        ori_dir = getcwd();
        chdir(repo_dir);
        ret = subprocess.call(["git", "checkout", "master", "-f"]);
        chdir(ori_dir);
        return ret == 0;
    # create php repository in repo_dir and get all fixes revision in log_file
    def create_repo(self, repo_dir, log_file):
        github_addr = "https://git.php.net/repository/php-src.git";
        if path.exists(repo_dir):
            shutil.rmtree(repo_dir);
        ret = system("git clone "+github_addr+" "+repo_dir);
        if ret != 0:
            print ("Failed to grab from github, check your network connection and make sure you have git!")
            return False;
        f = open(log_file, "w");
        ret = get_fix_revisions(repo_dir);
        for fix_r, previous_r, comment in ret:
            print >>f, "Fix Revision: " + fix_r;
            print >>f, "Previous Revision: " + previous_r;
            print >>f, "Comment:";
            print >>f, comment;
        f.close();
        return True;
    # extract test cases and put it in a directory
    def extract_tests(self, repo_dir, test_dir):
        ret = extract_test_cases(test_dir, repo_dir);
        return ret;

import psutil

def output_test(id,temp_dir):
    id_str=str(id).zfill(5)

    exp_data=''
    if isfile(temp_dir+'/'+id_str+'.exp'):
        exp_file=open(temp_dir+'/'+id_str+'.exp')
        exp_data=exp_file.read()
        exp_file.close()
    
    out_data=''
    if isfile(temp_dir+'/'+id_str+'.out'):
        out_file=open(temp_dir+'/'+id_str+'.out','rb')
        out_data=out_file.read()
        out_file.close()
    
    if type(out_data)==bytes:
        out_data=out_data.decode('utf-8','ignore')
    return 0

def run_test(src_dir,work_dir,profile_dir,i,timeout,temp_dir=''):
    if i==20 or i=='20' or i==6947 or i=='6947' or i==2246 or i=='2246' or i==7369 or i=='7369' or i==9710 or i=='9710' or i==10416 or i=='10416' or i==7036 or i=='7036':
        return (None,0,'','')
    # Remove randomly failed test
    elif int(i)==8940 or int(i)==6895 or int(i)==8677 or int(i)==8723 or i==9994 or i=='9994' or i==9962 or i=='9962' or i==10059 or i=='10059' or i==10061 or i=='10061' or i==9971 or i=='9971' or i==10198 or i=='10198' or i==10294 or i=='10294' or i==10072 or i=='10072':
        return (None,0,'','')
#    elif 4350 <= int(i) < 4600 and int(i)!= 4470 and int(i)!= 4364:
#        return (None,0,'','')
#    elif 8900 <= int(i) < 9000 and int(i)!= 8938 and int(i)!= 8945 and int(i)!= 8937 and int(i)!= 8903:
#        return (None,0,'','')

    if temp_dir=='':
        temp_dir=work_dir+'/__cleantests/'
    else:
        temp_dir=work_dir+'/__cleantests/'+temp_dir+'_tests'
    cur_temp_dir=temp_dir+'/'+str(i)
    if isdir(cur_temp_dir):
        shutil.rmtree(cur_temp_dir)
    mkdir(cur_temp_dir)
    chdir(cur_temp_dir)
    shutil.copyfile("../" + str(i).zfill(5) + ".phpt", "./" + str(i).zfill(5) + ".phpt")

    #print "###############3php_tester: _test()"
    assert(path.exists(src_dir+"/sapi/cli/php"));
    assert(path.exists(src_dir+"/run-tests.php"));
    # prog = self.repo_dir+"/sapi/cli/php";
    # helper = self.repo_dir+"/run-tests.php";
    prog = "./sapi/cli/php"
    if (path.exists(work_dir + "/../php-src/sapi/cli/php")):
        prog = work_dir + "/../php-src/sapi/cli/php";
        prog = path.abspath(prog);

    helper = "./run-tests.php";
    ori_dir = getcwd();
    chdir(src_dir);

    if (profile_dir == ""):
        test_prog = "./sapi/cli/php";
    else:
        if profile_dir[0] != "/":
            test_prog = work_dir + "/" + profile_dir + "/sapi/cli/php";
        else:
            test_prog = profile_dir + "/sapi/cli/php";

    arg=''
    if cur_temp_dir[0] != "/":
        arg= "./" + str(i).zfill(5) + ".phpt"
    else:
        arg=cur_temp_dir + "/" + str(i).zfill(5) + ".phpt"


    if "MSV_RUN_ORIGINAL" in environ:
        environ["__PID"] = f"{i}-{environ['__PID']}"

    ret=subprocess.Popen([prog,helper,'-p',test_prog,'-q',arg],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    try:
        so,se=ret.communicate(timeout=timeout)

        lines = so.splitlines()
        test_section = False;
        for line in lines:
            try:
                line=line.decode('utf-8')
            except UnicodeDecodeError:
                print('fail to decode output',file=sys.stderr)
                continue
            tokens = line.split();
            if len(tokens) == 0:
                continue;
            if (len(tokens) > 2) and (tokens[0] == "Running") and (tokens[1] == "selected") and (tokens[2] == "tests."):
                test_section = True;
            elif (tokens[0][0:6] == "======") and (test_section == True):
                test_section = False;
            elif (test_section == True) and (_is_start(tokens[0])):
                if (tokens[0] == "PASS") or ((len(tokens) > 3) and tokens[3] == "PASS"):
                    return (i,ret.returncode,so,se)
    except subprocess.TimeoutExpired:
        pid=ret.pid
        children=[]
        for child in psutil.Process(pid).children(True):
            if psutil.pid_exists(child.pid):
                children.append(child)

        for child in children:
            child.kill()
        ret.kill()

        return (None,1,'','Timeout expired!')

    return (None,ret.returncode,'success','')

class php_tester:
    def __init__(self, work_dir, repo_dir, test_dir,temp_dir="",timeout=None,max_cpu=1):
        self.repo_dir = repo_dir;
        self.test_dir = test_dir;
        self.work_dir = work_dir;
        self.temp_dir=temp_dir
        self.max_cpu=max_cpu

        f = open(test_dir + "/testfile.log", "r");
        line = f.readline();
        f.close();
        self.n = int(line.strip("\n"));
        self.time_out=timeout

    def getn(self):
        return self.n;

    # build a revision of php
    def set_revision(self, revision, deps_dir = ""):
        ori_dir = getcwd();
        print (self.repo_dir + " " + revision)
        try:
            if (deps_dir == ""):
                ret = switch_to(self.repo_dir, revision);
            else:
                ret = switch_to(self.repo_dir, revision, deps_dir);
        except KeyboardInterrupt:
            raise
        except:
            # It may changed the current directory due to failure
            chdir(ori_dir);
            print ("Build failed with exception: ", sys.exc_info()[0])
            return False;
        if ret:
            print ("Done reset revision to " + revision)
        else:
            print ("Failed to build revision " + revision)
        return ret;

    # test the php build with testcases [test_id, test_id+n)
    def _test(self, s, profile_dir = ""):
        ori_dir=getcwd()
        target=s.copy()        
        processes=[]

        pool=mp.Pool(self.max_cpu)

        for i in target:
            processes.append(pool.apply_async(run_test,args=(self.repo_dir,self.work_dir,profile_dir,i,self.time_out,self.temp_dir)))
        
        ret=set()
        pool.close()
        for r in processes:
            (res,_,_,_)=r.get()
            if res is not None:
                ret.add(res)
        pool.join()

        system(f'killall --wait {self.repo_dir}/sapi/cli/php > /dev/null 2>&1')
        chdir(ori_dir)
        return ret;

    # clean-up required before running test()
    def prepare_test(self, s = None):
        self.tmptest_dir = self.work_dir + "/__cleantests";
        if not path.exists(self.tmptest_dir):
            mkdir(self.tmptest_dir)
        if self.temp_dir!="":
            self.tmptest_dir+="/"+self.temp_dir+"_tests"
        if (path.exists(self.tmptest_dir)):
            shutil.rmtree(self.tmptest_dir);
        #print "Preparing clean test dir..."
        if (s == None):
            shutil.copytree(self.test_dir, self.tmptest_dir);
        else:
            mkdir(self.tmptest_dir);
            for i in s:
                shutil.copyfile(self.test_dir + "/" + str(i).zfill(5) + ".phpt", self.tmptest_dir + "/" + str(i).zfill(5) + ".phpt");

    def test(self, s, profile_dir = ""):
        self.prepare_test(s);
        new_s = [];
        ret = set();
        for i in s:
            new_s.append(i);
            if (len(new_s) >= 100):
                ret = ret | self._test(new_s, profile_dir);
                new_s = [];
        if len(new_s) != 0:
            ret = ret | self._test(new_s, profile_dir);
        # shutil.rmtree(self.tmptest_dir);
        return ret;

    # test the php build with all cases, and return set of passed id
    def test_all(self):
        ret = set()
        i = 0;
        self.prepare_test();
        while i < self.n:
            if self.n - i < 100:
                print ("Testing [" + str(i) + "," + str(self.n) + ")")
                s = list(i for i in range(i, self.n));
                r = self._test(s);
                i = self.n;
            else:
                print ("Testing [" + str(i) + "," + str(i + 100) + ")")
                s = list(i for i in range(i, i + 100));
                r = self._test(s)
                i = i + 100;
            ret = ret | r;
        shutil.rmtree(self.tmptest_dir);
        return ret;

