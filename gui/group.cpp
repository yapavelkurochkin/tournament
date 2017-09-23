#include <QDebug>
#include <QJsonArray>

#include "tournament.h"
#include "group.h"

Group::Group( QString name, unsigned int stage, PlayerList players )
 : _name( name ),
   _players( players ),
   _tournData( NULL ),
   _stage( stage )
{
}

Group::Group( QString name, 
               MatchList matches, PlayerList players )
 : _name( name ),
   _players( players ),
   _matches( matches ),
   _tournData( NULL ),
   _stage( 0 )
{
}

void Group::addPlayer( Player player )
{
  _players << player;
}

/** removes player from players list and removes
 *  all matches which he/she had participated
 */
void Group::removePlayer( Player player )
{
  _players.removeOne( player );
  QMutableListIterator<Match> i( _matches );
  while( i.hasNext() ) {
    Match m = i.next();
    if ( ( m.playerA() == player ) ||
         ( m.playerB() == player ) ) {
      i.remove();
    }
  }
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

    if ( m.participated( p ) ) {
      ml << m;
    } 
  }
 
  return ml; 
}

MatchList Group::matchList( Player a, Player b ) const
{
  MatchList ml;

  for ( int i = 0; i < _matches.count(); i ++ ) { 
    Match m = _matches.at( i );

    if ( m.participated( a ) && m.participated( b ) ) {
      if ( !( m.playerA() == a ) ) {
        // right player order required
        m.swapPlayers();
      }
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

  if ( _tournData ) {
     _tournData->groupChanged( this );
  } else {
    qWarning( "_tournData undefined" );
  }
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

/** \return earned rating for 'p' player. i.e. the sum of
 *          earned points per each match.
 */
double Group::earnedRating( Player p ) const
{
  double total = 0.0;
  for ( int i = 0; i < _matches.count(); i++) {
    Match m = _matches.at( i );

    if ( m.participated( p ) && m.played() ) {
      total += m.earnedRating( p );
    }
  }

  return total;
}

/** \return list of players who won at least 1 match
 */
PlayerList Group::winners() const
{
  PlayerList list;
  for ( int i = 0; i < _matches.count(); i++) {
    list << _matches.at( i ).winner();
  }

  return list;
}

/** \return list of players who lost at least 1 match.
 *          'bye' players are included in list
 */
PlayerList Group::loosers() const
{
  PlayerList list;
  for ( int i = 0; i < _matches.count(); i++) {
    Player p = _matches.at( i ).looser();
    list << p;
  }

  return list;
}

/** \return same as const_players(), but without BYE players.
 */
PlayerList Group::const_validPlayers() const
{ 
  PlayerList l = const_players();
  l.removeAll( byePlayer );
  return l;
}

/** \brief returns true if FIRST match is marked as qualification.
 *         normally all matches should have same flags (qualif or !qualif).
 */
bool Group::isQualif() const
{
  if ( _matches.count() ) {
    return _matches.at( 0 ).isQualif();
  } else {
    return false; 
  }
}

/** marks all matches with qualification flag.
 */
void Group::setQualif( bool q )
{
  for ( int i = 0; i < _matches.count(); i++ ) {
    _matches[ i ].setQualif( q );
  } 
}

/** export results to json
 */
void Group::write( QJsonObject &json ) const
{
  json["name"] = name();
  json["stage"] = (int)stage();
  json["type"] = type();
  json["qualif"] = isQualif();

  QJsonArray mArray;
  foreach( Match m, _matches ) {
    QJsonObject mObj;
    m.write( mObj );
    mArray.append( mObj );
  }
  
  json["matches"] = mArray; 

  QJsonArray mPlayersArray;
  foreach( Player p, _players ) {
      QJsonObject mObj;

      p.write( mObj );
      mPlayersArray.append( mObj );
  }

  json["players"] = mPlayersArray;
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
 
