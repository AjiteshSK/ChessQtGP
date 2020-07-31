#pragma once
#include <qdialog.h>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>

class NewGamePromptDialog :public QDialog{
	Q_OBJECT

public:
	NewGamePromptDialog();
signals:
	void startGame(QString p1, QString p2);
	void cancelClicked();
private:
	QLabel* player1;
	QLabel* player2;
	QLineEdit* player1_edit;
	QLineEdit* player2_edit;
	QPushButton* startGame;
	QPushButton* cancel;
};

