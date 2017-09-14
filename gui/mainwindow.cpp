#include <QMenu>
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTemporaryFile>
#include <QDir>
#include <QDebug>
#include <math.h>

#include "mainwindow.h"
#include "tournwidget.h"
#include "newtourndialog.h"
#include "ratingsdialog.h"
#include "rrplayoff.h"
#include "about.h"

LeagueMainWindow::LeagueMainWindow()
: QMainWindow(),
  tourn( NULL ),
  _history( new TournamentHistory() )
{
  progName = tr( "Tournament" );
  setWindowTitle( progName );
  createActions();
  createMenus();
 
  connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( saveLast() ) );
  loadTournament( QDir::toNativeSeparators( QDir::homePath() + "/lasttourn.dat" ) );
}

void LeagueMainWindow::createActions()
{
  loadT = newAction( tr("&Load"), QKeySequence::Open, 
                         SLOT( loadTournamentInteractive() ) );
  newT = newAction( tr("&New"), QKeySequence::New, 
                         SLOT( newTournament() ) );
  saveT = newAction( tr("&Save"), QKeySequence::Save,
                         SLOT( saveTournament() ) );
  exportT = newAction( tr("&Export to CSV"), QKeySequence::UnknownKey,
                         SLOT( exportTournament() ) );
  jsonT   = newAction( tr("&Export to JSON"), QKeySequence::UnknownKey,
                         SLOT( exportTournamentToJson() ) );
  ratings = newAction( tr("&Ratings"), 
                         SLOT( showRatingsTable() ) );
  about = newAction( tr("&About"), QKeySequence::HelpContents,
                         SLOT( showAboutDialog() ) );
  undoT= newAction( tr("&Undo"), QKeySequence::Undo,
	                       SLOT( undo() ) );
  redoT= newAction( tr("&Redo"), QKeySequence::Redo,
	                       SLOT( redo() ) );
  breakADBC = newAction( "A-D, B-C",
	                       SLOT( selectBreakADBC() ) );
  breakABCD = newAction( "A-B, C-D",
	                       SLOT( selectBreakABCD() ) );
  breakACBD = newAction( "A-C, B-D",
	                       SLOT( selectBreakACBD() ) );

  QList< QAction* > actions;
  actions << breakADBC << breakABCD << breakACBD;
  QActionGroup *ag = new QActionGroup( this );
  ag->setExclusive( true ); 
  
  foreach( QAction *a, actions ) {
    a->setCheckable( true );
    ag->addAction( a );
  }
  
  breakADBC->setChecked( true );
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
  fileMenu->addAction( newT );
  fileMenu->addAction( loadT );
  fileMenu->addAction( saveT );
  fileMenu->addSeparator();
  fileMenu->addAction( exportT );
  fileMenu->addAction( jsonT );
  fileMenu->addSeparator();
  fileMenu->addAction( ratings );

  QMenu* iMenu = menuBar()->addMenu(tr("&Information"));
  iMenu->addAction( about );

  QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction( undoT );
	editMenu->addAction( redoT );

  QMenu* bmMenu = menuBar()->addMenu(tr("&Breaking"));
  bmMenu->addAction( breakADBC );
  bmMenu->addAction( breakACBD );
  bmMenu->addAction( breakABCD );
}

void LeagueMainWindow::loadTournament( QString fName )
{
  if ( !fName.isNull() ) {
    tourn = Tournament::fromFile( fName );
    if ( tourn && tourn->isValid() ) {
      _history->reset( tourn );
      newTournamentWidget( tourn );

      if ( tourn->algo_const() ) { 
				switch ( tourn->algo_const()->breakAlgo() ) {
					case BreakAlgo::ABCD: breakABCD->setChecked( true ); break;
					case BreakAlgo::ACBD: breakACBD->setChecked( true ); break;
					case BreakAlgo::ADBC: breakADBC->setChecked( true ); break;
				}
      }
    }
  }
}

void LeagueMainWindow::loadTournamentInteractive( )
{
  QString fName = QFileDialog::getOpenFileName(this,
                  tr("Open tournament"), "", 
                  tr("Tournament Files (*.trn)"));

  loadTournament( fName );
}

/** creates new tournament widget. old widget is destroyed automatically
 *  by QScrollArea
 */
TournamentWidget* LeagueMainWindow::newTournamentWidget( Tournament *t )
{
  QScrollArea* scrollArea = new QScrollArea( this );
  TournamentWidget* tw = new TournamentWidget( t );
  scrollArea->setWidget( tw );
  scrollArea->setWidgetResizable( false );
  setCentralWidget( scrollArea );
  setWindowName();

	connect( t, SIGNAL( tournamentChanged( Tournament* ) ),
	         this, SLOT( pushToHistory( Tournament*) ) );

	return tw;
}

void LeagueMainWindow::saveTournament( )
{
  QString fName = QFileDialog::getSaveFileName(this,
                  tr("Save tournament"), "", 
                  tr("Tournament Files (*.trn)"));

  if ( !fName.isNull() ) {
    if ( tourn ) {
      tourn->save( fName );
      setWindowName();
    }
  }
}

void LeagueMainWindow::exportTournament( )
{
  QString fName = QFileDialog::getSaveFileName(this,
                  tr("Export tournament results"), "", 
                  tr("CSV Files (*.csv)"));

  if ( !fName.isNull() ) {
    if ( tourn ) {
      tourn->saveAsCSV( fName );
    }
  }
}

void LeagueMainWindow::exportTournamentToJson( )
{
  QString fName = QFileDialog::getSaveFileName(this,
                  tr("Export tournament results"), "", 
                  tr("JSON Files (*.json)"));

  if ( !fName.isNull() ) {
    if ( tourn ) {
      tourn->saveAsJson( fName );
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
    TournProps p = d.tournProps();
    
    tourn = new Tournament( p );
    _history->reset( tourn );
    
    newTournamentWidget( tourn );
  }  
}

/** composes window name from program name and the current tournament name.
 *  also displays useful information about tournament ( category, for example )
 */
void LeagueMainWindow::setWindowName()
{
  QString name = progName;

  if ( tourn ) {
    name += " -- " + tourn->algo_const()->props().category;

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
    Group g( tr("Total"), tourn->data_const()->matchList(), 
                          tourn->data_const()->playerList() ); 
    RatingsDialog d( &g, this );
    d.exec(); 
  } 
}

void LeagueMainWindow::undo( )
{
  Tournament* t = _history->reward();
	if ( t ) {
    tourn = t;
		newTournamentWidget( tourn ); 
  }
}

void LeagueMainWindow::redo( )
{
  Tournament* t = _history->forward();
	if ( t ) {
    tourn = t;
		newTournamentWidget( tourn ); 
  }
}

void LeagueMainWindow::pushToHistory( Tournament* t )
{
  _history->push( t );
}

void LeagueMainWindow::saveLast()
{
  if ( tourn ) {
    tourn->save( QDir::toNativeSeparators( QDir::homePath() + "/lasttourn.dat" ) );
  } 
}

void LeagueMainWindow::selectBreakADBC( )
{ 
  if ( tourn && tourn->algo() ) {
    tourn->algo()->setBreakAlgo( BreakAlgo::ADBC); 
  } 
}

void LeagueMainWindow::selectBreakABCD( )
{ 
  if ( tourn && tourn->algo() ) {
    tourn->algo()->setBreakAlgo( BreakAlgo::ABCD ); 
  } 
}

void LeagueMainWindow::selectBreakACBD( )
{ 
  if ( tourn && tourn->algo() ) {
    tourn->algo()->setBreakAlgo( BreakAlgo::ACBD ); 
  } 
}
