#include "widget.h"
#include "ui_widget.h"
#include "QPushButton.h"

Widget::Widget ( QWidget *parent ) : QWidget ( parent ), ui ( new Ui::Widget ) {
    ui->setupUi ( this );
    ui->switchButton->setFlat ( true );
    ui->switchButton->setFocusPolicy ( Qt::NoFocus );
    ui->switchButton->setIcon ( QIcon ( ":/image/switch_off.png" ) );
    ui->switchButton->setIconSize ( QSize ( 80, 40 ) );
    ui->switchButton->setFixedSize ( QSize ( 80, 40 ) );
    ui->switchButton->setStyleSheet ( "QPushButton{background: transparent;}" );
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_switchButton_clicked(){
    static int i = 1;

    if(i == 0){
        ui->switchButton->setIcon( QIcon( ":/image/switch_off.png" ) );
        i = 1;
    }
    else {
        ui->switchButton->setIcon( QIcon( ":/image/switch_on.png" ) );
        i = 0;
    }
}
