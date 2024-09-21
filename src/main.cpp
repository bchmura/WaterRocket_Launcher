#include <Arduino.h>
#include <ArduinoLog.h>
#include <Bounce2.h>
#include <ESPNowW.h>
#include <Wire.h>
#include <WiFi.h>
#include <TaskManagerIO.h>
#include <L298N.h>
#include <config.hpp>
#include <Libraries/ToggleDevice.hpp>
#include <Models/LaunchDisplay.hpp>
#include <Models/ApplicationState.hpp>
#include <Secrets/SecretConfig.hpp>


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
void setupEspNow();
void taskUpdateDisplay();
void taskUpdateMeasurements();
void loopCheckInputs();
void loopCheckSensors();
void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
void onSend(const uint8_t *mac_addr, esp_now_send_status_t status);
void sendMsg();

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
    setupEspNow();
    setupButtonsAndSwitches();
    setupSolenoidControl();
    myDisplay.Setup();
    taskManager.schedule(repeatMillis(1000), taskUpdateDisplay);
    taskManager.schedule(repeatMillis(250), taskUpdateMeasurements);
    Log.infoln("main setup complete");
}

void loop() {
    loopCheckInputs();
    loopCheckSensors();
    taskManager.runLoop();

    // ToggleState ts = ON;
    // solenoidControl.setState(ts);
    // delay(2000);
    // ts = OFF;
    // solenoidControl.setState(ts);
    // delay(2000);

}

void sendStatusToController() {
    uint8_t message[sizeof(appState) + 2];
    message[0] = 0x01;
    message[1] = 0x01;
    memcpy(&message[2], &appState, sizeof(appState));
    ESPNow.send_message(CONTROLLER_MAC, message, sizeof(message));
    Log.infoln("Sending app state size %d", sizeof(appState));
    Log.infoln("Sending app message size %d", sizeof(message));
    Log.infoln("Sending app state vert %T", appState.isLauncherVertical);

    for (int x = 0; x < sizeof(message); x++) {
        Serial.print(message[x], HEX);
        Serial.print(" ");
    }
}

// LOOP Code ///////////////////////////////////////////

void loopCheckInputs() {
    Log.traceln("loopCheckInputs");
    bouncePressurizeButton.update();
    if (bouncePressurizeButton.changed() && bouncePressurizeButton.pressed()) {
        Log.infoln("Pressure button was pressed");
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

void setupEspNow() {
    uint8_t baseMac[6];
    std::string macAddressString;

    // Get MAC address of the WiFi station interface
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    Log.infoln("Wifi station interface: %x:%x:%x:%x:%x:%x", baseMac[0],baseMac[1],baseMac[2],baseMac[3],baseMac[4],baseMac[5]);

    // Get the MAC address of the Wi-Fi AP interface
    esp_read_mac(baseMac, ESP_MAC_WIFI_SOFTAP);
    Log.infoln("Wifi softAP interface: %x:%x:%x:%x:%x:%x", baseMac[0],baseMac[1],baseMac[2],baseMac[3],baseMac[4],baseMac[5]);

    // Get the MAC address of the Ethernet interface
    esp_read_mac(baseMac, ESP_MAC_ETH);
    Log.infoln("Ethernet interface: %x:%x:%x:%x:%x:%x", baseMac[0],baseMac[1],baseMac[2],baseMac[3],baseMac[4],baseMac[5]);

    // Get the MAC address of the Bluetooth interface
    esp_read_mac(baseMac, ESP_MAC_BT);
    Log.infoln("Bluetooth interface: %x:%x:%x:%x:%x:%x", baseMac[0],baseMac[1],baseMac[2],baseMac[3],baseMac[4],baseMac[5]);

    WiFi.mode(WIFI_MODE_STA);
    //ESPNow.set_mac(LAUNCHER_MAC);
    WiFi.disconnect();
    ESPNow.init();
    ESPNow.add_peer(CONTROLLER_MAC);
    ESPNow.reg_recv_cb(onRecv);
    ESPNow.reg_send_cb(onSend);

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
    //Log.infoln("taskUpdateDisplay called");
    sendStatusToController();
    appState.isDirty = myDisplay.RunUpdate(appState);
    //appState.isDirty = false;
}

void taskUpdateMeasurements() {
    //Log.infoln("taskUpdateMeasurements called");
    // calls to pressure sensors





}

// Communications

void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
             mac_addr[5]);
    Serial.print("Last Packet Recv from: ");
    Serial.println(macStr);
    Serial.print("Last Packet Recv Data: ");
    // if it could be a string, print as one
    if (data[data_len - 1] == 0)
        Serial.printf("%s\n", data);
    // additionally print as hex
    for (int i = 0; i < data_len; i++) {
        Serial.printf("%x ", data[i]);
    }
    Serial.println("");
}

void sendMsg() {

}

// void setSolenoidOpen(bool reqState) {
//     if (reqState==appState.isSolendoidOpen) return;
//     Log.infoln("Setting Solenoid to %u ", reqState);
//     digitalWrite(SOLENOID_DATA_PIN, reqState);
//     appState.isSolendoidOpen = reqState;
//     appState.isDirty = true;
// }
