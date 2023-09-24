#include "tcpexample.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPExample w;
    w.show();
    return a.exec();
}
