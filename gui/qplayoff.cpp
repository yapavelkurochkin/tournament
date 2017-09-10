#include <math.h>

#include <QDebug>

#include "qplayoff.h"
#include "swissgroup.h"
#include "tournament.h"

extern unsigned int log2( unsigned int x );

QPlayoffAlgo::QPlayoffAlgo( TournProps p )
: TournAlgo( p )
{
  // we need such number of winners in qualification for full 
  // playoff.
  unsigned qualifWinNum = p.playoffNum - p.seededNum;

  // to reach qualifWinNum we need 2x players
  _qualifNum = qualifWinNum * 2;  

  _stagesCnt = 1 /* qualification */ + log2( p.playoffNum );

  qDebug() << __FUNCTION__ << "qualifNum:" << _qualifNum << "stages:" << _stagesCnt;
  // EXAMPLE: let p.count() = 20. p.seededNum = 8.
  //              _qualifNum = 16 (4 of them = BYE), 
  //              _stagesCnt = 5
  // one more EXAMPLE: p.count() = 24, p.seededNum = 8
}

/**
 *  List of players is sorted at first.
 *  Players who are not in seeded list should play simple playoff.
 */
QList<Group*> QPlayoffAlgo::initGroups( ) const
{
  QList< Group* > groups;
  
  PlayerList players = props().players;
  qSort( players.begin(), players.end(), qGreater< Player >() );

  PlayerList pls;
  for ( unsigned int i = props().seededNum; i < props().seededNum + _qualifNum; i ++ ) { 
    if ( (int)i < players.count() ) {
      pls << players.at( i );
    } else { 
			// create 'BYE' player. this player is fake: everyone wins him and winner
			// does no earn rating.
      pls << byePlayer;
    }

    qDebug() << "qualif #" << i << ":" << pls.last().name() << pls.last().rating();
  }

  SwissGroup *sg = new SwissGroup( 1, 0, permutePlayers( pls ) );
  sg->setName( QObject::tr( "Qualification" ) );
  sg->setQualif( );

  groups << sg;

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

    SwissGroup *wg, *lg;
		wg = new SwissGroup( 1, stage, permutePlayers( toppls ) );
		lg = new SwissGroup( 1 + toppls.count(), stage, permutePlayers( botpls ) );
    lg->setQualif( prevGroups.at(0)->isQualif() );
    groups << wg << lg;

    foreach( Group *g, groups ) {
      dynamic_cast< SwissGroup* >(g)->permuteMatches( breakAlgo() );
    }

    return groups;
  }

  return QList< Group* >();
}

PlayerList QPlayoffAlgo::qualifTopResults( QList< Group* > groups ) const
{
  Q_ASSERT( groups.count() == 1 );

  PlayerList players = props().players;
  qSort( players.begin(), players.end(), qGreater< Player >() );

  Group* prev = groups[ 0 ];
  PlayerList winners = prev->winners();

  // no sort here! we should preserve winners order here 

  PlayerList toppls;
  toppls << players.mid( 0, props().seededNum );
  toppls << winners;

  return toppls; 
}

/** loosers list size can be non-2-powered ;), so, 'bye' guys should be added.
 */
PlayerList QPlayoffAlgo::qualifBotResults( QList< Group* > groups ) const
{
  Q_ASSERT( groups.count() == 1 );

  Group* prev = groups[ 0 ];
  PlayerList loosers = prev->loosers();
  loosers.removeAll( byePlayer );
  // no sort here! save winners order

  int pow2count = 1 << (int) ceil( log2( (double) loosers.count() ) );

  for ( int i = loosers.count(); i < pow2count; i ++ ) {
    loosers << byePlayer;
  }

  return loosers; 
}
  
unsigned int QPlayoffAlgo::calcMatchNum() const
{
  unsigned int ret = log2( _props.playoffNum ) * _props.playoffNum / 2;

  if ( _qualifNum ) {
    unsigned qualifWinNum = _props.playoffNum - _props.seededNum;
    ret += log2( _qualifNum ) * qualifWinNum / 2;
  }

  return ret;  
}
