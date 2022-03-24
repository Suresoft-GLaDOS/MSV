libtoolize --force
aclocal
autoheader
autoreconf
automake --add-missing
./configure
make -j 10
make -j 10 install

