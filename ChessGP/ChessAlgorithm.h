#pragma once
#include <qobject.h>
#include"ChessBoard.h"


class ChessAlgorithm :public QObject{ //Essnetially become a base algorithm which can be used to implement a number of chess-based games
	Q_OBJECT
	Q_PROPERTY(Result result READ result)
	Q_PROPERTY(Player currentPlayer READ currentPlayer WRITE currentPlayerChanged)
public:
	enum Result{NoResult, Payer1Wins, Draw, Player2Wins};
	Q_ENUM(Result)
	enum Player{NoPlayer, Player1, Player2};
	Q_ENUM(Player)

	explicit ChessAlgorithm(QObject* parent =0);
	
	ChessBoard* board() const; //Getter for m_board
	inline Result result() const {
		return m_result;
	}
	inline Player currentPlayer() const {
		return m_currentPlayer;
	}
public slots:
	void newGame();
	virtual bool move(int colFrom, int rankFrom, int colTo, int rankTo);//It is supposed to be re-implemented by derived classes
	bool move(const QPoint &from, const QPoint &to);
	
signals:
	void boardChanged(ChessBoard* board);
	void gameOver(Result);
	void currentPlayerChanged(Player);
protected:
	virtual void setupBoard(); //Sets up the board, allocates it by calling setBoard and passing a newly created board on the stack as its argument
	void setBoard(ChessBoard* board); //Setter for m_board
	void setResult(Result value);
	void setCurrentPlayer(Player value);
	//void toggleBlackCheck();
	//void toggleWhiteCheck();
private:
	ChessBoard* m_board;
	Result m_result;
	Player m_currentPlayer;
	//bool isblackCheck = false, isWhiteCheck = false;
};

//Design Decision folks. To implement the full algorithm here or nay? Yay apparently.

class standardChess : public ChessAlgorithm {
	Q_OBJECT
public:
	standardChess(QObject* parent = 0);
	//void newGame();
	bool move(int colFrom, int rankFrom, int colTo, int rankTo) override;
	bool isPieceMovable(char piece, int colFrom, int rankFrom, int colTo, int rankTo);
	std::vector<std::pair<int, int>> horizontal(char piece, int curr_colummn, int curr_rank);
	std::vector<std::pair<int, int>> vertical(char piece,int curr_colummn, int curr_rank);
	std::vector<std::pair<int, int>> diagonal(char piece, int curr_colummn, int curr_rank);
	std::vector<std::pair<int, int>> knightPos(char piece, int curr_column, int curr_rank);
	std::vector<std::pair<int, int>> kingPos(char piece, int curr_column, int curr_rank);
	std::vector<std::pair<int, int>> pawnPos(char piece, int curr_column, int curr_rank);
	bool isCheck(char piece, int colFrom, int rankFrom, int colTo, int rankTo);
	bool isBlackCheckMate();
	bool isWhiteCheckMate();
};