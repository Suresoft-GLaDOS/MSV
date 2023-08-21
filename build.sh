libtoolize --force
aclocal
autoheader
autoreconf
automake --add-missing
./configure
make clean
make -j 10
make -j 10 install
