#include "mynotes.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myNotes w;
    w.show();

    return a.exec();
}
