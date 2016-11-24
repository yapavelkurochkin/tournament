linux debian 8
==============

To build debian project:

    sudo apt-get install dpkg-dev debhelper
    qmake
    make deb

Please do not forget that deb-file is placed at upper directory (i.e, '..').
Built package can be installed with 'dpkg -i tournament_xxx.deb'

To build binary:

    qmake
    make

Tested on Qt 5.7, which was built for static linking:

    ./configure -static

On Qt5.5 installation I had a problem when 'make' returned error:

    /usr/bin/ld: cannot find -lGL
    collect2: error: ld returned 1 exit status
    Makefile:311: recipe for target 'tournament' failed

There is no need in -lGL flag, you can remove it without any problem and program will be built.

win8
====

To build for win8 you should install mingw (http://www.mingw.org) and Qt. I tested on Qt5.4 with MinGW 4.9 32 bit.

In qt5.4 shell (installed with Qt and can be found in Start panel) change directory to gui/ and type

    qmake
    mingw32-make
    mingw32-make deploy

After that release/ directory is ready for deploy: make tournament.zip and send it to user.

