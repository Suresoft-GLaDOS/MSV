libtoolize --force
aclocal
autoheader
autoconf
automake --add-missing
./configure
make -j 10 install
cd AFLplusplus
make -j 10 install
for i in `ipcs | grep $LOGNAME | awk '{print $2}'`; do ipcrm -m $i; done;
