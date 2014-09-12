#ifndef MAINWINDOW__H
#define MAINWINDOW__H

#include <QMainWindow>
#include <QAction>
#include <QScrollArea>

#include "tournhist.h"
#include "tournament.h"

class TournamentWidget;

class LeagueMainWindow : public QMainWindow
{
  Q_OBJECT;

  public:
    LeagueMainWindow( );

  protected:
    QAction *loadT, *newT, *saveT, *about, 
		        *ratings, *exportT, *undoT, *redoT,
            *breakADBC, *breakACBD, *breakABCD;
  Tournament* tourn;
    QString progName;
    TournamentHistory* _history;

    void createMenus();
    void createActions();
    QAction* newAction( QString name, 
                        QKeySequence::StandardKey shortcuts, 
                        const char* slot );
    QAction* newAction( QString name, 
                        const char* slot );

    TournamentWidget* newTournamentWidget( Tournament* t );
    void closeEvent(QCloseEvent *event);

    void loadTournament( QString fName );

  protected slots:
    void pushToHistory( Tournament* t );
    void setWindowName();
    void loadTournamentInteractive();
    void saveTournament();
    void exportTournament();
    void newTournament();
    void showAboutDialog();
    void showRatingsTable();
    void undo();
    void redo();
    void saveLast();
    void selectBreakADBC( );
    void selectBreakABCD( );
    void selectBreakACBD( );
};

#endif // MAINWINDOW__H

