#ifndef PLAYERSCORES__H
#define PLAYERSCORES__H

#include "player.h"
#include "match.h"
#include "integralscores.h"

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

    Player player() const { return _player; }

    bool operator<( const PlayerResults& pr ) const;
    bool operator>( const PlayerResults& pr ) const;
};

typedef QList< PlayerResults > PlayerResultsList;

extern PlayerList toPlayerList ( const PlayerResultsList& l );

#endif // PLAYERSCORES__H

