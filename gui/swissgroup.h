#ifndef SWISSGROUP__H
#define SWISSGROUP__H

#include "group.h"

class SwissGroup : public Group
{
  unsigned int _fromPlace;
  public:
    SwissGroup( unsigned int fromPlace = 1,
                Tournament* tourn = NULL,
                unsigned int stage = 1,
                PlayerList players = PlayerList() ); 
    
    friend QDataStream &operator<<( QDataStream &s, const SwissGroup &g );
    friend QDataStream &operator>>( QDataStream &s, SwissGroup &g );

    QList< Group* > split() const;
};

#endif // SWISSGROUP__H

