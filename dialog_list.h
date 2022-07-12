#ifndef DIALOG_LIST_H
#define DIALOG_LIST_H

#include <QWidget>
#include<QToolButton>
#include<QVector>
#include<QColorDialog>
#include<QWidget>
#include<QMessageBox>
namespace Ui {
class Dialog_list;
}

class Dialog_list : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog_list(QWidget *parent = nullptr);
    ~Dialog_list();

private:
    Ui::Dialog_list *ui;


};

#endif // DIALOG_LIST_H
