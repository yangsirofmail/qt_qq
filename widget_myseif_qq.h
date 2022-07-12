#ifndef WIDGET_MYSEIF_QQ_H
#define WIDGET_MYSEIF_QQ_H

#include <QWidget>
#include<QUdpSocket>

namespace Ui {
class Widget_Myseif_QQ;
}

class Widget_Myseif_QQ : public QWidget
{
    Q_OBJECT

    //创建枚举、分别代表 聊天信息、新用户加入、用户退出
        enum MsgType{Msg,UserEnter,UserLeave};
public:
    explicit Widget_Myseif_QQ(QWidget *parent ,QString name);//给构造函数添加形参
    ~Widget_Myseif_QQ();

private:
    Ui::Widget_Myseif_QQ *ui;

    //自定义信号
signals:
      //关闭窗口发送信号
    void closewindow();
public:

    //关闭窗口事件  也就是关闭窗口时会发送的事件  重写后、发送自己的信号标识、别人接收后就直到关闭窗口了、吧需要改变的值改过来即可
    void closeEvent(QCloseEvent *);

    void sndMsg(MsgType type);//广播udp消息
    void userEndter(QString username);//处理新用户加入
    void userLeave(QString username,QString time);//处理用户离开
    QString getUser();//获取用户名
    QString getMsg();//获取聊天记录


private:
    QUdpSocket*udpSocket;//声明套接字
    qint16 port;//创建端口
    QString uName;//创建用户名
    void  ReceiveMessage();//接受udp消息
};

#endif // WIDGET_MYSEIF_QQ_H
