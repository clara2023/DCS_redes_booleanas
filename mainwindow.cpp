#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_runAllStepsButton_clicked()
{
    std::cout << "Run all steps clicked";
}

void MainWindow::on_addNodeButton_clicked()
{
    nodeCounter++;
    QString id = "N" + QString::number(nodeCounter);

    BooleanNode newNode(id.toStdString());
    newNode.defineLogicFunction([](const std::vector<bool>& entries) {
        /* definicao provisoria, somente testes  */
        return (std::rand() % 2) == 0;
    });
    network.addNode(newNode);

    qreal x = nodeCounter * 50;
    qreal y = nodeCounter * 20;

    GraphicNode* node = new GraphicNode(x, y, 40, 40);

    scene->addItem(node);
    nodeMap[id.toStdString()] = node;
}

void MainWindow::on_runStepButton_clicked()
{
    network.simulateStep();
    updateColors();
}

void MainWindow::updateColors()
{
    const auto& nodes = network.getNodes();
    for (const auto& node : nodes) {
        const std::string& id = node.getId();
        bool state = node.getCurrentState();

        if (nodeMap.find(id) != nodeMap.end()) {
            nodeMap[id]->updateColor(state);
        }
    }
}
