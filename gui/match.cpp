#include "match.h"

Match::Match( Player a, Player b )
: _a( a ),
  _b ( b )
{
}

void Match::addGameResult( GameResult res )
{
  _results << res;
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
