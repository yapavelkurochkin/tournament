#ifndef TOURNDATA__H
#define TOURNDATA__H


class TournAlgo;

/** Actual matches, player list and all data, which 
 *  is changing during tournament
 */
class TournData {
  TournAlgo *_algo;

  QList<Group*>* _groups;
  /*<< one list of groups per one stage */

  void initGroups( );

  public:
    TournData( TournAlgo *algo = NULL );

    void setAlgo( TournAlgo * algo ) { _algo = algo; }
    const TournAlgo *algo() const { return _algo; }

    QList<Group*>* groups() const { return _groups; }

    void setMatchResults( Player a, Player b, QList< Game > res );
    void setMatchResults( Match m );

    QList<Group*> groupList( unsigned int stage ) const 
                          { return _groups[ stage ]; }
    Group*        group( unsigned int stage, unsigned int i ) const 
                          { return _groups[ stage ][i]; }
    const Group*  group_const( unsigned int stage, unsigned int i ) const 
                          { return _groups[ stage ].at(i); }
    MatchList matchList( int stage = -1 /* all stages */ ) const;
    PlayerList playerList() const;
};

#endif // TOURNDATA__H

