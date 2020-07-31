#include "NewGamePromptDialog.h"
#include<QGridLayout>

NewGamePromptDialog::NewGamePromptDialog() {

	QGridLayout* grid = new QGridLayout(this);

	player1 = new QLabel(tr("White player name"));
	player2 = new QLabel(tr("Black player name"));

	player1_edit = new QLineEdit(tr("Enter player name"));
	player2_edit = new QLineEdit(tr("Enter player name"));

	startGame = new QPushButton(tr("Start Game"));
	cancel = new QPushButton(tr("Cancel"));

	grid->addWidget(player1,0,0,1,1,Qt::AlignRight);
	grid->addWidget(player2,1,0,1,1,Qt::AlignRight);
	grid->addWidget(player1_edit,0,1,1,1,Qt::AlignLeft);
	grid->addWidget(player2_edit,1,1,1,1,Qt::AlignLeft);
	grid->addWidget(startGame,2,1,1,1,Qt::AlignRight);
	grid->addWidget(cancel,2,2,1,1,Qt::AlignLeft);

	setLayout(grid);

	connect(startGame, &QAbstractButton::clicked, this, &NewGamePromptDialog::startGame);
	connect(cancel, &QAbstractButton::clicked, this, &NewGamePromptDialog::cancelClicked);
}