#include "mainwindow.h"
#include <QtGui\QtGui>

MainWindow::MainWindow()
{
    //textEdit = new QPlainTextEdit;
	textEdit = new QTextEdit;

    setCentralWidget(textEdit);
	//textEdit->setReadOnly(true);
	textEdit->setHtml("la-<b>la</b>-la<br>Аня - коза :)<br>味噌");
	
    createActions();
    createMenus();

	readSettings();
	

}

void MainWindow::readSettings()
{
    QSettings settings("pbanki", "Application");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //if (maybeSave()) {
      //  writeSettings();
        event->accept();
    //} else {
      //  event->ignore();
    //}
}

void MainWindow::about()
{
   //QMessageBox::about(this, tr("About Application"),
     //       tr("The <b>MindCraft </b> about"));

   QMessageBox::about(this, tr("About Application"),
	   textEdit->toHtml());

}


void MainWindow::createActions()
{
    actExit = new QAction(tr("E&xit"), this);
    actExit->setShortcut(tr("Ctrl+Q"));
    actExit->setStatusTip(tr("Exit the application"));
    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));

    actAbout = new QAction(tr("&About"), this);
    actAbout->setStatusTip(tr("Show the application's About box"));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
	menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actExit);

    menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->addAction(actAbout);

}
