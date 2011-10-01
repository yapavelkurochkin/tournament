#ifndef RRGROUP__H
#define RRGROUP__H

#include <QList>
#include <QChar>
#include "playerlist.h"
#include "match.h"

/** \brief Round robin group
 */
class RRGroup
{
  QChar _name;
  /*<< 'A', 'B', etc... */

  PlayerList _players;

  MatchList _matches;

  public:
    RRGroup( QChar name, PlayerList players = PlayerList() );

    PlayerList& players() { return _players; }
    PlayerList const_players() const { return _players; }
    MatchList& matches() { return _matches; }
    Match& match( Player a, Player b );
    QChar name() const { return _name; }
    int size() const { return _players.count(); }

    bool operator< (const RRGroup& gr) const { return _name < gr.name(); }
};

typedef QList< RRGroup > RRGroupList;

extern RRGroupList breakPlayers( PlayerList players, int groupSize ); 

#endif // RRGROUP__H
