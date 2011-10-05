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
              Match::Type matchType = Match::BestOf3, int groupSize = 4 );

  void groupChanged( Group* g );
  
  RRGroupList& groupList( ) { return _groupList; }

protected:
  PlayerList _players; 
  int _groupSize;
  RRGroupList _groupList;
  Match::Type _matchType;
  Category _category;

  void breakPlayers( PlayerList players );
};

#endif // TOURNAMENT__H

