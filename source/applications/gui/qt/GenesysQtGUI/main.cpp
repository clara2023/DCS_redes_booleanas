#include "mainwindow.h"
#include <QApplication>
//#include <QDesktopWidget> //removed from qt6
#include <QScreen>

#include "../../../GenesysApplication_if.h"
#include "../../../TraitsApp.h"
#include "../../../terminal/TraitsTerminalApp.h"
#include "TraitsGUI.h"

int mainGraphicQtApp(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	if (TraitsGUI<GMainWindow>::startMaximized) {
        w.setWindowState(Qt::WindowMaximized);
        QRect screenGeometry = QApplication::primaryScreen()->availableGeometry(); //QApplication::desktop()->availableGeometry();
        w.resize(screenGeometry.width(), screenGeometry.height());
        //w.showNormal();
        w.showMaximized();
    } else {
        w.show();
    }
	return a.exec();
}

int mainTerminalApp(int argc, char *argv[]) {
	GenesysApplication_if *app = new TraitsTerminalApp<GenesysApplication_if>::Application();
    return app->main(argc, argv);
}

int main(int argc, char *argv[]) {
    if (TraitsApp<GenesysApplication_if>::runGraphicalUserInterface) {
		return mainGraphicQtApp(argc, argv);
	} else {
		return mainTerminalApp(argc, argv);
	}
}

