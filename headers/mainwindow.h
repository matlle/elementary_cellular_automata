//
// Created by matlle on 10/15/19.
//
#ifndef SECOND_MAINWINDOW_H
#define SECOND_MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QGridLayout>
#include <QMenuBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QInputDialog>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <QStyleOptionGraphicsItem>
#include <QMessageBox>
#include <QRadioButton>
#include <QScrollArea>
#include <QFormLayout>
#include <QButtonGroup>
#include <QPushButton>

class MainWindow : public QMainWindow {
Q_OBJECT

private:
    QMenuBar *menuBar = nullptr;
    QMenu *settingsMenu = nullptr;
    QAction *ruleAction = nullptr;
    QAction *numberOfCellsAction = nullptr;
    QAction *numberOfGenerationAction = nullptr;
    QAction *initialConfigAction = nullptr;
    QAction *startSimulationAction = nullptr;
    QAction *exitAction = nullptr;
    QWidget *widget = nullptr;
    QGridLayout *mainLayout = nullptr;
    QDialog *dialogInitConfigWindow = nullptr;
    int rule = 0;
    QString ruleBin;
    int numberOfCells = 41;
    int numberOfGeneration = 20;
    std::vector<QGraphicsRectItem*> cells;
    std::map<std::string, int> possibleConfigurations = {
            {"111", 0},
            {"110", 0},
            {"101", 0},
            {"100", 0},
            {"011", 0},
            {"010", 0},
            {"001", 0},
            {"000", 0}
    };

    void createMenu();
    void createInitialConfigWindow();
    void intToBase2(int);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void setupMainGrid();
    void setRule();
    void setNumberOfCells();
    void setNumberOfGeneration();
    void setInitialConfiguration(bool);
    void startSimulation();
    void rejectDialogInitConfig();
    void acceptDialogInitConfig();
    void setCellState();
    void cleanMainGrid();
    void toggleStartSimulationAction();
    void exit();

};

#endif //SECOND_MAINWINDOW_H