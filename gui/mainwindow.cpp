#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <math.h>

#include "mainwindow.h"
#include "tournwidget.h"
#include "newtourndialog.h"
#include "ratingsdialog.h"
#include "about.h"

LeagueMainWindow::LeagueMainWindow()
: QMainWindow(),
  tourn( NULL )
{
  progName = tr( "Tournament" );
  setWindowTitle( progName );
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
  ratings = newAction( tr("&Ratings"), 
                         SLOT( showRatingsTable() ) );
  about = newAction( tr("&About"), QKeySequence::HelpContents,
                         SLOT( showAboutDialog() ) );
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

/** Action creation helper.
 */
QAction* LeagueMainWindow::newAction( QString name, 
                                      const char* slot )
{
  QAction* a = new QAction( name, this);

  connect( a, SIGNAL(triggered()), this, slot ); 
  return a;
}

void LeagueMainWindow::createMenus()
{
  QMenu* fileMenu = menuBar()->addMenu(tr("&Tournament"));
  fileMenu->addAction( loadT );
  fileMenu->addAction( newT );
  fileMenu->addAction( saveT );
  fileMenu->addAction( ratings );

  QMenu* iMenu = menuBar()->addMenu(tr("&Information"));
  iMenu->addAction( about );
}

void LeagueMainWindow::loadTournament( )
{
  QString fName = QFileDialog::getOpenFileName(this,
                  tr("Open tournament"), QDir::homePath(), 
                  tr("Tournament Files (*.trn)"));

  if ( !fName.isNull() ) {
    tourn = Tournament::fromFile( fName );
  
    QScrollArea* scrollArea = new QScrollArea( this );
    TournamentWidget* tw = new TournamentWidget( tourn );
    scrollArea->setWidget( tw );
    scrollArea->setWidgetResizable( false );
    setCentralWidget( scrollArea );
    setWindowName();
  }
}

void LeagueMainWindow::saveTournament( )
{
  QString fName = QFileDialog::getSaveFileName(this,
                  tr("Save tournament"), QDir::homePath(), 
                  tr("Tournament Files (*.trn)"));

  if ( !fName.isNull() ) {
    if ( tourn ) {
      tourn->save( fName );
      setWindowName();
    }
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
    unsigned int groups = d.groupCount();
    PlayerList players = d.players();

    tourn = new Tournament( players, cat, mtype, groups );
  
    QScrollArea* scrollArea = new QScrollArea( this );
    TournamentWidget* tw = new TournamentWidget( tourn );
    scrollArea->setWidget( tw );
    scrollArea->setWidgetResizable( false );
    setCentralWidget( scrollArea );
    setWindowName();
  }  
}

/** composes window name from program name and the current tournament name.
 *  also displays useful information about tournament ( category, for example )
 */
void LeagueMainWindow::setWindowName()
{
  QString name = progName;

  if ( tourn ) {
    name += " -- " + tourn->category();

    if ( !tourn->fileName().isEmpty() ) {
      name += " -- " + QFileInfo( tourn->fileName() ).baseName();
    }
  }

  setWindowTitle( name );
}

void LeagueMainWindow::closeEvent(QCloseEvent *event)
{
  QMessageBox::StandardButton ret =     
     QMessageBox::question( this, tr( "Closing the tournament..." ),
                            tr( "Are you sure you want to quit? " ),
                            QMessageBox::Yes | QMessageBox::No );
  if ( ret == QMessageBox::Yes ) {
    event->accept();
  } else {
    event->ignore();
  }
}

void LeagueMainWindow::showAboutDialog()
{
  AboutDialog d( this );
  d.exec();  
}

void LeagueMainWindow::showRatingsTable()
{
  if ( tourn ) {
    Group g( tr("Total"), tourn, tourn->matchList(), tourn->players() ); 
    RatingsDialog d( &g, this );
    d.exec(); 
  } 
}
