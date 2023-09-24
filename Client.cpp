//
// Created by alast on 24/09/2023.
//

#include <QMetaEnum>
#include "Client.h"
#include <iostream>

Client::Client(const QString& host, const QString& port) : tcpSocket(new QTcpSocket()) {
    connect(tcpSocket, &QAbstractSocket::errorOccurred, this, &Client::tcpError);
    connect(tcpSocket, &QAbstractSocket::connected, this, &Client::connected);
    connect(tcpSocket, &QAbstractSocket::disconnected, this, &Client::disconnected);

    tcpSocket->abort();
    std::cout << "Client Connecting to: " << host.toStdString() << ":" << port.toStdString() << std::endl;
    tcpSocket->connectToHost(host, port.toInt());
}

void Client::sendNewMessage(const QString& str) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    out << str;
    std::cout << "Client Send: " << str.toStdString() << std::endl;
    tcpSocket->startTransaction();
    tcpSocket->write(block);
    tcpSocket->commitTransaction();
}

void Client::tcpError(QAbstractSocket::SocketError socketError) {
    std::cout << "ERROR" << std::endl;
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    emit displayError(metaEnum.valueToKey(socketError));
}

Client::~Client() {
    tcpSocket->disconnect();
    tcpSocket->close();
    delete tcpSocket;
}

void Client::disconnect() {
    tcpSocket->disconnect();
    tcpSocket->close();
};
