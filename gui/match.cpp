#include "match.h"
#include "../rating/rating.h"

/*****************************
 * Game                      *
 ****************************/

/// Validates the result of a game.
// Game is valid if one player has more than 11 points
// and difference of points is more than 2.
// Valid games: 11:9, 13:11. Invalid games: 6:0, 11:10
bool Game::validate() const
{
  if ( a.isBye() || b.isBye() ) {
    return true;
  }

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

/** \return winner player if known or Undefined player with 
 *          name = nameA + "/" + nameB
 */
Player Game::winner() const
{
  if ( played() ) {
    if ( a.isBye() ) {
      return b;
    } else if ( b.isBye() ) {
      return a;
    } else {
      return ( aBalls > bBalls ) ? a : b;
    }
  } else {
    return Player::mkUndefinedPlayer( a, b );
  }
}

// number of balls won by player
unsigned int Game::balls( Player p ) const
{
  return ( p == a ) ? aBalls : bBalls;
}

// number of balls won by an alternate player
unsigned int Game::lostBalls( Player p ) const
{
  return ( p == a ) ? bBalls : aBalls;
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
Match::Match( Player a, Player b )
: _a( a ),
  _b ( b ),
  _qualif( false )
{
}

Player Match::winner() const 
{
  if ( played() ) {
    if ( _a.isBye() ) {
      return _b;
    } else if ( _b.isBye() ) {
      return _a;
    } else {
      return ( gamesWon( _a ) > gamesWon( _b ) ) ? _a : _b;
    }
  } else {
    return Player::mkUndefinedPlayer( _a, _b );
  }
}

Player Match::looser() const 
{
  if ( played ) {
    return ( winner() == _a ) ? _b : _a;
  } else {
    return Player::mkUndefinedPlayer( _a, _b );
  }
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
  if ( _a.isBye() || _b.isBye() ) {
    return true; 
  }

  unsigned int aGames = gamesWon( _a );
  unsigned int bGames = gamesWon( _b );
  unsigned int totalGames = aGames + bGames;

  if ( totalGames > maxGames() ) {
    return false;
  }

  // it does not matter whether it bestOf3 or bestOf5
  return ( totalGames >= 2 ) && ( aGames != bGames );
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
  if ( isBye() ) {
    return "";
  } else {
    return QString::number( gamesWon( _a ) ) 
               + " : " + QString::number( gamesWon( _b ) );
  }
}

// one of players is fake 'BYE' man
// \sa Player::isBye()
bool Match::isBye() const
{ 
  return ( _a.isBye() || _b.isBye() );
}

/** \return opponent of player.
 */
Player Match::opponent( Player p ) const
{
  return ( p == playerA() ) ? playerB() : playerA();
}

/** calculates earned rating points of player p relatively
 *  to opponent player. 
 */
double Match::earnedRating( Player p ) const
{
  if ( played() && !isBye() && !isQualif() ) {
    return calcEarnedRating( p.rating(), opponent( p ).rating(),
                             gamesWon( p ), gamesWon( opponent( p ) ) ); 
  } else {
    return 0.0;
  }
}

/* serialization operators
 */
QDataStream &operator >> ( QDataStream &s, Match &m )
{
  s >> m._a >> m._b >> m._results >> m._qualif;
  return s;
}

QDataStream &operator << ( QDataStream &s, const Match &m )
{
  s << m._a << m._b << m._results << m._qualif;
  return s;
}

