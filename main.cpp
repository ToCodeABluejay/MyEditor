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
    {
        w.set_filename(argv[1]);
        w.open_file();
    }

    w.load_settings();

    return a.exec();
}
