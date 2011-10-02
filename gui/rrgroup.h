#ifndef RRGROUP__H
#define RRGROUP__H

#include <QList>
#include <QChar>
#include "playerlist.h"
#include "playerscores.h"
#include "match.h"

/** \brief Round robin group
 */
class RRGroup
{
  QChar _name;
  /*<< 'A', 'B', etc... */

  PlayerList _players;

  MatchList _matches;

  void roundRobin();

  public:
    RRGroup( QChar name, PlayerList players = PlayerList() );

    void addPlayer( Player player );
    PlayerList const_players() const { return _players; }

//    MatchList& matches() { return _matches; }
    Match& match( Player a, Player b );
    MatchList matchList( Player p ) const;
    MatchList playedMatchList( Player p ) const;
    void setMatchResults( Player a, Player b, QList< Game > res );

    PlayerResultsList playersResults() const;
    unsigned int playerPlace( Player p ) const;

    QChar name() const { return _name; }
    int size() const { return _players.count(); }

    bool operator< (const RRGroup& gr) const { return _name < gr.name(); }
};

typedef QList< RRGroup > RRGroupList;

extern RRGroupList breakPlayers( PlayerList players, int groupSize ); 

#endif // RRGROUP__H
