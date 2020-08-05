#pragma once
#include <QDockWidget>
#include<QLabel>
#include<QString>

class GameInfo :public QWidget{
	Q_OBJECT
public:
	GameInfo();
	QSize minimumSizeHint() const override;
public slots:

	void setPlayerNames(QString p1, QString p2);

private:
	QLabel* player1Name;
	QLabel* player2Name;
};

