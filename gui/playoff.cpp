#include <QDebug>
#include "playoff.h"
#include "swissgroup.h"
#include "tournament.h"

extern unsigned int log2( unsigned int x );

PlayoffAlgo::PlayoffAlgo( TournProps p )
: TournAlgo( p )
{
  if ( (unsigned int) ( props().players.count() ) < props().playoffNum ) {
    unsigned int i = 0,
		 n = props().playoffNum - props().players.count();
    for ( i = 0; i < n; i ++ ) {
      _props.players << byePlayer;
    }
  }
  
  _stagesCnt = log2( p.playoffNum );

  qDebug() << __FUNCTION__ << "stages:" << _stagesCnt;
}

/**
 *  List of players is sorted at first.
 *  Players who are not in seeded list should play simple playoff.
 *  Intended, that _qualifNum is power of 2.
 */
QList<Group*> PlayoffAlgo::initGroups( ) const
{
  QList< Group* > groups;
  
  PlayerList players = props().players;
  qSort( players.begin(), players.end(), qGreater< Player >() );

  SwissGroup *sg = new SwissGroup( 1, 0, permutePlayers( players ) );
  sg->setQualif( false );
  sg->permuteMatches( breakAlgo() );  

  groups << sg;

  return groups;
}

/** build groups by results of previous stage 
 */
QList<Group*> PlayoffAlgo::buildGroups( unsigned int stage, 
                                          QList<Group*>  ) const
{
  if ( stage >= stagesCnt() ) {
    return QList< Group* >();
  }

  if ( stage == 0 ) {
    return initGroups( );
  }

  return QList< Group* >();
}

unsigned int PlayoffAlgo::calcMatchNum(  ) const
{
  PlayerList pl = props().players;
  pl.removeAll( byePlayer );
  return ( (int)(pl.count() / 2) ) * log2( props().playoffNum );
}
