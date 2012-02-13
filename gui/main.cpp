#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cxxabi.h>

#include <QProcess>
#include <QtGui/QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QX11Info>
#include <QDebug>
#include <QScrollArea>
#include <QFile>

#include "playerlist.h"
#include "playertable.h"
#include "tournwidget.h"
#include "mainwindow.h"
#include "about.h"

int main(int argc, char *argv[])
{
  // inline string literals conversion
  QTextCodec* c = QTextCodec::codecForName( "UTF-8" );
  QTextCodec::setCodecForTr( c );

  QApplication a( argc, argv, QApplication::GuiServer );

  LeagueMainWindow * w = new LeagueMainWindow();
  w->resize( 600, 400 );
  w->show();

  return a.exec();
}
