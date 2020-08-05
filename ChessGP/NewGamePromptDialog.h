#pragma once
#include <qdialog.h>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>

class NewGamePromptDialog :public QDialog{
	Q_OBJECT

public:
	NewGamePromptDialog();
	QString player1Name() const;
	QString player2Name() const;
	void setPLayer1Name(QString name);
	void setPLayer2Name(QString name);
signals:
	void playerNamesChanged();
private:
	QLabel* player1;
	QLabel* player2;
	QLineEdit* player1_edit;
	QLineEdit* player2_edit;
	QPushButton* startGame;
	QPushButton* cancel;

	QString player1name;
	QString player2name;
};

