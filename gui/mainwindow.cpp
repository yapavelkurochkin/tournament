#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>
#include <QFileDialog>
#include <QDir>

#include "mainwindow.h"
#include "tournament.h"
#include "tournwidget.h"
#include "newtourndialog.h"

LeagueMainWindow::LeagueMainWindow()
: QMainWindow()
{
  createActions();
  createMenus();
}

void LeagueMainWindow::createActions()
{
  loadT = newAction( tr("&Load"), QKeySequence::Open, 
                         SLOT( loadTournament() ) );
  newT = newAction( tr("&New"), QKeySequence::New, 
                         SLOT( newTournament() ) );
  saveT = newAction( tr("&Save"), QKeySequence::Save,
                         SLOT( saveTournament() ) );
}

/** Action creation helper.
 */
QAction* LeagueMainWindow::newAction( QString name, 
                                      QKeySequence::StandardKey shortcuts, 
                                      const char* slot )
{
  QAction* a = new QAction( name, this);
  a->setShortcuts( shortcuts );

  connect( a, SIGNAL(triggered()), this, slot ); 
  return a;
}

void LeagueMainWindow::createMenus()
{
  QMenu* fileMenu = menuBar()->addMenu(tr("&Tournament"));
  fileMenu->addAction( loadT );
  fileMenu->addAction( newT );
  fileMenu->addAction( saveT );
}

void LeagueMainWindow::loadTournament( )
{
  QString fName = QFileDialog::getOpenFileName(this,
                  tr("Open tournament"), QDir::homePath(), 
                  tr("Tournament Files (*.trn)"));

  if ( !fName.isNull() ) {
    Tournament* t = Tournament::fromFile( fName );
  
    QScrollArea* scrollArea = new QScrollArea( this );
    TournamentWidget* tw = new TournamentWidget( t );
    scrollArea->setWidget( tw );
    scrollArea->setWidgetResizable( true );
    setCentralWidget( scrollArea );
  }
}

/** shows dialog with new tournament properties. if user accepts
 * entered data then created tournament becomes active
 */
void LeagueMainWindow::newTournament( )
{
  NewTournDialog d( this );

  if ( QDialog::Accepted == d.exec() ) {
    QString cat = d.category();
    Match::Type mtype = d.matchType();
    unsigned int gsize = d.groupSize();
    PlayerList players = d.players();

    Tournament* t = new Tournament( players, cat, mtype, gsize );
  
    QScrollArea* scrollArea = new QScrollArea( this );
    TournamentWidget* tw = new TournamentWidget( t );
    scrollArea->setWidget( tw );
    scrollArea->setWidgetResizable( true );
    setCentralWidget( scrollArea );
  }  
}

