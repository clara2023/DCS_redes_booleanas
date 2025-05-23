#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <unordered_map>
#include "GraphicNode.h"
#include "booleannetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_runAllStepsButton_clicked();
    void on_addNodeButton_clicked();
    void on_runStepButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    int nodeCounter = 0;
    BooleanNetwork network;

    std::unordered_map<std::string, GraphicNode*> nodeMap;

    void updateColors();
};
#endif // MAINWINDOW_H
