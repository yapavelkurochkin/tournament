#include "match.h"

/// Validates the result of a game.
// Game is valid if one player has more than 11 points
// and difference of points is more than 2.
// Valid games: 11:9, 13:11. Invalid games: 6:0, 11:10
bool GameResult::validate() const
{
  if ( ( a >= 11 ) && ( (int)(a - b) >= 2 ) ) {
    return true;
  } 

  if ( ( b >= 11 ) && ( (int)(b - a) >= 2 ) ) {
    return true;
  }

  return false;
}

GameResult& GameResult::swap( )
{
  unsigned int A = a;
  a = b;
  b = A;
  
  return (*this);
}

Match::Match( Player a, Player b, Type type )
: _a( a ),
  _b ( b ),
  _type( type )
{
}

Player Match::won() const 
{
  return ( aScores() > bScores() ) ? _a : _b;
}

int Match::aScores() const 
{
  int a_scores = 0; 
  
  for ( int i = 0; i < _results.count(); i ++ ) {
    GameResult res = _results.at( i );
    if ( res.a > res.b ) {
      a_scores ++;
    } 
  }

  return a_scores;
}
 
int Match::bScores() const 
{
  return _results.count() - aScores();
}

bool Match::validate() const
{
  if ( ( aScores() + bScores() ) > maxGames() ) {
    return false;
  }

  if ( aScores() == bScores() ) {
    return false;
  }

  if ( _type == BestOf3 ) {
    if ( ( aScores() == 2 ) || ( bScores() == 2 ) ) {
      return true;
    }
  }
  
  if ( _type == BestOf5 ) {
    if ( ( aScores() == 3 ) || ( bScores() == 3 ) ) {
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

/** Prints results of match in a multiline string */
QString Match::resultsAsString( ) const
{
  QString ret;
  for ( int i = 0; i < _results.count(); i ++ ) {
    if ( i > 0 )
      ret += "\n";

    GameResult game = _results.at( i );
    ret += QString::number( game.a );
    ret += " : "; 
    ret += QString::number( game.b );
  }

  return ret;
}

QString Match::scoresAsString() const 
{
  return QString::number( aScores() ) + " : " + QString::number( bScores() );
}
