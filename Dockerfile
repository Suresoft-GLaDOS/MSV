FROM rastasheep/ubuntu-sshd:18.04
MAINTAINER YoungJae Kim <kyj1411@unist.ac.kr>
RUN apt -y -q update
RUN apt -y -q upgrade
# Other dependencies
RUN apt -y install xz-utils
RUN apt -y install build-essential
RUN apt -y install subversion
RUN apt -y install bison
RUN apt -y install flex
RUN apt -y install libcap-dev
RUN apt -y install cmake
RUN apt -y install libncurses5-dev
RUN apt -y install libboost-all-dev
RUN apt -y install wget
RUN apt -y install lsb-core
RUN apt -y install silversearcher-ag
RUN apt -y install global
RUN apt -y install python3-pip
RUN apt -y install python-pip
RUN apt -y install libblas-dev
RUN apt -y install maven
RUN apt -y install gdb
RUN apt -y install git autoconf automake libtool
RUN apt -y install libsqlite3-dev
RUN apt -y install libcap-dev
RUN apt -y install libacl1-dev
RUN apt-get install software-properties-common -y --no-install-recommends
# Install dejagnu
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt -y install dejagnu
# Install sbt
RUN echo "deb https://repo.scala-sbt.org/scalasbt/debian all main" | tee /etc/apt/sources.list.d/sbt.list
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 2EE0EA64E40A89B84B2DF73499E82A75642AC823
RUN apt-get update
RUN apt -y install sbt
ENV LC_CTYPE C.UTF-8
ENV LC_ALL C.UTF-8
RUN pip3 install watchdog
WORKDIR /root
# Installing other dependencies
RUN bash -c 'pip3 install numpy scipy theano pandas tqdm h5py'
RUN apt -y install ksh
RUN bash -c 'cpan Text::CSV Text::Trim'
ENTRYPOINT ["/usr/sbin/sshd", "-d"]