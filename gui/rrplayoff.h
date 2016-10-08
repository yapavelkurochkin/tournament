#ifndef RRPLAYOFF__H
#define RRPLAYOFF__H

#include "tournalgo.h"

class RRPlayoffAlgo : public TournAlgo {
  public:
    RRPlayoffAlgo( TournProps p );

    QList<Group*> initGroups( ) const;
    QList<Group*> buildGroups( unsigned int stage, 
                               QList<Group*> prevGroups ) const;

  protected:
    PlayerList roundRobinResults( QList< Group* > groups ) const;
};

#endif
