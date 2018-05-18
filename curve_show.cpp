#include "curve_show.h"
#include "ui_curve_show.h"
#include <QVector>
#include <QtSql>
#include <QDebug>
#include <QMetaEnum>
#include <QTime>
#include <QTimer>
static int i = 0;
 int const Room_Number=3;
//调用时会首先调用curve_show构造函数，然后调用on_dateEdit_dateChanged函数（因为日历框数据产生了变化）然后在on_dateEdit_dateChanged函数中调用init_curve
//初始化表格然后调用read_sqlite函数，将数据库中的当天所有数据保存在 temp_data,temp_time， humi_data,humi_time 容器中， 然后调用SimpleDemo函数将数据送到表格中。
void curve_show::init_curve ( QCustomPlot *CustomPlot ) { /* 初始化显示的背景 */
    /*--------------------------设置背景色--------------------------*/



   qDebug()<<"init_curve";
    QLinearGradient plotGradient;
    plotGradient.setStart ( 0, 0 );
    plotGradient.setFinalStop ( 0, 350 );
    plotGradient.setColorAt ( 0, QColor ( 0, 0, 0 ) );
    plotGradient.setColorAt ( 1, QColor ( 0,0, 0 ) );
    CustomPlot->setBackground ( plotGradient );//整个界面的背景色
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart ( 0, 0 );
    axisRectGradient.setFinalStop ( 0, 350 );
    axisRectGradient.setColorAt ( 0, QColor ( 125, 125, 125 ) );
    axisRectGradient.setColorAt ( 1, QColor ( 125, 125, 125 ) );
    CustomPlot->axisRect()->setBackground ( axisRectGradient );//填充表格的背景色
    CustomPlot->xAxis->setBasePen ( QPen ( Qt::white, 1 ) );//坐标轴的颜色
    CustomPlot->yAxis->setBasePen ( QPen ( Qt::white, 1 ) );
    CustomPlot->xAxis->setTickPen ( QPen ( Qt::black, 1 ) );//坐标轴大格点底部颜色
    CustomPlot->yAxis->setTickPen ( QPen ( Qt::white, 1 ) );
    CustomPlot->xAxis->setSubTickPen ( QPen ( Qt::white, 1 ) );//坐标轴小格点底部颜色
    CustomPlot->yAxis->setSubTickPen ( QPen ( Qt::red, 1 ) );
    CustomPlot->xAxis->setTickLabelColor ( Qt::white );//坐标轴数据的颜色
    CustomPlot->yAxis->setTickLabelColor ( Qt::white );
    CustomPlot->xAxis->grid()->setPen ( QPen ( QColor ( 255, 0, 0 ), 1, Qt::DotLine ) );//大格点的网格颜色
    CustomPlot->yAxis->grid()->setPen ( QPen ( QColor ( 140, 140, 140 ), 1, Qt::DotLine ) );
    CustomPlot->xAxis->grid()->setSubGridPen ( QPen ( QColor ( 0, 0, 255 ), 1, Qt::DotLine ) );//小格点的网格颜色
    CustomPlot->yAxis->grid()->setSubGridPen ( QPen ( QColor ( 80, 80, 80 ), 1, Qt::DotLine ) );
    CustomPlot->xAxis->grid()->setSubGridVisible ( true );//是否有小格点的网格显示
    CustomPlot->yAxis->grid()->setSubGridVisible ( true );
    CustomPlot->xAxis->grid()->setZeroLinePen ( Qt::NoPen );
    CustomPlot->yAxis->grid()->setZeroLinePen ( Qt::NoPen );
    CustomPlot->xAxis->setUpperEnding ( QCPLineEnding::esSpikeArrow );
    CustomPlot->yAxis->setUpperEnding ( QCPLineEnding::esSpikeArrow );
    /*-----------------------设置横纵坐标-----------------------------*/
    CustomPlot->xAxis->setLabelColor ( QColor ( 255, 0, 0 ) );//设置下标字体颜色
    CustomPlot->xAxis->setLabel ( QObject::tr ( "时间" ) );//设置下标文字
    CustomPlot->yAxis->setLabelColor ( QColor ( 255, 0, 0 ) );
    CustomPlot->yAxis->setLabel ( QObject::tr ( "曲线显示" ) );

    CustomPlot->yAxis->setRange ( -10, 100);//设置y坐标轴的大小范围
    CustomPlot->axisRect()->setupFullAxesBox();
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(CustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), CustomPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(CustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), CustomPlot->yAxis2, SLOT(setRange(QCPRange)));

    /*-----------------------设置图例-----------------------------*/
    CustomPlot->legend->setVisible ( true );
    CustomPlot->legend->setFont ( QFont ( "Helvetica", 9 ) );
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    if ( i == 0 ) {
        temp_graph = CustomPlot->addGraph(); /* 显示温度曲线 */
        humi_graph = CustomPlot->addGraph(); /* 显示湿度曲线 */
        //QCPScatterStyle设置数据显示的形状，这里是圆形；QPen是数据外围的颜色和数据显示的大小；QBrush表示图例中数据内部的颜色和大小
        temp_graph->setScatterStyle ( QCPScatterStyle ( QCPScatterStyle::ssCircle, QPen ( Qt::black, 1.5 ), QBrush ( Qt::green ), 9 ) );
        temp_graph->setPen ( QPen ( QColor ( 255, 0, 0 ), 2 ) );//图例中连接数据曲线的颜色和大小
        temp_graph->setName ( QObject::tr ( "温度" ) );//图例中数据的名称
        humi_graph->setScatterStyle ( QCPScatterStyle ( QCPScatterStyle::ssSquare, QPen ( Qt::blue, 1.5 ), QBrush ( Qt::blue ), 9 ) );
        humi_graph->setPen ( QPen ( QColor ( 0, 0, 255 ), 2 ) );
        humi_graph->setName ( QObject::tr ( "湿度" ) );
        i = 1;
        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
       temp_graph->rescaleAxes();
        humi_graph->rescaleAxes();
    }
 //ui->qCustomPlot->xAxis->setRange(1, 50);



}
 void curve_show::read_sqlite ( const QString sql_name ) /* 从数据库中读取信息 */
{
     qDebug()<<"read_sqlite";
 QSqlDatabase db;
 if ( QSqlDatabase::contains ( "qt_sql_default_connection" ) ) {
     db = QSqlDatabase::database ( "qt_sql_default_connection" );
 } else {
     db = QSqlDatabase::addDatabase ( "QSQLITE" );
     qDebug()<<tr("使用qsqlite数据库");
 }
 QString str=QString("_%1").arg(Select_Number);
 db.setDatabaseName ( sql_name +str + ".db" );
qDebug()<< "读取的数据库"<<sql_name +str + ".db";
 if ( !db.open() ) {
     qDebug() <<  QObject::tr ( "数据库不能打开" ) ;
 }

 QSqlQuery query;
 query.exec ( "select * from measure_temp" );

 for ( int num = 1; query.next()>0; ) {//将数据库中的数据都存到相应的QVector中

      qDebug()<<"辨别编号了";
     temp_time.append (query.value ( 1 ).toDouble());
     temp_data.append ( query.value ( 2 ).toDouble() );
     humi_time.append (query.value ( 1 ).toDouble());
     humi_data.append (query.value ( 3 ).toDouble());
  num+= 1;
}
db.close(); /* 关闭数据库 */

 }

 void curve_show::Clear(void)
 {
     qDebug()<<"Clear";
     ui->qCustomPlot->clearGraphs();
     temp_data.clear();
     temp_time.clear();
     humi_data.clear();
     humi_time.clear();

 }
void curve_show::SimpleDemo ( QCustomPlot *CustomPlot ) {
    qDebug()<<"SimpleDemo";
    i=0;
    init_curve(ui->qCustomPlot);
  temp_graph->addData( temp_time, temp_data );
  humi_graph->addData( humi_time, humi_data );
  CustomPlot->replot();
    //Clear();

}

curve_show::curve_show ( QWidget *parent ) : QDialog ( parent ), ui ( new Ui::curve_show ) {
     qDebug()<<"curve_show";
    ui->setupUi ( this );
    this->setWindowTitle ( tr ( "历史曲线显示" ) ); /* 设置标题 */
    ui->dateEdit ->setDate ( QDate::currentDate() ); /* 设置日期为当前日期 */
    ui->dateEdit->setCalendarPopup ( true ); /* 设置界面可以弹出日历 */
    Select_Number= (double)ui->House_Number->value();
    Timer = new QTimer();
    Timer->setInterval ( 1000 ); /* 设置定时时间为1秒钟 */
    connect ( Timer, SIGNAL ( timeout() ), this, SLOT ( flash( ) ) ); /* 超时信号和测量的槽连接 */
    Timer->start(); /* 启动定时器 */
    ui->label_right->setText(tr("IPv6农业大棚客户端V1.0"));
    qDebug()<<"Select_Number"<<Select_Number;
    ui->House_Number->setMaximum(Room_Number);
     ui->House_Number->setMinimum(1);
}

curve_show::~curve_show() {
    i = 0;
    delete ui;
     qDebug()<<"~curve_show";
}


void curve_show::on_dateEdit_dateChanged ( const QDate &date ) {
     qDebug()<<"on_dateEdit_dateChanged";
    Clear();
   // init_curve ( ui->qCustomPlot );
    select_date = date.toString ( "yyyy-MM-dd" );
    read_sqlite ( select_date );
    SimpleDemo ( ui->qCustomPlot );
    qDebug() << QObject::tr ( "打开数据库" ) << select_date << ".db";
    current_date=QDate::currentDate().toString("yyyy-MM-dd");
        if(select_date==current_date)
        {
            flash();
        }
flash();

}

void curve_show::flash(void )
{
    qDebug()<<"flash";
    current_date=QDate::currentDate().toString("yyyy-MM-dd");
        if(select_date==current_date)
            {
            Clear();
            QDate date=ui->dateEdit->date();
            QString date_s=date.toString("yyyy-MM-dd");
            read_sqlite(date_s);
            //read_sqlite(date_s);
            qDebug()<<"flash";
            //init_curve ( ui->qCustomPlot );
           // ui->qCustomPlot ->setLocale(QLocale(QLocale::Chinese,QLocale::China));
            // seconds of current time, we'll use it as starting point in time for data:
            QTime time(QTime::currentTime());
           // calculate two new data points:
            double now = time.msecsSinceStartOfDay()/1000+57600;
            QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
            dateTicker->setDateTimeFormat("hh:mm:ss");
             ui->qCustomPlot->xAxis->setTicker(dateTicker);
             ui->qCustomPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
             ui->qCustomPlot->xAxis->setRange((now-60), now);
            SimpleDemo ( ui->qCustomPlot );

        }


}
void curve_show:: history(void)
{
    Clear();
    qDebug()<<"history";
    QTime Begin(00,00,00);
    QTime End(23,59,59);
    double begin=Begin.msecsSinceStartOfDay()/1000+57600;
    double end=End.msecsSinceStartOfDay()/1000+57600;
    qDebug()<<current_date;
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
     ui->qCustomPlot->xAxis->setTicker(dateTicker);
     ui->qCustomPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->qCustomPlot->xAxis->setRange(begin, end);
    SimpleDemo ( ui->qCustomPlot );

}




void curve_show::on_House_Number_valueChanged(int arg1)
{
Clear();
     qDebug()<<"on_House_Number_valueChanged";
    QSqlDatabase db;
    if ( QSqlDatabase::contains ( "qt_sql_default_connection" ) ) {
        db = QSqlDatabase::database ( "qt_sql_default_connection" );
    } else {
        db = QSqlDatabase::addDatabase ( "QSQLITE" );
        qDebug()<<tr("使用qsqlite数据库");
    }
 QString str=QString("_%1").arg(arg1);
    db.setDatabaseName ( select_date +str+ ".db" );

    if ( !db.open() ) {
        qDebug() <<  QObject::tr ( "数据库不能打开" ) ;
    }

    QSqlQuery query;
    query.exec ( "select * from measure_temp" );

    for ( int num = 1; query.next()>0; ) {//将数据库中的数据都存到相应的QVector中

        temp_time.append (query.value ( 1 ).toDouble());
        temp_data.append ( query.value ( 2 ).toDouble() );
        humi_time.append (query.value ( 1 ).toDouble());
        humi_data.append (query.value ( 3 ).toDouble());
     num+= 1;
}
 db.close(); /* 关闭数据库 */
 Select_Number=(double)arg1;
 SimpleDemo(ui->qCustomPlot);
}
