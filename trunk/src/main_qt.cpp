#include <qt\qapplication.h>
#include <qt\qlabel.h>
#include <qt\qpushbutton.h>
#include <string>
#include <windows.h>

#include "include\GUI\QT\mainwindow.h"



int main(int argc, char *argv[])
{
     std::string directory = argv[0];
     size_t find;
     find = directory.find_last_of("/\\");
     directory = directory.substr(0,find);

	 Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    MainWindow mainWin;
	mainWin.model.currentDirectory=directory;
	mainWin.ShowDeckList();
    mainWin.show();
    return app.exec();


}
  