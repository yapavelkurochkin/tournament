#ifndef MAINWINDOW__H
#define MAINWINDOW__H

#include <QMainWindow>
#include <QAction>

class LeagueMainWindow : public QMainWindow
{
  Q_OBJECT;

  public:
    LeagueMainWindow( );

  protected:
    QAction *loadT, *newT, *saveT;

    void createMenus();
    void createActions();
    QAction* newAction( QString name, 
                        QKeySequence::StandardKey shortcuts, 
                        const char* slot );

  protected slots:
    void loadTournament();
    void newTournament();
};

#endif // MAINWINDOW__H

