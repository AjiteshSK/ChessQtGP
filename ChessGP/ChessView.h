#pragma once
#include <qwidget.h>
#include<QPointer>
#include"ChessBoard.h"
#include<QIcon>
#include<QMap>
#include<QMouseEvent>

class ChessView :public QWidget{
	Q_OBJECT
	Q_PROPERTY(QSize fieldSize //size of a single box on the board
			   READ fieldSize
		       WRITE setFieldSize
			   NOTIFY fieldSizeChanged)
public:
	void setBoard(ChessBoard* board);
	ChessBoard *board() const;
	QSize sizeHint() const override;
	void setFieldSize(QSize arg);
	void setPiece(char type, const QIcon& icon);//Maps a char to  QIcon in the QMap<char,QIcon> m_pieces map
	QIcon piece(char type) const;

	//Highlight getters and setters

	class Highlight {
	public:
		Highlight() {}
		virtual ~Highlight() {}
		virtual int type() const { return 0; }
	};

	class fieldHighlight :public Highlight {
	public:
		enum { Type = 1 };
		fieldHighlight(int column, int rank, QColor color) :m_field(column, rank), m_color(color) {}

		inline int column() const { return m_field.x(); }
		inline int rank() const { return m_field.y(); }
		inline QColor color() const { return m_color; }
		int type() const { return Type; }

	private:
		QPoint m_field;
		QColor m_color;
	};

	//class moveSuggestionHighlight{ public: moveSuggestionHighlight(int column, int rank, QColor color)}; use the row/column and piece info to generate a list of all possible positions, then draw a painterPath from the row/column to those positions

	void addHighlight(Highlight *hl);
	void removeHighlight(Highlight *hl);

	inline Highlight *highlight(int index) const {
		return m_highlights.at(index);
	}

	inline int highlightCount() const {
		return m_highlights.size();
	}

private:
	QList<Highlight*>m_highlights;
	QPointer<ChessBoard> m_board;//essentially a data structure, doesn't influence the size of the widget
	QMap<char, QIcon> m_pieces;
	QSize m_fieldSize; //Size of the widget
	
protected:
	void paintEvent(QPaintEvent* event);
	QSize fieldSize() const;
	void mouseReleaseEvent(QMouseEvent * event) override;
	QRect fieldRect(int column, int rank) const;			 //Converts column and rank co-ordinates to an area on the board
	QPoint fieldAt(const QPoint& pt) const;					 //Converts a clicked point to the column and rank co-ordinates (stores them in a QPoint class)
	void drawRank(QPainter* painter, int rank);				 //painteEvent() helper methods
	void drawColumn(QPainter* painter, int column);			 //painteEvent() helper methods
	void drawField(QPainter* painter, int column, int rank); //painteEvent() helper methods
	void drawPiece(QPainter* painter, int column, int rank); //painteEvent() helper methods
	virtual void drawHighlights(QPainter* painter);			 //painteEvent() helper methods
signals:
	void fieldSizeChanged(QSize arg);
	void clicked(const QPoint &);
};

