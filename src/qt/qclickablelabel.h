#ifndef QCLICKABLELABEL_H
#define QCLICKABLELABEL_H
#include <QLabel>
#include <QString>
#include <QMouseEvent>

class QClickableLabel : public QLabel
{
     
    Q_OBJECT
     
    public:
        explicit QClickableLabel( const QString& text ="", QWidget * parent = 0 );
        ~QClickableLabel();
     
    signals:
        void clicked();
     
    protected:
        void mousePressEvent ( QMouseEvent * event ) ;
};
#endif