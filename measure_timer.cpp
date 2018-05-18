#include "measure_timer.h"
#include <QDebug>
#include <QtSql>
#include "qdatetime.h"
#include "curve_show.h"
#include "main_dialog.h"
/* coap的payload格式为“序号#温度#湿度” */
double X_value;
measure_timer::measure_timer ( QObject *parent ) : QObject ( parent ) {//measure的构造函数，定时5秒发送请求数据包
    timer = new QTimer();
    timer->setInterval ( 1000 * 2 ); /* 设置定时时间为5秒钟 */
    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( getMeasure() ) ); /* 超时信号和测量的槽连接 */
    timer->start(); /* 启动定时器 */
    /*-------------------------------------------------------*/
    pdu = new CoapPDU();
    udpSocket = new QUdpSocket ( this );
    udpSocket->bind ( QHostAddress::AnyIPv6, 5683 );
    connect ( udpSocket, SIGNAL ( readyRead() ), this, SLOT ( processPendingDatagram() ) );
}

void measure_timer::parse_value( QString coap_payload ){

            switch(coap_payload.left(2).toInt())
            {
                case 01:{
                    Temp_Humi[0].number=coap_payload.left(2).toInt();
                    Temp_Humi[0].temp=coap_payload.mid(3,2);
                    Temp_Humi[0].humi=coap_payload.right(2);
                    qDebug()<<Temp_Humi[0].number;
                        };break;
                case 02:{
                    Temp_Humi[1].number=coap_payload.left(2).toInt();
                    Temp_Humi[1].temp=coap_payload.mid(3,2);
                    Temp_Humi[1].humi=coap_payload.right(2);
                    qDebug()<<Temp_Humi[1].number;
                };break;
                case 03:{
                    Temp_Humi[2].number=coap_payload.left(2).toInt();
                    Temp_Humi[2].temp=coap_payload.mid(3,2);
                    Temp_Humi[2].humi=coap_payload.right(2);
                    qDebug()<<Temp_Humi[2].number;
                };break;
            }


}

void measure_timer::processPendingDatagram ( void ) {//解析接收到的coap数据包
    qDebug() << tr ( "得到COAP反馈" );

    while ( udpSocket->hasPendingDatagrams() ) { /* 如果有等待的数据报 */
        QByteArray datagram;
        datagram.resize ( udpSocket->pendingDatagramSize() ); /* 让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据 */
        udpSocket->readDatagram ( datagram.data(), datagram.size() ); /* 接收数据报，将其存放到datagram中 */
        uint8_t *buffer = ( uint8_t * ) datagram.data();
        CoapPDU *recvPDU = new CoapPDU ( ( uint8_t * ) buffer, datagram.size() );

        if ( recvPDU->validate() != 1 ) {
            qDebug() << "Malformed CoAP packet";
        } else {
            qDebug() << "Valid CoAP PDU received";
        }

        coap_payload = QString ( QLatin1String ( ( const char * ) ( recvPDU->getPayloadPointer() ) ) ).left ( recvPDU->getPayloadLength() ); /* “char *”转QString */
        qDebug() << "------------------";
        qDebug() << coap_payload;
        qDebug() << "------------------";
        parse_value ( coap_payload );
        switch(coap_payload.left(2).toInt())
        {
            case 01:{
                 input_sqlite(1,Temp_Humi[0].number,Temp_Humi[0].temp,Temp_Humi[0].humi);
                    };break;
            case 02:{
                input_sqlite(2,Temp_Humi[1].number,Temp_Humi[1].temp,Temp_Humi[1].humi);
            };break;
            case 03:{
                input_sqlite(3,Temp_Humi[2].number,Temp_Humi[2].temp,Temp_Humi[2].humi);
            };break;
        }

    }

}

void measure_timer::getMeasure ( void ) {//发送请求数据包
    static int i=1;
    qDebug() << tr ( "测量中..." );
    if(i<3)
    {
        pdu->setType ( CoapPDU::COAP_CONFIRMABLE );
        pdu->setCode ( CoapPDU::COAP_GET );
        pdu->setToken ( ( uint8_t * ) "\3\2\1\0", 4 );
        pdu->setMessageID ( ++messageid );
        if(messageid>27)
            messageid=7;
        if(messageid<7)
        pdu->setURI ( ( char * ) "test/hello", 10 );
        //pdu->setURI ( ( char * ) "ds18b20/Temperature", 19 );
        QHostAddress address;
        address.setAddress (addr_set[i].ipv6_addr );
        //address.setAddress ( "192.168.1.115" );
        qDebug()<<messageid;
       if(udpSocket->writeDatagram ( ( const char * ) ( pdu->getPDUPointer() ), pdu->getPDULength(), address, 5683 )>0)
           qDebug()<<tr("发送测量成功");
       else
           qDebug()<<tr("发送测量失败");
        i++;
    }else
        i=0;

}

const QString insert_sql = "insert into measure_temp(room_id, time, temp, humi) values ( ?, ?, ?, ? )"; /* 向数据库中插入信息的SQL语句 */

void measure_timer::input_sqlite (double Number,int number,QString temp_1,QString humi_1 ) { /* 定期向数据库中填写信息 */
    qDebug() << tr ( "向数据库中写入数据" );
    QString db_name = NULL; /* 需要打开的数据库的名字 */
    QSqlDatabase db;

    if ( QSqlDatabase::contains ( "qt_sql_default_connection" ) ) {
        db = QSqlDatabase::database ( "qt_sql_default_connection" );
       // qDebug()<<"写入yiban数据库";
    } else {
        db = QSqlDatabase::addDatabase ( "QSQLITE" );
        qDebug()<<"写入sqlite数据库";
    }
 QString str=QString("_%1").arg(Number);
    db_name = QDateTime::currentDateTime().toString ( "yyyy-MM-dd" ) +str+".db"; /* 数据库的名称是日期 */
    db.setDatabaseName ( db_name );
    db_name.clear();

    if ( !db.open() ) {
        qDebug ()<<db_name<<"数据库不能打开";
    }
     QTime time(QTime::currentTime());
    // calculate two new data points:
     X_value = time.msecsSinceStartOfDay()/1000+57600;// time elapsed since start of demo, in seconds
    QSqlQuery sql_query;
    sql_query.prepare ( insert_sql );
    sql_query.addBindValue ( number); /* 大棚的序号 */
    sql_query.addBindValue (X_value ); /* 数据库的名称是当前日期 */
    sql_query.addBindValue (temp_1);
    sql_query.addBindValue ( humi_1);
qDebug()<<"X_value="<<X_value ;
    if ( !sql_query.exec() ) {
        qDebug() << sql_query.lastError();
    } else {
        qDebug() << "inserted!";
    }
    db.close(); /* 关闭数据库 */
}
