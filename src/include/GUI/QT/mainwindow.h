#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui\QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private:
    void createActions();
    void createMenus();
	void readSettings();

	QPlainTextEdit *textEdit;

	QMenu *menuFile;
    QAction *actExit;
    QAction *actAbout;


};

#endif //MAINWINDOW_H

