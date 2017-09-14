#ifndef MATCH__H
#define MATCH__H

#include <QList>
#include <QDataStream>
#include <QJsonObject>
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
  unsigned int lostBalls( Player p ) const;

  friend QDataStream &operator>>(QDataStream &s, Game &g);
  friend QDataStream &operator<<(QDataStream &s, const Game &g);

  void write( QJsonObject &json ) const;
};

#define MAX_GAMES_PER_MATCH 5

class Match 
{
  public:
    Match( Player a = Player(), Player b = Player() );

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

    unsigned int maxGames() const { return MAX_GAMES_PER_MATCH;
                                     /*real number of games depends from admin */}
    bool played() const { return validate(); }
    bool isBye() const;

    void setQualif( bool q = true ) { _qualif = q; }
    bool isQualif( ) const { return _qualif; }
 
    bool validate() const;

    void write( QJsonObject &json ) const;

  protected:
    Player _a;
    Player _b;
    QList< Game > _results;
    bool _qualif;
    /*!<< means that this match is in qualification stage and 
          rating will not be calculated */

    friend QDataStream &operator >> ( QDataStream &s, Match &m );
    friend QDataStream &operator << ( QDataStream &s, const Match &m );
};

typedef QList< Match > MatchList;

#endif // MATCH__H

