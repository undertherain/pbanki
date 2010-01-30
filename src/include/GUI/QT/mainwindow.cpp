#include "mainwindow.h"

#ifdef ARCH_LINUX
#include <QtGui/QtGui>
#else
#include <QtGui\QtGui>

#endif

MainWindow::MainWindow()
{

	
	//textEdit = new QPlainTextEdit;

	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	textEdit = new QTextEdit(centralWidget);
	textEdit->setReadOnly(true);

	btnShowAnswer = new QPushButton("ShowAnswer",centralWidget);
    //connect(btnShowAnswer, SIGNAL(clicked()), this, SLOT(ShowAnswer()));
	//setCentralWidget(btnShowAnswer );



	centralLayout = new QVBoxLayout;
	//centralLayout->setMargin(0);
	//centralLayout->setSpacing(1);
	centralLayout->addWidget(textEdit);
	centralLayout->addWidget(btnShowAnswer);
	
	//answerButtons= new QWidget;
	//QPushButton *btnAgain = new QPushButton("Again");
	//QPushButton *btnHard = new QPushButton("Hard");
	//QPushButton *btnGood = new QPushButton("Good");
	//QPushButton *btnEasy = new QPushButton("Easy");
	//QHBoxLayout * answerButtonsLayout = new QHBoxLayout;
	//answerButtons->setLayout(answerButtonsLayout);
	//answerButtonsLayout->setMargin(0);
	//answerButtonsLayout->setSpacing(1);
	//answerButtonsLayout->addWidget(btnAgain);
	//answerButtonsLayout->addWidget(btnHard);
	//answerButtonsLayout->addWidget(btnGood);
	//answerButtonsLayout->addWidget(btnEasy);
	
	
	
	centralWidget->setLayout(centralLayout);
		
	//setCentralWidget(textEdit);
	


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

void MainWindow::ShowAnswer()
{
	centralLayout->removeWidget(btnShowAnswer);
	//centralLayout->addWidget(btnShowAnswer);
	centralLayout->addWidget(answerButtons);
	
	std::string innerHtml="question <hr /> answer";

	textEdit->setHtml(QString::fromUtf8(innerHtml.c_str()));
	
}

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