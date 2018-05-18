#ifndef CURVE_SHOW_H
#define CURVE_SHOW_H

#include <QDialog>
#include "qcustomplot/qcustomplot.h"
#include "measure_timer.h"

namespace Ui {
    class curve_show;
}

class curve_show : public QDialog {
    Q_OBJECT
public:
    explicit curve_show ( QWidget *parent = 0 );
    ~curve_show();

private slots:
    void on_dateEdit_dateChanged(const QDate &date);//用于日历数据变化时，更新曲线图
    void flash( void);//用于定时刷新曲线图中的实时数据
    void history(void);//用于不是当天日期的历史记录曲线图
    //void on_dateEdit_editingFinished();
   // void realtimeDataSlot( QCustomPlot *CustomPlot);

    void on_House_Number_valueChanged(int arg1);//用于当显示不同大棚的数据

private:
    Ui::curve_show *ui;
    double Select_Number;//用于保存曲线图上不同大棚的编号
    QString current_date; /* 当前日期字符串 */
    QString select_date; /* 选择日期字符串 */
    QCPGraph *temp_graph; /* 绘制温度曲线 */
    QCPGraph *humi_graph; /* 绘制湿度曲线 */
    QVector<double> temp_data,temp_time; /* 定义温度的数据以及对应的时间 */
    QVector<double> humi_data,humi_time ; /* 定义温度的数据以及对应的时间 */
    void SimpleDemo ( QCustomPlot *CustomPlot );//用于将更新的数据显示在曲线图中
    void init_curve ( QCustomPlot *CustomPlot );//初始化或更新曲线图基本设置
    void read_sqlite ( const QString sql_name ); /* 从数据库中读取信息 */
    void Clear(void);//清除缓存区数据和清除两个曲线
    QTimer dataTimer;
     QTimer *Timer;//为了实时刷新曲线图

};

/* 定义号码和IPv6地址集合的结构体 */
typedef struct room_addr {
    int index;
    QString ipv6_addr;
} room_addr;

typedef struct temp_humi {
    int number;
    QString temp;
    QString humi;
} temp_humi;

extern const room_addr addr_set[];

#endif // CURVE_SHOW_H
