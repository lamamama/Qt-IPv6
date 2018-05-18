#include "tcp_client.h"
#include <QtNetwork>
#include <QDebug>

tcp_client::tcp_client ( QWidget *parent ) : QDialog ( parent ) , ui ( new Ui::Dialog ) {
    tcpSocket = new QTcpSocket ( this );
    this->connect ( tcpSocket, SIGNAL ( readyRead() ), this, SLOT ( readMessage() ) );
    this->connect ( tcpSocket, SIGNAL ( error ( QAbstractSocket::SocketError ) ), this, SLOT ( displayError ( QAbstractSocket::SocketError ) ) );
    tcpSocket->abort(); /* 取消已有的连接 */
    tcpSocket->connectToHost ( "fe::8477:7d4b:1cf8:11bc", 8888 ); /* 和服务器之间建立连接 */

    QString strMesg = "coap://[aaaa::212:4b00:ef3:418c]/test/hello get";
    qDebug() << strMesg;
    tcpSocket->write ( strMesg.toStdString().c_str(), strlen ( strMesg.toStdString().c_str() ) );
}

void tcp_client::readMessage() {
    QByteArray qba = tcpSocket->readAll(); /* 读取信息 */
    QString ss = QVariant ( qba ).toString();
    qDebug() << ss;
}

void tcp_client::displayError ( QAbstractSocket::SocketError ) {
}

void tcp_client::sendMessage() {

}

