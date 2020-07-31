#pragma once
#include <qobject.h>
#include<QVector>

class ChessBoard :public QObject{
	Q_OBJECT
	Q_PROPERTY(int ranks READ ranks NOTIFY ranksChanged)
	Q_PROPERTY(int columns READ columns NOTIFY columnsChanged)
public:
	ChessBoard(int ranks, int columns, QObject* parent);
	
	int ranks();
	int columns() const;
	char data(int column, int rank) const;
	void setData(int column, int rank, char Value);
	void movePiece(int fromColumn, int fromRank, int toColumn, int toRank);
	void pseduo_movePiece(int fromColumn, int fromRank, int toColumn, int toRank);
	void setFen(const QString &fen);
protected:
	void setRanks(int newRanks);
	void setColumns(int newColumns);
	bool setDataInternal(int column, int rank, char value);
	void initBoard();
	
 signals:
	void ranksChanged(int newRanks);
	void columnsChanged(int newColumns);
	void dataChanged(int c, int r);
	void boardReset();
private:
	int m_ranks, m_columns;
	QVector<char>m_boardData;
};

