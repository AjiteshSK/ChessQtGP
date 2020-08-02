#pragma once
#include<QDialog>
#include<QDialogButtonBox>
#include<QPushButton>
#include<QLabel>

class ConfigurationBox :public QDialog{
	Q_OBJECT

public:
	ConfigurationBox();
	void setWinner(QString winner);
signals:
	void OkClicked();
	void ExitGame();
	void rematch();
private:
	QPushButton* NewGame;
	QPushButton* Exit;
	QPushButton* reGame;
	QLabel* Winner;
};

