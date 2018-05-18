#ifndef MEASURE_TIMER_H
#define MEASURE_TIMER_H

#include <QObject>
#include "qtimer.h"
#include "cantcoap.h"
#include <QtNetwork>
#include <QtSql>
extern double X_value;
class measure_timer : public QObject {
    Q_OBJECT
private:
    QTimer *timer;
    QUdpSocket *udpSocket;
    CoapPDU *pdu;
    QString coap_payload; /* coap接收到的负载 */
     int messageid=0x0005;

public:

    explicit measure_timer( QObject *parent = 0 );
private slots:
    void getMeasure( void );
    void input_sqlite( double Number,int namber,QString temp_1,QString humi_1 );
    void processPendingDatagram( void ); /* 接收到COAP数据 */
    void parse_value( QString coap_payload ); /* 对coap数据进行解析 */
};

#endif // MEASURE_TIMER_H
