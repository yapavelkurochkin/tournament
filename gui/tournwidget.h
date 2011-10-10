#ifndef TOURNWIDGET__H
#define TOURNWIDGET__H

#include <QWidget>
#include <QHBoxLayout>
#include "rrgroup.h"
#include "tournament.h"

class SwissGroup;

class TournamentWidget : public QWidget {
  Q_OBJECT;

  Tournament* _tourn;
  QHBoxLayout* _hLayout;

  void addGroupsToLayout( QVBoxLayout* vl );

protected slots:
  void newSwissGroupCreated( SwissGroup* g );

public:
  TournamentWidget( Tournament* tourn, QWidget* parent = NULL );
};

#endif // TOURNWIDGET__H

