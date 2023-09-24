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
    QAbstractButton::connect( ui->tcpText, SIGNAL( returnPressed()), this, SLOT( sendMessage() ) );
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
    QObject::connect( connectDialog, &Connect::rejected, this, &TCPExample::disconnected);
    connectDialog->show();
}

void TCPExample::connected(Client* newClient) {
    client = std::unique_ptr<Client>(newClient);
    QObject::connect( newClient, &Client::displayError, this, &TCPExample::error);
    ui->lblInfo->setText("Connected!...");
    ui->connect->setText("Connect");
    ui->connect->setHidden(true);
    ui->connect->setDisabled(false);
    ui->send->setDisabled(false);
    ui->tcpText->setDisabled(false);
}

void TCPExample::disconnected() {
    client->disconnect();
    ui->lblInfo->setText("Disconnected...");
    ui->connect->setText("Connect");
    ui->connect->setDisabled(false);
    ui->connect->setHidden(false);
    ui->listen->setDisabled(false);
    ui->tcpText->setDisabled(true);
    ui->send->setDisabled(true);
}

void TCPExample::listen() {
    if (server != nullptr) {
        stopListening();
        return;
    }
    ui->listen->setText("Stop Listening");
    ui->lblInfo->setText("Listening!...");
    ui->tcpText->setDisabled(true);
    server = std::make_unique<Server>();
    QObject::connect( server.get(), SIGNAL( newConnection() ), this, SLOT( listening() ) );
    QObject::connect( server.get(), &Server::newMessage, this, &TCPExample::incomingMessage );
}

void TCPExample::stopListening() {
    server = nullptr;
    ui->listen->setText("Listen");
    ui->lblInfo->setText("Stopped listening...");
    ui->connect->setDisabled(false);
}

void TCPExample::listening(const QHostAddress& ipAddress) {
    ui->lblInfo->setText(tr("The server is running on\n\nIP: %1\nport: %2")
                                 .arg(ipAddress.toString()).arg(6000));
}

void TCPExample::error(const QString &string) {
    ui->lblInfo->setText(string);
    disconnected();
}

void TCPExample::sendMessage() {
    client->sendNewMessage(ui->tcpText->text());
    ui->tcpText->setText("");
}

void TCPExample::incomingMessage(const QString &string) {
    ui->lstMessages->addItem(string);
}

