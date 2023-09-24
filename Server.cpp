
#include <QMessageBox>
#include "Server.h"
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QTcpSocket>
#include <iostream>

Server::Server() {
    initServer();
}

void Server::initServer() {
    tcpServer = new QTcpServer(this);
    QObject::connect( tcpServer, SIGNAL( clicked() ), this, SLOT( listen() ) );
    if (!tcpServer->listen(QHostAddress::Any, 6000)) {
        error(tcpServer->errorString());
        return;
    }

    QString ipAddress;
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (const QHostAddress &entry : ipAddressesList) {
        if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
            ipAddress = entry.toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    std::cout << "Server Connected: " << ipAddress.toStdString() << std::endl;
    emit newConnection(QHostAddress(ipAddress));
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::nextPendingConnection);
}

void Server::nextPendingConnection() {
    std::cout << "Server: New Connection" << std::endl;
    QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_6_5);
    QObject::connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    QObject::connect(clientConnection, &QAbstractSocket::readyRead, this, &Server::incomingMessage);
}

void Server::incomingMessage() {
    std::cout << "Server: Incoming Message" << std::endl;
    in.startTransaction();

    QString message;
    in >> message;

    if (!in.commitTransaction())
        return;

    emit newMessage(message);
}

Server::~Server() {
    tcpServer->disconnect();
    tcpServer->close();
}
