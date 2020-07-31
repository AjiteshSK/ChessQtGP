#include "ConfigurationBox.h"
#include<QGridLayout>

ConfigurationBox::ConfigurationBox(){

	NewGame = new QPushButton(tr("&New Game"));
	Exit = new QPushButton(tr("&Exit"));
	Winner = new QLabel;
	QGridLayout* grid = new QGridLayout(this);
	grid->addWidget(NewGame,1,0,1,1);
	grid->addWidget(Winner, 0, 0, 1, 1);
	grid->addWidget(Exit, 1, 1, 1, 1);
	connect(NewGame, &QAbstractButton::clicked, this, &ConfigurationBox::OkClicked);
	connect(NewGame, &QAbstractButton::clicked, this, &QDialog::close);
	connect(Exit, &QAbstractButton::clicked, this, &ConfigurationBox::ExitGame);
	connect(Exit, &QAbstractButton::clicked, this, &QDialog::close);
}

void ConfigurationBox::setWinner(QString winner){
	Winner->setText(winner);
}
