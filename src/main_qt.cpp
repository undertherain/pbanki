#include <qt\qapplication.h>
#include <qt\qlabel.h>
#include <qt\qpushbutton.h>

#include "include\GUI\QT\mainwindow.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
//    QLabel *label = new QLabel(QString::fromUtf8("__‘_ - ó_úø!",-1));
	
//    QLabel *label = new QLabel(QString("lalala!"));
//    label->show();
    MainWindow mainWin;
    mainWin.show();

    return app.exec();
 

}
  