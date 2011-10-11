#ifndef TOURNWIDGET__H
#define TOURNWIDGET__H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "rrgroup.h"
#include "tournament.h"

class SwissGroup;

class TournamentWidget : public QWidget {
  Q_OBJECT;

  Tournament* _tourn;
  QList<QVBoxLayout*> _groupLayouts;
  /*<< one layout per group */

  void createTablesForGroups(); 
  void setupLayout( QHBoxLayout* );

protected slots:
  void newSwissGroupCreated( SwissGroup* g );

public:
  TournamentWidget( Tournament* tourn, QWidget* parent = NULL );
};

#endif // TOURNWIDGET__H

