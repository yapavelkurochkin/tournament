#include <QDebug>
#include "playerscores.h"

PlayerResults::PlayerResults( Player p )
: _player( p ),
  _scores( )
{
}

PlayerResults::PlayerResults( Player p, MatchList matches )
: _player( p ),
  _scores( )
{
  addMatches( matches );
}

void PlayerResults::addMatches( MatchList matches )
{
  _matches << matches;
  calcScores();
}

/** calculates a scores earned by player.
 *  scores are calculated on the count of games won and games lost. 
 */
void PlayerResults::calcScores( )
{
  int matchesDiff = 0, gamesDiff = 0, ballsDiff = 0;

  for ( int i = 0; i < _matches.count(); i ++ ) {
    Match m = _matches.at( i );
    if ( m.participated( _player ) ) {
      if ( m.winner() == _player )   matchesDiff ++;
	  	else                           matchesDiff --;

      for ( int j = 0; j < m.games_const().count(); j ++ ) {
        Game g = m.games_const().at( j );

        if ( g.winner() == _player )  gamesDiff ++;
        else                          gamesDiff --;

        ballsDiff += g.balls( _player );
		  	ballsDiff -= g.lostBalls( _player ); 
      }
	  }
  }

  _scores = IntegralScores( matchesDiff, gamesDiff, ballsDiff );
}


bool PlayerResults::operator<( const PlayerResults& pr ) const
{
  return ( _scores < pr.scores() );
}


bool PlayerResults::operator>( const PlayerResults& pr ) const
{
  return ( _scores > pr.scores() );
}

PlayerList toPlayerList ( const PlayerResultsList& l )
{
  PlayerList ret;
  for ( int i = 0; i < l.count(); i ++ ) {
    ret << l.at(i).player();
  }

  return ret;
}
