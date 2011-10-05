#ifndef GROUP__H
#define GROUP__H

#include <QList>
#include <QChar>
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

  public:
    Group( QString name, Tournament* tourn, PlayerList players = PlayerList() );

    virtual void addPlayer( Player player );
    PlayerList const_players() const { return _players; }

    Match& match( Player a, Player b );
    MatchList matchList( Player p ) const;
    MatchList playedMatchList( Player p ) const;
    void setMatchResults( Player a, Player b, QList< Game > res );

    PlayerResultsList playersResults() const;
    unsigned int playerPlace( Player p ) const;

    QString name() const { return _name; }
    int size() const { return _players.count(); }
    bool completed() const;

    virtual bool operator< (const Group& gr) const { return _name < gr.name(); }
};

#endif // GROUP__H
