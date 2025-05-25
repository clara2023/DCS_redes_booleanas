#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <unordered_map>
#include "graphicnode.h"
#include "booleannetwork.h"
#include <QVector>
#include <QPushButton>

enum class InteractionMode {
    Standard,
    Linking,
    Excluding
};

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
    void on_runStepButton_clicked();
    void onInputButtonClicked();
    void on_removeNodeButton_clicked();
    void on_linkNodesButton_clicked();
    void resetSelection();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QVector<QPushButton*> inputButtonsList;

    int nodeCounter = 0;
    BooleanNetwork network;

    InteractionMode currentMode = InteractionMode::Standard;
    GraphicNode* originNode = nullptr;
    std::unordered_map<std::string, GraphicNode*> nodeMap;

    void createButtons();
};
#endif // MAINWINDOW_H
