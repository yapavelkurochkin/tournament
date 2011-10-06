#include "match.h"

/*****************************
 * Game                      *
 ****************************/

/// Validates the result of a game.
// Game is valid if one player has more than 11 points
// and difference of points is more than 2.
// Valid games: 11:9, 13:11. Invalid games: 6:0, 11:10
bool Game::validate() const
{
  if ( ( aBalls >= 11 ) && ( (int)(aBalls - bBalls) >= 2 ) ) {
    return true;
  } 

  if ( ( bBalls >= 11 ) && ( (int)(bBalls - aBalls) >= 2 ) ) {
    return true;
  }

  return false;
}

Game& Game::swap( )
{
  unsigned int ab = aBalls;
  aBalls = bBalls;
  bBalls = ab;
 
  Player A = a;
  a = b;
  b = A;
 
  return (*this);
}

Player Game::winner() const
{
  return ( aBalls > bBalls ) ? a : b;
}

unsigned int Game::balls( Player p ) const
{
  return ( p == a ) ? aBalls : bBalls;
}

QDataStream &operator>>(QDataStream &s, Game &g)
{
  s >> g.a >> g.b >> g.aBalls >> g.bBalls;
  return s;
}

QDataStream &operator<<(QDataStream &s, const Game &g)
{
  s << g.a << g.b << g.aBalls << g.bBalls;
  return s;
}

/******************************
 * Match                      *
 *****************************/
Match::Match( Player a, Player b, Type type )
: _a( a ),
  _b ( b ),
  _type( type )
{
}

Player Match::winner() const 
{
  return ( gamesWon( _a ) > gamesWon( _b ) ) ? _a : _b;
}

unsigned int Match::gamesWon( Player p ) const 
{
  unsigned int games = 0; 
  
  for ( int i = 0; i < _results.count(); i ++ ) {
    Game res = _results.at( i );
    if ( res.winner() == p ) { 
      games ++;
    } 
  }

  return games;
}
 
bool Match::validate() const
{
  unsigned int aGames = gamesWon( _a );
  unsigned int bGames = gamesWon( _b );

  if ( ( aGames + bGames ) > maxGames() ) {
    return false;
  }

  if ( aGames == bGames ) {
    return false;
  }

  if ( _type == BestOf3 ) {
    if ( ( aGames == 2 ) || ( bGames == 2 ) ) {
      return true;
    }
  }
  
  if ( _type == BestOf5 ) {
    if ( ( aGames == 3 ) || ( bGames == 3 ) ) {
      return true;
    }
  }

  return false;
}

Match& Match::swapPlayers()
{
  for ( int i = 0; i < _results.count(); i ++ ) {
    _results[i].swap();
  }

  Player A = _a;
  _a = _b;
  _b = A;

  return (*this);
}

/** Prints results of games in a multiline string */
QString Match::gamesToString( ) const
{
  QString ret;
  for ( int i = 0; i < _results.count(); i ++ ) {
    if ( i > 0 )
      ret += "\n";

    Game game = _results.at( i );
    ret += QString::number( game.aBalls );
    ret += " : "; 
    ret += QString::number( game.bBalls );
  }

  return ret;
}

QString Match::toString() const 
{
  return QString::number( gamesWon( _a ) ) 
             + " : " + QString::number( gamesWon( _b ) );
}

/* serialization operators
 */
QDataStream &operator >> ( QDataStream &s, Match &m )
{
  qint32 type;
  s >> m._a >> m._b >> m._results >> type;
  m._type = (Match::Type) type;
  return s;
}

QDataStream &operator << ( QDataStream &s, const Match &m )
{
  qint32 type = (Match::Type) m._type;
  s << m._a << m._b << m._results << type;
  return s;
}

