#include "mainwindow.h"
#include<QLayout>
#include<iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	m_selectedField = nullptr;
	
	cv = new ChessView;
	cv->setPiece('k', QIcon("D:\\VS projects\\ChessGP\\resources\\king.svg"));//black king
	cv->setPiece('b', QIcon("D:\\VS projects\\ChessGP\\resources\\rook.svg"));
	cv->setPiece('r', QIcon("D:\\VS projects\\ChessGP\\resources\\castle.svg"));
	cv->setPiece('n', QIcon("D:\\VS projects\\ChessGP\\resources\\knight.svg"));
	cv->setPiece('p', QIcon("D:\\VS projects\\ChessGP\\resources\\pawn.svg"));
	cv->setPiece('q', QIcon("D:\\VS projects\\ChessGP\\resources\\queen.svg"));

	cv->setPiece('K', QIcon("D:\\VS projects\\ChessGP\\resources\\KingW.svg"));
	cv->setPiece('Q', QIcon("D:\\VS projects\\ChessGP\\resources\\QueenW.svg"));
	cv->setPiece('R', QIcon("D:\\VS projects\\ChessGP\\resources\\CastleW.svg"));
	cv->setPiece('B', QIcon("D:\\VS projects\\ChessGP\\resources\\BishopW.svg"));
	cv->setPiece('N', QIcon("D:\\VS projects\\ChessGP\\resources\\KnightW.svg"));
	cv->setPiece('P', QIcon("D:\\VS projects\\ChessGP\\resources\\PawnW.svg"));
	ca = new standardChess;
	dialog = new ConfigurationBox;
	ca->newGame();//This call creates a new board
	cv->setBoard(ca->board());//Now, both ca and cv have pointers to the same board. CA alters its state, cv alters its view
	setCentralWidget(cv);


	createActions();
	creatMenu();

	connect(cv, &ChessView::clicked, this, &MainWindow::viewClicked);
	connect(ca, &ChessAlgorithm::gameOver, this, &MainWindow::isOver);

	connect(dialog, &ConfigurationBox::OkClicked, this, &MainWindow::newGame);
	connect(dialog, &ConfigurationBox::ExitGame, this, &QMainWindow::close);
	cv->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	cv->setFieldSize(QSize(50, 50));
	
	layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void MainWindow::viewClicked(const QPoint &field) { //slot that is connected to clicked signal emitted from withint the mouseReleaseEvent. The argument field contains the column and rank info derived from the mouseReleas event
	
	/*
	if (m_clickPoint.isNull()) {//First mouseReleaseEvent(click), where the piece to be moved is selected
		m_clickPoint = field;
	}
	else {//Second mouseReleaseEvent, when the piece is already selected and it's position is stored in the m_clickPoint data member
		if (field != m_clickPoint) {//Check to see if the second click isn't on the same box
			cv->board()->movePiece(m_clickPoint.x(), m_clickPoint.y(), field.x(), field.y());//m_clickPoint(where the Piece already is (from the first click's execution of the above if-statement). Field is curren click's data.
		}
		m_clickPoint = QPoint();//Reset m_clickPoint
	}	

	LET'S NOT DELETE THIS AND LET IT REMAIN AS A REMNINDER OF YOUR IMBECILITY 
	*/

	if (field.y() == 0) {
		return;
	}

	if (m_clickPoint.isNull()) {//Checks is this is the first click, i.e, we haven't selected a piece already
		if (cv->board()->data(field.x(), field.y()) != ' ') {
			m_clickPoint = field;//
			m_selectedField = new ChessView::fieldHighlight(field.x(), field.y(), QColor(255, 0, 0, 50));
			cv->addHighlight(m_selectedField);
		}
	}
	else {//This is the second-click, i.e., we've already clicked on a piece before (stored its position in m_clickPoint
		if (field != m_clickPoint) {
			ca->move(m_clickPoint, field);
			//cv->board()->movePiece(m_clickPoint.x(), m_clickPoint.y(), field.x(), field.y());//The piece was stored in m_clickPoint (first click location), now we move it to field(second click location)
		};

		m_clickPoint = QPoint();
		cv->removeHighlight(m_selectedField);
		delete m_selectedField;
		m_selectedField = nullptr;
	}
}

void MainWindow::newGame(){
	ca->newGame();
	cv->setBoard(ca->board());
	cv->update();
	//cv->board()->setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void MainWindow::createActions(){
	newAct = new QAction(tr("&New Game"), this);
	newAct->setShortcut(QKeySequence::New);
	connect(newAct, &QAction::triggered, this, &MainWindow::newGame);

}

void MainWindow::creatMenu(){
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
}

void MainWindow::isOver(ChessAlgorithm::Result result) {
	

	if (result == ChessAlgorithm::Result::Payer1Wins) {
		dialog->setWinner("White won!");
	}
	else if (result == ChessAlgorithm::Result::Player2Wins) {
		dialog->setWinner("Black Won!");
	}
	else {
		dialog->setWinner("No winners. It's a draw");
	}

	dialog->exec();

}