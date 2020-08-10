#include "GameInfo.h"
#include<QVBoxLayout>
#include<QPainter>

//DEfine two widgets for dead pieces, to get paintEvent to work
GameInfo::GameInfo() {

	player1Name = new QLabel(tr("White King"));
	player2Name = new QLabel(tr("Black King"));


	QVBoxLayout* white_layout = new QVBoxLayout;
	QVBoxLayout* black_layout = new QVBoxLayout;
	db = new dead_black;
	dw = new dead_white;
	db->populatePieces();
	dw->populatePieces();
	QVBoxLayout* layout = new QVBoxLayout(this);

	white_layout->addWidget(player1Name, 0, Qt::AlignCenter);
	white_layout->addWidget(dw, 1, Qt::AlignCenter);

	black_layout->addWidget(player2Name, 0, Qt::AlignCenter);
	black_layout->addWidget(db, 1, Qt::AlignCenter);

	layout->addLayout(white_layout, 1);
	layout->addLayout(black_layout, 1);

	setLayout(layout);
	hasMouseTracking();
	updateGeometry();
}

void GameInfo::setPlayerNames(QString p1, QString p2) {
	player1Name->setText(p1);
	player2Name->setText(p2);

	update();
}

QSize GameInfo::minimumSizeHint() const{
	//Create a slot/connect it to board's fieldSizechanged signal
	return QSize(fontMetrics().width('M') * 35, 404);
}

void GameInfo::addDeadPiece(char q){
	if (isupper(q)) {
		dw->addDeadPiece(q);
	}
	else {
		db->addDeadPiece(q);
	}
}


/*hol up
void GameInfo::paintEvent(QPaintEvent * event){
	QPainter* painter = new QPainter(this);
	QRect rect;
	rect.setTopLeft(QPoint(10, 101));
	rect.setWidth(fontMetrics().width('M') * 2);
	rect.setHeight(fontMetrics().width('M') * 2);
	for (auto x : dead_pieces) {
		if (x != ' ') {
			pieces[x].paint(painter, rect, Qt::AlignCenter);
		}
	}
}
*/

void dead_black::addDeadPiece(char q){
	dead_pieces.push_back(q);
	update();
}

void dead_black::populatePieces(){
	pieces.insert('k', QIcon("D:\\VS projects\\ChessGP\\resources\\king.svg"));
	pieces.insert('b', QIcon("D:\\VS projects\\ChessGP\\resources\\rook.svg"));
	pieces.insert('r', QIcon("D:\\VS projects\\ChessGP\\resources\\castle.svg"));
	pieces.insert('n', QIcon("D:\\VS projects\\ChessGP\\resources\\knight.svg"));
	pieces.insert('p', QIcon("D:\\VS projects\\ChessGP\\resources\\pawn.svg"));
	pieces.insert('q', QIcon("D:\\VS projects\\ChessGP\\resources\\queen.svg"));

	pieces.insert('K', QIcon("D:\\VS projects\\ChessGP\\resources\\KingW.svg"));
	pieces.insert('Q', QIcon("D:\\VS projects\\ChessGP\\resources\\QueenW.svg"));
	pieces.insert('R', QIcon("D:\\VS projects\\ChessGP\\resources\\CastleW.svg"));
	pieces.insert('B', QIcon("D:\\VS projects\\ChessGP\\resources\\BishopW.svg"));
	pieces.insert('N', QIcon("D:\\VS projects\\ChessGP\\resources\\KnightW.svg"));
	pieces.insert('P', QIcon("D:\\VS projects\\ChessGP\\resources\\PawnW.svg"));
}

QSize dead_black::minimumSizeHint() const{
	return QSize(fontMetrics().width('M')*34,50);
}

void dead_black::paintEvent(QPaintEvent * event){
	QPainter* painter = new QPainter(this);
	QRect rect;
	rect.setTopLeft(QPoint(0,0));
	rect.setWidth(fontMetrics().width('M') * 2);
	rect.setHeight(fontMetrics().width('M') * 2);
	for (auto x : dead_pieces) {
		if (x != ' ') {
			pieces[x].paint(painter, rect, Qt::AlignCenter);
		}
	}
}

void dead_white::addDeadPiece(char q){
	dead_pieces.push_back(q);
	update();
}

void dead_white::populatePieces(){
	pieces.insert('k', QIcon("D:\\VS projects\\ChessGP\\resources\\king.svg"));
	pieces.insert('b', QIcon("D:\\VS projects\\ChessGP\\resources\\rook.svg"));
	pieces.insert('r', QIcon("D:\\VS projects\\ChessGP\\resources\\castle.svg"));
	pieces.insert('n', QIcon("D:\\VS projects\\ChessGP\\resources\\knight.svg"));
	pieces.insert('p', QIcon("D:\\VS projects\\ChessGP\\resources\\pawn.svg"));
	pieces.insert('q', QIcon("D:\\VS projects\\ChessGP\\resources\\queen.svg"));

	pieces.insert('K', QIcon("D:\\VS projects\\ChessGP\\resources\\KingW.svg"));
	pieces.insert('Q', QIcon("D:\\VS projects\\ChessGP\\resources\\QueenW.svg"));
	pieces.insert('R', QIcon("D:\\VS projects\\ChessGP\\resources\\CastleW.svg"));
	pieces.insert('B', QIcon("D:\\VS projects\\ChessGP\\resources\\BishopW.svg"));
	pieces.insert('N', QIcon("D:\\VS projects\\ChessGP\\resources\\KnightW.svg"));
	pieces.insert('P', QIcon("D:\\VS projects\\ChessGP\\resources\\PawnW.svg"));
}

QSize dead_white::minimumSizeHint() const
{
	return QSize(fontMetrics().width('M') * 34, 50);
}

void dead_white::paintEvent(QPaintEvent * event){
	QPainter* painter = new QPainter(this);
	QRect rect;
	rect.setTopLeft(QPoint(0, 0));
	rect.setWidth(fontMetrics().width('M') * 2);
	rect.setHeight(fontMetrics().width('M') * 2);
	for (auto x : dead_pieces) {
		if (x != ' ') {
			pieces[x].paint(painter, rect, Qt::AlignCenter);
		}
	}
}
