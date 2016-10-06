#include "group.h"
#include "tournalgo.h"

TournAlgo::TournAlgo( TournProps p )
: _stagesCnt( 0 ),
   _props( p ),
   _break( BreakAlgo::ADBC ) 
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

/** \brief rebuild player list accordingly with break algorithm. 
 *         assumed that pls.count() is even.
 */
PlayerList TournAlgo::permutePlayers( PlayerList pls ) const
{
  PlayerList out;
  
  if ( _break == BreakAlgo::ADBC ) {
    // first - last
    // second - pre-last
		for ( int i = 0; i < pls.count() / 2; i ++ ) {
			out << pls.at( i ) << pls.at( pls.count() - i - 1 );
		}
  } else if ( _break == BreakAlgo::ACBD ) {
    // first - middle
    // second - middle + 1
		for ( int i = 0; i < pls.count() / 2; i ++ ) {
			out << pls.at( i ) << pls.at( pls.count() / 2 + i );
		}
  } else {
    // ABCD does not require permutation
  }

  return out;
}
 
