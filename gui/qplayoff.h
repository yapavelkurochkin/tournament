#ifndef QPLAYOFF__H
#define QPLAYOFF__H

#include "tournalgo.h"

/** Qualification + Playoff
 */
class QPlayoffAlgo : public TournAlgo {
  unsigned int _qualifNum; 
  /**< number of players in qualification, incl. BYE guys */

  public:
    QPlayoffAlgo( TournProps p );

    QList<Group*> initGroups( ) const;
    QList<Group*> buildGroups( unsigned int stage, 
                               QList<Group*> prevGroups ) const;

  protected:
    PlayerList qualifTopResults( QList< Group* > groups ) const;
    PlayerList qualifBotResults( QList< Group* > groups ) const;

    PlayerList permutePlayers( PlayerList pls ) const;
};

#endif
