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
#define Pin_8 8
#define Pin_10 10
#define Pin_8 8

typedef struct struct_message {
  char a[1];
  int b;
} struct_message;

struct_message testData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
int val = 0;

void setup() {
	Serial.begin(115200);

	// Wire.begin(MPU_SDA, MPU_SCL, 400000U);
	// Wire.setClock(400000);
	
	// pinMode(Power_pin, OUTPUT);

	// pinMode(pinin1, OUTPUT); 
	pinMode(A0, OUTPUT);

	// pinMode(pintrig, OUTPUT);
	pinMode(Pin_10, INPUT); 
	pinMode(Pin_8, INPUT);

	

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
	// analogWrite(A0, 100);
	// analogWrite(A1, 1);
	// delay(500);
	// val = analogRead(A2);
	// if ((val) < 4095) {
	// 	Serial.print(val/4);
	// }
	testData.a[0] = '\0';
	testData.b = 1;

	

	// digitalWrite(A0, HIGH);

	// if (digitalRead(Pin_10) == HIGH) {
	// 	testData.b = 2;
	// 	testData.a[0] = 'a';
	// }
	// if (digitalRead(Pin_8) == LOW) {
	// 	testData.b = 2;
	// 	testData.a[0] = 'b';
	// }
	// digitalWrite(A0, LOW);

	// analogRead(Pin_10);
	// Serial.println(" V");
	// if (val%2 == 0) {
	// 	testData.a[0] = 'b';
		
	// } else {
	// 	testData.a[0] = 'a';
	// }
	// val++;
	
	Serial.printf("Sending status: ");
	Serial.printf("%i ", testData.b);
	Serial.println(testData.a[0]);
	esp_err_t result = esp_now_send(0, (uint8_t *) &testData, sizeof(struct_message));
	
	delay(1000);
}