#ifndef RRGROUP__H
#define RRGROUP__H

#include "group.h"

/** \brief Round robin group
 */
class RRGroup: public Group
{
  void roundRobin();

  public:
    RRGroup( QChar name, Tournament* tourn, PlayerList players = PlayerList() );

    void addPlayer( Player player );
};

typedef QList< RRGroup > RRGroupList;

#endif // RRGROUP__H
