#include "myeditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MyEditor");
    a.setApplicationVersion("0.1");

    MyEditor w;
    w.show();

    if (argc)
        w.open_file(argv[1]);

    w.load_settings();

    return a.exec();
}
