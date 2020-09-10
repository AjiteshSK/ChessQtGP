#include "ChessView.h"
#include<QPainter>

void ChessView::setBoard(ChessBoard * board){ //Not to be confused with ChessAlgorithm::setupBoard
	if (m_board == board) {
		return;
	}
	
	m_board = board;

	if (board) {
		connect(board, SIGNAL(dataChanged(int, int)),
			this, SLOT(update())); //When we encounter a dataChanged() SIGNAL, we call update() which calls paintEvent(), which entails going through the m_board->m_pieces data structure and re-drawing the board with the altered sequence of characters
		connect(board, SIGNAL(boardReset()),
			this, SLOT(update()));//Does the same, when game restarts
	}
	updateGeometry();
}

ChessBoard * ChessView::board() const{
	return m_board;
}
 
QSize ChessView::sizeHint() const
{
	if (!m_board) {
		return QSize(100, 100); //Default size
	}
	QSize boardSize = QSize(fieldSize().width()* m_board->columns() + 1, fieldSize().height() * m_board->ranks() + 1); //width of one box * number of columns, height of one box * number of ranks
	int rankSize = fontMetrics().width('M') + 4;
	int columnSize = fontMetrics().height() + 4;
	return boardSize + QSize(rankSize, columnSize);
}

void ChessView::paintEvent(QPaintEvent * event){
	if (!m_board) {
		return;
	}

	QPainter painter(this);


	for (int r = m_board->ranks(); r > 0; --r) {
		painter.save();
		drawRank(&painter, r);
		painter.restore();
	}
	for (int c = 1; c <= m_board->columns(); ++c) {
		painter.save();
		drawColumn(&painter, c);
		painter.restore();
	}
	for (int r = 1; r <= m_board->ranks(); ++r) {
		for (int c = 1; c <= m_board->columns(); ++c) {
			painter.save();
			drawField(&painter, c, r);
			painter.restore();
		}
	}

	drawHighlights(&painter);

	for (int r = m_board->ranks(); r > 0; --r) {
		for (int c = 1; c <= m_board->columns(); ++c) {
			drawPiece(&painter, c, r);
		}
	}
}

QSize ChessView::fieldSize() const
{
	return m_fieldSize;
}

void ChessView::mouseReleaseEvent(QMouseEvent * event){

	QPoint pt = fieldAt(event->pos());//Extract the column and rank values from the point clicked on by the mouse

	if (pt.isNull()) {
		return;
	}

	emit clicked(pt); //emit signal with those column and rank values
}

void ChessView::setFieldSize(QSize arg){
	if (m_fieldSize == arg) {
		return;
	}
	m_fieldSize = arg;
	emit fieldSizeChanged(arg);
	updateGeometry();
}

void ChessView::setPiece(char type, const QIcon & icon){
	m_pieces.insert(type, icon);
	update();
}

QIcon ChessView::piece(char type) const{
	return m_pieces.value(type, QIcon());
}

void ChessView::addHighlight(Highlight * hl){
	m_highlights.append(hl);
	update();
}

void ChessView::removeHighlight(Highlight * hl){
	m_highlights.removeOne(hl);
	update();
}

QRect ChessView::fieldRect(int column, int rank) const //Returns the chosen field from column & rank index
{
	if (!m_board) {
		return QRect();
	}

	const QSize fs = fieldSize();//Size of an individual box
	QPoint topLeft((column - 1)*fs.width(), (m_board->ranks() - rank)*fs.height()); //Calculates the top-left point of a square on the board, represented by a column and rank int
	QRect fRect = QRect(topLeft, fs);
	int offset = fontMetrics().width('M');
	return fRect.translated(offset + 4, 0);
}

QPoint ChessView::fieldAt(const QPoint & pt) const{//returns a column&rank index from a given point
	if (!m_board) {
		return QPoint();
	}
	
	const QSize fs = fieldSize();
	int offset = fontMetrics().width('M') + 4;

	if (pt.x() < offset) {
		return QPoint();
	}

	int c = (pt.x() - offset) / fs.width(); //The chosen point's column, yielded by dividing the number of coordinate units along the x-axis by the width of one box
	int r = pt.y() / fs.height();

	if (c<0 || c>m_board->columns() || r<0 || r>m_board->ranks()) {
		return QPoint();
	}

	return QPoint(c + 1, m_board->ranks() - r); //Return a point instead of a QRect or pair<int, int> for some reason
	//turns out, it's the most convenient way to represent a two-dimensional value in Qt
}

void ChessView::drawRank(QPainter *painter, int rank)
{
	QRect r = fieldRect(1, rank);
	QRect rankRect = QRect(0, r.top(), r.left(), r.height()).adjusted(2, 0, -2, 0);
	QString rankText = QString::number(rank);
	painter->drawText(rankRect, Qt::AlignVCenter | Qt::AlignRight, rankText);
}

void ChessView::drawColumn(QPainter *painter, int column)
{
	QRect r = fieldRect(column, 1);
	QRect columnRect =QRect(r.left(), r.bottom(), r.width(), height() - r.bottom()).adjusted(0, 2, 0, -2);
	painter->drawText(columnRect, Qt::AlignHCenter | Qt::AlignTop, QChar('a' + column - 1));
}

void ChessView::drawField(QPainter *painter, int column, int rank)
{
	QRect rect = fieldRect(column, rank);
	QColor fillColor = (column + rank) % 2 ?
		palette().color(QPalette::Light) :
		palette().color(QPalette::Mid);
	painter->setPen(palette().color(QPalette::Dark));
	painter->setBrush(fillColor);
	painter->drawRect(rect);

}

void ChessView::drawPiece(QPainter * painter, int column, int rank){
	QRect r = fieldRect(column, rank);
	char value = m_board->data(column, rank);//Returns the character at a certain index position. The for-loops in the paintEvent() ensure that all the index positions are accounted for
	if (value != ' ') {
		QIcon icon = piece(value);
		if (!icon.isNull()) {
			icon.paint(painter, r, Qt::AlignCenter);
		}
	}
}

void ChessView::drawHighlights(QPainter *painter)
{
	for (int idx = 0; idx < highlightCount(); ++idx) {
		Highlight *hl = highlight(idx);//at index 0, we have the fieldHighlight, so the next statement evaluates to TRUE
		if (hl->type() == fieldHighlight::Type) {//h1->type() is a virtual function, overrided by fieldHighlight::type(), which returns Type
			fieldHighlight *fhl = static_cast<fieldHighlight*>(hl);//h1 is to be considered a fieldHighlight
			QRect rect = fieldRect(fhl->column(), fhl->rank());//Get the area-location of the chosen box
			painter->fillRect(rect, fhl->color());//Paint that area the appropriate color
		}
	}
}