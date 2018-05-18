#ifndef __MYSPLASHSCREEN_H
#define __MYSPLASHSCREEN_H
#include <QtGui>

#include <QProgressBar>
#include <QSplashScreen>
class MySplashScreen: public QSplashScreen {
    Q_OBJECT

private:
    QProgressBar *ProgressBar;

public:
    MySplashScreen ( const QPixmap &pixmap );
    ~MySplashScreen();
    void setProgress ( int value ); /* 外部改变进度 */
    void show_started ( void );
private slots:
    void progressChanged ( int );
};

#endif // __MYSPLASHSCREEN_H
