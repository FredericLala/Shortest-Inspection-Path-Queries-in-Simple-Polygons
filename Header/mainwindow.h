#pragma once

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "PolygonWidget.h"
#include "OnePointQuery.h"
#include "TwoPointQuery.h"
#include "ShortestPath.h"
#include <QMainWindow>
#include <QApplication>
#include <QSlider>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private Q_SLOTS:
    void onNextStep();            // Advances the stepper to the next stage
    void onPrevStep();            // Moves the stepper back to the previous stage
    void onQueryTypeChanged(int); // Updates UI based on query type selection

    void onStartClicked();
    void onRegenerateClicked();
    void runStepperMode();
    void runAutorunMode();

    void onPolyChanged(int);

    void drawChosenPolygon();

private:
    // Stepper control buttons
    QPushButton *nextButton;
    QPushButton *prevButton;

    // Stepper stages
    int currentStep;
    QLabel *stepLabel;

    // Polygon display widget
    PolygonWidget *polygonWidget;

    // Query controls
    QComboBox *queryTypeSelector;
    OnePointQuery *onePointQuery;
    TwoPointQuery *twoPointQuery;

    // Shortest path handler
    ShortestPath *shortestPath;

    // Main layout
    QVBoxLayout *mainLayout;

    // Initializes the stepper stages and query options
    void setupStepper();
    void setupQueryControls();
    void updateUIForStepQ1();

    void tickLabel();

    void polygonMode();


    //
    QComboBox *modeSelector;
    int index;
    QString queryMode;
    QSlider *slider;
};
