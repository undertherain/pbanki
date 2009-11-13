#include <qt\qapplication.h>
#include <qt\qlabel.h>
#include <qt\qpushbutton.h>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    QLabel *label = new QLabel(QString::fromUtf8("__‘_ - ó_úø!",-1));
	
    QLabel *label = new QLabel(QString("lalala!"));
    label->show();
    return app.exec();
 

}
  