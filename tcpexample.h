#ifndef TCPEXAMPLE_H
#define TCPEXAMPLE_H

#include <QMainWindow>
#include <QPushButton>
#include <QDialog>
#include <QtGui>
#include "Server.h"
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TCPExample; }
QT_END_NAMESPACE

class TCPExample : public QMainWindow
{
    Q_OBJECT

public:
    TCPExample(QWidget *parent = nullptr);
    ~TCPExample();

public slots:
    void connect();
    void connected(Client* newClient);
    void disconnected();
    void listen();
    void listening(const QHostAddress&);
    void error(const QString& string);
    void sendMessage();
    void incomingMessage(const QString& string);
private:
    void stopListening();
    Ui::TCPExample *ui;
    std::unique_ptr<Server> server;
    std::unique_ptr<Client> client;
};

#endif // TCPEXAMPLE_H
