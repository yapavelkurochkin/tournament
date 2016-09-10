#include <QList>
#include "tourndata.h"
#include "tournalgo.h"
#include "group.h"

TournData::TournData( const TournAlgo *algo  )
:_algo( algo )
{
  if ( _algo ) {
    _groups = new QList<Group*>[ _algo->stagesCnt() ];
    Q_CHECK_PTR( _groups );
  }
}

/** build actual player list. NOTE, that playerlist can be changed
 *  by user (rrtable allows to add or remove user via menu).
 *  that is why Tournament class does not store player list but builds it 
 *  on demand.
 *
 * \todo this function should be used for synchonization between actual list
 *       of players and initial list passed as TournProps
 */
PlayerList TournData::playerList() const
{
  PlayerList list;
  for ( int i = 0; i < _groups[0].count(); i ++ ) {
    const Group* g = _groups[0].at( i );
    list << g->const_players();
	}
   
  return list;
}

/** \return total matches list
 */
MatchList TournData::matchList( int stage ) const 
{
  MatchList ml;
  if ( stage >= 0 ) {
    for ( int j = 0; ( stage < _algo->stagesCnt() ) && ( j < _groups[ stage ].count() ); j ++ ) {
      ml << _groups[ stage ].at( j )->matchList();
    }   
  } else { 
		for ( unsigned int i = 0; i < _algo->stagesCnt(); i ++ ) {
			QList<Group*> gl = _groups[ i ];
			for ( int j = 0; j < gl.count(); j ++ ) {
				ml << gl.at( j )->matchList();
			}   
		}
  }
  return ml;
}

