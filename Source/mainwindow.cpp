#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent), currentStep(0)
{
	// Main layout setup
	mainLayout = new QVBoxLayout(this);
	polygonWidget = new PolygonWidget(this);
	mainLayout->setSpacing(10);

	// Selection
	polygonSelection();
	mainLayout->addWidget(createLine());
	querySelection();
	mainLayout->addWidget(createLine());
	runSelection();
	mainLayout->addWidget(createLine());

	// Start button
	QPushButton* startButton = new QPushButton("Start", this);
	connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
	mainLayout->addWidget(startButton);

	// Polygon display widget
	mainLayout->addWidget(polygonWidget);

	log = new QLabel("Log: ", this);
	log->setFixedHeight(20);
	mainLayout->addWidget(log);
	//log->setText("Log: " + polygonWidget->getLog());
}

QFrame* MainWindow::createLine()
{
	QFrame* line = new QFrame(this);
	line->setFrameShape(QFrame::HLine);  // Horizontal line
	line->setFrameShadow(QFrame::Sunken);
	return line;
}

////////////////////////////////////////////////////////////////

void MainWindow::runSelection()
{
	runModeWidget = new QStackedWidget(this);
	runModeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setupStepper();
	setupAuto();
	runModeWidget->addWidget(stepperLayoutWidget);
	runModeWidget->addWidget(autoLayoutWidget);

	// Create a group of radio buttons for mode selection
	QRadioButton* runRadio1 = new QRadioButton("Stepper", this);
	QRadioButton* runRadio2 = new QRadioButton("Autorun", this);

	runRadio1->setChecked(true); // Default to "Stepper"
	updateRunSelection(STEPPER);

	// Group Buttons
	QButtonGroup* runButtons = new QButtonGroup();
	runButtons->addButton(runRadio1);
	runButtons->addButton(runRadio2);

	// Add Buttons to a Layout
	QHBoxLayout* runLayout = new QHBoxLayout();
	runLayout->addWidget(runRadio1);
	runLayout->addWidget(runRadio2);

	// Connect the radio buttons to handle mode switching
	connect(runRadio1, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updateRunSelection(STEPPER);
		});
	connect(runRadio2, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updateRunSelection(AUTO);
		});

	mainLayout->addLayout(runLayout);
	mainLayout->addWidget(runModeWidget);
}

void MainWindow::updateRunSelection(RunMode mode)
{
	runModeEnum = mode;
	switch (runModeEnum)
	{
	case MainWindow::STEPPER:
		runModeWidget->setCurrentWidget(stepperLayoutWidget);
		break;
	case MainWindow::AUTO:
		runModeWidget->setCurrentWidget(autoLayoutWidget);
		break;
	default:
		break;
	}
}

void MainWindow::setupStepper()
{
	stepperLayoutWidget = new QWidget(this);
	QHBoxLayout* stepperLayout = new QHBoxLayout(stepperLayoutWidget);

	// Stepper control buttons
	prevButton = new QPushButton("Previous", this);
	nextButton = new QPushButton("Next", this);
	connect(prevButton, &QPushButton::clicked, this, &MainWindow::onPrevStep);
	connect(nextButton, &QPushButton::clicked, this, &MainWindow::onNextStep);

	// Step label to show current step description
	stepLabel = new QLabel("Click in Polygon to draw Start and Query", this);

	// Add to layout
	stepperLayout->addWidget(prevButton);
	stepperLayout->addWidget(stepLabel);
	stepLabel->setAlignment(Qt::AlignCenter);
	updateUIForStepQ1();
	stepperLayout->addWidget(nextButton);
}

void MainWindow::onNextStep()
{
	if (currentStep != 0)
	{
		currentStep++;
		polygonWidget->increaseStep();
		updateUIForStepQ1();
	}
}

void MainWindow::onPrevStep()
{
	if (currentStep > 1)
		currentStep--;
	polygonWidget->decreaseStep();
	updateUIForStepQ1();
}


void MainWindow::setupAuto()
{
	autoLayoutWidget = new QWidget(this);
	QHBoxLayout* autoLayout = new QHBoxLayout(autoLayoutWidget);
}

////////////////////////////////////////////////////////////////

void MainWindow::onStartClicked()
{
	switch (runModeEnum)
	{
	case MainWindow::STEPPER:
		switch (queryModeEnum)
		{
		case MainWindow::ONE:
			currentStep = 1;
			polygonWidget->startStepperQ1();
			updateUIForStepQ1();
			break;
		case MainWindow::TWO:
			currentStep = 1;
			polygonWidget->startStepperQ1();
			break;
		default:
			break;
		}
		break;

	case MainWindow::AUTO:
		switch (queryModeEnum)
		{
		case MainWindow::ONE:
			polygonWidget->startAutoQ1(1000);
			break;
		case MainWindow::TWO:
			polygonWidget->startAutoQ2(1000);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}
////////////////////////////////////////////////////////////////

void MainWindow::querySelection()
{
	// Create a group of radio buttons for mode selection
	QRadioButton* queryRadio1 = new QRadioButton("One Point Query", this);
	QRadioButton* queryRadio2 = new QRadioButton("Two Point Query", this);
	queryRadio1->setChecked(true); // Default to "One Point Query"
	updateQuerySelection(ONE);

	// Group Buttons
	QButtonGroup* queryButtons = new QButtonGroup();
	queryButtons->addButton(queryRadio1);
	queryButtons->addButton(queryRadio2);

	// Add Buttons to a Layout
	QHBoxLayout* queryLayout = new QHBoxLayout();
	queryLayout->addWidget(queryRadio1);
	queryLayout->addWidget(queryRadio2);

	// Connect the radio buttons to handle mode switching
	connect(queryRadio1, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updateQuerySelection(ONE);
		});
	connect(queryRadio2, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updateQuerySelection(TWO);
		});

	mainLayout->addLayout(queryLayout);
}

void MainWindow::updateQuerySelection(QueryMode mode)
{
	queryModeEnum = mode;
	switch (queryModeEnum) {
	case ONE:
		polygonWidget->setMode(1);
		break;
	case TWO:
		polygonWidget->setMode(2);
		break;
	}
}

////////////////////////////////////////////////////////////////

void MainWindow::polygonSelection()
{
	polygonModeWidget = new QStackedWidget(this);
	polygonModeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setupRandomPolygon();
	setupDrawPolygon();
	setupGivenPolygon();
	polygonModeWidget->addWidget(randomLayoutWidget);
	polygonModeWidget->addWidget(drawnLayoutWidget);
	polygonModeWidget->addWidget(givenLayoutWidget);

	// Create a group of radio buttons for mode selection
	QRadioButton* polyRadio1 = new QRadioButton("Random Polygon", this);
	QRadioButton* polyRadio2 = new QRadioButton("Draw a Polygon", this);
	QRadioButton* polyRadio3 = new QRadioButton("Pick a Polygon", this);
	polyRadio1->setChecked(true); // Default to "Random Polygon"
	updatePolySelection(RANDOM);

	// Group Buttons
	QButtonGroup* polyButtons = new QButtonGroup();
	polyButtons->addButton(polyRadio1);
	polyButtons->addButton(polyRadio2);
	polyButtons->addButton(polyRadio3);

	// Add Buttons to a Layout
	QHBoxLayout* polyLayout = new QHBoxLayout();
	polyLayout->addWidget(polyRadio1);
	polyLayout->addWidget(polyRadio2);
	polyLayout->addWidget(polyRadio3);


	// Connect the radio buttons to handle mode switching
	connect(polyRadio1, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updatePolySelection(RANDOM);
		});
	connect(polyRadio2, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updatePolySelection(DRAW);
		});
	connect(polyRadio3, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updatePolySelection(PICK);
		});

	polyLayout->setSpacing(5);

	mainLayout->addLayout(polyLayout);
	mainLayout->addWidget(polygonModeWidget);
}

void MainWindow::updatePolySelection(PolyMode mode)
{
	switch (mode)
	{
	case MainWindow::RANDOM:
		polygonModeWidget->setCurrentWidget(randomLayoutWidget);
		polygonWidget->setPolygonMode(0);
		break;
	case MainWindow::DRAW:
		polygonModeWidget->setCurrentWidget(drawnLayoutWidget);
		polygonWidget->setPolygonMode(1);
		break;
	case MainWindow::PICK:
		polygonModeWidget->setCurrentWidget(givenLayoutWidget);
		polygonWidget->setPolygonMode(2);
		break;
	default:
		break;
	}
}

void MainWindow::setupRandomPolygon()
{
	randomLayoutWidget = new QWidget(this);
	QVBoxLayout* randomLayout = new QVBoxLayout(randomLayoutWidget);

	// Create the slider
	slider = new QSlider(Qt::Horizontal);
	// Add ticks to the slider
	slider->setTickPosition(QSlider::TicksAbove);
	slider->setTickInterval(10);
	slider->setRange(10, 100); // Range of polygon size
	slider->setValue(40);    // Default size

	// Create labels for the ticks
	QHBoxLayout* tickLabelsLayout = createTickLabel();

	// Regenerate button
	QPushButton* regenerateButton = new QPushButton("Regenerate Polygon", this);
	// Connect the regenerate button to polygon generation handling
	connect(regenerateButton, &QPushButton::clicked, this, &MainWindow::onRegenerateClicked);

	// Add to layout
	randomLayout->addWidget(slider);
	randomLayout->addWidget(regenerateButton);
}

QHBoxLayout* MainWindow::createTickLabel()
{
	// Create a layout for tick labels
	QHBoxLayout* tickLabelsLayout = new QHBoxLayout();
	tickLabelsLayout->setSpacing(60);

	int minValue = slider->minimum();
	int maxValue = slider->maximum();
	int tickInterval = slider->tickInterval();

	// Add tick labels
	for (int i = minValue; i <= maxValue; i += tickInterval) {
		QLabel* label = new QLabel(QString::number(i));
		label->setFixedHeight(20);
		//label->setAlignment(Qt::AlignCenter);
		tickLabelsLayout->addWidget(label);
	}

	//mainLayout->addLayout(tickLabelsLayout);
	return tickLabelsLayout;
}

void MainWindow::onRegenerateClicked()
{
	polygonWidget->clearCanvas();
	polygonWidget->constructPolygon(slider->value());
}


void MainWindow::setupDrawPolygon()
{
	drawnLayoutWidget = new QWidget(this);
	QVBoxLayout* drawnLayout = new QVBoxLayout(drawnLayoutWidget);

	QPushButton* finishPolygon = new QPushButton("Finish Polygon", this);
	connect(finishPolygon, &QPushButton::clicked, this, &MainWindow::drawChosenPolygon);

	// Add to layout
	drawnLayout->addWidget(finishPolygon);
}

void MainWindow::drawChosenPolygon() {
	polygonWidget->setPolygonMode(3);
}


void MainWindow::setupGivenPolygon()
{
	givenLayoutWidget = new QWidget(this);
	QVBoxLayout* givenLayout = new QVBoxLayout(givenLayoutWidget);
}

////////////////////////////////////////////////////////////////

void MainWindow::updateUIForStepQ1()
{
	// Update UI elements based on the current step
	switch (currentStep)
	{
	case 0:
		stepLabel->setText("Click in Polygon to Add Points and Press Start");
		prevButton->setEnabled(false);
		nextButton->setEnabled(false);
		break;
	case 1:
		stepLabel->setText("Perform Visibility Check");
		nextButton->setEnabled(true);
		break;
	case 2:
		stepLabel->setText("Shortest Path Calculation");
		prevButton->setEnabled(true);
		break;
	case 3:
		stepLabel->setText("Calculate Point a");
		break;
	case 4:
		stepLabel->setText("Calculate Point b and Draw Window");
		break;
	case 5:
		stepLabel->setText("Calculate Point Funnel Root");
		break;
	case 6:
		stepLabel->setText("Calculate Funnel Sides");
		nextButton->setEnabled(true);
		break;
	case 7:
		stepLabel->setText("Draw Optimal Point c | END");
		nextButton->setEnabled(false);
		break;
	default:
		break;
	}
}





