//
// Created by alast on 24/09/2023.
//

#ifndef TCPEXAMPLE_CONNECT_H
#define TCPEXAMPLE_CONNECT_H

#include <QDialog>
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Connect; }
QT_END_NAMESPACE

class Connect : public QDialog {
Q_OBJECT

public:
    explicit Connect(QWidget *parent = nullptr);
    ~Connect() override;

signals:
    void newConnection(Client* client);

public slots:
    void tcpConnect();

private:
    Ui::Connect *ui;
};


#endif //TCPEXAMPLE_CONNECT_H
