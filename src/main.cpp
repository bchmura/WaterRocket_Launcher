#include <Arduino.h>
#include <ArduinoLog.h>
#include <Bounce2.h>
#include <Wire.h>
#include <TaskManagerIO.h>
#include <config.hpp>
#include <Libraries/ToggleDevice.hpp>
#include <Models/LaunchDisplay.hpp>
#include <Models/ApplicationState.hpp>

//#define LOG_LEVEL_SILENT  0
//#define LOG_LEVEL_FATAL   1
//#define LOG_LEVEL_ERROR   2
//#define LOG_LEVEL_WARNING 3
//#define LOG_LEVEL_INFO    4 - changes to state
//#define LOG_LEVEL_NOTICE  4 - what is running
//#define LOG_LEVEL_TRACE   5 - specific code within func
//#define LOG_LEVEL_VERBOSE 6 - detailed output

void setupLogging(int LogLevel);
void setupButtonsAndSwitches();
void setupI2CBus();
void setupSolenoidControl();
void taskUpdateDisplay();
void taskUpdateMeasurements();
void loopCheckInputs();
void loopCheckSensors();

ApplicationState appState;
Bounce2::Button bouncePressurizeButton = Bounce2::Button();
Bounce2::Button bounceVerticalSwitch = Bounce2::Button();
Bounce2::Button bounceLaunchButton = Bounce2::Button();
LaunchDisplay myDisplay;
ToggleDevice solenoidControl;

void setup() {
    Serial.begin(9600);
    delay(2000);
    Serial.println("BR-L starting");
    ///setupLogging(LOG_LEVEL_VERBOSE);
    setupLogging(LOG_LEVEL_NOTICE);
    setupI2CBus();
    setupButtonsAndSwitches();
    setupSolenoidControl();
    myDisplay.Setup();
    taskManager.schedule(repeatMillis(250), taskUpdateDisplay);
    taskManager.schedule(repeatMillis(250), taskUpdateMeasurements);
    Log.infoln("main setup complete");
}

void loop() {
    loopCheckInputs();
    loopCheckSensors();
    taskManager.runLoop();
}

// LOOP Code ///////////////////////////////////////////

void loopCheckInputs() {
    bouncePressurizeButton.update();
    if (bouncePressurizeButton.changed() && bouncePressurizeButton.pressed()) {
        Log.infoln("pressure button was pressed");
        appState.isSolendoidOpen = solenoidControl.toggle();
        appState.isDirty = true;
    }
};

void loopCheckSensors() {
    bounceVerticalSwitch.update();
    if (bounceVerticalSwitch.changed() && bounceVerticalSwitch.pressed()){
        Log.infoln("Launcher changed to vertical");
        appState.isLauncherVertical = true;
        appState.isDirty = true;
    } else if (bounceVerticalSwitch.changed() && bounceVerticalSwitch.released()) {
        Log.infoln("Launcher changed to not vertical");
        appState.isLauncherVertical = false;
        appState.isDirty = true;
    }

    if (appState.isDirty) {
        unsigned int rnd = 0;
        rnd = rand() % 100;
        Log.errorln("fake res pressure set to %d", rnd);
        appState.resevoirPressurePsi = rand() % RES_PRESSURE_LED_MAX_VALUE;
        rnd = rand() % 100;
        Log.errorln("fake rocket pressure set to %d", rnd);
        appState.rocketPressurePsi = rand() % ROCKET_PRESSURE_LED_MAX_VALUE;
    }
};


// SETUP CODE /////////////////////////////////////

void setupButtonsAndSwitches() {
    Log.traceln("setting up buttons and switches");
    bouncePressurizeButton.attach(BUTTON_TOGGLE_SOLENOID, INPUT_PULLDOWN); // Pressurize button
    bouncePressurizeButton.setPressedState(HIGH);
    bouncePressurizeButton.interval(5);

    bounceVerticalSwitch.attach(LAUNCHER_VERTICAL_PIN, INPUT_PULLDOWN); // vertical detection switch
    Log.noticeln("Button and switch setup complete");
}

void setupI2CBus() {
    Wire.begin(I2C_1_SDA, I2C_1_SDL, 100000);
    Log.verboseln("I2C Clock speed is %d", Wire.getClock());
    Log.noticeln("Setup I2C bus complete");
}

void setupLogging(const int LogLevel) {
    Log.begin(LogLevel, &Serial);
    Log.infoln("Logging started at level %d", LogLevel);
}

void setupSolenoidControl() {
    solenoidControl.setup(SOLENOID_DATA_PIN, OFF);
}

// TASKS ////////////
void taskUpdateDisplay() {
    Log.traceln("taskUpdateDisplay called");
    appState.isDirty = myDisplay.RunUpdate(appState);
}

void taskUpdateMeasurements() {
    Log.traceln("taskUpdateMeasurements called");
    // calls to pressure sensors
}
