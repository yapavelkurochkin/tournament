#include <QScrollArea>
#include <QDebug>

#include "tournwidget.h"
#include "rrtable.h"
#include "swissgroup.h"
#include "swisstable.h"

/** widget contains all tables (round-robin, swiss)
 *  and enables to dinamically add group tables.
 *  removing of groups are not neccessary.
 */
TournamentWidget::TournamentWidget( Tournament* tourn, QWidget* parent )
 : QWidget( parent ),
   _tourn( tourn )
{
  QHBoxLayout* hLayout = new QHBoxLayout();
  setLayout( hLayout );

  setupLayout( hLayout );

  createTablesForGroups( );

  connect( tourn, SIGNAL( tournamentChanged( Tournament * ) ),
           this, SLOT( syncTables(  ) ) );
}

TournamentWidget::~TournamentWidget()
{
  delete _tourn;
}

/** \brief Looks up new tables in tournament and adds a table for new one to
 *         layout. 
 */
void TournamentWidget::syncTables( )
{
  for ( unsigned int gr = 0; gr < _tourn->algo_const()->stagesCnt(); gr ++ ) {
    // todo: may be groupList should return sorted list?
    QList< Group* > groups = _tourn->data_const()->groupList( gr );

    for ( int i = 0; i < groups.count(); i ++ ) {
      Group* g = groups.at( i );

      if ( !_tables.contains( g ) ) {
        // found new group in tournament
        SwissGroup *sg = dynamic_cast< SwissGroup* >( g );
        if ( sg ) {
          _tables[ g ] = createSwissTable( sg );
        } else {
          // TODO: now only swiss groups are created dynamically, but if you read
          //       this, maybe that's not true.
        }
      }
    }
  }
}

/** initializes necessary number of columns in horizontal layout
 *  and adds vertical layout into each column.
 *  column count = number of stages in tournament
 */
void TournamentWidget::setupLayout( QHBoxLayout* hLayout )
{
  hLayout->setSizeConstraint( QLayout::SetMinimumSize );

  for ( unsigned int i = 0; i < _tourn->algo_const()->stagesCnt(); i ++ ) {
    QVBoxLayout* l = new QVBoxLayout( ); 
    l->setSizeConstraint( QLayout::SetMinimumSize );

    hLayout->addLayout( l );

    _groupLayouts << l;
  }
}

/** scans Tournament for all groups in each stage, creates
 *  proper table and inserts it to corresponding group layout
 */
void TournamentWidget::createTablesForGroups( )
{
  for ( unsigned int gr = 0; gr < _tourn->algo_const()->stagesCnt(); gr ++ ) {
    // todo: may be groupList should return sorted list?
    QList< Group* > groups = _tourn->data_const()->groupList( gr );

    if ( gr > 0 ) {
      // TODO: 0th stage can contain only swiss groups!
      // there are swiss groups in list
      qSort( groups.begin(), groups.end(), SwissGroup::lessThan );
    }

    for ( int i = 0; i < groups.count(); i ++ ) {
      GroupTable *t;
      Group *g = groups.at( i );
      SwissGroup *sg = dynamic_cast< SwissGroup* >( g );
      RRGroup *rrg = dynamic_cast< RRGroup* >( g );
      if ( rrg ) {
        t = new RRTable( g, this );
      } else if ( sg ) {
        t = new SwissTable( g, this );
      } else {
        t = NULL;
        Q_ASSERT( t );
      }
      
      _groupLayouts.at( gr )->addWidget( t );
      _tables[ g ] = t;
    }
  }
}

/** creates table for group and inserts it to layout.
 */
SwissTable* TournamentWidget::createSwissTable( SwissGroup* g )
{
  SwissTable* newst = new SwissTable( g, this );
  
  // because of swiss group creation context we assume that g->stage()
  // is more than 0.
  QVBoxLayout* l = _groupLayouts.at( g->stage() );

  // ensuring that groups are sorted in proper order!
  // otherwise, games for 1-8 places can be inserted into 
  // layout after games for 9-15
  int i = 0;
  for ( i = 0; i < l->count(); i ++ ) {
    QWidget* w = l->itemAt( i )->widget();
    SwissTable* st = dynamic_cast< SwissTable* >( w );
    if ( st ) {
      if ( SwissGroup::lessThan( newst->group(), st->group() ) )
        break;
    }
  }

  l->insertWidget( i, newst ); 
  return newst;
}

