#include "widget_myseif_qq.h"
#include "ui_widget_myseif_qq.h"
#include<QWidget>
#include<QString>
#include<QDataStream>//导入流头文件
#include<QMessageBox>
#include<QDateTime>
#include<QComboBox>
#include<QColorDialog>
#include<QFileDialog>
Widget_Myseif_QQ::Widget_Myseif_QQ(QWidget *parent,QString name) :
    QWidget(parent),
    ui(new Ui::Widget_Myseif_QQ)
{
    ui->setupUi(this);














//初始化


//在构造函数进行数据准备和初始化
    udpSocket=new QUdpSocket(this);
    //用户名获取
    uName=name;
    //端口号设置
    this->port=9999;
    //套接字绑定端口号、绑定模式(共享地址|断线重连服务器)
    //QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint 这个是共享关键代码、共享和重连是配套使用的
    udpSocket->bind(this->port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);





//发送新用户进入消息
     sndMsg(UserEnter);



//发送和接受信号

    //创建事件 点击发送按钮 、触发 sndMsg函数进行广播msg信号
    connect(ui->sendBtn,&QPushButton::clicked,[=](){  sndMsg(Msg);  });

    //创建快捷键


    //监听别人发送的数据 ,不用lambda式 将信号处理提炼出来ReceiveMessage
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget_Myseif_QQ::ReceiveMessage);

    //点击按钮关闭窗口
    connect(ui->exitBtn,&QPushButton::clicked,this,&Widget_Myseif_QQ::close);





    //辅助功能
    //字体、设置字体
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,[=](const QFont &font){
    ui->mesTextEdit->setCurrentFont(font);//将ui界面选中的字体传到currenFont
    ui->mesTextEdit->setFocus();//光标回到起始位置
    });

    //字号
    void (QComboBox::* cbxsingal)(const QString &text)=&QComboBox::currentIndexChanged;
    connect(ui->sizeCbx,cbxsingal,[=](const QString &text){

        ui->mesTextEdit->setFontPointSize(text.toDouble());
        ui->mesTextEdit->setFocus();
    });
    //加粗
    connect(ui->boldTBtn,&QToolButton::clicked,[=](bool ischeck){
        if(ischeck)
        {
            ui->mesTextEdit->setFontWeight(QFont::Bold);
        }else {
            {
                ui->mesTextEdit->setFontWeight(QFont::Normal);
            }
        }
    });

    //倾斜
    connect(ui->italicTBtn,&QToolButton::clicked,[=](bool check){

        ui->mesTextEdit->setFontItalic(check);
    });
    //下划线

    connect(ui->underlineTBtn,&QToolButton::clicked,[=](bool check){

        ui->mesTextEdit->setFontUnderline(check);
    });
    //颜色版

    connect(ui->colorTBtn,&QToolButton::clicked,[=](){
        QColor color=QColorDialog::getColor(Qt::red);
        ui->mesTextEdit->setTextColor(color);

    });
    //清空

connect(ui->clearTBtn,&QToolButton::clicked,[=](){ui->msgBrowser->clear();});

//保存

connect(ui->saveTBtn,&QToolButton::clicked,[=](){

    if(ui->msgBrowser->document()->isEmpty())//如果聊天记录框为空
    {
        QMessageBox::warning(this,"警告","内容不能为空");
        return ;
    }else
    {//保存对话框
        QString path=QFileDialog::getSaveFileName(this,"保存记录","聊天记录","(*.txt)");//构造函数 默认名称和保存过滤格式
        QFile file(path);
        //打开模式加上换行操作需要追加QIODevice::Text这个
        file.open(QIODevice::WriteOnly |QIODevice::Text);
        QTextStream stream(&file);
        stream<<ui->msgBrowser->toPlainText();//保存文本形式
        file.close();
    }



});


}






//广播udp消息  核心群聊功能
  void Widget_Myseif_QQ::sndMsg(MsgType type)
  {

      //消息分为三种类型、聊天  进入 离开
      //数据流可以对QByteArray进行分段处理

      //发送的数据分段处理、第一段类型、第二段、用户名、第三代、具体的内容
      QByteArray arr;
      QDataStream stream(&arr,QIODevice::WriteOnly);//只写模式打开
      //获取输入的类型和用户名 、添加到流中
      stream <<type<<getUser();

      //根据对应的类型 进行不同的操作
      switch (type)
      {
      case Msg://msg类型就是消息发送、将类型用户名消息三段进行拼接通过广播发送
          if(ui->mesTextEdit->toPlainText()=="")//获取输入框的内容 、如果为空就警告
          {
              QMessageBox::warning(this,"警告","发送内容不能为空");
              return ;
          }//追加第三段数据 、获取Msg的具体说话内容
          stream<<getMsg();
          break;

      case UserEnter://UserEnter类型就是新用户进入、将类型用户名新用户进入消息三段进行拼接通过广播发送
          break;
      case UserLeave://UserLeave用户离开、将类型用户名用户退出消息三段进行拼接通过广播发送
          break;
      default:
          break;


      }
   //书写报文 、将已经拼接好的bytearray进行广播、也就是发送共享地址 Broadcast默认是255.255.255.0 port是设置的同一端口号9999
      udpSocket->writeDatagram(arr,QHostAddress::Broadcast,port);
      //发送如果是全部人都看到那么是广播发送、需要传入QHostAddress::Broadcast、默认是255.255.255.255，直接传入也行、
      //如果是指定人看到、需要传入对方的端口和ip
      //上面第一个是类型和用户名、第二个是根据类型选择三种不同的情况进行拼接
      //将拼接好的两段报文、通过udpSocket套接字进行广播发送
  }




# if 1
//接受udp消息
void Widget_Myseif_QQ::ReceiveMessage()
{

//获取长度
    qint64 size=udpSocket->pendingDatagramSize();   //pendingDatagramSize获取数据长度

    //创建容器
       QByteArray array=QByteArray(size,0);

    //解析之前先拿到数据、需要临时容器和要获取数据的大小     readDatagram获取数据
   udpSocket->readDatagram(array.data(),size);




  //解析数据段后的数据存放容器
    int msgType;  //数据类型 0 是普通消息 1是用户进入 2是用户离开
    QString userName;  //用户名
    QString msg;  //消息

     //解析数据
    //第一段 类型 第二段 用户名 第三段 数据
    QDataStream stream(&array,QIODevice::ReadOnly);//只读形式打开


    //获取当前时间
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //读数据第一段、存放的msgType
    stream>>msgType;

    switch (msgType) //根据对应的类型进行判断 如果是0 那就是普通的消息、如果是1就是用户进入 如果 是2 那就是用户退出
    {
    case Msg://普通聊天
        stream>>userName>>msg;
        //设置聊天记录显示框内发送的文本颜色
        ui->msgBrowser->setTextColor(Qt::blue);
         //追加日期到聊天记录显示框
        ui->msgBrowser->append("["+userName+"]"+time);
        //同步聊天记录到聊天记录显示框
        ui->msgBrowser->append(msg);
        break;
    case UserEnter://用户进入

        //更新右侧talewidget
        stream>>userName;
        userEndter(userName);

        break;

      case UserLeave://出来用户离开
            stream>>userName;
        userLeave(userName,time);



    break;
    default:
        break;

    }


}
#endif
//处理新用户加入
   void Widget_Myseif_QQ::userEndter(QString username)
   {
       bool isempty=ui->userTableWidget->findItems(username,Qt::MatchExactly).isEmpty();//判断用户是否存在
       if(isempty)
       {
           QTableWidgetItem *user=new QTableWidgetItem(username);

           //插入行
           ui->userTableWidget->insertRow(0);
           //插入对象
           ui->userTableWidget->setItem(0,0,user);

           //设置文本颜色
           ui->msgBrowser->setTextColor(Qt::gray);
           //追加聊天记录
           ui->msgBrowser->append(QString("%1 上线了").arg(username));

           //更新在线人数
           ui->userNumLabel->setText(QString("在线人数：%1人").arg(ui->userTableWidget->rowCount()));

           //把自身信息广播出去
           sndMsg(UserEnter);

       }


   }



//处理用户离开
    void Widget_Myseif_QQ::userLeave(QString username,QString time)
    {

        //更新右侧列表

bool isout=ui->userTableWidget->findItems(username,Qt::MatchExactly).isEmpty();//判断用户是否存在
if(!isout)
{
    QTableWidgetItem *user=new QTableWidgetItem(username);

//获取当前行
    int row =ui->userTableWidget->findItems(username,Qt::MatchRegExp).first()->row();//获取当前行 存储在row
    //删除行
   ui->userTableWidget->removeRow(row);
   //修改文本颜色
   ui->msgBrowser->setTextColor(Qt::gray);
   //追加聊天记录
    ui->msgBrowser->append(QString("%1 离线了 在%2 离开").arg(username).arg(time));

    //更新在线人数
    ui->userNumLabel->setText(QString("在线人数：%1人").arg(ui->userTableWidget->rowCount()));


    }
    }



//获取用户名
  QString Widget_Myseif_QQ::getUser()
  {
      return this->uName;
  }



//获取聊天记录
QString Widget_Myseif_QQ::getMsg()
{
    QString str=ui->mesTextEdit->toHtml();//获取输入的字体内容、因为用到了字体改变所以不能写纯文本toPlainText

    //清空输入框
    ui->mesTextEdit->clear();

    //光标恢复到初始位置
    ui->mesTextEdit->setFocus();
    return str;
}



//关闭事件 重写 发送自己创建的信号 closewindow
void Widget_Myseif_QQ::closeEvent(QCloseEvent * e)
{
    emit this->closewindow();

    //发送用户离开的信号

    //把自身信息广播出去
    sndMsg(UserLeave);
    //断开套接字
    udpSocket->close();
    //销毁套接字
    udpSocket->destroyed();

    //如果还有别的事件 转发给基类处理
    QWidget::closeEvent(e);

}

///这时一个析构
Widget_Myseif_QQ::~Widget_Myseif_QQ()
{
    delete ui;
}
