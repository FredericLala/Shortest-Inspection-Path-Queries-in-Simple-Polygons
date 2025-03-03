#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent), currentStep(0)
{
	// Main layout setup
    mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);

    settingLayout = new QVBoxLayout();
    settingLayout->setContentsMargins(0,0,0,0);
    settingLayout->setSpacing(5);
    QWidget* settingContainer = new QWidget();
    settingContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    settingContainer->setLayout(settingLayout);

	polygonWidget = new PolygonWidget(this);
    polygonWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    polygonWidget->setStyleSheet(
        "background-color: #ffffff;"  // Background color
        "border-radius: 20px;"        // Rounded corners
        );

	// Selection
	polygonSelection();
    settingLayout->addWidget(createLine());
	querySelection();
    settingLayout->addWidget(createLine());
	runSelection();
    settingLayout->addWidget(createLine());

	// Clear Points Button
	QPushButton* clearPointsButton = new QPushButton("Clear Points", this);
	connect(clearPointsButton, &QPushButton::clicked, this, &MainWindow::onClearPointsClicked);
    settingLayout->addWidget(clearPointsButton);

	// Start Button
	QPushButton* startButton = new QPushButton("Start", this);
	connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    settingLayout->addWidget(startButton);

    // Log
    log = new QLabel("Log: ", this);
    log->setFixedHeight(20);
    settingLayout->addWidget(log);
    logOutput = new QPlainTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setFont(QFont("Courier", 10));
    logOutput->setStyleSheet("background-color: white; color: black;");
    logOutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    settingLayout->addWidget(logOutput);
    logRedirect = new LogRedirect(logOutput);
    std::cout.rdbuf(logRedirect);

    //settingLayout->addStretch();

    mainLayout->addWidget(settingContainer, 1);
    mainLayout->addWidget(polygonWidget, 2);
}

QFrame* MainWindow::createLine()
{
	QFrame* line = new QFrame(this);
	line->setFrameShape(QFrame::HLine);  // Horizontal line
	line->setFrameShadow(QFrame::Sunken);
	return line;
}

void MainWindow::onHideClicked() {
    polygonWidget->setVisible(false);
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

	runRadio2->setChecked(true); // Default to "Stepper"
	updateRunSelection(AUTO);

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



    settingLayout->addLayout(runLayout);
    settingLayout->addWidget(runModeWidget);
}

void MainWindow::updateRunSelection(RunMode mode)
{
	runModeEnum = mode;
	switch (runModeEnum)
	{
	case MainWindow::STEPPER:
		runModeWidget->setCurrentWidget(stepperLayoutWidget);
        polygonWidget->reset();
        currentStep = 0;
        updateLabel();
		break;
	case MainWindow::AUTO:
		runModeWidget->setCurrentWidget(autoLayoutWidget);
        polygonWidget->reset();
        currentStep = 0;
		break;
	default:
		break;
	}
}

void MainWindow::setupStepper()
{
	stepperLayoutWidget = new QWidget(this);
    QVBoxLayout* stepperLayout = new QVBoxLayout(stepperLayoutWidget);
    QHBoxLayout* buttonBox = new QHBoxLayout();

	// Stepper control buttons
	prevButton = new QPushButton("Previous", this);
	nextButton = new QPushButton("Next", this);
	connect(prevButton, &QPushButton::clicked, this, &MainWindow::onPrevStep);
	connect(nextButton, &QPushButton::clicked, this, &MainWindow::onNextStep);
    buttonBox->addWidget(prevButton);
    buttonBox->addWidget(nextButton);

    // Step label to show current step description
    stepLabel = new QLabel("Click on the Polygon to Select Points and Press Start", this);
    // Add to layout
    stepperLayout->addWidget(stepLabel);
    stepperLayout->addLayout(buttonBox);
}

void MainWindow::onNextStep()
{
	if (currentStep != 0)
	{
		currentStep++;
		polygonWidget->increaseStep();
        updateLabel();
	}
}

void MainWindow::onPrevStep()
{
	if (currentStep > 1)
		currentStep--;
	polygonWidget->decreaseStep();
    updateLabel();
}


void MainWindow::setupAuto()
{
    autoLayoutWidget = new QWidget(this);
    QVBoxLayout* autoLayout = new QVBoxLayout(autoLayoutWidget);

    // Create the slider
    intervalSlider = new QSlider(Qt::Horizontal);
    intervalSlider->setTickPosition(QSlider::TicksAbove);
    intervalSlider->setTickInterval(1);
    intervalSlider->setRange(0, 10);
    intervalSlider->setValue(0);    // Default value

    QLabel* valueLabel = new QLabel("Interval: 0 s");
    connect(intervalSlider, &QSlider::valueChanged, [=](double value) {
        valueLabel->setText(QString("Interval: %1 s").arg(value / 10));
    });

    // Add to layout
    autoLayout->addWidget(valueLabel);
    autoLayout->addWidget(intervalSlider);
}

////////////////////////////////////////////////////////////////

void MainWindow::onStartClicked()
{
	switch (runModeEnum)
	{
	case MainWindow::STEPPER:
		switch (queryModeEnum)
		{
		case PolygonWidget::EXACT:

			if (polygonWidget->isQueryPoint2Set()) {
				currentStep = 1;
				polygonWidget->startTwoPointQuery(0, true);
			}
			else if (polygonWidget->isQueryPoint1Set()) {
				currentStep = 1;
				polygonWidget->startOnePointQuery(0, true);
			}
			break;
		case PolygonWidget::APPROX:
            currentStep = 1;
            polygonWidget->startNApproximateQuery(0, true, epsilonSlider->value() / static_cast<double>(10));
			break;
		default:
			break;
		}
		break;

	case MainWindow::AUTO:
		switch (queryModeEnum)
		{
		case PolygonWidget::EXACT:
			if (polygonWidget->isQueryPoint2Set()) {
                polygonWidget->startTwoPointQuery(intervalSlider->value() * 100, false);
			}
			else if (polygonWidget->isQueryPoint1Set()) {
                polygonWidget->startOnePointQuery(intervalSlider->value() * 100, false);
			}

			break;
		case PolygonWidget::APPROX:
            polygonWidget->startNApproximateQuery(intervalSlider->value() * 100, false, epsilonSlider->value() / static_cast<double>(10));
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
    updateLabel();
}

void MainWindow::onClearPointsClicked() {
	polygonWidget->clearComputation();
}

void MainWindow::updateLabel()
{
    switch (queryModeEnum)
    {
    case PolygonWidget::EXACT:
        if (polygonWidget->isQueryPoint2Set()) {
            switch (polygonWidget->resultQ2.currentCase) {
            case TwoPointQuery::INTERSECTION:
                updateUIForIntersection();
                break;
            case TwoPointQuery::DOMINATION:
                updateUIForDomination();
                break;
            case TwoPointQuery::GENERAL:
                updateUIForGeneral();
                break;
            default:
                updateUIForQ2();
                break;
            }
        } else if (polygonWidget->isQueryPoint1Set()) {
            updateUIForStepQ1();
        }
        break;
    case PolygonWidget::APPROX:
        updateUIForApprox();
        break;
    default:
        break;
    }
}

void MainWindow::updateUIForStepQ1()
{
    // Update UI elements based on the current step
    switch (currentStep)
    {
    case 0:
        stepLabel->setText("Click on the Polygon to Select Points and Press Start");
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
        break;
    case 7:
        stepLabel->setText("Draw Optimal Point c");
        nextButton->setEnabled(true);
        break;
    case 8:
        stepLabel->setText("Draw Optimal Path | END");
        nextButton->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::updateUIForQ2()
{
    // Update UI elements based on the current step
    switch (currentStep)
    {
    case 0:
        stepLabel->setText("Click on the Polygon to Select Points and Press Start");
        prevButton->setEnabled(false);
        nextButton->setEnabled(false);
        break;
    case 1:
        stepLabel->setText("Perform Visibility Check");
        prevButton->setEnabled(false);
        nextButton->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::updateUIForIntersection()
{
    // Update UI elements based on the current step
    switch (currentStep)
    {
    case 0:
        stepLabel->setText("Click on the Polygon to Select Points and Press Start");
        prevButton->setEnabled(false);
        nextButton->setEnabled(false);
        break;
    case 1:
        stepLabel->setText("Perform Visibility Check");
        nextButton->setEnabled(true);
        break;
    case 2:
        stepLabel->setText("Draw the First Window");
        prevButton->setEnabled(true);
        break;
    case 3:
        stepLabel->setText("Draw the Second Window");
        break;
    case 4:
        stepLabel->setText("Calculate the Shortest Path to the First Mutually Visible Segment");
        break;
    case 5:
        stepLabel->setText("Calculate the Shortest Path to the Second Mutually Visible Segment");
        break;
    case 6:
        stepLabel->setText("Calculate the Shortest Path to the Remaining Segments");
        nextButton->setEnabled(true);
        break;
    case 7:
        stepLabel->setText("Highlight the Optimal Path | END");
        nextButton->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::updateUIForDomination()
{
    switch (currentStep)
    {
    case 0:
        stepLabel->setText("Click on the Polygon to Select Points and Press Start");
        prevButton->setEnabled(false);
        nextButton->setEnabled(false);
        break;
    case 1:
        stepLabel->setText("Perform Visibility Check");
        nextButton->setEnabled(true);
        break;
    case 2:
        stepLabel->setText("Draw the First Window");
        prevButton->setEnabled(true);
        break;
    case 3:
        stepLabel->setText("Draw the Second Window");
        break;
    case 4:
        stepLabel->setText("Draw Optimal Point c");
        nextButton->setEnabled(true);
        break;
    case 5:
        stepLabel->setText("Draw Optimal Path | END");
        nextButton->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::updateUIForGeneral()
{
    // Update UI elements based on the current step
    switch (currentStep)
    {
    case 0:
        stepLabel->setText("Click on the Polygon to Select Points and Press Start");
        prevButton->setEnabled(false);
        nextButton->setEnabled(false);
        break;
    case 1:
        stepLabel->setText("Perform Visibility Check");
        nextButton->setEnabled(true);
        break;
    case 2:
        stepLabel->setText("Draw the First Window");
        prevButton->setEnabled(true);
        break;
    case 3:
        stepLabel->setText("Draw the Second Window");
        break;
    case 4:
        stepLabel->setText("Calculate the Funnel");
        break;
    case 5:
        stepLabel->setText("Calculate the Hourglass");
        break;
    case 6:
        stepLabel->setText("Determine the Common Tangents");
        break;
    case 7:
        stepLabel->setText("Concatenate");
        break;
    case 8:
        stepLabel->setText("Draw Optimal Point c");
        nextButton->setEnabled(true);
        break;
    case 9:
        stepLabel->setText("Draw Optimal Path | END");
        nextButton->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::updateUIForApprox()
{
    // Update UI elements based on the current step
    switch (currentStep)
    {
    case 0:
        stepLabel->setText("Click on the Polygon to Select Points and Press Start");
        prevButton->setEnabled(false);
        nextButton->setEnabled(false);
        break;
    case 1:
        stepLabel->setText("Determine (2n-1)-Approximate Path");
        nextButton->setEnabled(true);
        break;
    case 2:
        stepLabel->setText("Draw Windows");
        prevButton->setEnabled(true);
        break;
    case 3:
        stepLabel->setText("Draw Disc");
        break;
    case 4:
        stepLabel->setText("Compute Intersection Window");
        break;
    case 5:
        stepLabel->setText("Generate Equally Spaced Points");
        break;
    case 6:
        stepLabel->setText("Calculate (1+Epsilon)-Appproximate Path | END");
        nextButton->setEnabled(true);
        break;
    case 7:
        stepLabel->setText("Calculate Exact Inspection-Path (only if |Q| = 2) | END");
        nextButton->setEnabled(false);
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////

void MainWindow::querySelection()
{
	queryModeWidget = new QStackedWidget(this);
	queryModeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setupApproximateQuery();
	setupExactQuery();
	queryModeWidget->addWidget(approximateLayoutWidget);
	queryModeWidget->addWidget(exactLayoutWidget);

	// Create a group of radio buttons for mode selection
	QRadioButton* queryRadio1 = new QRadioButton("Exact Query", this);
	QRadioButton* queryRadio2 = new QRadioButton("Approximate Query", this);
	queryRadio1->setChecked(true); // Default to "Exact Query"
	updateQuerySelection(PolygonWidget::EXACT);

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
		if (checked) updateQuerySelection(PolygonWidget::EXACT);
		});
	connect(queryRadio2, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) updateQuerySelection(PolygonWidget::APPROX);
		});

    settingLayout->addLayout(queryLayout);
    //settingLayout->addWidget(queryModeWidget);
    settingLayout->addWidget(approximateLayoutWidget);
    settingLayout->addWidget(exactLayoutWidget);
}

void MainWindow::updateQuerySelection(PolygonWidget::QueryMode mode)
{
	queryModeEnum = mode;
	switch (queryModeEnum) {
	case PolygonWidget::EXACT:
		polygonWidget->setQueryMode(PolygonWidget::EXACT);
        //queryModeWidget->setCurrentWidget(exactLayoutWidget);
        approximateLayoutWidget->setVisible(false);
        exactLayoutWidget->setVisible(true);
		break;
	case PolygonWidget::APPROX:
		polygonWidget->setQueryMode(PolygonWidget::APPROX);
        //queryModeWidget->setCurrentWidget(approximateLayoutWidget);
        approximateLayoutWidget->setVisible(true);
        exactLayoutWidget->setVisible(false);
		break;
	}
}

void MainWindow::setupApproximateQuery() {
	approximateLayoutWidget = new QWidget(this);
    QVBoxLayout* approximateLayout = new QVBoxLayout(approximateLayoutWidget);

	// Create the slider
	epsilonSlider = new QSlider(Qt::Horizontal);
	epsilonSlider->setTickPosition(QSlider::TicksAbove);
    epsilonSlider->setTickInterval(1);
    epsilonSlider->setRange(1, 10); // Range of epsilon values
    epsilonSlider->setValue(5);    // Default value

    QLabel* valueLabel = new QLabel("Epsilon Value: 0.5");
    connect(epsilonSlider, &QSlider::valueChanged, [=](double value) {
        valueLabel->setText(QString("Epsilon Value: %1").arg(value / 10));
    });

	// Add to layout
    approximateLayout->addWidget(valueLabel);
	approximateLayout->addWidget(epsilonSlider);
}

void MainWindow::setupExactQuery() {
	exactLayoutWidget = new QWidget(this);
    exactLayoutWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	QVBoxLayout* exactLayout = new QVBoxLayout(exactLayoutWidget);
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
    polyRadio1->setChecked(true); // Default to "Random"
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

    settingLayout->addLayout(polyLayout);
    settingLayout->addWidget(polygonModeWidget);
}

void MainWindow::updatePolySelection(PolyMode mode)
{
	switch (mode)
	{
	case RANDOM:
		polygonModeWidget->setCurrentWidget(randomLayoutWidget);
        polygonWidget->constructRandomPolygon(slider->value() * 10);
		break;
	case DRAW:
		polygonModeWidget->setCurrentWidget(drawnLayoutWidget);
		polygonWidget->prepareDrawnPolygon();
		break;
	case PICK:
		polygonModeWidget->setCurrentWidget(givenLayoutWidget);
		onGivenPolygonChanged(3);
		break;
	default:
		break;
	}
}
///
void MainWindow::setupRandomPolygon()
{
	randomLayoutWidget = new QWidget(this);
	QVBoxLayout* randomLayout = new QVBoxLayout(randomLayoutWidget);

	// Create the slider
	slider = new QSlider(Qt::Horizontal);
	slider->setTickPosition(QSlider::TicksAbove);
    slider->setTickInterval(1);
    slider->setRange(1, 10); // Range of polygon size
    slider->setValue(4);    // Default size

    QLabel* valueLabel = new QLabel("Polygon Size: 40");
    connect(slider, &QSlider::valueChanged, [=](double value) {
        valueLabel->setText(QString("Polygon Size: %1").arg(value * 10));
    });

	// Regenerate button
	QPushButton* regenerateButton = new QPushButton("Regenerate Polygon", this);
	connect(regenerateButton, &QPushButton::clicked, this, &MainWindow::onRegenerateClicked);

    randomLayout->addWidget(valueLabel);
	randomLayout->addWidget(slider);
	randomLayout->addWidget(regenerateButton);
}

void MainWindow::onRegenerateClicked()
{
	polygonWidget->clearCanvas();
    polygonWidget->constructRandomPolygon(slider->value() * 10);
}


void MainWindow::setupDrawPolygon()
{
	drawnLayoutWidget = new QWidget(this);
	QVBoxLayout* drawnLayout = new QVBoxLayout(drawnLayoutWidget);

	QPushButton* finishPolygon = new QPushButton("Finish Polygon", this);
	connect(finishPolygon, &QPushButton::clicked, this, &MainWindow::finishDrawnPolygon);

	// Add to layout
	drawnLayout->addWidget(finishPolygon);
}

void MainWindow::finishDrawnPolygon() {
	polygonWidget->finishDrawnPolygon();
}


void MainWindow::setupGivenPolygon()
{
	givenLayoutWidget = new QWidget(this);
	QVBoxLayout* givenLayout = new QVBoxLayout(givenLayoutWidget);

	QComboBox* givenPolygonSelector = new QComboBox(this);
    givenPolygonSelector->addItem("Q1: c at Foot of Perpendicular to r");
    givenPolygonSelector->addItem("Q1: c = a");
    givenPolygonSelector->addItem("Q1: c at Foot of Perpendicular to v1");
	givenPolygonSelector->addItem("Q2: Window Intersection");
	givenPolygonSelector->addItem("Q2: Window Domination");
    givenPolygonSelector->addItem("Q2: General Case - Closed Hourglass");
    givenPolygonSelector->addItem("Q2: General Case - Closed Hourglass | Alt. is Open");
    givenPolygonSelector->addItem("Q2: General Case - Open Hourglass");
    givenPolygonSelector->addItem("Test Polygon");
	connect(givenPolygonSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &MainWindow::onGivenPolygonChanged);

	givenLayout->addWidget(givenPolygonSelector);
}

void MainWindow::onGivenPolygonChanged(int index)
{
	polygonWidget->chooseExamplePolygon(index);
	polygonWidget->setFixedPoints(index);
}

////////////////////////////////////////////////////////////////





