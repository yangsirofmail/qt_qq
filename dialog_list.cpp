#include "dialog_list.h"
#include "ui_dialog_list.h"

#include"widget_myseif_qq.h"

//全局vector
QVector<bool> isshow;//保存用户状态
QVector<QToolButton *>vtoolbtn;//保存用户数据

Dialog_list::Dialog_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog_list)
{
    ui->setupUi(this);


    //修改图片和标题
    setWindowIcon(QPixmap("://man/2.png"));
    setWindowTitle("QQ");

    //准备图片和姓名资源
    QList<QString>listname;
    listname<<"熊大"<<"熊二"<<"汤姆"<<"杰瑞"<<"海绵宝宝"<<"哆啦A梦";

    QStringList listicon;
    listicon<<"1"<<"2"<<"3"<<"4"<<"5"<<"33";



   for(int i=0;i<5;i++)
   {
    //设置图像
    QToolButton *btn=new QToolButton;
   // btn->setText("斧头帮主");
    //btn->setIcon(QPixmap("://man/1.png"));
//将固定的值改成准备好的名字和图片
    btn->setText(listname[i]);
    QString str=QString("://man/%1.png").arg(listicon.at(i));//格式化字符串
        btn->setIcon(QPixmap(str));
    //设置图像大小
  // btn->setIconSize(QPixmap(str).size());// 根据图片大小进行设置
   btn->setIconSize(QSize(110,110));//将·图片设置成固定大小



     //设置字体
     QFont font;
     font.setPixelSize(18);//设置大小
     font.setBold(true);//设置加粗
     font.setUnderline(true);//下划线
     font.setItalic(true);//斜体

     //设置调色板
     QPalette palat;
     //通过设置调试版的按钮文本给toolbutton设置颜色
     palat.setColor(QPalette::ButtonText,Qt::red);//红色
     //给按钮的文本上色
     btn->setPalette(palat);

btn->setFont(font);
    //设置透明
    btn->setAutoRaise(true);
    //设置文字和图片一起显示
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //加到垂直布局也就是找爹
ui->vLayout->addWidget(btn);


//vector 保存5个按钮
vtoolbtn.push_back(btn);//对数据进行维护
isshow.push_back(false);//设置初始状态
   }


   //对按钮添加信号槽
   for(int i=0;i<vtoolbtn.size();i++)
   {
       connect(vtoolbtn[i],&QToolButton::clicked,[=](){
//如果打开就不用执行下面代码
           if(isshow[i])
           {QString str=QString("%1 窗口已经被打开了").arg(vtoolbtn[i]->text());
               QMessageBox::warning(this,"警告",str);
               return ;
           }
           isshow[i]=true;
//弹出对话框、告诉窗口调用的对话框名字
           Widget_Myseif_QQ *widge=new Widget_Myseif_QQ(0,vtoolbtn[i]->text());//创建已经搭建好的窗口布局对象 0代表顶部弹出 传参是为了获取名字

           widge->setWindowTitle(vtoolbtn[i]->text());//设置弹出的窗口标题
           widge->setWindowIcon(vtoolbtn[i]->icon());//设置弹出的窗口图标
           widge->show();


           //接受信号
           connect(widge,&Widget_Myseif_QQ::closewindow,[=](){          isshow[i]=false;        });

       });
   }
}

Dialog_list::~Dialog_list()
{
    delete ui;
}
