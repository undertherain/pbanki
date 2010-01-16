#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui\QMainWindow>
#include <QtGui\QWidget>

#include "../../model.hpp"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QLabel;
//class QPlainTextEdit;
class QTextEdit;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
	//mindcraft:
	Model model;
	void ShowDeckList();
	bool CloseIfRequired();

protected:
    void closeEvent(QCloseEvent *event);
private slots: 
    void ShowAbout();
    void OpenDeck();

private:
    void CreateActions();
    void CreateMenus();
	void ReadSettings();


//	QPlainTextEdit *textEdit;
	QTextEdit *textEdit;
    QWidget  *centralWinget;

	QMenu *menuFile;
	QMenu *menuHelp;

    QAction *actExit;
    QAction *actAbout;
    QAction *actOpenDeck;




};

#endif //MAINWINDOW_H

