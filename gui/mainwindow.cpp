#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>

#include "mainwindow.h"
#include "tournament.h"
#include "tournwidget.h"

LeagueMainWindow::LeagueMainWindow()
: QMainWindow(),
  loadTourn( NULL )
{
  createActions();
  createMenus();
}

void LeagueMainWindow::createActions()
{
  loadTourn = new QAction( tr("&Load"), this);
  loadTourn->setShortcuts(QKeySequence::Open);

  connect( loadTourn, SIGNAL(triggered()), this, SLOT( loadTournament() ) ); 
}

void LeagueMainWindow::createMenus()
{
  QMenu* fileMenu = menuBar()->addMenu(tr("&Tournament"));
  fileMenu->addAction( loadTourn );
  
  menuBar()->addSeparator();
  
  QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
}

void LeagueMainWindow::loadTournament( )
{
  Tournament* t = Tournament::fromFile( "tourn.dat" );
  
  QScrollArea* scrollArea = new QScrollArea( this );
  TournamentWidget* tw = new TournamentWidget( t );
  scrollArea->setWidget( tw );
  scrollArea->setWidgetResizable( true );
  setCentralWidget( scrollArea );
}
