#include "ChessBoard.h"

ChessBoard::ChessBoard(int ranks, int columns, QObject* parent):
	m_ranks(ranks),
	m_columns(columns),
	QObject(parent){
	initBoard();
}


int ChessBoard::ranks(){
	return m_ranks;
}

int ChessBoard::columns() const{
	return m_columns;
}

char ChessBoard::data(int column, int rank) const{
	if (rank == 0) {
		rank = 1;
	}
	if (column == 0) {
		column = 1;
	}
	int index = (rank - 1) * columns() + (column - 1);
	return m_boardData.at(index);
}

void ChessBoard::setData(int column, int rank, char Value){
	if (setDataInternal(column, rank, Value)) {
		emit dataChanged(column, rank);
	}
}

void ChessBoard::movePiece(int fromColumn, int fromRank, int toColumn, int toRank){
	setData(toColumn, toRank, data(fromColumn, fromRank));//The occupied box
	setData(fromColumn, fromRank, ' ');//The vacated box, also emits dataChanged signal
}

void ChessBoard::pseduo_movePiece(int fromColumn, int fromRank, int toColumn, int toRank){//Used to check for possible checks/check-mates
	
		char restore = data(toColumn, toRank);
		setData(toColumn, toRank, data(fromColumn, fromRank));//The occupied box
		setData(fromColumn, fromRank, ' ');//The vacated box, also emits dataChanged signal
	
}

void ChessBoard::setRanks(int newRanks){
	if (ranks() == newRanks) {
		return;
	}
	m_ranks = newRanks;
	initBoard();
	emit ranksChanged(m_ranks);
}

void ChessBoard::setColumns(int newColumns){
	if (columns() == newColumns) {
		return;
	}
	m_columns = newColumns;
	initBoard();
	emit columnsChanged(m_columns);
}

bool ChessBoard::setDataInternal(int column, int rank, char value){
	if (rank == 0) {
		rank = 1;
	}
	if (column == 0) {
		column = 1;
	}
	int index = (rank - 1)*columns() + (column - 1);
	if (m_boardData.at(index) == value) {
		return false;
	}
	m_boardData[index] = value; //Alters the QVector's data, exchanges a char value for a ' ' value and vice-versa
	return true;
}

void ChessBoard::initBoard(){
	m_boardData.fill(' ', ranks()*columns()); //' ', 64
	emit boardReset();
}

void ChessBoard::setFen(const QString& fen){
	int index = 0;
	int skip = 0;
	const int columnCount = columns();
	QChar ch;

	for (int rank = ranks(); rank > 0; --rank) {
		for (int column = 1; column <= columnCount; ++column) {
			if (skip > 0) {
				ch = ' ';
				skip--; //This is for the empty boxes signified by single digit 8
			}
			else {
				ch = fen.at(index++);
				if (ch.isDigit()) {
					skip = ch.toLatin1() - '0';
					ch = ' ';
					skip--;
				}
			}
			setDataInternal(column, rank, ch.toLatin1());
		}
		QChar next = fen.at(index++);
		if (next != '/' && next != ' ') {
			initBoard();
			return; // fail on error
		}
	}
	emit boardReset();
}
