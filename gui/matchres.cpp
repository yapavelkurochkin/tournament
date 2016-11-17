#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include "matchres.h"

MatchResDialog::MatchResDialog( Match match, QWidget* parent )
: QDialog( parent ),
  _match( match ),
  _okButton( new QPushButton( tr( "Save" ) ) )
{
  setWindowTitle( tr( "Match score" ) );

  _okButton->setEnabled( false );

  QGridLayout* l = new QGridLayout( this );

  QLabel* playerA = new QLabel( match.playerA().name(), this ); 
  QLabel* playerB = new QLabel( match.playerB().name(), this ); 

  l->addWidget( playerA, 0, 0, Qt::AlignJustify );
  l->addWidget( playerB, 0, 1, Qt::AlignJustify );

  unsigned int games = match.games_const().count();
  for ( unsigned int i = 0; i < match.maxGames(); i ++ ) {
    QLineEdit* ballsA = new QLineEdit( this );
    QLineEdit* ballsB = new QLineEdit( this );

    if ( i < games ) {
      // this should be done before connecting of lineedits to textChanged
      // slot
      ballsA->setText( QString::number( match.games_const().at( i ).aBalls ) );
      ballsB->setText( QString::number( match.games_const().at( i ).bBalls ) );
    }

    connect( ballsA, SIGNAL( textChanged( const QString& ) ),
             this, SLOT( textChanged( ) ) );    
    connect( ballsB, SIGNAL( textChanged( const QString& ) ),
             this, SLOT( textChanged( ) ) );    

    l->addWidget( ballsA, i + 1, 0 );
    l->addWidget( ballsB, i + 1, 1 );
  }

  if ( games ) {
    textChanged();
  }

  l->addWidget( _okButton, l->rowCount(), 0, 1, 2 );

  connect( _okButton, SIGNAL( clicked() ), this, SLOT( okButtonClicked() ) );
}

/** Slot processes 
 */
void MatchResDialog::okButtonClicked( )
{
  accept();
}

/** Slot checks whether entered scores are valid and
 * enables or disables okButton
 */ 
void MatchResDialog::textChanged( )
{
  QList< Game > res;
  QGridLayout* l = qobject_cast< QGridLayout* >( layout () );

  for ( unsigned int i = 0; i < _match.maxGames(); i ++ ) {
    QWidget* w = l->itemAtPosition( i + 1, 0 )->widget(); 
    QLineEdit* leA = qobject_cast< QLineEdit* >( w ); 
  
    w = l->itemAtPosition( i + 1, 1 )->widget(); 
    QLineEdit* leB = qobject_cast< QLineEdit* >( w ); 

    if ( leB->text().isEmpty() || leA->text().isEmpty() ) {
      continue;
    } 
  
    Game game;
    game.a = _match.playerA();
    game.b = _match.playerB(); 
    game.aBalls = leA->text().toInt(); 
    game.bBalls = leB->text().toInt(); 

    if ( !game.validate() ) {
      _okButton->setEnabled( false );
      return;
    }

    res << game;
  }

  // all games are correct at this time.

  _match = Match( _match.playerA(), _match.playerB() );
  _match.games() << res;
 
  _okButton->setEnabled( _match.validate() );
}
