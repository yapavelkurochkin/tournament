#ifndef NRBPLAYOFF__H
#define NRBPLAYOFF__H

#include "playoff.h"

/** Russian National Rating tounament B.
 *  Simple playoff, but with specific seeding at the start.
 */
class NRBPlayoffAlgo : public PlayoffAlgo {
  public:
    NRBPlayoffAlgo( TournProps p );

    QList<Group*> initGroups( ) const;
};

#endif
