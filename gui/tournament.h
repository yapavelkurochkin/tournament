#ifndef TOURNAMENT__H
#define TOURNAMENT__H

#include <QObject>
#include "playerlist.h"
#include "rrgroup.h"

class SwissGroup;

#define TOURN_MAGIC_NUMBER 0xfaeb163a

class Tournament : public QObject {
  Q_OBJECT;

  public:
    Tournament( PlayerList players, QString category,
                Match::Type matchType = Match::BestOf3, unsigned int groupSize = 3, 
                unsigned int stagesCnt = 4 );

    void groupChanged( Group* g );
    QList<Group*> groupList( unsigned int stage ) const 
                          { return _groups[ stage ]; }
 
    unsigned int stagesCnt() const { return _stagesCnt; }
    Match::Type matchType() const { return _matchType; }
    QString category() const { return _category; }

    static Tournament* fromFile( QString fileName );
    void save( QString fname );

    void setFileName( QString fn ) { _fileName = fn; }
    QString fileName( ) const { return _fileName; }
 
    unsigned int matchesCount( unsigned int numOfPlayers,
                               unsigned int rrGroupSize, 
                               unsigned int stages ) const;
  signals:
    void newSwissGroupCreated( SwissGroup* g );
  public slots:
    void save();

  protected:
    unsigned int _magic;
    /**< Used for identification of validity of tournament object*/

    unsigned int _groupSize;
    unsigned int _stagesCnt;
    /*<< playing stages including round robin */

    QList<Group*>* _groups;
    /*<< one list of groups per one stage */

    Match::Type _matchType;
    QString _category;

    QString _fileName;
    /*<< file which corresponds to this tournament */

    void breakPlayers( PlayerList players );
    bool roundRobinCompleted() const;
    PlayerList roundRobinResults() const;

    SwissGroup* newSwissGroup( unsigned int fromPlace, unsigned int stage, 
                               PlayerList players );

    void buildGroups( );
    void splitSwissGroup( SwissGroup* g );

    bool isValid() const { return _magic == TOURN_MAGIC_NUMBER; }

    Tournament(  );
    friend QDataStream &operator>>(QDataStream &, Tournament&);
    friend QDataStream &operator<<(QDataStream &, const Tournament&);
};

#endif // TOURNAMENT__H

