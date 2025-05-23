#include "mainwindow.h"
#include "booleannetwork.h"
#include "booleannode.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

void createNetwork()
{
    BooleanNetwork network;

    BooleanNode nodeA("A");
    BooleanNode nodeB("B");
    BooleanNode nodeC("C");

    nodeA.defineLogicFunction([](const std::vector<bool>&) {
        return true;
    });

    nodeB.defineLogicFunction([](const std::vector<bool>& entries) {
        return entries.empty() ? false : entries[0];
    });

    nodeC.defineLogicFunction([](const std::vector<bool>& entries) {
        bool result;
        for (bool entry : entries) {
            result = result || entry;
        }
        return result;
    });

    network.addNode(nodeA);
    network.addNode(nodeB);
    network.addNode(nodeC);

    network.simulate(5);
    return;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DCS_redes_booleanas_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();

    //createNetwork();

    return a.exec();
}

