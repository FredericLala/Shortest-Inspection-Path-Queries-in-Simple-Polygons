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
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	// Polygon display widget
	PolygonWidget* polygonWidget;
	QFrame* createLine();

	QLabel* log;

	QHBoxLayout* createTickLabel();

	QStackedWidget* runModeWidget;
	QWidget* stepperLayoutWidget;
	QWidget* autoLayoutWidget;

	QStackedWidget* polygonModeWidget;
	QWidget* randomLayoutWidget;
	QWidget* drawnLayoutWidget;
	QWidget* givenLayoutWidget;


	// Stepper control buttons
	QPushButton* nextButton;
	QPushButton* prevButton;

	// Stepper stages
	int currentStep;
	QLabel* stepLabel;


	// Query controls
	QComboBox* queryTypeSelector;
	OnePointQuery* onePointQuery;
	TwoPointQuery* twoPointQuery;

	// Shortest path handler
	ShortestPath* shortestPath;

	// Main layout
	QVBoxLayout* mainLayout;

	// Initializes the stepper stages and query options
	void setupStepper();
	void updateUIForStepQ1();


	//
	QComboBox* modeSelector;
	int index;
	QString queryMode;
	QSlider* slider;
	enum RunMode {STEPPER, AUTO};
	enum PolyMode { RANDOM, DRAW, PICK };

	void querySelection();
	void runSelection();
	void polygonSelection();

	PolygonWidget::QueryMode queryModeEnum;
	RunMode runModeEnum;

private Q_SLOTS:
	void onNextStep();            // Advances the stepper to the next stage
	void onPrevStep();            // Moves the stepper back to the previous stage


	void setupRandomPolygon();

	void onStartClicked();
	void onClearPointsClicked();
	void onRegenerateClicked();
	void setupDrawPolygon();
	void finishDrawnPolygon();
	void setupAuto();

	void updateQuerySelection(PolygonWidget::QueryMode mode);
	void updateRunSelection(RunMode mode);
	void updatePolySelection(PolyMode mode);

	void setupGivenPolygon();
	void onGivenPolygonChanged(int index);
};
