#ifndef TOURNALGO__H
#define TOURNALGO__H

#include <QList>
#include "tournprops.h"

class Group;

class TournAlgo {

  protected:
    unsigned int _stagesCnt;
    TournProps   _props;

  // TODO: make TournAlgoFactory a friend and make constructor protected?
  public:
    TournAlgo( TournProps p );
    virtual ~TournAlgo(){};

    unsigned int stagesCnt() const { return _stagesCnt; }
    TournProps props() const { return _props; }
   
    bool stageCompleted( QList< Group*> groups ) const;
    unsigned int maxGroupSize( QList< Group* > groups ) const;

    bool isStageLast( unsigned stage ) { return ( stage == ( _stagesCnt - 1 ) ); }

    virtual QList<Group*> initGroups( ) = 0;
    virtual QList<Group*> buildGroups( unsigned int stage, 
                                 QList<Group*> prevGroups = QList< Group* >() ) = 0;

};

#endif // TOURNALGO__H

