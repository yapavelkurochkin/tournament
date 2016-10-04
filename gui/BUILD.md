linux debian 8
==============

To build debian project:

    sudo apt-get install dpkg-dev debhelper
    qmake
    make deb

To build binary:

    qmake
    make

Tested on Qt 5.7, which was built for static linking:

    ./configure -static
