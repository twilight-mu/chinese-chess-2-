#include "zhujiemian.h"
#include <QApplication>
#include "board.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
