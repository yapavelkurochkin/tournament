#ifndef SWISSGROUP__H
#define SWISSGROUP__H

#include "group.h"

class SwissGroup : public Group
{
  unsigned int _fromPlace;

  void initGroupName();

  public:
    SwissGroup( unsigned int fromPlace,
                unsigned int stage,
                PlayerList players );
    SwissGroup(); 
   
    QList< Group* > split() const;
    unsigned int fromPlace() const { return _fromPlace; }

    QList< QPair< unsigned int, Player > > absPlaces( );

    bool isFinal() const 
         { return ( _players.count() == 2 ) && ( _fromPlace == 1 ); }
    bool isHalfFinal() const 
         { return ( _players.count() == 4 ) && ( _fromPlace == 1 ); }
    bool isQuarterFinal() const  
         { return ( _players.count() == 8 ) && ( _fromPlace == 1 ); }

    static unsigned int matchesCount( unsigned int numOfPlayers );
    static bool lessThan(const Group* g1, const Group* g2);

    QString type() const { return "swiss"; }

    void permuteMatches( BreakAlgo::Algo br );

    // serializers 
    friend QDataStream &operator<<( QDataStream &s, const SwissGroup &g );
    friend QDataStream &operator>>( QDataStream &s, SwissGroup &g );
    QString csvResult( QChar sep ) const;
    void write( QJsonObject &json ) const;
};


#endif // SWISSGROUP__H

