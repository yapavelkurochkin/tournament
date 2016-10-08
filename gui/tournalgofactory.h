#ifndef TOURNALGOFACTORY__H
#define TOURNALGOFACTORY__H

#include "tournalgo.h"

class TournAlgoFactory {
  public:
    static TournAlgo* algo( TournProps p );
};

#endif // TOURNALGOFACTORY__H

