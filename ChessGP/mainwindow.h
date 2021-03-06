#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "ChessView.h"
#include"ChessAlgorithm.h"
#include "ConfigurationBox.h"
#include <QAction>
#include "GameInfo.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass ui;
	ChessView* cv;
	GameInfo* gi;
	ChessAlgorithm* ca;
	QPoint m_clickPoint;
	ChessView::fieldHighlight* m_selectedField;
	ConfigurationBox* dialog;
	QMenu* fileMenu;
	QAction* newAct;
	QAction* openAct;
	QAction* exitAct;
	QAction* saveAct;
	QAction* reAct;
	void createActions();
	void creatMenu();
public slots:
	void viewClicked(const QPoint& field);
	void isOver(ChessAlgorithm::Result result);
	void newGame();
	void ReMatch();
};
