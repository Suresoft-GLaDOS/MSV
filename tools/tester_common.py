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
from os import getcwd, chdir
import os
import subprocess

def get_fix_revisions(out_dir):
    ori_dir = getcwd();
    chdir(out_dir);
    subprocess.call(["git", "checkout", "master", "-f"]);
    p = subprocess.Popen(["git", "log"], stdout=subprocess.PIPE);
    chdir(ori_dir);
    (out, err) = p.communicate();
    lines = out.decode('utf-8').split("\n");
    # parse git log to get bug-fix revision and previous revision
    cur_revision = "";
    last_fix_revision = "";
    comment = "";
    last_is_author = False;
    ret = []
    for line in lines:
        tokens = line.strip("\n").strip(" ").split();
        if len(tokens) == 0:
            continue;
        if (len(tokens) == 2) and (tokens[0] == "commit") and (len(tokens[1]) > 10):
            cur_revision = tokens[1];
            if last_fix_revision != "":
                chdir(out_dir);
                p = subprocess.Popen(["git", "rev-list", "--parents", "-n", "1", last_fix_revision], stdout=subprocess.PIPE);
                chdir(ori_dir);
                (out, err) = p.communicate();
                tokens = out.split();
                if len(tokens) == 2:
                    chdir(out_dir);
                    p = subprocess.Popen(["git", "diff", "--name-only", last_fix_revision, last_fix_revision+"^1"], stdout=subprocess.PIPE);
                    chdir(ori_dir);
                    (out,err) = p.communicate();
                    out_lines = out.split("\n");
                    for out_line in out_lines:
                        name_str = out_line.strip();
                        idx = name_str.rfind(".");
                        extension = name_str[idx+1:];
                        if (extension == "c") or (extension == "h") or (extension == "cpp") or (extension == "hpp"):
                            ret.append((last_fix_revision, tokens[1], comment));
                            break;
            last_fix_revision = "";
            comment = "";
        elif (tokens[0] == "Date:") and last_is_author:
            year = int(tokens[5]);
            if (year < 2010):
                break;
        elif (tokens[0] != "Author:" and tokens[0] != "Merge:"):
            comment = comment + "\n" + line;
            is_fix = False;
            for token in tokens:
                if (token.lower() == "fixed") or (token.lower() == "bug") or (token.lower() == "fix"):
                    is_fix = True;
                    break;
            if is_fix:
                last_fix_revision = cur_revision;
        if tokens[0] == "Author:":
            last_is_author = True;
        else:
            last_is_author = False;
    return ret;

def extract_arguments(out_dir, src_file,sub_dir='.'):
    print(f'src file: {src_file}')
    ori_dir = getcwd();
    chdir(out_dir);
    # trigger the make again
    subprocess.call(["touch", src_file]);
    # eliminate the path, leave only the file name for search
    idx = src_file.rfind("/");
    if idx != -1:
        file_name = src_file[idx+1:];
    else:
        file_name = src_file;
    cur_dir=getcwd()
    chdir(sub_dir)
    p = subprocess.Popen(["make", "--debug=j"], stdout = subprocess.PIPE);
    (out, err) = p.communicate();
    chdir(cur_dir)
    lines = out.decode('utf-8').strip().split("\n");
    directory = ".";
    last_line = "";
    for line in lines:
        strip_line = line.strip();
        if (len(strip_line) > 0) and (strip_line[len(strip_line) - 1] == "\\"):
            last_line = last_line + " " + strip_line[0:len(strip_line) - 1];
        else:
            if last_line != "":
                line = last_line + " " + line;
                last_line = "";
            if line.find("Entering directory") != -1:
                idx = line.find("Entering directory");
                idx1 = line.find("`", idx);
                if idx1==-1:
                    idx1=line.find("'")
                idx2 = line.find("'", idx1+1);
                directory = line[idx1 + 1:idx2];

            if line.find(file_name) != -1:
                tokens = line.strip().split();
                idx = 0;
                for i in range(0, len(tokens)):
                    if tokens[i] == "cc" or tokens[i] == "gcc" or tokens[i]=='g++':
                        idx = i + 1;
                        break;
                ret = "";
                for token in tokens[idx:]:
                    if token[0] == '`':
                        break;
                    if token == '&&':
                        break;
                    if token.find("excess-precision") != -1:
                        continue;
                    if token.find(file_name) == -1:
                        token=token.replace("\\'","'")
                        token=token.replace("\\\"","\"")
                        token=token.replace("\"\"",'"')
                        ret = ret + token + " ";
                chdir(ori_dir);
                print(f'args: {ret}')
                return directory, ret;

    # we try another way to get around it
    subprocess.call(["touch", src_file]);
    cur_dir=getcwd()
    chdir(sub_dir)
    p = subprocess.Popen(["make", "-n"], stdout = subprocess.PIPE);
    (out, err) = p.communicate();
    chdir(cur_dir)
    lines = out.decode('utf-8').strip().split("\n");
    directory = ".";
    for line in lines:
        if line.find("Entering directory") != -1:
            idx = line.find("Entering directory");
            idx1 = line.find("`", idx);
            if idx1==-1:
                idx1=line.find("'")
            idx2 = line.find("'", idx1+1);
            directory = line[idx1 + 1:idx2];
        if line.find(file_name) != -1:
            tokens = line.strip().split();
            idx = -1;
            for i in range(0, len(tokens)):
                if tokens[i] == "cc" or tokens[i] == "gcc" or tokens[i].find(";gcc") != -1:
                    idx = i + 1;
                    break;
            if idx != -1:
                ret = "";
                for token in tokens[idx:]:
                    if token[0] == '`':
                        break;
                    if token == '&&':
                        break;
                    if token.find("excess-precision") != -1:
                        continue;
                    if token.find(file_name) == -1:
                        token=token.replace("\\'","'")
                        token=token.replace("\\\"","\"")
                        token=token.replace('""','"')
                        ret = ret + token + " ";
                chdir(ori_dir);
                print(f'args: {ret}')
                return directory, ret;
    chdir(ori_dir);
    return "","";

def extract_arguments_cmake(out_dir:str, src_file:str,sub_dir:str='.',last_cmd:str=''):
    temp_env=os.environ.copy()
    temp_env['CC']='gcc'
    temp_env['CXX']='g++'

    print(f'cmake src file: {src_file}')
    ori_dir = getcwd()
    chdir(out_dir)

    subprocess.run(["touch", src_file])
    
    if last_cmd=='':
        # Run default Unix Makefile in debug mode
        cur_dir=getcwd()
        chdir(sub_dir)
        p = subprocess.Popen(["make", "-n"], stdout = subprocess.PIPE,env=temp_env)
        (out, err) = p.communicate()
        chdir(cur_dir)
        print(out.decode('utf-8'))
        lines = out.decode('utf-8').strip().split("\n");
        directory = ".";
        for line in lines:
            if line.strip().endswith('.c'):
                splitted=line.strip().split('&&')
                directory=splitted[0].replace('cd','').strip()
                cc_cmd=splitted[1].strip().split(' ')[0]
                ret=splitted[1].replace(cc_cmd,'').strip()
                ret_splitted=ret.split(' ')

                is_target=False
                for i,arg in enumerate(ret_splitted.copy()):
                    if arg=='-o':
                        is_target=True
                    elif is_target:
                        ret_splitted[i]=directory+'/'+arg
                        is_target=False

                    if arg.endswith('.c'):
                        ret_splitted.remove(arg)

                ret=' '.join(ret_splitted)
                print(f'args: {ret}')
                chdir(ori_dir)
                return directory, ret

    else:
        # Run last make command if exist
        p = subprocess.Popen([last_cmd+' -v'], stdout = subprocess.PIPE,env=temp_env,shell=True,stderr=subprocess.STDOUT)
        (out, err) = p.communicate()
        print(out.decode('utf-8'))
        lines = out.decode('utf-8').strip().split("\n")
        directory = "."
        for line in lines:
            if line.strip().endswith('.c'):
                # directory=src_file.rsplit('/',2)[0]
                directory='.'
                temp=line.find(' ')
                start_index=line.find(' ',temp+1)+1
                end_index=line.rfind(' ')
                ret=line[start_index:end_index]
                print(f'args: {ret}')
                chdir(ori_dir)
                return directory, ret

    chdir(ori_dir);
    return "","";
