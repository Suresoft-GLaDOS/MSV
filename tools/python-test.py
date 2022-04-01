#!/usr/bin/env python3
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
from sys import argv
import getopt
from os import chdir, getcwd, system, path, environ, remove
import subprocess
import multiprocessing as mp
import uuid
from difflib import SequenceMatcher
import psutil

cases = [
    "test_grammar",
    "test_opcodes",
    "test_dict",
    "test_builtin",
    "test_exceptions",
    "test_types",
    "test_unittest",
    "test_doctest",
    "test_doctest2",
    "test___all__",
    "test___future__",
    "test__locale",
    "test_abc",
    "test_abstract_numbers",
    "test_aifc",
    "test_argparse",
    "test_array",
    "test_ast",
    "test_asynchat",
    "test_asyncore",
    "test_atexit",
    "test_audioop",
    "test_augassign",
    "test_base64",
    "test_bigaddrspace",
    "test_bigmem",
    "test_binascii",
    "test_binhex",
    "test_binop",
    "test_bisect",
    "test_bool",
    "test_bufio",
    "test_bytes",
    "test_bz2",
    "test_calendar",
    "test_call",
    "test_capi",
    "test_cgi",
    "test_charmapcodec",
    "test_class",
    "test_cmath",
    "test_cmd",
    "test_cmd_line",
    "test_cmd_line_script",
    "test_code",
    "test_codeccallbacks",
    "test_codecencodings_cn",
    "test_codecencodings_hk",
    "test_codecencodings_jp",
    "test_codecencodings_kr",
    "test_codecencodings_tw",
    "test_codecs",
    "test_codeop",
    "test_coding",
    "test_collections",
    "test_colorsys",
    "test_compare",
    "test_compile",
    "test_compileall",
    "test_complex",
    "test_concurrent_futures",
    "test_configparser",
    "test_contains",
    "test_contextlib",
    "test_copy",
    "test_copyreg",
    "test_cprofile",
    "test_crashers",
    "test_crypt",
    "test_csv",
    "test_ctypes",
    "test_datetime",
    "test_dbm",
    "test_dbm_dumb",
    "test_dbm_gnu",
    "test_dbm_ndbm",
    "test_decimal",
    "test_decorators",
    "test_defaultdict",
    "test_deque",
    "test_descr",
    "test_descrtut",
    "test_dictcomps",
    "test_dictviews",
    "test_difflib",
    "test_dis",
    "test_distutils",
    "test_docxmlrpc",
    "test_dummy_thread",
    "test_dummy_threading",
    "test_dynamic",
    "test_email",
    "test_enumerate",
    "test_eof",
    "test_epoll",
    "test_errno",
    "test_exception_variations",
    "test_extcall",
    "test_faulthandler",
    "test_fcntl",
    "test_file",
    "test_filecmp",
    "test_fileinput",
    "test_fileio",
    "test_float",
    "test_flufl",
    "test_fnmatch",
    "test_fork1",
    "test_format",
    "test_fractions",
    "test_frozen",
    "test_ftplib",
    "test_funcattrs",
    "test_functools",
    "test_future",
    "test_future3",
    "test_future4",
    "test_future5",
    "test_gc",
    "test_generators",
    "test_genericpath",
    "test_genexps",
    "test_getargs2",
    "test_getopt",
    "test_gettext",
    "test_glob",
    "test_global",
    "test_grp",
    "test_gzip",
    "test_hash",
    "test_hashlib",
    "test_heapq",
    "test_hmac",
    "test_html",
    "test_htmlparser",
    "test_http_cookiejar",
    "test_http_cookies",
    "test_httplib",
    "test_httpservers",
    "test_imaplib",
    "test_imp",
    "test_import",
    "test_importhooks",
    "test_importlib",
    "test_index",
    "test_inspect",
    "test_int",
    "test_int_literal",
    "test_io",
    "test_ioctl",
    "test_isinstance",
    "test_iter",
    "test_iterlen",
    "test_itertools",
    "test_json",
    "test_keywordonlyarg",
    "test_largefile",
    "test_lib2to3",
    "test_linecache",
    "test_list",
    "test_listcomps",
    "test_locale",
    "test_long",
    "test_longexp",
    "test_macpath",
    "test_mailbox",
    "test_marshal",
    "test_math",
    "test_memoryio",
    "test_memoryview",
    "test_metaclass",
    "test_mimetypes",
    "test_minidom",
    "test_mmap",
    "test_module",
    "test_modulefinder",
    "test_multibytecodec",
    "test_multibytecodec_support",
    "test_multiprocessing",
    "test_mutants",
    "test_netrc",
    "test_nis",
    "test_nntplib",
    "test_normalization",
    "test_ntpath",
    "test_numeric_tower",
    "test_openpty",
    "test_operator",
    "test_optparse",
    "test_os",
    "test_osx_env",
    "test_packaging",
    "test_parser",
    "test_pdb",
    "test_peepholer",
    "test_pep247",
    "test_pep263",
    "test_pep277",
    "test_pep292",
    "test_pep3120",
    "test_pep3131",
    "test_pep352",
    "test_pickle",
    "test_pickletools",
    "test_pipes",
    "test_pkg",
    "test_pkgimport",
    "test_pkgutil",
    "test_platform",
    "test_plistlib",
    "test_poll",
    "test_popen",
    "test_poplib",
    "test_posix",
    "test_posixpath",
    "test_pow",
    "test_pprint",
    "test_print",
    "test_profile",
    "test_property",
    "test_pstats",
    "test_pty",
    "test_pulldom",
    "test_pwd",
    "test_pyclbr",
    "test_pydoc",
    "test_pyexpat",
    "test_queue",
    "test_quopri",
    "test_raise",
    "test_random",
    "test_range",
    "test_re",
    "test_readline",
    "test_reprlib",
    "test_resource",
    "test_richcmp",
    "test_rlcompleter",
    "test_robotparser",
    "test_runpy",
    "test_sax",
    "test_sched",
    "test_scope",
    "test_select",
    "test_set",
    "test_setcomps",
    "test_shelve",
    "test_shlex",
    "test_shutil",
    "test_signal",
    "test_site",
    "test_slice",
    "test_smtpd",
    "test_smtplib",
    "test_sndhdr",
    "test_socket",
    "test_sort",
    "test_sqlite",
    "test_ssl",
    "test_strftime",
    "test_string",
    "test_stringprep",
    "test_strlit",
    "test_strptime",
    "test_strtod",
    "test_struct",
    "test_structmembers",
    "test_structseq",
    "test_subprocess",
    "test_sunau",
    "test_sundry",
    "test_super",
    "test_symtable",
    "test_syntax",
    "test_sys",
    "test_sys_setprofile",
    "test_sys_settrace",
    "test_sysconfig",
    "test_syslog",
    "test_tarfile",
    "test_telnetlib",
    "test_tempfile",
    "test_textwrap",
    "test_thread",
    "test_threading",
    "test_threaded_import",
    "test_threadedtempfile",
    "test_threading_local",
    "test_threadsignals",
    "test_time",
    "test_timeit",
    "test_tokenize",
    "test_trace",
    "test_traceback",
    "test_tuple",
    "test_typechecks",
    "test_ucn",
    "test_unary",
    "test_unicode",
    "test_unicode_file",
    "test_unicodedata",
    "test_univnewlines",
    "test_unpack",
    "test_unpack_ex",
    "test_urllib",
    "test_urllib2",
    "test_urllib2_localnet",
    "test_urllib_response",
    "test_urlparse",
    "test_userdict",
    "test_userlist",
    "test_userstring",
    "test_uu",
    "test_uuid",
    "test_wait3",
    "test_wait4",
    "test_warnings",
    "test_wave",
    "test_weakref",
    "test_weakset",
    "test_with",
    "test_wsgiref",
    "test_xdrlib",
    "test_xml_etree",
    "test_xml_etree_c",
    "test_xmlrpc",
    "test_zipfile",
    "test_zipimport",
    "test_zipimport_support",
    "test_zlib"];

def run_test(case_str,id,timeout):
    if id == 243:
        print("243")
        return
    # print(case_str)
    msv_tmp_out = f"/tmp/{uuid.uuid4()}.out"
    if "MSV_OUTPUT_DISTANCE_FILE" in environ:
        environ["MSV_TMP_OUT"] = msv_tmp_out
    proc = subprocess.Popen(["./python Lib/test/regrtest.py -w " + case_str], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # ret = subprocess.call(["./python Lib/test/regrtest.py " + case_str], shell = True);
    try:
        so,se = proc.communicate(timeout=timeout)
        if "MSV_OUTPUT_DISTANCE_FILE" in environ:
            with open(msv_tmp_out, "w") as f:
                # print("so:" + so.decode("utf-8"))
                # print("se:" + se.decode("utf-8"))
                f.write(so.decode("utf-8"))
                f.write(se.decode("utf-8"))
        if proc.returncode == 0:
            print (id)
    except:
        pid = proc.pid
        children = list()
        for child in psutil.Process(pid).children(recursive=True):
            if psutil.pid_exists(child.pid):
                children.append(child.pid)
        for child in children:
            child.kill()
        proc.kill()
    
    if "MSV_OUTPUT_DISTANCE_FILE" in environ:
        exp_file = path.join(environ["MSV_PATH"], "benchmarks", "python-exp", str(id) + ".exp")
        exp = ""
        out = ""
        if path.exists(exp_file):
            with open(exp_file, "r") as f:
                exp = f.read()
        if path.exists(msv_tmp_out):
            with open(msv_tmp_out, "r") as f:
                out = f.read()
            # print(f"cp {msv_tmp_out} {exp_file}")
            # system(f"cp {msv_tmp_out} {exp_file}")
            #remove(msv_tmp_out)
        dist = 0
        if len(exp) > 10000:
            dist = abs(len(exp) - len(out))
        else:
            seqMatch = SequenceMatcher(None, exp, out)
            match = seqMatch.find_longest_match(0, len(exp), 0, len(out)).size
            dist = max(len(out) - match, len(exp) - match)
        with open(environ["MSV_OUTPUT_DISTANCE_FILE"], "w") as myfile:
            myfile.write(str(dist))

if __name__ == "__main__":
    opts, args = getopt.getopt(argv[1 :], "p:t:j:i:");
    profile_dir = "";
    timeout=None
    max_parallel=1
    for o, a in opts:
        if o == "-p":
            profile_dir = a;
        elif o=='-t':
            timeout=int(a)
        elif o=='-j':
            max_parallel=int(a)
        elif o=='-i':
            pass

    src_dir = args[0];
    test_dir = args[1];
    work_dir = args[2];
    tdir = path.join(src_dir, "Lib", "test")
    # if (not path.exists(path.join(tdir, "msv-test.set"))):
    #     system(f"rm -rf {tdir}")
    #     system(f"cp -rf {test_dir} {tdir}")
    #     system(f"rm -rf {path.join(src_dir, 'Lib', 'unittest')}")
    #     system(f"cp -rf {path.join(environ['MSV_PATH'], 'benchmarks', 'python-unittest')} {path.join(src_dir, 'Lib', 'unittest')}")
    #     with open(path.join(tdir, "msv-test.set"), "w") as f:
    #         f.write("Initialized!")
    # system(f"rm -rf {path.join(src_dir, 'Lib', 'unittest')}")
    # system(f"cp -rf /root/project/MSV-experiment/benchmarks/python/python-case-69946/python-src/Lib/unittest {path.join(src_dir, 'Lib', 'unittest')}")
    if (len(args) > 3):
        ids = args[3 :];
        cur_dir = src_dir;
        if (profile_dir != ""):
            cur_dir = profile_dir;

        if (not path.exists(cur_dir + "/Lib/oldtest")):
            system("mv " + cur_dir + "/Lib/test " + cur_dir + "/Lib/oldtest");
            system("cp -rf " + test_dir + " " + cur_dir + "/Lib/test");
            # system("cp -rf " + cur_dir + "/Lib/oldtest/regrtest.py " + cur_dir + "/Lib/test/regrtest.py");

        ori_dir = getcwd();
        chdir(cur_dir);
        my_env = environ;
        result=[]
        pool=mp.Pool(max_parallel)
        for i in ids:
            case_str = cases[int(i) - 1]
            result.append(pool.apply_async(run_test,(case_str,int(i),timeout)))
        chdir(ori_dir);

        pool.close()
        for r in result:
            r.wait(timeout)
        pool.join()
