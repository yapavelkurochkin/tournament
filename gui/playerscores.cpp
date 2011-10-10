#include "playerscores.h"

IntegralScores integralScores( unsigned int matchesWon, 
                               unsigned int gamesWon,
                               unsigned int ballsWon )
{
  return ( matchesWon << 16 ) + ( gamesWon << 8 ) + ballsWon;
}

PlayerResults::PlayerResults( Player p )
: _player( p ),
  _scores( 0 )
{
}

PlayerResults::PlayerResults( Player p, MatchList matches )
: _player( p ),
  _scores( 0 )
{
  addMatches( matches );
}

void PlayerResults::addMatches( MatchList matches )
{
  _matches << matches;
  calcScores();
}

void PlayerResults::calcScores( )
{
  unsigned int matchesWon = 0,
               gamesWon = 0,
               ballsWon = 0;

  for ( int i = 0; i < _matches.count(); i ++ ) {
    Match m = _matches.at( i );
    if ( m.winner() == _player ) {
      matchesWon ++;
    }

    for ( int j = 0; j < m.games_const().count(); j ++ ) {
      Game g = m.games_const().at( j );

      if ( g.winner() == _player ) {
        gamesWon ++;
      }

      ballsWon += g.balls( _player ); 
    }
  }

  _scores = integralScores( matchesWon, gamesWon, ballsWon );
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
