#pragma once

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "polygonwidget.h"
#include "onepointquery.h"
#include "twopointquery.h"
#include "shortestpath.h"
#include <QMainWindow>
#include <QApplication>
#include <QSlider>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QSplitter>
#include "logredirect.h"
#include <QPlainTextEdit>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	// Polygon display widget
	PolygonWidget* polygonWidget;
    QWidget* settingWidget;
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

	QStackedWidget* queryModeWidget;
	QWidget* approximateLayoutWidget;
	QWidget* exactLayoutWidget;


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
    QHBoxLayout* mainLayout;
    QVBoxLayout* settingLayout;

	// Initializes the stepper stages and query options
	void setupStepper();
    void updateLabel();
	void updateUIForStepQ1();
    void updateUIForQ2();
    void updateUIForIntersection();
    void updateUIForDomination();
    void updateUIForGeneral();
    void updateUIForApprox();

	//
	QComboBox* modeSelector;
	int index;
	QString queryMode;
	QSlider* slider;
	QSlider* epsilonSlider;
	QSlider* intervalSlider;
	enum RunMode {STEPPER, AUTO};
	enum PolyMode { RANDOM, DRAW, PICK };

	void querySelection();
	void runSelection();
	void polygonSelection();

	PolygonWidget::QueryMode queryModeEnum;
	RunMode runModeEnum;

    QPlainTextEdit* logOutput;
    LogRedirect* logRedirect;

private Q_SLOTS:
	void onNextStep();            // Advances the stepper to the next stage
	void onPrevStep();            // Moves the stepper back to the previous stage


	void setupRandomPolygon();

	void onStartClicked();
    void onHideClicked();
	void onClearPointsClicked();
	void onRegenerateClicked();
	void setupDrawPolygon();
	void finishDrawnPolygon();
	void setupAuto();

	void updateQuerySelection(PolygonWidget::QueryMode mode);
	void setupApproximateQuery();
	void setupExactQuery();
	void updateRunSelection(RunMode mode);
	void updatePolySelection(PolyMode mode);

	void setupGivenPolygon();
	void onGivenPolygonChanged(int index);
};
