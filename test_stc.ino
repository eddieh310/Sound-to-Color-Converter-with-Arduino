// Pin definitions for RGB LED
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  Serial.begin(115200); // Set baud rate to match Python script
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setColor(0, 0, 0); 
}

void loop() {
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n'); // Read input from serial, this is sent from
    // the python script
    double receivedFrequency = receivedData.toDouble();

    // Process received frequency and set LED color
    processFrequency(receivedFrequency);
  }
}

// Function to set RGB LED color
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Function to process frequency and set LED color
void processFrequency(double frequency) {
  if (frequency < 50) {
    setColor(255, 0, 0); // Red
  } else if (frequency < 100) {
    setColor(255, 128, 0); // Orange
  } else if (frequency < 150) {
    setColor(255, 255, 0); // Yellow
  } else if (frequency < 200) {
    setColor(0, 255, 0); // Green
  } else if (frequency < 300) {
    setColor(0, 255, 255); // Cyan
  } else if (frequency < 400) {
    setColor(0, 0, 255); // Blue
  } else if (frequency < 500) {
    setColor(128, 0, 255); // Violet
  } else {
    setColor(255, 0, 255); // Magenta
  }
}
