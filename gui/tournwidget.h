#ifndef TOURNWIDGET__H
#define TOURNWIDGET__H

#include <QWidget>
#include <QHBoxLayout>
#include "rrgroup.h"
#include "tournament.h"

class TournamentWidget : public QWidget {
  Tournament* _tourn;
  QHBoxLayout* _hLayout;

  void addGroupsToLayout( QVBoxLayout* vl );
public:
  TournamentWidget( Tournament* tourn, QWidget* parent = NULL );
};

#endif // TOURNWIDGET__H

