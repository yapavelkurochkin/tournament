#ifndef TOURNAMENT__H
#define TOURNAMENT__H

#include <QObject>
#include "playerlist.h"
#include "rrgroup.h"

class SwissGroup;

class Tournament : public QObject {
  Q_OBJECT;

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
    Tournament(  );

    void groupChanged( Group* g );
    QList<Group*> groupList( unsigned int stage ) const 
                          { return _groups[ stage ]; }
 
    unsigned int stagesCnt() const { return _stagesCnt; }
    Match::Type matchType() const { return _matchType; }

    static Tournament* fromFile( QString fileName );
 
  signals:
    void newSwissGroupCreated( SwissGroup* g );
  public slots:
    void save();

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
    PlayerList roundRobinResults() const;

    SwissGroup* newSwissGroup( unsigned int fromPlace, unsigned int stage, 
                               PlayerList players );

    void buildGroups( );
    void splitSwissGroup( SwissGroup* g );

    friend QDataStream &operator>>(QDataStream &, Tournament&);
    friend QDataStream &operator<<(QDataStream &, const Tournament&);
};

#endif // TOURNAMENT__H

