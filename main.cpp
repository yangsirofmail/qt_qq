#include "widget_myseif_qq.h"
#include <QApplication>
#include<dialog_list.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //默认运行的myseif_qq
//    Widget_Myseif_QQ w;
//    w.show();

    //改成运行设计师类 dialog list
    Dialog_list list;
    list.show();

    return a.exec();
}
