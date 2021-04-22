import time
import subprocess
from os import environ,chdir

count=0
env=environ
env["AFL_SKIP_CPUFREQ"]="1"
env["AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES"]="1"
env["AFL_NO_UI"]="1"
env["AFL_FAST_CAL"]="1"

cmd=["/root/project/prophet-gpl/tools/php-test.py", "-f", "/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/src", "/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/tests", "/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir", "1442"]
chdir("src")

while count<20:
    print("Test #"+str(count))
    count+=1
    start=time.perf_counter()
    p=subprocess.Popen(cmd,stdout=subprocess.PIPE)
    out,err=p.communicate()
    end=time.perf_counter()
    if out=="1442":
        print("Success!")
    else:
        print("Fail")
    print("Time: "+str(end-start)+"s")
    print()