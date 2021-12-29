libtoolize --force
aclocal
autoheader
autoconf
automake --add-missing
./configure
make -j 10 install
cd AFLplusplus
make -j 10 install

# Generate llvm bit-code for runtime library, for symbolic execution
cd ../src/.libs
wllvm++ -g -fPIC -O0 -c -o _test_runtime.o ../_test_runtime.cpp
llvm-ar rcs libtest_runtime.a _test_runtime.o
extract-bc libtest_runtime.a
cd ../..
for i in `ipcs | grep $LOGNAME | awk '{print $2}'`; do ipcrm -m $i; done;
