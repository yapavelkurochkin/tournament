#ifndef TOURNALGO__H
#define TOURNALGO__H

#include <QList>
#include "tournprops.h"

class Group;

namespace BreakAlgo {
  enum Algo {
    ABCD=0,
    ADBC,
    ACBD
  };
};

class TournAlgo {

  protected:
    unsigned int _stagesCnt;
    TournProps   _props;
    BreakAlgo::Algo _break;

  // TODO: make TournAlgoFactory a friend and make constructor protected?
  public:
    TournAlgo( TournProps p );
    virtual ~TournAlgo(){};

    unsigned int stagesCnt() const { return _stagesCnt; }
    TournProps props() const { return _props; }

    BreakAlgo::Algo breakAlgo() const { return _break; }  
    void setBreakAlgo ( BreakAlgo::Algo a ) { _break = a; }  
 
    bool stageCompleted( QList< Group*> groups ) const;
    unsigned int maxGroupSize( QList< Group* > groups ) const;

    bool isStageLast( unsigned stage ) const { return ( stage == ( _stagesCnt - 1 ) ); }

    virtual QList<Group*> initGroups( ) const = 0;
    virtual QList<Group*> buildGroups( unsigned int stage, 
                                 QList<Group*> prevGroups = QList< Group* >() ) const = 0;

    virtual unsigned int calcMatchNum( ) const { return 0; }

    PlayerList permutePlayers( PlayerList pls ) const;

    
};

#endif // TOURNALGO__H

