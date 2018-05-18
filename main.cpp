#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include "mysplashscreen.h"
#include "qcustomplot/qcustomplot.h"
#include <QtSql>
#include "main_dialog.h"
#include "curve_show.h"
#include "cantcoap.h"
#include "control.h"
#include <QTextCodec>
#define DEBUG 1

#if !DEBUG
int main ( int argc, char *argv[] ) {
    QApplication app ( argc, argv );
    //QTextCodec::setCodecForTr ( QTextCodec::codecForName ( "UTF-8" ) ); /* 指定字符集为UTF-8 */

    MySplashScreen *splash = new MySplashScreen ( QPixmap ( ":/image/miaojie.png" ) );
    splash->show_started();
    app.processEvents();
    log_Dialog log_window; /* 打开主界面 */
//    tcp_client client;
    log_window.show();
    splash->finish ( &log_window );
    return app.exec();
}

#else

int main ( int argc, char *argv[] ) {
    QApplication app ( argc, argv );
    //QTextCodec::setCodecForTr ( QTextCodec::codecForName ( "UTF-8" ) ); /* 指定字符集为UTF-8 */
    main_Dialog main_dialog; /* 打开控制界面 */
    main_dialog.show();
    return app.exec();
}

#endif
