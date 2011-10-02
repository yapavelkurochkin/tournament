#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <cxxabi.h>

#include <QProcess>
#include <QtGui/QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QX11Info>
#include <QDebug>
#include <QScrollArea>

#include "playerlist.h"
#include "playertable.h"
#include "tournwidget.h"

/* This structure mirrors the one found in /usr/include/asm/ucontext.h */
typedef struct _sig_ucontext {
 unsigned long     uc_flags;
 struct ucontext   *uc_link;
 stack_t           uc_stack;
 struct sigcontext uc_mcontext;
 sigset_t          uc_sigmask;
} sig_ucontext_t;

void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
{
 void *             array[50];
 char **            messages;
 int                size, i;

 size = backtrace(array, 50);

 /* overwrite sigaction with caller's address */
// array[1] = caller_address;

 qDebug() << __PRETTY_FUNCTION__;
 messages = backtrace_symbols(array, size);
// backtrace_symbols_fd( array, size, 0);
 QRegExp regexp("([^(]+)\\(([^)^+]+)(\\+[^)]+)\\)\\s(\\[[^]]+\\])");

 /* skip first stack frame (points here) */
 for (i = 1; i < size && messages != NULL; ++i)
 {
   if ( regexp.indexIn(messages[i]) != -1 ) {
   QString symbol = regexp.cap(2);

   int status = 0;

   char* buf = abi::__cxa_demangle( qPrintable(symbol), 0, 0, &status );
   fprintf(stderr, "[bt]: (%d) %s %s\n", i, messages[i], buf);
   
   free(buf);
   }
 }

 free(messages);

 exit(EXIT_FAILURE);
}


/** Program's entry point. Fisrt, it calls #QTextCodec::setCodecForTr
  to convert inline string literals from source file encodoing to
  UTF16. Then, it adds support for command-line arguments like \e "-h"
  and \e "-v" using QtArgCmdLine.

  \todo
  - Link to QtArgCmdLine documentation... if it exists.

  Next, it installs a global StyleOverride to make controls usable on
  a touchscreen. And last, it hides mouse cursor (no use on
  touchscreen!), shows LauncherWindow and enters an event loop.
   */
int main(int argc, char *argv[])
{
 struct sigaction sigact;

 sigact.sa_sigaction = crit_err_hdlr;
 sigact.sa_flags = SA_RESTART | SA_SIGINFO;

 if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0)
 {
  fprintf(stderr, "error setting signal handler for %d (%s)\n",
    SIGSEGV, strsignal(SIGSEGV));

  exit(EXIT_FAILURE);
 }

  // inline string literals conversion
  QTextCodec* c = QTextCodec::codecForName( "UTF-8" );
  QTextCodec::setCodecForTr( c );

  QApplication a( argc, argv, QApplication::GuiServer );

  PlayerList players = loadPlayerList( "players.utf8.txt" );
  qSort( players.begin(), players.end() ); 
  for ( int i = 0; i < players.count(); i ++ ) {
    qDebug() << players.at( i ).name() << players.at( i ).rating();
  }

  PlayerTable table( players );
  table.show();

  Tournament* t = new Tournament( players, Tournament::M2, Match::BestOf3, 4 );
  QScrollArea* scrollArea = new QScrollArea();
  TournamentWidget* tw = new TournamentWidget( t );
  scrollArea->setWidget( tw );
  scrollArea->show();  
  scrollArea->resize( 800, 600 );

//  qDebug() << tw->sizeHint();
  tw->show();
  tw->setMinimumWidth( 700 );

  return a.exec();
}
