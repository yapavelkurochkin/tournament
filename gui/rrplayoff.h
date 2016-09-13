#ifndef RRPLAYOFF__H
#define RRPLAYOFF__H

#include "tournalgo.h"

class RRPlayoffAlgo : public TournAlgo {
  public:
    RRPlayoffAlgo( TournProps p );

    QList<Group*> initGroups( ) const;
    QList<Group*> buildGroups( unsigned int stage, 
                               QList<Group*> prevGroups ) const;

		typedef enum {
			ABCD = 1,
			ADBC,
			ACBD
		} RRBreakAlgorithm; // algorithm of breaking of RR players to swiss groups
    
    void setRRBreakAlgo( RRBreakAlgorithm a ) { _rrBreakAlgo = a; }
    RRBreakAlgorithm rrBreakAlgo( ) const { return _rrBreakAlgo; }
  protected:
    PlayerList roundRobinResults( QList< Group* > groups ) const;
    
    RRBreakAlgorithm _rrBreakAlgo;
    /*<< algorithm of breaking of RR players to swiss groups */
};

#endif
