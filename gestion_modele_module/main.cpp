#include "boisstylemodelewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BoisStyleModeleWindow w;
    w.show();
    return a.exec();
}
