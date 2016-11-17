TEMPLATE=app
QMAKE_CXXFLAGS += -g
CONFIG += release
TARGET = tournament
QT += widgets
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6

OBJECTS_DIR=.o
MOC_DIR=.moc

SOURCES=playerlist.cpp player.cpp main.cpp playertable.cpp rrgroup.cpp\
        match.cpp rrtable.cpp tournament.cpp tournwidget.cpp matchres.cpp\
        playerscores.cpp group.cpp swissgroup.cpp grouptable.cpp swisstable.cpp\
        mainwindow.cpp newtourndialog.cpp about.cpp newplayerdialog.cpp \
        ratingsdialog.cpp ratingstable.cpp tournhist.cpp tournprops.cpp \
        tournalgo.cpp rrplayoff.cpp qplayoff.cpp tourndata.cpp tournalgofactory.cpp \
        playoff.cpp  \
        ../rating/rating.c ../rating/polynom.c ../rating/polycoeff.c 

HEADERS=playerlist.h player.h playertable.h rrgroup.h\
        match.h rrtable.h tournament.h tournwidget.h matchres.h\
        playerscores.h group.h swissgroup.h grouptable.h swisstable.h\
        mainwindow.h newtourndialog.h about.h version.h newplayerdialog.h\
        ratingsdialog.h ratingstable.h integralscores.h tournhist.h tournprops.h \
        tournalgo.h rrplayoff.h  qplayoff.h tourndata.h  tournalgofactory.h \
        playoff.h \
        ../rating/rating.h ../rating/polynom.h ../rating/polycoeff.h

RESOURCES=tournament.qrc

unix {
  CONFIG -= debug

  target.path = /usr/bin
  target.files = $$TARGET

  INSTALLS = target

  # for build with qt statically
  QTPLUGIN.platforms = qxcb

  # for building deb-package
  deb.commands = dpkg-buildpackage -B 

  QMAKE_EXTRA_TARGETS += deb
}

macx {
  deploy.depends = all 
  deploy.commands = macdeployqt tournament.app -dmg

  QMAKE_EXTRA_TARGETS += deploy
}


