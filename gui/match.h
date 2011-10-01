#ifndef MATCH__H
#define MATCH__H

#include <QList>
#include "playerlist.h"

struct GameResult
{
  int a;
  int b;
};

class Match 
{
  Player _a;
  Player _b;
  QList< GameResult > _results;
  public:
    Match( Player a, Player b );
    void addGameResult( GameResult res );
    Player won() const;

    Player playerA() const { return _a; }
    Player playerB() const { return _b; }
 
    int aScores() const;
    int bScores() const;

  typedef enum { 
    BestOf3,
    BestOf5
  } Type;
    
};

typedef QList< Match > MatchList;

#endif // MATCH__H
