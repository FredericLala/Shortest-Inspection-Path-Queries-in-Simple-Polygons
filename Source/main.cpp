#include "mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "polygonwidget.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    // Create an instance of MainUI
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Shortest Inspection-Path Queries");

    // Show the main window
    mainWindow.showMaximized();

    // Execute the application
    return app.exec();
}
