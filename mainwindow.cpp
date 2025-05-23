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

    qreal x = nodeCounter * 50;
    qreal y = nodeCounter * 20;

    GraphicNode* node = new GraphicNode(x, y, 50, 50);

    scene->addItem(node);
}

void MainWindow::on_runStep_clicked()
{

}
