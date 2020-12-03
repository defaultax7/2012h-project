#include <QApplication>
#include <mania_test/mania_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mania_window w; //lets see
    w.show();
    return a.exec();
}
