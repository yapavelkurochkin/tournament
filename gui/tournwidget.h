#ifndef TOURNWIDGET__H
#define TOURNWIDGET__H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMap>
#include "rrgroup.h"
#include "grouptable.h"
#include "tournament.h"

class SwissGroup;
class SwissTable;

class TournamentWidget : public QWidget {
  Q_OBJECT;

  Tournament* _tourn;
  QList<QVBoxLayout*> _groupLayouts;
  QMap<Group*, GroupTable*> _tables; 
  /*<< one layout per group */

  void createTablesForGroups(); 
  void setupLayout( QHBoxLayout* );
  SwissTable* createSwissTable( SwissGroup* g );

protected slots:
  void syncTables();

public:
  TournamentWidget( Tournament* tourn, QWidget* parent = NULL );
  ~TournamentWidget( );
};

#endif // TOURNWIDGET__H

