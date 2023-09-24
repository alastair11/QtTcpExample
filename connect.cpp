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
    QAbstractButton::connect( ui->btnConnect, SIGNAL( clicked() ), this, SLOT( tcpConnect() ) );
}

Connect::~Connect() {
    delete ui;
}

void Connect::tcpConnect() {
    Client* client = new Client(ui->txtIp->text(), ui->txtPort->text());
    emit newConnection(client);
    this->accept();
}
