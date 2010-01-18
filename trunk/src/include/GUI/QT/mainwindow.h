#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui\QMainWindow>
#include <QtGui\QWidget>

#include "../../model.hpp"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QWidget;
class QLabel;
class QPushButton;
class QVBoxLayout;
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
    void ShowAnswer();

private:
    void CreateActions();
    void CreateMenus();
	void ReadSettings();


//	Controls-------------
	QTextEdit *textEdit;
    QWidget  *centralWidget;
	QPushButton *btnShowAnswer;
	QWidget *answerButtons;

	QMenu *menuFile;
	QMenu *menuHelp;
	QVBoxLayout * centralLayout;
	//actions--------------
    QAction *actExit;
    QAction *actAbout;
    QAction *actOpenDeck;
  //  QAction *actShowAnswer;




};

#endif //MAINWINDOW_H

