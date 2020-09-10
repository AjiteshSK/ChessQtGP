#pragma once
#include <QDockWidget>
#include<QLabel>
#include<QString>
#include"ChessView.h"


class dead_black;
class dead_white;

class GameInfo :public QWidget{
	Q_OBJECT
public:
	GameInfo();
	QSize minimumSizeHint() const override;
public slots:
	void setPlayerNames(QString p1, QString p2);
	void addDeadPiece(char q);
	void reSet();
private:
	QLabel* player1Name;
	QLabel* player2Name;
	
	QVector<char> dead_pieces;//Book-keeping
	dead_black* db;
	dead_white* dw;
};

class dead_black :public QWidget {//adds the circuitry for adding dead players and related signals/slots
public:
	void addDeadPiece(char q);
	void populatePieces();
	void emptyDead();
	QSize minimumSizeHint() const override;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QMap<char, QIcon> pieces;//for painting
	QVector<char> dead_pieces;//book-keeping
};

class dead_white :public QWidget {
public:
	void addDeadPiece(char q);
	void populatePieces();
	void emptyDead();
	QSize minimumSizeHint() const override;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QMap<char, QIcon> pieces;//for painting
	QVector<char> dead_pieces;//book-keeping
};