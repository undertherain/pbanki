#include "mainwindow.h"
#include <QtGui\QtGui>

MainWindow::MainWindow()
{
    textEdit = new QPlainTextEdit;
    setCentralWidget(textEdit);

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

void MainWindow::createActions()
{
    actExit = new QAction(tr("E&xit"), this);
    actExit->setShortcut(tr("Ctrl+Q"));
    actExit->setStatusTip(tr("Exit the application"));
    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));

}

void MainWindow::createMenus()
{
	menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actExit);

}
