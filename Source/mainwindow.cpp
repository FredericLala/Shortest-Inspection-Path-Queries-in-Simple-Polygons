#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent), currentStep(0)
{
	// Layout setup
	mainLayout = new QVBoxLayout(this);

	setupQueryControls();

	// Mode selection (Stepper or Autorun)
	modeSelector = new QComboBox(this);
	modeSelector->addItem("Stepper");
	modeSelector->addItem("Autorun");

	// Start button
	QPushButton* startButton = new QPushButton("Start", this);
	// Regenerate button
	QPushButton* regenerateButton = new QPushButton("Regenerate Polygon", this);
	// Create the slider
	slider = new QSlider(Qt::Horizontal);
	slider->setTickPosition(QSlider::TicksAbove);
	slider->setTickInterval(10);
	slider->setRange(10, 100); // Range of polygon size
	slider->setValue(40);    // Default size

	// Polygon display widget
	polygonWidget = new PolygonWidget(this);

	// Add widgets to layout
	mainLayout->addWidget(queryTypeSelector);
	mainLayout->addWidget(modeSelector);
	tickLabel();
	mainLayout->addWidget(slider);
	mainLayout->addWidget(regenerateButton);
	mainLayout->addWidget(startButton);
	// Step label to show current step description
	stepLabel = new QLabel("Click in Polygon to draw Start and Query", this);
	stepLabel->setFixedHeight(20); // Set the height to 20 pixels
	mainLayout->addWidget(stepLabel);
	setupStepper();
	mainLayout->addWidget(polygonWidget);

	// Connect the start button to the mode handling function
	connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);

	// Connect the regenerate button to polygon generation handling
	connect(regenerateButton, &QPushButton::clicked, this, &MainWindow::onRegenerateClicked);

	// Set the initial query mode to One Point Query
	queryTypeSelector->setCurrentText("Two Point Query"); // Select "One Point Query" by default
	onQueryTypeChanged(1);                 // Trigger mode change to initialize the mode

	modeSelector->setCurrentText("Autorun");
	updateUIForStepQ1();
}

void MainWindow::setupQueryControls()
{
	// Query type selector
	queryTypeSelector = new QComboBox(this);
	queryTypeSelector->addItem("One Point Query");
	queryTypeSelector->addItem("Two Point Query");

	connect(queryTypeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &MainWindow::onQueryTypeChanged);
}

void MainWindow::onQueryTypeChanged(int index)
{
	if (index == 0)
	{
		// Switch to One Point Query
		polygonWidget->setMode(1);
	}
	else
	{
		// Switch to Two Point Query
		polygonWidget->setMode(2);
	}
}

void MainWindow::onStartClicked()
{
	// Check selected mode
	QString mode = modeSelector->currentText();

	if (mode == "Stepper")
	{
		currentStep = 1;
		runStepperMode();
	}
	else if (mode == "Autorun")
	{
		runAutorunMode();
	}
}

void MainWindow::onRegenerateClicked()
{
	polygonWidget->clearCanvas();
	polygonWidget->constructPolygon(slider->value());
}

void MainWindow::runStepperMode()
{
	// Enable step-by-step controls for the user
	queryMode = queryTypeSelector->currentText();
	if (queryMode == "One Point Query")
	{
		polygonWidget->startStepperQ1();
		updateUIForStepQ1();
	}
}

void MainWindow::runAutorunMode()
{
	// Automatically perform all steps
	queryMode = queryTypeSelector->currentText();
	if (queryMode == "One Point Query")
	{
		polygonWidget->startAutoQ1(1000);
	}
	else
	{
		polygonWidget->startAutoQ2(1000);
	}
}

void MainWindow::setupStepper()
{
	// Stepper control buttons
	prevButton = new QPushButton("Previous", this);
	nextButton = new QPushButton("Next", this);

	connect(prevButton, &QPushButton::clicked, this, &MainWindow::onPrevStep);
	connect(nextButton, &QPushButton::clicked, this, &MainWindow::onNextStep);

	QHBoxLayout* stepperLayout = new QHBoxLayout();
	stepperLayout->addWidget(prevButton);
	stepperLayout->addWidget(nextButton);

	mainLayout->addLayout(stepperLayout);
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

void MainWindow::updateUIForStepQ1()
{
	// Update UI elements based on the current step
	switch (currentStep)
	{
	case 0:
		stepLabel->setText("Click in Polygon to Add Start and Query");
		break;
	case 1:
		stepLabel->setText("Perform Visibility Check");
		break;
	case 2:
		stepLabel->setText("Shortest Path Calculation");
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
		break;
	case 7:
		stepLabel->setText("Draw Optimal Point c");
		break;
	default:
		stepLabel->setText("End");
		nextButton->setEnabled(false);
		break;
	}
}

void MainWindow::tickLabel() {
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
		label->setAlignment(Qt::AlignCenter);
		tickLabelsLayout->addWidget(label);
	}

	mainLayout->addLayout(tickLabelsLayout);
}
