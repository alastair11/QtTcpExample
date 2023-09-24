#ifndef TCPEXAMPLE_SERVER_H
#define TCPEXAMPLE_SERVER_H

#include <QtNetwork/QTcpServer>

class Server : public QObject {
Q_OBJECT
public:
    Server();
    ~Server() override {delete tcpServer;};
signals:
    void newConnection(const QHostAddress&);
    void error(const QString&);
    void newMessage(const QString&);
public slots:
    void incomingMessage();
private:
    void initServer();

    QTcpServer *tcpServer = nullptr;
    QDataStream in;

    void nextPendingConnection();
};


#endif //TCPEXAMPLE_SERVER_H
