// Microphone Test Script
// This script reads analog input from a microphone connected to the Arduino
// and prints the values to the serial monitor.

const int microphonePin = A0;  // Pin where the microphone is connected

void setup() {
  Serial.begin(9600);          // Start serial communication at 9600 baud
  pinMode(microphonePin, INPUT); // Set the microphone pin as an input
}

void loop() {
  int microphoneValue = analogRead(microphonePin); // Read value from the microphone
  Serial.println(microphoneValue);                 // Print the value to the serial monitor
  delay(100);                                      // Wait a bit before the next reading
}
