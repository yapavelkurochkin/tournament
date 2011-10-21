#ifndef RRGROUP__H
#define RRGROUP__H

#include "group.h"

/** \brief Round robin group
 */
class RRGroup: public Group
{
  void roundRobin();

  public:
    RRGroup( QString name = QString(), Tournament* tourn = NULL, 
             PlayerList players = PlayerList() );

    void addPlayer( Player player );

    static unsigned int matchesCount( unsigned int numOfPlayers );

    friend QDataStream &operator<<( QDataStream &s, const RRGroup &g );
    friend QDataStream &operator>>( QDataStream &s, RRGroup &g );
};

typedef QList< RRGroup > RRGroupList;

#endif // RRGROUP__H
