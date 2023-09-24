//
// Created by alast on 24/09/2023.
//

#ifndef TCPEXAMPLE_CLIENT_H
#define TCPEXAMPLE_CLIENT_H

#include <QTcpSocket>

class Client : public QObject {
Q_OBJECT
public:
    Client(const QString& host, const QString& port);
    ~Client() override { delete tcpSocket;};
signals:
    void displayError(const QString& string);

public slots:
    void sendNewMessage(const QString& str);
    void tcpError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* tcpSocket = nullptr;
};


#endif //TCPEXAMPLE_CLIENT_H
