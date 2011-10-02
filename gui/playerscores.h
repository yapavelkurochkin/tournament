#ifndef PLAYERSCORES__H
#define PLAYERSCORES__H

#include "player.h"
#include "match.h"

typedef unsigned long IntegralScores;

extern IntegralScores integralScores( unsigned int matchesWon, 
                                      unsigned int gamesWon,
                                      unsigned int ballsDiff );

class PlayerResults {
    Player _player;
    MatchList _matches;
    IntegralScores _scores;

    void calcScores();
    void calcRating();
  public:
    PlayerResults( Player p );
    PlayerResults( Player p, MatchList matches );

    void addMatches( MatchList matches );
    MatchList matches() const { return _matches; }

    IntegralScores scores() const { return _scores; }
    IntegralScores new_rating() const;

    Player player() const { return _player; }

    bool operator<( const PlayerResults& pr ) const;
    bool operator>( const PlayerResults& pr ) const;
};

typedef QList< PlayerResults > PlayerResultsList;

#endif // PLAYERSCORES__H

