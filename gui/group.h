#ifndef GROUP__H
#define GROUP__H

#include <QList>
#include <QChar>
#include <QDataStream>
#include "playerlist.h"
#include "playerscores.h"
#include "match.h"

class Tournament;

/** represents the group of players. it may be round-robin group or
 * swiss tournament group. group's has a name and a list of players
 * which plays the set of matches. group is meant completed when 
 * all matches are played. 
 * players places is detected if at least one match were played.
 */
class Group 
{
  protected:
    QString _name;
    /*<< 'A', 'B', 'semi-final', etc... */

    PlayerList _players;
    MatchList _matches;
    Tournament* _tournament;
    unsigned int _stage;

    friend QDataStream &operator>>(QDataStream &, Group&);
    friend QDataStream &operator<<(QDataStream &, const Group&);

  public:
    Group( QString name = QString(), Tournament* tourn = NULL, 
           unsigned int stage = 0, PlayerList players = PlayerList() );

    virtual void addPlayer( Player player );
    PlayerList const_players() const { return _players; }

    Match& match( Player a, Player b );
    MatchList matchList( Player p ) const;
    MatchList matchList( ) const { return _matches; };
    MatchList playedMatchList( Player p ) const;
    void setMatchResults( Player a, Player b, QList< Game > res );
    void setMatchResults( Match m );

    PlayerResultsList playersResults() const;
    PlayerResults playerResults( Player p ) const;
    unsigned int playerPlace( Player p ) const;
    Player playerByPlace( unsigned int place ) const;

    void setTournament( Tournament* t ) { _tournament = t; }

    QString name() const { return _name; }
    int size() const { return _players.count(); }
    bool completed() const;
    unsigned int stage() const { return _stage; }

    virtual bool operator< (const Group& gr) const { return _name < gr.name(); }
};

#endif // GROUP__H
