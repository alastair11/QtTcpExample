//
// Created by alast on 24/09/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Connect.h" resolved

#include "connect.h"
#include "ui_Connect.h"
#include <iostream>


Connect::Connect(QWidget *parent) :
        QDialog(parent), ui(new Ui::Connect) {
    ui->setupUi(this);
    QAbstractButton::connect( ui->btnConnect, SIGNAL( clicked() ), this, SLOT( attemptConnection() ) );
}

Connect::~Connect() {
    delete ui;
}

void Connect::attemptConnection() {
    ui->btnConnect->setDisabled(true);
    ui->btnConnect->setText("Connecting...");
    client = new Client(ui->txtIp->text(), ui->txtPort->text());
    QObject::connect( client, SIGNAL( connected() ), this, SLOT( tcpConnect() ) );
    QAbstractButton::connect( client, &Client::displayError, this, &Connect::showError);
}

void Connect::tcpConnect() {
    emit newConnection(client);
    this->accept();
}

void Connect::showError(const QString& txt) {
    ui->lblError->setText(txt);
    ui->btnConnect->setDisabled(false);
}
