#ifndef GROUP__H
#define GROUP__H

#include <QList>
#include <QChar>
#include <QDataStream>
#include "playerlist.h"
#include "playerscores.h"
#include "match.h"

class TournData;

/** represents the group of players. it may be round-robin group or
 * swiss tournament group. group's has a name and a list of players
 * which plays the set of matches. group is meant completed when 
 * all matches are played. 
 * players places is detected if at least one match were played.
 */
class Group 
{
  protected:
    QString _name;
    /*<< 'A', 'B', 'semi-final', etc... */

    PlayerList _players;
    MatchList _matches;
    TournData* _tournData;
    unsigned int _stage;

    friend QDataStream &operator>>(QDataStream &, Group&);
    friend QDataStream &operator<<(QDataStream &, const Group&);

  public:
    Group( QString name = QString(),
           unsigned int stage = 0, PlayerList players = PlayerList() );
    Group( QString name = QString(), 
           MatchList matches = MatchList(), PlayerList players = PlayerList() );

    virtual void addPlayer( Player player );
    virtual void removePlayer( Player player );
    PlayerList const_players() const { return _players; }
    PlayerList const_validPlayers() const;

    Match& match( Player a, Player b );
    Match const_match( Player a, Player b ) const;
    MatchList matchList( Player p ) const;
    MatchList matchList( Player p, Player b ) const;
    MatchList matchList( ) const { return _matches; };
    MatchList playedMatchList( Player p ) const;

    void setMatchResults( Player a, Player b, QList< Game > res );
    void setMatchResults( Match m );

    PlayerResultsList playersResults() const;
    PlayerResults playerResults( Player p ) const;
    unsigned int playerPlace( Player p ) const;
    Player playerByPlace( unsigned int place ) const;
    PlayerList winners() const;
    PlayerList loosers() const;

    double earnedRating( Player p ) const;

    void setTournData( TournData* t ) { _tournData = t; }
    const TournData* tournData_const( ) { return _tournData; }
    TournData* tournData( ) { return _tournData; }

    QString name() const { return _name; }
    void setName( QString s ) { _name = s; }

    int size() const { return _players.count(); }
    bool completed() const;
    unsigned int stage() const { return _stage; }

    bool readOnly() const { return completed(); }
    /**< group should not be edited when it is completed */

    virtual QString type() const { return ""; };
    /**< string should describe a type of group (for example "swiss") */

    virtual bool operator< (const Group& gr) const { return _name < gr.name(); }
    virtual QString csvResult( QChar ) const { return ""; }
};

#endif // GROUP__H
