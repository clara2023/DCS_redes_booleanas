#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicswitch.h"

#include <iostream>
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);


    connect(ui->addNodeButton, &QPushButton::clicked, this, [=]() {
       GraphicNode *newNode = new GraphicNode(10, 10);
       scene->addItem(newNode);

       connect(newNode, &GraphicNode::clicked, this, [=](GraphicNode* clicked) {
           handleItemClicked(clicked, nullptr);
       });
       /*

       connect(newNode, &GraphicNode::clicked, this, [=](GraphicNode* clicked) {
           if (currentMode == InteractionMode::Linking) {
               if (originNode == nullptr) {
                   originNode = clicked;
                   originNode->setBrush(QBrush(Qt::yellow));
                   ui->statusLabel->setText("Mode: Linking");
               } else {
                   QLineF newLine(originNode->scenePos(), clicked->scenePos());
                   QGraphicsLineItem* line = scene->addLine(newLine, QPen(Qt::black, 2));

                   originNode->updateColor();
                   originNode->addLine(line);
                   clicked->addLine(line);
                   originNode = nullptr;

                   currentMode = InteractionMode::Standard;
                   ui->statusLabel->setText("");
                   resetSelection();
               }
           } else if (currentMode == InteractionMode::Excluding) {
               clicked->removeAllLines(scene); // será???
               scene->removeItem(clicked);;
               delete clicked;

               ui->statusLabel->setText("");
               currentMode = InteractionMode::Standard;
           }
       });

       */


    });
    createSwitches();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_runStepButton_clicked()
{
    network.simulateStep();
}

void MainWindow::createSwitches()
{
    QWidget *lateralColumn = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(lateralColumn);
    layout->setAlignment(Qt::AlignTop);

    for (int i = 0; i < 10; i++) {
        GraphicSwitch *sw = new GraphicSwitch(this);
        layout->addWidget(sw);

        switchesList.append(sw);

        connect(sw, &GraphicSwitch::clicked, this, [=](GraphicSwitch* clicked) {
            handleItemClicked(nullptr, clicked);
        });
    }

    lateralColumn->setLayout(layout);
    lateralColumn->setGeometry(50, 50, 100, 500);
    lateralColumn->show();
}

void MainWindow::handleItemClicked(GraphicNode* node, GraphicSwitch* sw)
{
    if (currentMode == InteractionMode::Linking) {
        if (originNode == nullptr && originSwitch == nullptr) {
            if (node) {
                originNode = node;
                originNode->setBrush(QBrush(Qt::yellow));
            } else if (sw) {
                originSwitch = sw;
                sw->setStyleSheet("background-color: yellow;");
            }
            ui->statusLabel->setText("Mode: Linking");
        } else {
            QPointF p1, p2;

            if (originNode) {
                p1 = originNode->scenePos();
            } else if (originSwitch) {
                QPoint globalCenter = originSwitch->mapToGlobal(originSwitch->rect().center());
                QPoint viewCenter = ui->graphicsView->mapFromGlobal(globalCenter);
                p1 = ui->graphicsView->mapToScene(viewCenter);
            }

            if (node) {
                p2 = node->scenePos();
            } else if (sw) {
                QPoint globalCenter = sw->mapToGlobal(sw->rect().center());
                QPoint viewCenter = ui->graphicsView->mapFromGlobal(globalCenter);
                p2 = ui->graphicsView->mapToScene(viewCenter);
            }

            QLineF newLine(p1, p2);
            QGraphicsLineItem* line = scene->addLine(newLine, QPen(Qt::black, 2));

            if (originNode) originNode->addLine(line);
            if (originSwitch) originSwitch->addLine(line);
            if (node) node->addLine(line);
            if (sw) sw->addLine(line);

            if (originNode) {
                originNode->updateColor();
                originNode = nullptr;
            }
            if (originSwitch) {
                originSwitch->updateAppearance();
                originSwitch = nullptr;
            }

            currentMode = InteractionMode::Standard;
            ui->statusLabel->setText("");
        }
    }
    else if (currentMode == InteractionMode::Excluding) {
        if (node) {
            node->removeAllLines(scene);
            scene->removeItem(node);
            delete node;
        }
        else if (sw) {
            sw->removeAllLines(scene);
        }

        currentMode = InteractionMode::Standard;
        ui->statusLabel->setText("");
    } else {
        if(sw) {
            if(sw->text() == "0") {
                sw->setText("1");
                sw->setStyleSheet("background-color: green; color: white;");
            } else {
                sw->setText("0");
                sw->setStyleSheet("");
            }
        }
    }
}

/*

void MainWindow::createButtons()
{
    QWidget *lateralColumn = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(lateralColumn);

    layout->setAlignment(Qt::AlignTop);

    for (int i = 0; i < 10; i++) {
        QPushButton *newButton = new QPushButton("0", this);
        newButton->setFixedSize(50,50);
        newButton->setStyleSheet("");

        layout->addWidget(newButton);
        connect(newButton, &QPushButton::clicked, this, &MainWindow::onInputButtonClicked);

        inputButtonsList.append(newButton);
    }

    lateralColumn->setLayout(layout);
    lateralColumn->setGeometry(50, 50, 100, 500);
    lateralColumn->show();
}

void MainWindow::onInputButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if(clickedButton){
        if(clickedButton->text() == "0") {
            clickedButton->setText("1");
            clickedButton->setStyleSheet("background-color: green; color: white;");
        } else {
            clickedButton->setText("0");
            clickedButton->setStyleSheet("");
        }
    }
}
*/

void MainWindow::on_removeNodeButton_clicked()
{
    if (currentMode == InteractionMode::Standard || currentMode == InteractionMode::Linking) {
        currentMode = InteractionMode::Excluding;
        ui->statusLabel->setText("Mode: Excluding");
    } else {
        currentMode = InteractionMode::Standard;
        ui->statusLabel->setText("");
        resetSelection();
    }
}

void MainWindow::on_linkNodesButton_clicked()
{
    if (currentMode == InteractionMode::Standard || currentMode == InteractionMode::Excluding) {
        currentMode = InteractionMode::Linking;
        ui->statusLabel->setText("Mode: Linking");
    } else {
        currentMode = InteractionMode::Standard;
        ui->statusLabel->setText("");
        resetSelection();
    }
}

void MainWindow::resetSelection()
{
    if (originNode) {
        originNode->updateColor();
        originNode = nullptr;
    }
    if (originSwitch) {
        originSwitch->updateAppearance();
        originSwitch = nullptr;
    }
}
