#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicswitch.h"
#include "logictypes.h"
#include "booleannode.h"

#include <iostream>
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QStringList>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    connect(ui->exitButton, &QPushButton::clicked, this, [](){
        qApp->quit();
    });

    connect(ui->addNodeButton, &QPushButton::clicked, this, [=]() {
        QStringList functionOptions = allLogicFunctionNames();
        bool ok;
        QString selected = QInputDialog::getItem(this, "Função Lógica",
                                                 "Escolha a função lógica:",
                                                 functionOptions, 0, false, &ok);
        if (!ok || selected.isEmpty()) return;

        LogicFunction func = LogicFunction::NOT;  // valor padrão
        for (LogicFunction f : allLogicFunctions()) {
            if (toQString(f) == selected) {
                func = f;
                break;
            }
        }

        BooleanNode *newBooleanNode = new BooleanNode();
        newBooleanNode->setFunction(func);
        network.addNode(newBooleanNode);

        GraphicNode *newNode = new GraphicNode(10, 10);
        newNode->setLogicNode(newBooleanNode);
        newNode->setFunctionText(selected);
        scene->addItem(newNode);

        connect(newNode, &GraphicNode::clicked, this, [=](GraphicNode* clicked) {
           handleItemClicked(clicked, nullptr);
        });
    });
    createSwitches();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_runStepButton_clicked()
{
    network.simulateNetworkStep();
    for (QGraphicsItem* item : scene->items()) {
        GraphicNode* node = dynamic_cast<GraphicNode*>(item);
        if (node && node->getBooleanNode()) {
            node->updateColor();  // Atualiza cor com base no estado
            qDebug() << "Estado visual: " << node->state;
        }
    }
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

            QGraphicsLineItem* line = scene->addLine(QLineF(p1, p2), QPen(Qt::black, 2));

            if (originNode) originNode->addLine(line);
            if (originSwitch) originSwitch->addLine(line);
            if (node) node->addLine(line);
            if (sw) sw->addLine(line);

            GraphicNode* destinationNode = node ? node : originNode;
            BooleanNode* logicNode = destinationNode->getBooleanNode();

            if (logicNode) {
                if (originSwitch && destinationNode == node) {
                    logicNode->addInputSwitch(originSwitch);
                } else if (originNode && destinationNode == node) {
                    logicNode->addInputNode(originNode);
                } else if (sw && destinationNode == originNode) {
                    logicNode->addInputSwitch(sw);
                } else if (node && destinationNode == originNode) {
                    logicNode->addInputNode(node);
                }
            } else {
                qDebug() << "Erro: destino não possui BooleanNode!";
            }

            if (originNode) {
                originNode->updateColor();
                originNode->setFlag(QGraphicsItem::ItemIsMovable, false);
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
                sw->state = true;
            } else {
                sw->setText("0");
                sw->setStyleSheet("");
                sw->state = false;
            }
        }
    }
}

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
