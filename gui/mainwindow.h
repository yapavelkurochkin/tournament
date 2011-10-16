#ifndef MAINWINDOW__H
#define MAINWINDOW__H

#include <QMainWindow>

class LeagueMainWindow : public QMainWindow
{
  Q_OBJECT;

  public:
    LeagueMainWindow( );

  protected:
    QAction* loadTourn;

    void createMenus();
    void createActions();

  protected slots:
    void loadTournament();
};

#endif // MAINWINDOW__H

