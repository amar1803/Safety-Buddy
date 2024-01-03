#include <SoftwareSerial.h>

// Configure the GSM and GPS modules
SoftwareSerial gsmModule(7, 8); // RX, TX for the GSM module
SoftwareSerial gpsModule(10, 11); // RX, TX for the GPS module

// Define the button pin, buzzer pin, and default location
int buttonPin = 2;
int buzzerPin = 3; // Change to the actual pin where your buzzer is connected
String defaultLatitude = "40.7128"; // Replace with your default latitude
String defaultLongitude = "-74.0060"; // Replace with your default longitude

unsigned long buzzerStartTime = 0;
const unsigned long buzzerDuration = 10000; // 10 seconds

void setup() {
  // Initialize the serial communication with the computer
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port boards only
  }
  Serial.println("Serial communication initialized");

  // Set up the button, buzzer, and initialize the GSM and GPS communication
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, LOW);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  gsmModule.begin(9600);
  gpsModule.begin(9600);
  Serial.println("GSM and GPS modules initialized");

  delay(1000);

  // Initialize the GSM module by sending an AT command
  gsmModule.println("AT");
  delay(1000);
  gsmModule.println("AT+CMGF=1"); // Set the GSM module to send SMS in text mode
  delay(1000);
  Serial.println("GSM module initialized");
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Button pressed");

    // Activate the buzzer for 10 seconds
    buzzerStartTime = millis();
    while (millis() - buzzerStartTime < buzzerDuration) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
    Serial.println("Buzzer activated for 10 seconds");

    // Fetch GPS data
    String gpsData = getGPSData();
    Serial.println("GPS Data: " + gpsData);
    
    // Customize the message with the location data as a Google Maps link
    String message;

    if (gpsData.length() > 0) {
      message = "Help! I need assistance. My location is: https://www.google.com/maps/place/" + gpsData;
    } else {
      // Use default values if GPS data is not available
      message = "Help! I need assistance. My location is: https://www.google.com/maps/place/" + defaultLatitude + "," + defaultLongitude;
    }

    Serial.println("Sending message: " + message);

    // Send the message
    gsmModule.println("AT+CMGS=\"+917087874715\""); // Replace with the recipient's phone number
    delay(1000);
    gsmModule.println(message);
    delay(100);
    gsmModule.println((char)26); // ASCII code for Ctrl+Z
    delay(5000);
    Serial.println("Message sent");

    // Deactivate the buzzer
    digitalWrite(buzzerPin, LOW);
    Serial.println("Buzzer deactivated");
  }

  delay(1000); // Adjust the delay based on your requirements
}
 
String getGPSData() {
  String data;
  
  while (gpsModule.available() > 0) {
    char c = gpsModule.read();
    data += c;
  }
  
  
  
  return data;
}
