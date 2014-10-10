TEMPLATE=app
QMAKE_CXXFLAGS += -g
CONFIG += release
TARGET = tournament

OBJECTS_DIR=.o
MOC_DIR=.moc

SOURCES=playerlist.cpp player.cpp main.cpp playertable.cpp rrgroup.cpp\
        match.cpp rrtable.cpp tournament.cpp tournwidget.cpp matchres.cpp\
        playerscores.cpp group.cpp swissgroup.cpp grouptable.cpp swisstable.cpp\
        mainwindow.cpp newtourndialog.cpp about.cpp newplayerdialog.cpp \
        ratingsdialog.cpp ratingstable.cpp tournhist.cpp \
        ../rating/rating.c ../rating/polynom.c ../rating/polycoeff.c 

HEADERS=playerlist.h player.h playertable.h rrgroup.h\
        match.h rrtable.h tournament.h tournwidget.h matchres.h\
        playerscores.h group.h swissgroup.h grouptable.h swisstable.h\
        mainwindow.h newtourndialog.h about.h version.h newplayerdialog.h\
        ratingsdialog.h ratingstable.h integralscores.h tournhist.h \
        ../rating/rating.h ../rating/polynom.h ../rating/polycoeff.h

RESOURCES=tournament.qrc

deploy.depends = all 
deploy.commands = /usr/local/Trolltech/Qt-4.8.0/bin/macdeployqt tournament.app

QMAKE_EXTRA_TARGETS += deploy

