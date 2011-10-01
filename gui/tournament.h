#ifndef TOURNAMENT__H
#define TOURNAMENT__H

#include "playerlist.h"
#include "rrgroup.h"
#include "match.h"

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

  RRGroupList& groupList( ) { return _groupList; }

protected:
  PlayerList _players; 
  int _groupSize;
  RRGroupList _groupList;
  Match::Type _matchType;
  Category _category;
};

#endif // TOURNAMENT__H

