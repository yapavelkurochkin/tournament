#ifndef PLAYOFF__H
#define PLAYOFF__H

#include "tournalgo.h"

/** Simple Playoff
 */
class PlayoffAlgo : public TournAlgo {
  public:
    PlayoffAlgo( TournProps p );

    QList<Group*> initGroups( ) const;
    QList<Group*> buildGroups( unsigned int stage, 
                               QList<Group*> prevGroups ) const;

  unsigned int calcMatchNum(  ) const;
};

#endif
