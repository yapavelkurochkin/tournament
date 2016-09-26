#ifndef QPLAYOFF__H
#define QPLAYOFF__H

#include "tournalgo.h"

/** Qualification + Playoff
 */
class QPlayoffAlgo : public TournAlgo {
  public:
    QPlayoffAlgo( TournProps p );

    QList<Group*> initGroups( ) const;
    QList<Group*> buildGroups( unsigned int stage, 
                               QList<Group*> prevGroups ) const;

  protected:
    PlayerList qualificationResults( QList< Group* > groups ) const;
};

#endif
