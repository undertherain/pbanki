#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui\QMainWindow>
#include "../../model.hpp"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
//class QPlainTextEdit;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private slots: 
    void about();

private:
    void createActions();
    void createMenus();
	void readSettings();

//	QPlainTextEdit *textEdit;
	QTextEdit *textEdit;

	QMenu *menuFile;
	QMenu *menuHelp;

    QAction *actExit;
    QAction *actAbout;

	//mindcraft:
	Model model;


};

#endif //MAINWINDOW_H

