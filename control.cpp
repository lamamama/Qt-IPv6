#include "control.h"
#include "ui_control.h"
#include "QTextCodec"
#include "curve_show.h"

switch_state room_switch[3] = {SWITCH_OFF, SWITCH_OFF, SWITCH_OFF};

control::control ( QWidget *parent ) : QDialog ( parent ), ui ( new Ui::control ) {
    //QTextCodec::setCodecForTr ( QTextCodec::codecForName ( "UTF-8" ) ); /* 指定字符集为UTF-8 */
    ui->setupUi ( this );
    ui->room1_light->setStyleSheet ( "QPushButton{background: transparent;}" ); /* 防止按键出现黑影 */
    ui->room1_light->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    ui->room2_light->setStyleSheet ( "QPushButton{background: transparent;}" ); /* 防止按键出现黑影 */
    ui->room2_light->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    ui->room3_light->setStyleSheet ( "QPushButton{background: transparent;}" ); /* 防止按键出现黑影 */
    ui->room3_light->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    setWindowTitle ( tr ( "控制界面" ) );
    get_switch_state = new CoapPDU();
    udpSocket = new QUdpSocket ( this );
    udpSocket->bind ( QHostAddress::AnyIPv6, 7755 );
    connect ( udpSocket, SIGNAL ( readyRead() ), this, SLOT ( room_switch_state() ) );
    send_coap_test_cmd( "hello" );
}

void control::send_coap_test_cmd ( QString coap_url_cmd ){
//    qDebug() << tr ( "测试中..." );
//    get_switch_state->setType ( CoapPDU::COAP_CONFIRMABLE );
//    get_switch_state->setCode ( CoapPDU::COAP_GET );
//    get_switch_state->setToken ( ( uint8_t * ) "\3\2\1\0", 4 );
//    get_switch_state->setMessageID ( 0x0005 );
//    //pdu->setURI ( ( char * ) "test/hello", 10 );
//    QHostAddress address;
//    address.setAddress ( "::1" );
//    udpSocket->writeDatagram ( ( const char * ) ( get_switch_state->getPDUPointer() ), get_switch_state->getPDULength(), address, 5683 );
}

control::~control() {
    delete ui;
}

void control::room_switch_state ( void ) {
//    qDebug() << tr ( "得到COAP反馈" );

//    while ( udpSocket->hasPendingDatagrams() ) { /* 如果有等待的数据报 */
//        QByteArray datagram;
//        datagram.resize ( udpSocket->pendingDatagramSize() ); /* 让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据 */
//        udpSocket->readDatagram ( datagram.data(), datagram.size() ); /* 接收数据报，将其存放到datagram中 */
//        uint8_t *buffer = ( uint8_t * ) datagram.data();
//        CoapPDU *recvPDU = new CoapPDU ( ( uint8_t * ) buffer, datagram.size() );

//        if ( recvPDU->validate() != 1 ) {
//            qDebug() << "Malformed CoAP packet";
//        } else {
//            qDebug() << "Valid CoAP PDU received";
//        }

//        coap_payload = QString ( QLatin1String ( ( const char * ) ( recvPDU->getPayloadPointer() ) ) ).left ( recvPDU->getPayloadLength() ); /* “char *”转QString */
//        qDebug() << "------------------";
//        qDebug() << coap_payload;
//        qDebug() << "------------------";
//    }
}

void control::on_room1_light_clicked ( bool checked ) {

    //    ui->room1_light->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    if ( room_switch[0] == SWITCH_OFF ) {
        send_coap_test_cmd ( "OPEN" );
        qDebug()<<tr("open");
        room_switch[0] = SWITCH_ON;
        ui->room1_light->setIcon ( QIcon ( ":/image/switch_on.png" ) );
    } else if ( room_switch[0] == SWITCH_ON  ) {
        send_coap_test_cmd ( "close" );
        qDebug()<<tr("close");
        room_switch[0] = SWITCH_OFF;
        ui->room1_light->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    }
}

void control::on_room2_light_clicked ( bool checked ) {
    if ( room_switch[1] == SWITCH_OFF ) {
        send_coap_test_cmd ( "OPEN" );
        qDebug()<<tr("open");
        room_switch[1] = SWITCH_ON;
        ui->room2_light->setIcon ( QIcon ( ":/image/switch_on.png" ) );
    } else if ( room_switch[1] == SWITCH_ON  ) {
        send_coap_test_cmd ( "close" );
        qDebug()<<tr("close");
        room_switch[1] = SWITCH_OFF;
        ui->room2_light->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    }
}

void control::on_room3_light_clicked ( bool checked ) {
    if ( room_switch[2] == SWITCH_OFF ) {
        send_coap_test_cmd ( "OPEN" );
        qDebug()<<tr("open");
        room_switch[2] = SWITCH_ON;
        ui->room3_light->setIcon ( QIcon ( ":/image/switch_on.png" ) );
    } else if ( room_switch[2] == SWITCH_ON  ) {
        send_coap_test_cmd ( "close" );
        qDebug()<<tr("close");
        room_switch[2] = SWITCH_OFF;
        ui->room3_light->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    }
}


