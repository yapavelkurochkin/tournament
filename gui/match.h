#ifndef MATCH__H
#define MATCH__H

#include <QList>
#include "playerlist.h"

struct GameResult
{
  unsigned int a;
  unsigned int b;

  bool validate() const;
};

class Match 
{
  public:
    typedef enum { 
      BestOf3,
      BestOf5
    } Type;
    
    Match( Player a, Player b, Type type = BestOf3 );

    QList< GameResult > results_const( ) const { return _results; }
    QList< GameResult >& results( ) { return _results; }
    QString resultsAsString( ) const;
    Player won() const;

    Player playerA() const { return _a; }
    Player playerB() const { return _b; }
 
    int aScores() const;
    int bScores() const;
    QString scoresAsString() const;

    Type type() const { return _type; }
    int maxGames() const { return ( _type == BestOf3 ) ? 3 : 5; }

    bool validate() const;

  protected:
    Player _a;
    Player _b;
    QList< GameResult > _results;
    Type _type;  
};

typedef QList< Match > MatchList;

#endif // MATCH__H
