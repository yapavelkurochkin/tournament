#include <QDebug>

#include "tournament.h"
#include "group.h"

Group::Group( QString name, Tournament* t, unsigned int stage, PlayerList players )
 : _name( name ),
   _players( players ),
   _tournament( t ),
   _stage( stage )
{
}

void Group::addPlayer( Player player )
{
  _players << player;
}

/** Find match which corresponds to player 'a' and 'b' pair.
 */
Match& Group::match( Player a, Player b )
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
MatchList Group::matchList( Player p ) const
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

MatchList Group::playedMatchList( Player p ) const
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

void Group::setMatchResults( Player a, Player b, QList< Game > res )
{
  Match& m = match( a, b );
  m.games().clear();
  m.games() << res;

  _tournament->groupChanged( this );
}

void Group::setMatchResults( Match m )
{
  setMatchResults( m.playerA(), m.playerB(), m.games_const() );
}

/** Calculates scores for each player (who played at least one match)
 *  and returns list of results
 */
PlayerResultsList Group::playersResults() const 
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

PlayerResults Group::playerResults( Player p ) const
{
  PlayerResultsList list = playersResults();
  for ( int i = 0; i < list.count(); i ++ ) {
    if ( list.at( i ).player() == p ) {
      return list.at( i );
    }
  } 

  return PlayerResults( p );
}

/** \return a place of player in a group. from 1 (best) to 
 *   _players.count() (worst )
 */
unsigned int Group::playerPlace( Player p ) const
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

/** reverse to playerPlace()
 */
Player Group::playerByPlace( unsigned int place ) const
{
  PlayerResultsList prl = playersResults(); 

  qSort( prl.begin(), prl.end(), qGreater< PlayerResults >() );

  for ( int i = 0; i < prl.count(); i ++ ) {
    if ( ( i + 1 ) == (int)place ) {
      return prl.at( i ).player();
    }
  }

  return Player();
}

/** \return true if all group matches where played. 
 */
bool Group::completed() const
{
  for ( int i = 0; i < _matches.count(); i++) {
    if ( ! _matches.at( i ).played() ) {
      return false;
    }
  }

  return true;
}

/** Serialization operators
  */
QDataStream &operator<<( QDataStream &s, const Group &g )
{
  s << g._name;
  s << g._players;
  s << g._matches;
  s << g._stage;

  return s;
}

QDataStream &operator>>( QDataStream &s, Group &g )
{
  s >> g._name;
  s >> g._players;
  s >> g._matches;
  s >> g._stage;
  
  return s;
}
