#include "myeditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyEditor w;
    w.show();
    return a.exec();
}
