#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QDialog>
#include <QAbstractSocket>

class QTcpSocket;

class tcp_client : public QDialog {
    Q_OBJECT
private:
    Ui::tcp_client *ui;
    QTcpSocket *tcpSocket;
public:
    explicit tcp_client ( QWidget *parent = 0 );
signals:

public slots:
    void readMessage();
    void displayError ( QAbstractSocket::SocketError );
    void sendMessage();
};

#endif // TCP_CLIENT_H
