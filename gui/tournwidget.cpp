#include <QScrollArea>
#include <QDebug>
#include "tournwidget.h"
#include "rrtable.h"

TournamentWidget::TournamentWidget( Tournament* tourn, QWidget* parent )
 : QWidget( parent ),
   _tourn( tourn ),
   _hLayout( new QHBoxLayout() )
{
  setLayout( _hLayout );

  QVBoxLayout* groupLayout = new QVBoxLayout();
  groupLayout->setSizeConstraint( QLayout::SetMinAndMaxSize ); 

  _hLayout->addLayout( groupLayout );

  addGroupsToLayout( groupLayout );
}


void TournamentWidget::addGroupsToLayout( QVBoxLayout* vl )
{
  RRGroupList& groups = _tourn->groupList();
  for ( int i = 0; i < groups.count(); i ++ ) {
    RRTable* rt = new RRTable( &groups[ i ], this );
    vl->addWidget( rt );
  }
}
