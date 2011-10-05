#ifndef TOURNAMENT__H
#define TOURNAMENT__H

#include "playerlist.h"
#include "rrgroup.h"

class Tournament {
public:
  typedef enum {
    M1, 
    M2, 
    M3,
    Elite
  } Category;

  Tournament( PlayerList players, Category category,
              Match::Type matchType = Match::BestOf3, unsigned int groupSize = 3, 
              unsigned int stagesCnt = 4 );

  void groupChanged( Group* g );
  QList<Group*> groupList( unsigned int stage ) const 
                 { return _groups[ stage ]; }
  
protected:
  PlayerList _players; 
  unsigned int _groupSize;
  unsigned int _stagesCnt;
  /*<< playing stages including round robin */

  QList<Group*>* _groups;
  /*<< one list of groups per one stage */

  Match::Type _matchType;
  Category _category;

  void breakPlayers( PlayerList players );
  bool roundRobinCompleted() const;
  PlayerResultsList roundRobinResults() const;
};

#endif // TOURNAMENT__H

