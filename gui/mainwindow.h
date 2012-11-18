#ifndef MAINWINDOW__H
#define MAINWINDOW__H

#include <QMainWindow>
#include <QAction>
#include <QScrollArea>

#include "tournament.h"

class LeagueMainWindow : public QMainWindow
{
  Q_OBJECT;

  public:
    LeagueMainWindow( );

  protected:
    QAction *loadT, *newT, *saveT, *about, *ratings, *exportT;
    Tournament* tourn;
    QString progName;

    void createMenus();
    void createActions();
    QAction* newAction( QString name, 
                        QKeySequence::StandardKey shortcuts, 
                        const char* slot );
    QAction* newAction( QString name, 
                        const char* slot );

    void closeEvent(QCloseEvent *event);
  protected slots:
    void setWindowName();
    void loadTournament();
    void saveTournament();
    void exportTournament();
    void newTournament();
    void showAboutDialog();
    void showRatingsTable();
};

#endif // MAINWINDOW__H

