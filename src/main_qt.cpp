
#include <Qt/qapplication.h>
#include <Qt/qlabel.h>
#include <Qt/qpushbutton.h>
#include "include/GUI/QT/mainwindow.h"

#include <string>


int main(int argc, char *argv[])
{
     std::string directory = argv[0];
     size_t find;
     find = directory.find_last_of("/\\");
     directory = directory.substr(0,find);

//	 Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    MainWindow mainWin;
	mainWin.model.currentDirectory=directory;
	mainWin.ShowDeckList();
    mainWin.show();


    //Dialog dialog;
    //dialog.show();

    return app.exec();

}
