#include <QDebug>
#include "rrplayoff.h"
#include "rrgroup.h"
#include "swissgroup.h"
#include "tournament.h"

extern unsigned int log2( unsigned int x );

QPlayoffAlgo::QPlayoffAlgo( TournProps p )
: TournAlgo( p )
{
  _qualifNum = p.players.count() - p.seededNum;
  
  // round qualifNum to the nearest power of 2
  unsigned int l2 = log( _qualifNum );
  _qualifNum = ( ( 1 << l2 ) < _qualifNum ) ? ( 1 << ( l2 + 1 ) ) : 1 << l2;  
 
  _stagesCnt = 1 /* qualification */ + log2( p.seededNum + _qualifNum / 2 );

  // EXAMPLE: let p.count() = 20. p.seededNum = 8.
  //              _qualifNum = 16 (4 of them = BYE), 
  //              _stagesCnt = 5
}

/**
 *  List of players is sorted at first.
 *  Players who are not in seeded list should play simple playoff.
 *  Intended, that _qualifNum is power of 2.
 */
QList<Group*> QPlayoffAlgo::initGroups( ) const
{
  QList< Group* > groups;
  
  PlayerList players = props().players;
  qSort( players );

  PlayerList pls;
  for ( i = p.seededNum; i < _qualifNum; i ++ ) { 
    if ( i < p.players.count() ) {
      pls << p.players.at( i );
    } else { 
			// create 'BYE' player. this player is fake: everyone wins him and winner
			// does no earn rating.
      pls << Player( "BYE", 0.0 ); 
    }
  }
 
  // TODO: 1st should play with last. 2nd with pre-last.
 
  groups << new SwissGroup( 0, 0, pls );

  return groups;
}

/** build groups by results of previous stage 
 */
QList<Group*> QPlayoffAlgo::buildGroups( unsigned int stage, 
                                          QList<Group*> prevGroups ) const
{
  if ( stage >= stagesCnt() ) {
    return QList< Group* >();
  }
  if ( stage == 0 ) {
    return initGroups( );
  }

  if ( stage == 1 ) { // next to qualification stage
    QList< Group* > groups;

    PlayerList toppls = qualifTopResults( prevGroups );
    PlayerList botpls = qualifBotResults( prevGroups );

		qDebug() << __PRETTY_FUNCTION__ << "players count:", players.count();

		groups << new SwissGroup( 1, stage, toppls );
		groups << new SwissGroup( 1 + toppls.count(), stage, botpls );

    return groups;
  }

  return QList< Group* >();
}

PlayerList QPlayoffAlgo::qualifTopResults( QList< Group* > groups ) const
{
  Q_ASSERT( groups.count() == 1 );

  PlayerList players = props().players;
  qSort( players );

  Group* prev = groups[ 0 ];
  PlayerList winners = prev->winners();
  qSort( winners );

  PlayerList toppls;
  toppls << players.mid( 0, props().seededNum );
  toppls << winners;

  return toppls; 
}

PlayerList QPlayoffAlgo::qualifBotResults( QList< Group* > groups ) const
{
  Q_ASSERT( groups.count() == 1 );

  Group* prev = groups[ 0 ];
  PlayerList loosers = prev->loosers();
  qSort( winners );

  // FIXME: should I remove 'BUY' players from list?

  return loosers; 
}
