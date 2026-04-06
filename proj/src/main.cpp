#include <Arduino.h>

#include <WiFi.h>
#include <esp_now.h>


// #include <Wire.h>
// #include <driver/adc.h> 

// #define MPU_SDA 5
// #define MPU_SCL 4

// Adafruit_MPU6050 mpu;

// REPLACE WITH YOUR RECEIVER MAC Address
// f0:f5:bd:c9:66:50
// f0:f5:bd:c9:59:b0
// f0:f5:bd:c9:59:b0 publisher
// cc:ba:97:12:08:1c esp32s3
uint8_t broadcastAddress[] = {0xcc, 0xba, 0x97, 0x12, 0x08, 0x1c}; //
// uint8_t broadcastAddress[] = {0xf0, 0xf5, 0xbd, 0xc9, 0x66, 0x50};

// #include "MPU.cpp"
#define b_pin 5
#define a_pin 10
// #define Pin_8 9

typedef struct struct_message {
  char msg_char[1];
  int print_msg;
  char type[1];
} struct_message;

struct_message testData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\rLast Packet Send Status:\t");
  if (status == ESP_NOW_SEND_SUCCESS) {
	// Serial.print("\rTest:\t");
	digitalWrite(8, HIGH);
  } else {
	digitalWrite(8, LOW);
  }
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
//   digitalWrite(8, LOW);
}
int val = 0;

void setup() {
	Serial.begin(115200);

	// Wire.begin(MPU_SDA, MPU_SCL, 400000U);
	// Wire.setClock(400000);
	
	// pinMode(Power_pin, OUTPUT);

	pinMode(5, INPUT_PULLDOWN);
	pinMode(4, INPUT_PULLDOWN); 
	// pinMode(2,INPUT_PULLDOWN);

	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	// pinMode(4, OUTPUT); 
	// pinMode(b_pin, OUTPUT);
	digitalWrite(8, LOW);
	pinMode(8, OUTPUT);

	while (!Serial) {
		delay(10);
	}
	Serial.println("Starting.....");
	WiFi.mode(WIFI_STA);
	// // delay(5000);
	
	
	Serial.println("Trying to init esp-now");
	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
		return;
	} else {
		Serial.println("Succsefull initializing ESP-NOW");
	}

	esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

	peerInfo.channel = 0;  
	peerInfo.encrypt = false;
	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
	if (esp_now_add_peer(&peerInfo) != ESP_OK){
		Serial.println("Failed to add peer");
		return;
	}


	// mpuConnect();
	Serial.println("Ready.");
}

void loop() {
	// Serial.println("work?.");
	// analogWrite(4, 50);
	// analogWrite(5, 1);
	// // delay(500);
	// val = analogRead(2);
	// // if ((val) < 4095) {
	// 	Serial.println(val);
	// // }

	testData.msg_char[0] = ' ';
	testData.print_msg = 0;
	// digitalWrite(8, LOW);
	

	digitalWrite(1, HIGH);

	if (digitalRead(4) == HIGH) {
		testData.print_msg = 1;
		testData.msg_char[0] = '1';
		testData.type[0] = 's';
	}
	if (digitalRead(5) == HIGH) {
		testData.print_msg = 1;
		testData.msg_char[0] = '2';
		testData.type[0] = 's';
	}
	digitalWrite(1, LOW);

	digitalWrite(2, HIGH);

	if (digitalRead(4) == HIGH) {
		testData.print_msg = 1;
		testData.msg_char[0] = 'e';
		testData.type[0] = 'c';
	}
	if (digitalRead(5) == HIGH) {
		testData.print_msg = 1;
		testData.msg_char[0] = '+';
		testData.type[0] = 'c';
	}
	digitalWrite(2, LOW);

	// analogRead(Pin_10);
	// Serial.println(" V");
	// if (val%2 == 0) {
	// 	testData.a[0] = 'b';1
		
	// } else {
	// 	testData.a[0] = 'a';
	// }
	// val++;
	
	Serial.printf("\nSending status: ");
	Serial.printf("%i and msg: ", testData.print_msg);
	Serial.println(testData.msg_char[0]);
	esp_err_t result = esp_now_send(0, (uint8_t *) &testData, sizeof(struct_message));
	
	delay(100);
}