libtoolize --force
aclocal
autoheader
autoconf
automake --add-missing
./configure
make -j 10 install
cd AFLplusplus
make -j 10
