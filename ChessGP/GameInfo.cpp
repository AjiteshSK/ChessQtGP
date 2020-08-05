#include "GameInfo.h"
#include<QVBoxLayout>

GameInfo::GameInfo() {

	player1Name = new QLabel(tr("White King"));
	player2Name = new QLabel(tr("Black King"));

	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->addWidget(player1Name, 0, Qt::AlignTop);
	layout->addWidget(player2Name, 1, Qt::AlignBottom);

	setLayout(layout);

	updateGeometry();
}

void GameInfo::setPlayerNames(QString p1, QString p2) {
	player1Name->setText(p1);
	player2Name->setText(p2);

	update();
}

QSize GameInfo::minimumSizeHint() const{

	return QSize(fontMetrics().width('M') * 35, 404);
}
