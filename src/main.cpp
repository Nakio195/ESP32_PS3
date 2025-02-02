#include <Arduino.h>
#include <Ps3Controller.h>
#include <driver/twai.h>

#include "MainController.h"

void printError(esp_err_t err) {
    switch (err) {
        case ESP_ERR_INVALID_ARG:
            Serial.println("Invalid Argument");
            break;
        case ESP_ERR_TIMEOUT:
            Serial.println("TX Queue Full (Timeout)");
            break;
        case ESP_FAIL:
            Serial.println("TWAI Controller in Bus-Off or Stopped");
            break;
        case ESP_ERR_INVALID_STATE:
            Serial.println("TWAI Driver Not Installed or Not Running");
            break;
        default:
            Serial.printf("Unknown Error: 0x%X\n", err);
            break;
    }
}

void checkTwaiStatus() {
    twai_status_info_t status;
    twai_get_status_info(&status);

    Serial.printf("TX Buffer: %d | RX Queue: %d | TX Error Counter: %d | RX Error Counter: %d\n",
                  status.msgs_to_tx, status.msgs_to_rx, status.tx_error_counter, status.rx_error_counter);

    Serial.printf("Bus State: ");
    switch (status.state) {
        case TWAI_STATE_STOPPED:
            Serial.println("Stopped");
            break;
        case TWAI_STATE_RUNNING:
            Serial.println("Running");
            break;
        case TWAI_STATE_BUS_OFF:
            Serial.println("Bus-Off (Critical Error)");
            break;
        default:
            Serial.println("Unknown State");
            break;
    }
}

void checkTwaiAlerts() {
    uint32_t alerts;
    if (twai_read_alerts(&alerts, pdMS_TO_TICKS(10)) == ESP_OK) {
        Serial.print("TWAI Alerts: ");

        if (alerts & TWAI_ALERT_TX_FAILED) Serial.print("[TX Failed] ");
        if (alerts & TWAI_ALERT_TX_SUCCESS) Serial.print("[TX Success] ");
        if (alerts & TWAI_ALERT_RX_QUEUE_FULL) Serial.print("[RX Queue Full] ");
        if (alerts & TWAI_ALERT_BUS_ERROR) Serial.print("[Bus Error] ");
        if (alerts & TWAI_ALERT_BUS_OFF) Serial.print("[Bus-Off] ");
        if (alerts & TWAI_ALERT_RX_FIFO_OVERRUN) Serial.print("[RX FIFO Overrun] ");

        Serial.println();
    }
}

bool TransmissionEnable = false;
int player = 0;
int battery = 0;

unsigned long previousTime = millis();
unsigned long controllerDataTimer = 0;
unsigned long controllerStatusTimer = 0;

void setup() {
  Serial.begin(115200);
  
  // TWAI Configuration
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NO_ACK);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  g_config.alerts_enabled = TWAI_ALERT_ALL; 

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("TWAI driver installed.");
  } else {
    Serial.println("Failed to install TWAI driver.");
    return;
  }

  // Start the TWAI driver
  if (twai_start() == ESP_OK)
    Serial.println("TWAI driver started.");
  else 
    Serial.println("Failed to start TWAI driver.");

  Ps3.attach(Controller.update);
  Ps3.attachOnConnect(Controller.onConnect);
  Ps3.attachOnDisconnect(Controller.onDisconnect);
  Ps3.begin("8c:7c:b5:2f:a1:ee"); // Seb
  //Ps3.begin("28:0d:fc:0f:22:4b"); // EA

  Serial.println("Ready.");
  
}

void loop() {
	unsigned long dt = millis() - previousTime;
	previousTime = millis();


	controllerDataTimer += dt;
	controllerStatusTimer += dt;

    if(Controller.isConnected == 1 && controllerDataTimer >= 200 && TransmissionEnable)
    {

    	uint8_t* controllerData = Controller.getControllerData();
    	twai_message_t dataFrame = { 0 };
    	dataFrame.identifier = 0x10; // Default OBD2 address;
    	dataFrame.extd = 0;
    	dataFrame.data_length_code = 8;
    	dataFrame.data[0] = controllerData[0];
    	dataFrame.data[1] = controllerData[1];
    	dataFrame.data[2] = controllerData[2];
    	dataFrame.data[3] = controllerData[3];    
    	dataFrame.data[4] = controllerData[4];    
    	dataFrame.data[5] = controllerData[5];
    	dataFrame.data[6] = controllerData[6];
    	dataFrame.data[7] = controllerData[7];
      
      twai_transmit(&dataFrame, 10);

    	controllerDataTimer = 0;
    	delay(1);
    }

    if(controllerStatusTimer > 1500 && TransmissionEnable)
    {
    	//uint8_t* controllerStatus = Controller.getControllerStatus();
    	twai_message_t statusFrame = { 0 };
    	statusFrame.identifier = 0x12;
    	statusFrame.extd = 0;
    	statusFrame.data_length_code = 6;
    	statusFrame.data[0] = 0x01;
    	statusFrame.data[1] = 0x02;
    	statusFrame.data[2] = 0x02;
    	statusFrame.data[3] = 0x02;    // Best to use 0xAA (0b10101010) instead of 0
    	statusFrame.data[4] = 0x02;    // CAN works better this way as it needs
    	statusFrame.data[5] = 0x02;    // to avoid bit-stuffing
        // Accepts both pointers and references
        uint8_t success = twai_transmit(&statusFrame, 10);

    	controllerStatusTimer = 0;
    	delay(1);
    }

    twai_message_t RX_Frame;
    if (twai_receive(&RX_Frame, pdMS_TO_TICKS(1)) == ESP_OK)
    {
        Serial.print("Received ID: 0x"); Serial.println(RX_Frame.identifier, HEX);

      // Check for specific message (ID = 0x123, first byte = 0xAA)
      if (RX_Frame.identifier == 0x123 && RX_Frame.data[0] == 0xAA)
          Serial.println("Specific Message Received!");
      else
          Serial.println("Message does not match criteria.");
    }
    else
    {
        Serial.println("No message received.");
    }
}
