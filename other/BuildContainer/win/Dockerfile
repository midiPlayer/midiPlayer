FROM debian
#RUN zypper ref && zypper in -y libqt5-qtquick1-deve llibqt5-qtwebsockets-devel
#RUN apt-get update && apt-get -y install  autoconf automake autopoint bash bison bzip2 flex gettext git g++ gperf intltool libffi-dev libgdk-pixbuf2.0-dev libtool libltdl-dev libssl-dev libxml-parser-perl make openssl p7zip-full patch perl pkg-config python ruby scons sed unzip wget xz-utils
#RUN git clone https://github.com/mxe/mxe.git && cd mxe && make qt5
RUN echo "deb http://pkg.mxe.cc/repos/apt/debian wheezy main" > /etc/apt/sources.list.d/mxeapt.list && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB && apt-get update
RUN apt-get -y install mxe-x86-64-w64-mingw32.static-qtwebsockets mxe-x86-64-w64-mingw32.static-qtquick1
#RUN export PATH=/usr/lib/mxe/usr/bin:$PATH && PATH=/usr/lib/mxe/usr/x86_64-w64-mingw32.static/qt5/bin/:$PATH
