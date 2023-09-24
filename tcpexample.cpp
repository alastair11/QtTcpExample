#include "tcpexample.h"
#include "./ui_tcpexample.h"
#include "connect.h"

TCPExample::TCPExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TCPExample)
{
    ui->setupUi(this);
    QAbstractButton::connect( ui->connect, SIGNAL( clicked() ), this, SLOT( connect() ) );
    QAbstractButton::connect( ui->disconnect, SIGNAL( clicked() ), this, SLOT( disconnected() ) );
    QAbstractButton::connect( ui->listen, SIGNAL( clicked() ), this, SLOT( listen() ) );
    QAbstractButton::connect( ui->send, SIGNAL( clicked() ), this, SLOT( sendMessage() ) );
}

TCPExample::~TCPExample()
{
    delete ui;
}

void TCPExample::connect() {
    ui->connect->setText("Connecting...");
    ui->connect->setDisabled(true);

    Connect* connectDialog = new Connect();
    QObject::connect( connectDialog, &Connect::newConnection, this, &TCPExample::connected);
    connectDialog->show();
}

void TCPExample::connected(Client* newClient) {
    client = std::unique_ptr<Client>(newClient);
    QObject::connect( newClient, &Client::displayError, this, &TCPExample::error);
    ui->connect->setText("Connect");
    ui->connect->setHidden(true);
    ui->connect->setDisabled(false);
    ui->send->setDisabled(false);
    ui->tcpText->setDisabled(false);
}

void TCPExample::disconnected() {
    client = nullptr;
    ui->connect->setHidden(false);
    ui->listen->setDisabled(false);
    ui->tcpText->setDisabled(true);
}

void TCPExample::listen() {
    ui->listen->setText("Stop Listening");
    ui->tcpText->setDisabled(true);
    server = std::make_unique<Server>();
    QObject::connect( server.get(), SIGNAL( newConnection() ), this, SLOT( listening() ) );
    QObject::connect( server.get(), &Server::newMessage, this, &TCPExample::incomingMessage );
}

void TCPExample::stopListening() {
    ui->listen->setText("Listen");
    ui->connect->setDisabled(false);
}

void TCPExample::listening(const QHostAddress& ipAddress) {
    ui->lblInfo->setText(tr("The server is running on\n\nIP: %1\nport: %2")
                                 .arg(ipAddress.toString()).arg(6000));
}

void TCPExample::error(const QString &string) {
    ui->lblInfo->setText(string);
}

void TCPExample::sendMessage() {
    client->sendNewMessage(ui->tcpText->document()->toPlainText());
}

void TCPExample::incomingMessage(const QString &string) {
    ui->lstMessages->addItem(string);
}

