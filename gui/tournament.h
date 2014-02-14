#ifndef TOURNAMENT__H
#define TOURNAMENT__H

#include <QObject>
#include "playerlist.h"
#include "rrgroup.h"

class SwissGroup;
class QFile;

#define TOURN_MAGIC_NUMBER 0xfaeb163e

class Tournament : public QObject {
  Q_OBJECT;

  public:
    Tournament( PlayerList players, QString category,
                Match::Type matchType = Match::BestOf3, unsigned int groupCnt = 4); 
    Tournament(  );

    void groupChanged( Group* g );
    QList<Group*> groupList( unsigned int stage ) const 
                          { return _groups[ stage ]; }
 
    unsigned int stagesCnt() const { return _stagesCnt; }
    unsigned int groupCount() const; 
    Match::Type matchType() const { return _matchType; }
    QString category() const { return _category; }
    PlayerList players() const;

    static Tournament* fromFile( QString fileName );
    bool save( QString fname );
    bool save( QFile* f );
    void saveAsCSV( QString fname );
    QString totalRatingAsCSV( QChar sep );

    void setFileName( QString fn ) { _fileName = fn; }
    QString fileName( ) const { return _fileName; }

    MatchList matchList() const;
    unsigned int matchesCount( unsigned int numOfPlayers,
                               unsigned int rrGroupSize, 
                               unsigned int stages ) const;
    
  signals:
    void newSwissGroupCreated( SwissGroup* g );
    void tournamentChanged( Tournament* t );

  public slots:
    
  protected:
    unsigned int _magic;
    /**< Used for identification of validity of tournament object*/

    unsigned int _groupCnt;
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

    friend QDataStream &operator>>(QDataStream &, Tournament&);
    friend QDataStream &operator<<(QDataStream &, const Tournament&);
};

#endif // TOURNAMENT__H

