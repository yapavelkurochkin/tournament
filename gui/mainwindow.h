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
    QAction *loadT, *newT, *saveT, *about;
    Tournament* tourn;
    QString progName;

    void createMenus();
    void createActions();
    QAction* newAction( QString name, 
                        QKeySequence::StandardKey shortcuts, 
                        const char* slot );

  protected slots:
    void setWindowName();
    void loadTournament();
    void saveTournament();
    void newTournament();
    void showAboutDialog();
};

#endif // MAINWINDOW__H

