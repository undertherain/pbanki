#include "mainwindow.h"
#include <QtGui\QtGui>

MainWindow::MainWindow()
{

	//textEdit = new QPlainTextEdit;
	textEdit = new QTextEdit;
	textEdit->setReadOnly(true);

	QWidget *centralWinget = new QWidget;
	QPushButton *button1 = new QPushButton("One");
	QPushButton *button2 = new QPushButton("Two");
	QPushButton *button3 = new QPushButton("Three");
	QPushButton *button4 = new QPushButton("Four");
	QPushButton *button5 = new QPushButton("Five");

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(textEdit);
	layout->addWidget(button1);
	layout->addWidget(button2);
	layout->addWidget(button3);
	layout->addWidget(button4);
	layout->addWidget(button5);

	centralWinget->setLayout(layout);
	//window->show();

	//setCentralWidget(textEdit);
	setCentralWidget(centralWinget);



	CreateActions();
	CreateMenus();

	ReadSettings();


}

void MainWindow::ReadSettings()
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


// ---------------slots---------------------------------
void MainWindow::ShowAbout()
{
	QMessageBox::about(this, tr("About Application"),
		tr("The <b>MindCraft </b> about"));

	//QMessageBox::about(this, tr("About Application"),
	//  textEdit->toHtml());
}
void MainWindow::OpenDeck()
{
	if (CloseIfRequired()) 
	{
		QString cdir=model.currentDirectory.c_str();
		textEdit->setHtml(cdir);
		//QMessageBox::about(this, "open deck",cdir);
		QString fileName = QFileDialog::getOpenFileName(this,"open deck",cdir);
		if (!fileName.isEmpty())
		{
			//loadFile(fileName);
			textEdit->setHtml(fileName+"<hr /> la-la-la");

		}
	}
}


void MainWindow::CreateActions()
{

	actOpenDeck = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	actOpenDeck->setShortcuts(QKeySequence::Open);
	actOpenDeck->setStatusTip(tr("Open an existing file"));
	connect(actOpenDeck, SIGNAL(triggered()), this, SLOT(OpenDeck()));

	actExit = new QAction(tr("E&xit"), this);
	actExit->setShortcut(tr("Ctrl+Q"));
	actExit->setStatusTip(tr("Exit the application"));
	connect(actExit, SIGNAL(triggered()), this, SLOT(close()));

	actAbout = new QAction(tr("&About"), this);
	actAbout->setStatusTip(tr("Show the application's About box"));
	connect(actAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
}

void MainWindow::CreateMenus()
{
	menuFile = menuBar()->addMenu(tr("&File"));
	menuFile->addAction(actOpenDeck);
	menuFile->addSeparator();
	menuFile->addAction(actExit);

	menuHelp = menuBar()->addMenu(tr("&Help"));
	menuHelp->addAction(actAbout);

}


//mindcraft
void MainWindow::ShowDeckList()
{
	DeckInfoList decks = model.getDeckList();
	std::string strDeckList="<b>available decks:</b><br/>";
	if (decks.size()>0)
	{
		for (DeckInfoList::iterator i=decks.begin();i!=decks.end();i++)
		{	
			strDeckList+=i->GetName() + "<br />";
		}
		textEdit->setHtml(QString::fromUtf8(strDeckList.c_str()));
	}
	else
	{
		textEdit->setHtml("No decks found");
	}
	//textEdit->setHtml("la-<b>la</b>-la<br>Аня - коза :)<br>味噌");
}

bool MainWindow::CloseIfRequired()
{
	//Save and close priviously opened deck when opening new
	return true;
}