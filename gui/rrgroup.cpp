#include <QDebug>
#include <math.h>
#include "rrgroup.h"

RRGroup::RRGroup( QChar name, PlayerList players )
 : _name( name ),
   _players( players )
{
  roundRobin();
}

void RRGroup::roundRobin()
{
  _matches.clear();

  for ( int i = 0; i < _players.count(); i ++ ) {
    for ( int j = i + 1; j < _players.count(); j ++ ) {
      qDebug() << __FUNCTION__ << _players.at( i ).name() << _players.at( j ).name(); 
      _matches << Match( _players.at( i ), _players.at( j ) );
    }
  }
}

void RRGroup::addPlayer( Player player )
{
  _players << player;
  roundRobin();
}

/** Find match which corresponds to player 'a' and 'b' pair.
 */
Match& RRGroup::match( Player a, Player b )
{
  for ( int i = 0; i < _matches.count(); i ++ ) { 
    Match m = _matches.at( i );

    if ( ( m.playerA() == a ) && ( m.playerB() == b ) ) {
      return _matches[ i ];
    } else if ( ( m.playerB() == a ) && ( m.playerA() == b ) ) {
      _matches[i].swapPlayers();
      return _matches[ i ];
    } 
  }

  // should never get here!  
  qWarning() <<  "can't find match of " << a.name() << "and" << b.name();
  return (* new Match( a, b ) );
}

/** Find all matches which player 'p' is participating.
 */
MatchList RRGroup::matchList( Player p ) const
{
  MatchList ml;

  for ( int i = 0; i < _matches.count(); i ++ ) { 
    Match m = _matches.at( i );

    if ( ( m.playerA() == p ) || ( m.playerB() == p ) ) {
      ml << m;
    } 
  }
 
  return ml; 
}

MatchList RRGroup::playedMatchList( Player p ) const
{
  MatchList ml;

  for ( int i = 0; i < _matches.count(); i ++ ) { 
    Match m = _matches.at( i );

    if ( ( m.playerA() == p ) || ( m.playerB() == p ) ) {
      if ( m.played() ) {
        ml << m;
      }
    } 
  }
 
  return ml; 

}

void RRGroup::setMatchResults( Player a, Player b, QList< Game > res )
{
  Match& m = match( a, b );
  m.games().clear();
  m.games() << res;
}

/** Calculates scores for each player (who played at least one match)
 *  and returns list of results
 */
PlayerResultsList RRGroup::playersResults() const 
{
  PlayerResultsList list;
  for ( int i = 0; i < _players.count(); i ++ ) {
    Player p = _players.at( i );
    MatchList ml = playedMatchList( p );

    if ( !ml.isEmpty() ) {
      PlayerResults res( p, ml );
      list << res;
    }
  }

  return list;
}

/** \return a place of player in a group. from 1 (best) to 
 *   _players.count() (worst )
 */
unsigned int RRGroup::playerPlace( Player p ) const
{
  PlayerResultsList prl = playersResults(); 

  qSort( prl.begin(), prl.end(), qGreater< PlayerResults >() );

  for ( int i = 0; i < prl.count(); i ++ ) {
    if ( prl.at( i ).player() == p ) { 
      return ( i + 1 );
    }
  }

  return 0xdeadbeef;
}

/** Sorts player list and pushes cool players into first places
 * of groups.
 * Function assumes that 'players' is sorted list.
 */
RRGroupList breakPlayers( PlayerList players, int groupSize )
{
  int groupCnt = ceil( (double) players.count() / groupSize );
  
  RRGroupList groups;

  for ( int i = 0; i < groupCnt; i ++ ) {
    groups << RRGroup( QChar( 'A' + i ) );
  }

  while ( players.count() ) {
    for ( int i = 0; ( i < groupCnt ) && players.count(); i ++ ) {
      Player player = players.takeLast();
     
      groups[i].addPlayer( player );
    }
  }
  
  return groups;
}


