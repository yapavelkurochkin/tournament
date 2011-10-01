#ifndef MATCHRES__H
#define MATCHRES__H

#include <QDialog>
#include <QPushButton>
#include <match.h>

/** Dialog asks the user about the result of match.
 *  It contains 5 or 3 field pairs for best of 5 or best of 3
 *  game type.
 */
class MatchResDialog: public QDialog 
{
  Q_OBJECT;
  public:
    MatchResDialog( Match match, QWidget* parent = NULL );

    Match match() { return _match; }

  protected slots:
    void okButtonClicked( );
    void textChanged( );

  protected:
    Match _match;
    QPushButton* _okButton;
};

#endif // MATCHRES__H

