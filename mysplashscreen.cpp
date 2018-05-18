#include "mysplashscreen.h"
#include <QDateTime>

MySplashScreen::MySplashScreen ( const QPixmap &pixmap ) : QSplashScreen ( pixmap ) {
    ProgressBar = new QProgressBar ( this ); /* 父类为MySplashScreen */
    ProgressBar->setGeometry ( 0, pixmap.height() - 50, pixmap.width(), 30 );
    ProgressBar->setRange ( 0, 100 );
    ProgressBar->setValue ( 0 );
    connect ( ProgressBar, SIGNAL ( valueChanged ( int ) ), this, SLOT ( progressChanged ( int ) ) ); /* 值改变时，立刻repaint */
    QFont font;
    font.setPointSize ( 32 );
    ProgressBar->setFont ( font ); /* 设置进度条里面的字体 */
}

MySplashScreen::~MySplashScreen() {
}

void MySplashScreen::setProgress ( int value ) {
    ProgressBar->setValue ( value );
}

void MySplashScreen::progressChanged ( int ) {
    repaint();
}

void MySplashScreen::show_started ( void ) {
    this->show(); /* 显示 */
    this->setProgress ( 30 ); /* 显示30% */
    /*-----------------------------------------------------*/
    QTime now;
    QTime n;
    n = QTime::currentTime();

    do {
        now = QTime::currentTime();
    }
    while ( n.msecsTo ( now ) <= 500 );
    /*----------------------------------------------------*/
    this->setProgress ( 60 );
    /*-----------------------------------------------------*/
    n = QTime::currentTime();

    do {
        now = QTime::currentTime();
    }
    while ( n.msecsTo ( now ) <= 500 );
    /*----------------------------------------------------*/
    this->setProgress ( 90 );
    /*-----------------------------------------------------*/
    n = QTime::currentTime();

    do {
        now = QTime::currentTime();
    }
    while ( n.msecsTo ( now ) <= 500 );
    /*----------------------------------------------------*/
    this->setProgress ( 100 );
    /*-----------------------------------------------------*/
    n = QTime::currentTime();

    do {
        now = QTime::currentTime();
    }
    while ( n.msecsTo ( now ) <= 500 );
    /*----------------------------------------------------*/
}
