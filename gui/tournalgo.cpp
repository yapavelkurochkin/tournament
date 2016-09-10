#include "group.h"
#include "tournalgo.h"

TournAlgo::TournAlgo( TournProps p )
: _stagesCnt( 0 ),
   _props( p ) 
{
}


bool TournAlgo::stageCompleted( QList< Group*> groups ) const
{
  for ( int i = 0; i < groups.count(); i ++ ) {
    if ( groups.at( i ) && ( ! groups.at( i )->completed() ) ) {
      return false;
    }
  }
 
  return true;
}

/** \brief searches for biggest group and returns its size
 */
unsigned int TournAlgo::maxGroupSize( QList< Group* > groups ) const
{
  int max = 0;
  unsigned cnt = groups.count();
  for ( unsigned i = 0; i < cnt; i ++ ) {
    const Group* g = groups.at( i );
    if ( g->size() > max ) {
      max = g->size(); 
    }
  }

  return max;
}

