#ifndef SWISSGROUP__H
#define SWISSGROUP__H

#include "group.h"

class SwissGroup : public Group
{
  public:
    SwissGroup( QString name = QString(), Tournament* tourn = NULL,
                unsigned int stage = 1,
                PlayerList players = PlayerList() ); 
    
    friend QDataStream &operator<<( QDataStream &s, const SwissGroup &g );
    friend QDataStream &operator>>( QDataStream &s, SwissGroup &g );
};

#endif // SWISSGROUP__H

