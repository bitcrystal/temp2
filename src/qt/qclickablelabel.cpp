#include "qclickablelabel.h"

QClickableLabel::QClickableLabel( const QString& text, QWidget * parent ) :
    QLabel(parent)
     
	{
       this->setText(text);
    }
     
    QClickableLabel::~QClickableLabel()
    {
	}
     
    void QClickableLabel::mousePressEvent ( QMouseEvent * event )
     
    {
        emit clicked();
    }
