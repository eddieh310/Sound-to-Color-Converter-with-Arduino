#include "arduinoFFT.h"

// In this project I use the arduinoFFT library, this library has built in functionality for performing FFT
// github link: https://github.com/kosme/arduinoFFT/tree/master

// RGB LED pins
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

// Microphone pin
const int micPin = A0;

// Sound level threshold for LED activation
const int threshold = 550; 

// here is initalization of our samples and sampling Frequency
const uint16_t samples = 64;
const double samplingFrequency = 1000;

// vReal stores the sound data, and vImag (set to 0) is used by FFT for frequency calculations
double vReal[samples];
double vImag[samples];  

// initializing FFT object
// this object is able to perform the essential FFT operations
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // here i will iterate for as many samples as I have
  // and read in sound input 
  for (int i = 0; i < samples; i++){
    vReal[i] = analogRead(micPin);
    vImag[i] = 0;
    delayMicroseconds(1000000 / samplingFrequency);
    // using microseconds for smoother data 
  }
  // how that we have the input data, 
  // we perform FFT
  // the FFT library allows us to perform the essential operations in 3 steps
  // we will use Hamming Window function, and are going to perform forward FFT
  FFT.windowing(vReal,samples,FFT_WIN_TYP_HAMMING,FFT_FORWARD);
  // after windowing we will run FFT algorithm
  FFT.compute(vReal,vImag,samples,FFT_FORWARD);
  // now we will comput magnitude
  FFT.complexToMagnitude(vReal,vImag,samples);

  // at this point, vReal contains magnitudes

  // from here, we need to find the most dominant frequency
  // this is the frequency that will be displayed on the LED
  int domFreq = 0; // this will hold dominant frequency
  int maxIndex = 0; // this will be index of maximum magnitude
  double maxMag = 0; // this will hold maximum magnitude

  for (int i = 1; i < samples / 2; i++){
    // only need half because its mirror image
    if (vReal[i] > maxMag){
      maxMag = vReal[i];
      maxIndex = i;
    }
  }

  int dominantFrequency = maxIndex * (samplingFrequency / samples);
  Serial.println("Dominant Frequency: " + String(dominantFrequency));

  if (dominantFrequency < 100) {
    setColor(255, 0, 0);     // Red for very low frequencies
  } else if (dominantFrequency < 200) {
      setColor(255, 127, 0);   // Orange
  } else if (dominantFrequency < 300) {
      setColor(255, 255, 0);   // Yellow
  } else if (dominantFrequency < 400) {
      setColor(0, 255, 0);     // Green
  } else if (dominantFrequency < 500) {
      setColor(0, 255, 127);   // Aqua green
  } else if (dominantFrequency < 600) {
      setColor(0, 255, 255);   // Cyan
  } else if (dominantFrequency < 700) {
      setColor(0, 127, 255);   // Light blue
  } else if (dominantFrequency < 800) {
      setColor(0, 0, 255);     // Blue
  } else if (dominantFrequency < 900) {
      setColor(75, 0, 130);    // Indigo
  } else {
      setColor(148, 0, 211);   // Violet for very high frequencies
  }

  delay(100); 
}

// Function to set RGB color
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);   // Set the red pin (0-255)
  analogWrite(greenPin, green); // Set the green pin (0-255)
  analogWrite(bluePin, blue);  // Set the blue pin (0-255)
}
