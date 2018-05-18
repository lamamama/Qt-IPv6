#ifndef MAIN_DIALOG_H
#define MAIN_DIALOG_H

#include <QDialog>
#include "qcustomplot/qcustomplot.h"
#include "measure_timer.h"
#include "curve_show.h"
#include <QObject>
namespace Ui {
    class main_Dialog;
}
extern int current_sel; /* 当前选择的大棚 */
extern temp_humi Temp_Humi[3];

class main_Dialog : public QDialog {
    Q_OBJECT
public:
    explicit main_Dialog ( QWidget *parent = 0 );
    ~main_Dialog();
    friend class measure_timer;

private:
     QTimer *timer;
    Ui::main_Dialog *ui;
    measure_timer *temp_timer; /* 测量节点的定时器 */
    void sql_create ( QString sql_name ); /* create a sql */
    QString current_date;

    curve_show *curveShow;
private slots:
    void receivelogin ( void );
    void on_temp_data_clicked();
    void on_choose_class_currentIndexChanged(int index);
    void on_control_button_clicked();
    void show_temp_humi ( void ); /* 在主界面上显示温度和湿度 */

};



#endif // MAIN_DIALOG_H
