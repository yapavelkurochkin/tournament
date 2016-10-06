#ifndef TOURNDATA__H
#define TOURNDATA__H

#include <QList>
#include "player.h"
#include "playerlist.h"
#include "match.h"

class TournAlgo;
class Group;
class Tournament;

/** Actual matches, player list and all data, which 
 *  is changing during tournament
 */
class TournData {
  const TournAlgo   *_algo;

  QList<Group*>     *_groups;
  /*<< one list of groups per one stage */

  Tournament        *_tournament;

  public:
    TournData( const TournAlgo *algo = NULL );

    void initGroups( );

    void setTournament( Tournament* t ) { _tournament = t; }

    void setAlgo( TournAlgo * algo ) { _algo = algo; }
    const TournAlgo *algo() const { return _algo; }

    QList<Group*>* groups() const { return _groups; }
    void groupChanged( Group *g );

    QList<Group*> groupList( unsigned int stage ) const 
                          { return _groups[ stage ]; }
    Group*        group( unsigned int stage, unsigned int i ) const 
                          { return _groups[ stage ][i]; }
    const Group*  group_const( unsigned int stage, unsigned int i ) const 
                          { return _groups[ stage ].at(i); }
    MatchList matchList( int stage = -1 /* all stages */ ) const;
    PlayerList playerList() const;

    friend QDataStream &operator>>(QDataStream &s, TournData& t );
    friend QDataStream &operator<<(QDataStream &s, const TournData& t );
};

#endif // TOURNDATA__H

