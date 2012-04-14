#ifndef MATCH__H
#define MATCH__H

#include <QList>
#include <QDataStream>
#include "playerlist.h"

struct Game
{
  Player a;
  Player b;
  unsigned int aBalls;
  unsigned int bBalls;

  bool validate() const;
  Game& swap();
  Player winner() const;
  unsigned int balls( Player p ) const;

  friend QDataStream &operator>>(QDataStream &s, Game &g);
  friend QDataStream &operator<<(QDataStream &s, const Game &g);
};

class Match 
{
  public:
    typedef enum { 
      BestOf3,
      BestOf5
    } Type;
    
    Match( Player a = Player(), Player b = Player(), Type type = BestOf3 );

    QList< Game > games_const( ) const { return _results; }
    QList< Game >& games( ) { return _results; }
    QString gamesToString( ) const;

    Player playerA() const { return _a; }
    Player playerB() const { return _b; }
    Player opponent( Player p ) const;
    bool participated( Player p ) const 
         { return ( p == playerA() ) || ( p == playerB() ); };
    Match& swapPlayers();
 
    Player winner() const;
    Player looser() const;

    double earnedRating( Player ) const;

    unsigned int gamesWon( Player p ) const;
    unsigned int ballsWon( Player p ) const;

    QString toString() const;

    Type type() const { return _type; }
    unsigned int maxGames() const { return ( _type == BestOf3 ) ? 3 : 5; }
    bool played() const { return validate(); }

    bool validate() const;

  protected:
    Player _a;
    Player _b;
    QList< Game > _results;
    Type _type;  

    friend QDataStream &operator >> ( QDataStream &s, Match &m );
    friend QDataStream &operator << ( QDataStream &s, const Match &m );
};

typedef QList< Match > MatchList;

#endif // MATCH__H

