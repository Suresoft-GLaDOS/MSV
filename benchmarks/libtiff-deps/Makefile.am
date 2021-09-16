.PHONY: all-local clean-local

all-local: jpeg-9a-build

jpeg-9a-build:
	wget http://www.ijg.org/files/jpegsrc.v9a.tar.gz
	tar xvzf jpegsrc.v9a.tar.gz
	cd jpeg-9a &&\
	./configure --prefix=$(abs_builddir)/jpeg-9a-build &&\
	make -j 4 &&\
	make install
	ln -s $(abs_builddir)/jpeg-9a-build/lib/libjpeg.so.9.1.0 $(abs_builddir)/jpeg-9a-build/lib/libjpeg.so

clean-local:
	rm *.gz
	rm -rf *-build
	rm -rf jpeg-9a

