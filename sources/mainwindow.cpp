//
// Created by matlle on 10/15/19.
//

#include <headers/mainwindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->createMenu();

    widget = new QWidget(this);
    mainLayout = new QGridLayout(widget);
    mainLayout->setMenuBar(menuBar);

    widget->setLayout(mainLayout);

    setupMainGrid();

    this->setCentralWidget(widget);
    this->setWindowTitle("Elementary Cellular Automata v0.0.1 -- by Martial Babo");
    this->showMaximized();
}

void MainWindow::createMenu() {
    menuBar = new QMenuBar;
    settingsMenu = new QMenu("&Settings", this);
    ruleAction = settingsMenu->addAction("R&ule");
    numberOfCellsAction = settingsMenu->addAction("Number of C&ells");
    numberOfGenerationAction = settingsMenu->addAction("Number of G&eneration");
    initialConfigAction = settingsMenu->addAction("I&nitial configuration");
    exitAction = settingsMenu->addAction("&Exit");
    menuBar->addMenu(settingsMenu);
    startSimulationAction = menuBar->addAction("Start simulation");
    startSimulationAction->setDisabled(true);


    QObject::connect(ruleAction, SIGNAL(triggered()), this, SLOT(setRule()));
    QObject::connect(numberOfCellsAction, SIGNAL(triggered()), this, SLOT(setNumberOfCells()));
    QObject::connect(numberOfGenerationAction, SIGNAL(triggered()), this, SLOT(setNumberOfGeneration()));
    QObject::connect(initialConfigAction, SIGNAL(triggered(bool)), this, SLOT(setInitialConfiguration(bool)));
    QObject::connect(startSimulationAction, SIGNAL(triggered()), this, SLOT(startSimulation()));
    QObject::connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));
}

void MainWindow::setupMainGrid() {
    auto *scene = new QGraphicsScene();

    int cellWidth = 25;
    int cellHeight = 25;
    int x = 0;
    int y = 0;
    int x1 = 8;
    int y1 = -16;

    auto *textItem = new QGraphicsSimpleTextItem();
    textItem->setText("Current rule: " + QString::number(this->rule) + " (10) = " + (!this->ruleBin.isEmpty() ? this->ruleBin : "0") + " (2)");
    //textItem->setText("Current rule: " + QString::number(this->rule) + " (10) = " + QString::fromStdString(this->intToBase2(this->rule)) + " (2)");
    QFont serifFont("Times", 18, QFont::Bold);
    textItem->setFont(serifFont);
    textItem->setX(x1);
    textItem->setY(y1 - 70);
    scene->addItem(textItem);

    for (int i = 0; i < numberOfCells; i++) {
        textItem = new QGraphicsSimpleTextItem();
        textItem->setText(QString::number(i + 1));
        textItem->setX(x1);
        textItem->setY(y1);
        scene->addItem(textItem);
        x1 += cellWidth;
    }
    x1 = y1 = 0;
    this->cells = {};
    for(int i = 0; i < numberOfGeneration; i++) {
        for(int j = 0; j < numberOfCells; j++) {
            auto *rect = new QGraphicsRectItem();
            rect->setRect(x, y, cellWidth, cellHeight);
            scene->addItem(rect);
            this->cells.push_back(rect);
            if(j == numberOfCells - 1) {
                x += cellWidth;
                textItem = new QGraphicsSimpleTextItem();
                QString s = " Gen " + QString::number(i);
                textItem->setText(s);
                textItem->setX(x);
                textItem->setY(y + 4);
                scene->addItem(textItem);
                x = 0;
                break;
            }
            x += cellWidth;
        }
        y += cellHeight;
    }
    QObject::connect(scene, SIGNAL(selectionChanged()), this, SLOT(exit()));
    auto *graphicsView = new QGraphicsView(scene, widget);
    mainLayout->addWidget(graphicsView, 0, 0);
    toggleStartSimulationAction();
}

void MainWindow::toggleStartSimulationAction() {
    this->rule > 0 ? startSimulationAction->setDisabled(false) : startSimulationAction->setDisabled(true);
}

void MainWindow::setRule() {
    this->rule = QInputDialog::getInt(widget, "Set a rule 0-255", "Rule:", this->rule, 0, 255, 1, nullptr);
    this->intToBase2(this->rule);
    setupMainGrid();
}

void MainWindow::setNumberOfCells() {
    this->numberOfCells = QInputDialog::getInt(widget, "Set the number of cells", "Total number of cells:", this->numberOfCells, 0, 60, 1, nullptr);
    setupMainGrid();
}

void MainWindow::setNumberOfGeneration() {
    this->numberOfGeneration = QInputDialog::getInt(widget, "Set the number of generation", "Total number of generations:", this->numberOfGeneration, 0, 40, 1, nullptr);
    setupMainGrid();
}

void MainWindow::setInitialConfiguration(bool value) {
    if(value) {
        srand(time(nullptr));
        int numberOfBlackCells = rand() % this->numberOfCells / 2;
        if(numberOfBlackCells == 0 || numberOfBlackCells == 1) {
            int middleCell = (this->numberOfCells / 2);
            for(int i = 0; i < this->numberOfCells; i++) {
                if(i == middleCell) {
                    this->cells.at(i)->setBrush(QBrush(0x423e3f, Qt::SolidPattern));
                }
            }
        } else {
            for(int i = 0; i < numberOfBlackCells; i++) {
                if(i % 2 == 0) {
                    if(this->cells.at(i) != nullptr) {
                        this->cells.at(i)->setBrush(QBrush(0x423e3f, Qt::SolidPattern));
                    }
                }
            }
        }
    } else {
        QMessageBox::StandardButton btnClicked = QMessageBox::question(widget, QString("Initial configuration"), QString("Do you want a random initialisation?"));
        cleanMainGrid();
        if(btnClicked == QMessageBox::No) {
            createInitialConfigWindow();
        } else {
            setInitialConfiguration(true);
        }
    }
    toggleStartSimulationAction();
}

void MainWindow::createInitialConfigWindow() {
    dialogInitConfigWindow = new QDialog(widget);
    dialogInitConfigWindow->setModal(true);
    dialogInitConfigWindow->setWindowTitle("Initial configuration");
    dialogInitConfigWindow->setMaximumWidth(250);
    dialogInitConfigWindow->setMinimumHeight(600);
    auto *scroll = new QScrollArea(widget);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget *viewport = new QWidget(dialogInitConfigWindow);
    scroll->setWidget(viewport);
    scroll->setWidgetResizable(true);

    auto *formLayout = new QFormLayout();
    viewport->setLayout(formLayout);
    for(int i = 0; i < this->numberOfCells; i++) {
        auto *group = new QButtonGroup(viewport);
        auto *hboxLayout = new QHBoxLayout();
        hboxLayout->setAlignment(viewport, Qt::AlignLeft);
        QRadioButton *btnWhite  = new QRadioButton("White");
        btnWhite->setAccessibleName(QString::number(i) + ":w");
        QRadioButton *btnBlack = new QRadioButton("Black");
        btnBlack->setAccessibleName(QString::number(i) + ":b");
        if(this->cells.at(i) != nullptr && this->cells.at(i)->brush() == QBrush()) {
            btnWhite->setChecked(true);
        } else if(this->cells.at(i) != nullptr && this->cells.at(i)->brush() != QBrush()) {
            btnBlack->setChecked(true);
        }
        group->addButton(btnWhite);
        group->addButton(btnBlack);
        hboxLayout->addWidget(btnWhite);
        hboxLayout->addWidget(btnBlack);

        QObject::connect(btnWhite, SIGNAL(toggled(bool)), this, SLOT(setCellState()));
        QObject::connect(btnBlack, SIGNAL(toggled(bool)), this, SLOT(setCellState()));

        formLayout->addRow("Cell " + QString::number(i + 1) + ":", hboxLayout);
    }

    auto *vboxLayout = new QVBoxLayout(dialogInitConfigWindow);
    vboxLayout->setSpacing(20);
    dialogInitConfigWindow->setLayout(vboxLayout);
    dialogInitConfigWindow->layout()->addWidget(scroll);

    auto *hboxLayout1 = new QHBoxLayout();
    QPushButton *btnCancel = new QPushButton("Cancel", dialogInitConfigWindow);
    QPushButton *btnOk  = new QPushButton("Ok", dialogInitConfigWindow);
    hboxLayout1->addWidget(btnCancel);
    hboxLayout1->addWidget(btnOk);
    vboxLayout->addLayout(hboxLayout1);

    QObject::connect(btnCancel, SIGNAL(clicked()), this, SLOT(rejectDialogInitConfig()));
    QObject::connect(btnOk, SIGNAL(clicked()), this, SLOT(acceptDialogInitConfig()));

    dialogInitConfigWindow->move(100, this->y() + 250);
    dialogInitConfigWindow->show();
}

void MainWindow::setCellState() {
    auto *btnRadio = qobject_cast<QRadioButton*>(sender());
    if(btnRadio != nullptr) {
        QRegExp rx("(\\:)");
        QStringList query = btnRadio->accessibleName().split(rx);
        int i = query.at(0).toInt();
        if(this->cells.at(i) != nullptr && query.at(1) == "w") {
            this->cells.at(i)->setBrush(QBrush());
        } else if (this->cells.at(i) != nullptr && query.at(1) == "b") {
            this->cells.at(i)->setBrush(QBrush(0x423e3f, Qt::SolidPattern));
        }
    }
}

void MainWindow::rejectDialogInitConfig() {
    cleanMainGrid();
    dialogInitConfigWindow->reject();
}

void MainWindow::acceptDialogInitConfig() {
    dialogInitConfigWindow->accept();
}

void MainWindow::startSimulation() {
    if(this->numberOfCells > 0 && !this->cells.empty()) {
        startSimulationAction->setDisabled(true);
        int currentGeneration = 0;
        int nextGeneration = 1;
        int j = this->ruleBin.length() - 1;
        QByteArray ba = this->ruleBin.toLocal8Bit();
        auto *res = (unsigned char *)strdup(ba.constData());
        std::map<std::string, int>::iterator iterator;
        for(iterator = this->possibleConfigurations.begin(); iterator != this->possibleConfigurations.end(); iterator++) {
            //std::cout << "bit: " << res[i2] << std::endl;
            if(res[j] == '0') {
                this->possibleConfigurations[iterator->first] = 0;
            } else if(res[j] == '1') {
                this->possibleConfigurations[iterator->first] = 1;
            }
            j--;
        }

        int lastIndexPreviousGeneration = 0;
        int counterCells = 0;
        for(unsigned long i = 0; i < this->cells.size(); i++) {
            if(currentGeneration == 0) {
                if(this->cells.at(i) != nullptr) {
                    std::string config;
                    int leftNeighbor = i - 1;
                    int rightNeighbor = i + 1;
                    if(leftNeighbor < 0) {
                        leftNeighbor = this->numberOfCells - 1;
                    }
                    if(rightNeighbor >= this->numberOfCells) {
                        rightNeighbor = 0;
                    }
                    if(this->cells.at(leftNeighbor) != nullptr) {
                        config += this->cells.at(leftNeighbor)->brush() == QBrush() ? "0" : "1";
                    }
                    if(this->cells.at(i) != nullptr) {
                        config += this->cells.at(i)->brush() == QBrush() ? "0" : "1";
                    }
                    if(this->cells.at(rightNeighbor) != nullptr) {
                        config += this->cells.at(rightNeighbor)->brush() == QBrush() ? "0" : "1";
                    }
                    std::map<std::string, int>::iterator iterator1;
                    for(iterator1 = this->possibleConfigurations.begin(); iterator1 != this->possibleConfigurations.end(); iterator1++) {
                        if(iterator1->first.compare(config) == 0) {
                            if(iterator1->second == 1) {
                                if(i + this->numberOfCells < this->cells.size()) {
                                    if(this->cells.at(i + this->numberOfCells) != nullptr) {
                                        this->cells.at(i + this->numberOfCells)->setBrush(QBrush(0x423e3f, Qt::SolidPattern));
                                    }
                                }
                            }
                        }
                    }
                }
            } else if (currentGeneration > 0 && currentGeneration < this->numberOfGeneration) {
                if(this->cells.at(i) != nullptr) {
                    std::string config;
                    int leftNeighbor = i - 1;
                    int rightNeighbor = i + 1;
                    if(leftNeighbor == lastIndexPreviousGeneration) {
                        leftNeighbor = i + (this->numberOfCells - 1);
                    }
                    if(this->cells.at(leftNeighbor) != nullptr) {
                        config += this->cells.at(leftNeighbor)->brush() == QBrush() ? "0" : "1";
                    }
                    if(this->cells.at(i) != nullptr) {
                        config += this->cells.at(i)->brush() == QBrush() ? "0" : "1";
                    }
                    if(counterCells + 1 == this->numberOfCells) {
                        rightNeighbor = i - (this->numberOfCells - 1);
                    }
                    if(this->cells.at(rightNeighbor) != nullptr) {
                        config += this->cells.at(rightNeighbor)->brush() == QBrush() ? "0" : "1";
                    }
                    std::map<std::string, int>::iterator iterator1;
                    for(iterator1 = this->possibleConfigurations.begin(); iterator1 != this->possibleConfigurations.end(); iterator1++) {
                        if(iterator1->first.compare(config) == 0) {
                            if(iterator1->second == 1) {
                                if(i + this->numberOfCells < this->cells.size()) {
                                    if(this->cells.at(i + this->numberOfCells) != nullptr) {
                                        this->cells.at(i + this->numberOfCells)->setBrush(QBrush(0x423e3f, Qt::SolidPattern));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            counterCells++;
            if(counterCells == this->numberOfCells) {
                counterCells = 0;
                currentGeneration++;
                lastIndexPreviousGeneration = i;
            }
        }
    }
}

void MainWindow::cleanMainGrid() {
    for(unsigned long i = 0; i < this->cells.size(); i++) {
        if(this->cells.at(i) != nullptr) {
            this->cells.at(i)->setBrush(QBrush());
        }
    }
}

void MainWindow::intToBase2(int value) {
    QString b = "";
    while(value != 0) {
        b = (value % 2 == 0 ? "0" : "1") + b;
        value /= 2;
    }
    unsigned long nb = b.length();
    if(nb < this->possibleConfigurations.size()) {
        unsigned long rb = this->possibleConfigurations.size() - nb;
        for(unsigned long i = 0; i < rb; i++) {
            b = "0" + b;
        }
    }
    this->ruleBin = b;
}

void MainWindow::exit() {
    QApplication::quit();
}

MainWindow::~MainWindow() {
    delete menuBar;
    delete settingsMenu;
    delete ruleAction;
    delete numberOfCellsAction;
    delete initialConfigAction;
    delete exitAction;
    delete widget;
    delete mainLayout;
    delete  startSimulationAction;
    delete dialogInitConfigWindow;
    delete numberOfGenerationAction;
}

