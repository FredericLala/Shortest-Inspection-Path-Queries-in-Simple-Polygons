#include "mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "polygonwidget.h"

int main(int argc, char *argv[])
{
    /*     QApplication a(argc, argv);

        QMainWindow window;
        PolygonWidget *polygonWidget = new PolygonWidget;

        // Button to switch to Mode 1
        QPushButton *mode1Button = new QPushButton("Mode 1 (Start + 1 Query Point)");
        QObject::connect(mode1Button, &QPushButton::clicked, [=]()
                         { polygonWidget->setMode(1); });

        // Button to switch to Mode 2
        QPushButton *mode2Button = new QPushButton("Mode 2 (Start + 2 Query Points)");
        QObject::connect(mode2Button, &QPushButton::clicked, [=]()
                         { polygonWidget->setMode(2); });

        QPushButton *mode3Button = new QPushButton("Start");
        QObject::connect(mode3Button, &QPushButton::clicked, [=]()
                         { polygonWidget->setStep(1); });

        // Layout to place buttons and widget
        QWidget *centralWidget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(mode1Button);
        layout->addWidget(mode2Button);
        layout->addWidget(mode3Button);
        layout->addWidget(polygonWidget);
        centralWidget->setLayout(layout);

        window.setCentralWidget(centralWidget);
        window.resize(600, 600);
        window.show();

        return a.exec(); */

    QApplication app(argc, argv);

    // Create an instance of MainUI
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Polygon Query Application");
    mainWindow.resize(800, 600); // Set an initial window size

    // Show the main window
    mainWindow.show();

    // Execute the application
    return app.exec();
}
