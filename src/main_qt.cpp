#include <qt\qapplication.h>
#include <qt\qlabel.h>
#include <qt\qpushbutton.h>

#include "include\GUI\QT\mainwindow.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    return app.exec();


}
  