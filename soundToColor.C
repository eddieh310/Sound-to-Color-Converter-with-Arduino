#include "arduinoFFT.h"

// here is a link to the github repository for the arduinoFFT library: https://github.com/kosme/arduinoFFT/tree/master
// i use the examples in there as a reference for performing the FFT operations
// specifically I read FFT_01 in the examples folder to really get a good grasp of how to use the library
 
#define SAMPLES 128             //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.
 
arduinoFFT FFT = arduinoFFT();

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int micPin = A0;
 
unsigned int samplingPeriod;
unsigned long microSeconds;
 
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

const int buttonPin1 = 7; // Normal button
const int buttonPin2 = 4; // Warm button
const int buttonPin3 = 2; // Cold button
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int mode = 0; // 0 = normal, 1 = warm, 2 = cold
 
void setup() 
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(buttonPin1,INPUT_PULLUP);
    pinMode(buttonPin2,INPUT_PULLUP);
    pinMode(buttonPin3,INPUT_PULLUP);
    Serial.begin(9600); //Baud rate for the Serial Monitor
   // Serial.println("chicken");
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
}
 
void loop() 
{  
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);  
    buttonState3 = digitalRead(buttonPin3); 
    Serial.println(buttonState1);
    Serial.println(buttonState2);
    Serial.println(buttonState3);

    // Check if the button is pressed
    if (buttonState1 == LOW) {    // LOW means button pressed in pull-up configuration
      mode = 0;
    } else if (buttonState2 ==LOW) {
      mode = 1;
    }else if (buttonState3 ==LOW){
      mode = 2;
    }
    
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
     
        vReal[i] = analogRead(0); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
        vImag[i] = 0; //Makes imaginary term 0 always

        /*remaining wait time between samples if necessary*/
        while(micros() < (microSeconds + samplingPeriod))
        {
          //do nothing
        }
    }
 
    /*Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    /*Find peak frequency and print peak*/
    double dominantFrequency = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    Serial.println(dominantFrequency);     //Print out the most dominant frequency.

    switch (mode) {
      case 0: // Normal mode
        if (dominantFrequency < 50) {
          setColor(255, 0, 0);       // Red
        } else if (dominantFrequency < 100) {
          Serial.println("i knew it");
          setColor(255, 25, 0);      // Orange-Red
        } else if (dominantFrequency < 150) {
          setColor(255, 170, 0);     // Orange
        } else if (dominantFrequency < 200) {
          setColor(255, 255, 0);     // Yellow
        } else if (dominantFrequency < 250) {
          setColor(170, 255, 0);     // Yellow-Green
        } else if (dominantFrequency < 300) {
          setColor(85, 255, 0);      // Greenish Yellow
        } else if (dominantFrequency < 350) {
          setColor(0, 255, 0);       // Green
        } else if (dominantFrequency < 400) {
          setColor(0, 255, 85);      // Green-Cyan
        } else if (dominantFrequency < 450) {
          setColor(0, 255, 170);     // Cyan
        } else if (dominantFrequency < 500) {
          setColor(0, 255, 255);     // Aqua
        } else if (dominantFrequency < 550) {
          setColor(0, 170, 255);     // Light Blue
        } else if (dominantFrequency < 600) {
          setColor(0, 85, 255);      // Blue
        } else if (dominantFrequency < 650) {
          setColor(0, 0, 255);       // Dark Blue
        } else if (dominantFrequency < 700) {
          setColor(75, 0, 130);      // Indigo
        } else if (dominantFrequency < 750) {
          setColor(139, 0, 255);     // Purple
        } else {
          setColor(255, 0, 255);     // Magenta
        }
        break;
        
      case 1: // Warm mode
        if (dominantFrequency < 50) {
          setColor(255, 40, 0);      // Reddish
        } else if (dominantFrequency < 100) {
          setColor(255, 70, 0);      // Reddish-Orange
        } else if (dominantFrequency < 150) {
          setColor(255, 90, 0);      // Orange-Red
        } else if (dominantFrequency < 200) {
          setColor(255, 120, 0);     // Orange
        } else if (dominantFrequency < 250) {
          setColor(255, 150, 0);     // Orange-Yellow
        } else if (dominantFrequency < 300) {
          setColor(255, 180, 0);     // Yellowish
        } else if (dominantFrequency < 350) {
          setColor(255, 210, 0);     // Yellow
        } else if (dominantFrequency < 400) {
          setColor(255, 240, 0);     // Brighter Yellow
        } else if (dominantFrequency < 450) {
          setColor(230, 230, 20);    // Light Yellow-Green
        } else if (dominantFrequency < 500) {
          setColor(210, 240, 30);    // Greenish Yellow
        } else if (dominantFrequency < 550) {
          setColor(200, 230, 34);    // Light Yellow-Green
        } else if (dominantFrequency < 600) {
          setColor(185, 230, 36);    // Greenish Yellow
        } else if (dominantFrequency < 650) {
          setColor(174, 223, 38);    // Light Yellow
        } else if (dominantFrequency < 700) {
          setColor(160, 215, 40);    // Yellow-Green
        } else if (dominantFrequency < 750) {
          setColor(180, 200, 35);    // Light Green
        } else {
          setColor(174, 223, 38);    // Light Yellow-Green for high frequencies
        }
        break;

      case 2: // Cold mode
        if (dominantFrequency < 50) {
          setColor(150, 225, 245);   // Light Blue
        } else if (dominantFrequency < 100) {
          setColor(130, 210, 240);   // Softer Blue
        } else if (dominantFrequency < 150) {
          setColor(100, 180, 230);   // Lighter Blue
        } else if (dominantFrequency < 200) {
          setColor(75, 140, 220);    // Blue
        } else if (dominantFrequency < 250) {
          setColor(90, 120, 225);    // Light Violet
        } else if (dominantFrequency < 300) {
          setColor(120, 100, 222);   // Violet
        } else if (dominantFrequency < 350) {
          setColor(140, 80, 220);    // Purple
        } else if (dominantFrequency < 400) {
          setColor(160, 60, 218);    // Darker Violet
        } else if (dominantFrequency < 450) {
          setColor(175, 70, 210);    // Purple
        } else if (dominantFrequency < 500) {
          setColor(180, 60, 210);    // Deep Violet
        } else if (dominantFrequency < 550) {
          setColor(185, 50, 205);    // Light Purple
        } else if (dominantFrequency < 600) {
          setColor(190, 45, 200);    // Softer Purple
        } else if (dominantFrequency < 650) {
          setColor(140, 30, 200);    // Indigo
        } else if (dominantFrequency < 700) {
          setColor(150, 20, 190);    // Deep Indigo
        } else if (dominantFrequency < 750) {
          setColor(140, 0, 255);     // Dark Purple
        } else {
          setColor(120, 0, 240);     // Very Dark Purple for high frequencies
        }
        break;
    }
 
  delay(1000);
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

#include "arduinoFFT.h"

// here is a link to the github repository for the arduinoFFT library: https://github.com/kosme/arduinoFFT/tree/master
// i use the examples in there as a reference for performing the FFT operations
// specifically I read FFT_01 in the examples folder to really get a good grasp of how to use the library
 
#define SAMPLES 128             //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.
 
arduinoFFT FFT = arduinoFFT();

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int micPin = A0;
 
unsigned int samplingPeriod;
unsigned long microSeconds;
 
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

const int buttonPin1 = 7; // Normal button
const int buttonPin2 = 4; // Warm button
const int buttonPin3 = 2; // Cold button
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int mode = 0; // 0 = normal, 1 = warm, 2 = cold
 
void setup() 
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(buttonPin1,INPUT_PULLUP);
    pinMode(buttonPin2,INPUT_PULLUP);
    pinMode(buttonPin3,INPUT_PULLUP);
    Serial.begin(9600); //Baud rate for the Serial Monitor
   // Serial.println("chicken");
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
}
 
void loop() 
{  
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);  
    buttonState3 = digitalRead(buttonPin3); 
    Serial.println(buttonState1);
    Serial.println(buttonState2);
    Serial.println(buttonState3);

    // Check if the button is pressed
    if (buttonState1 == LOW) {    // LOW means button pressed in pull-up configuration
      mode = 0;
    } else if (buttonState2 ==LOW) {
      mode = 1;
    }else if (buttonState3 ==LOW){
      mode = 2;
    }
    
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
     
        vReal[i] = analogRead(0); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
        vImag[i] = 0; //Makes imaginary term 0 always

        /*remaining wait time between samples if necessary*/
        while(micros() < (microSeconds + samplingPeriod))
        {
          //do nothing
        }
    }
 
    /*Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    /*Find peak frequency and print peak*/
    double dominantFrequency = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    Serial.println(dominantFrequency);     //Print out the most dominant frequency.

    switch (mode) {
      case 0: // Normal mode
        if (dominantFrequency < 50) {
          setColor(255, 0, 0);       // Red
        } else if (dominantFrequency < 100) {
          Serial.println("i knew it");
          setColor(255, 25, 0);      // Orange-Red
        } else if (dominantFrequency < 150) {
          setColor(255, 170, 0);     // Orange
        } else if (dominantFrequency < 200) {
          setColor(255, 255, 0);     // Yellow
        } else if (dominantFrequency < 250) {
          setColor(170, 255, 0);     // Yellow-Green
        } else if (dominantFrequency < 300) {
          setColor(85, 255, 0);      // Greenish Yellow
        } else if (dominantFrequency < 350) {
          setColor(0, 255, 0);       // Green
        } else if (dominantFrequency < 400) {
          setColor(0, 255, 85);      // Green-Cyan
        } else if (dominantFrequency < 450) {
          setColor(0, 255, 170);     // Cyan
        } else if (dominantFrequency < 500) {
          setColor(0, 255, 255);     // Aqua
        } else if (dominantFrequency < 550) {
          setColor(0, 170, 255);     // Light Blue
        } else if (dominantFrequency < 600) {
          setColor(0, 85, 255);      // Blue
        } else if (dominantFrequency < 650) {
          setColor(0, 0, 255);       // Dark Blue
        } else if (dominantFrequency < 700) {
          setColor(75, 0, 130);      // Indigo
        } else if (dominantFrequency < 750) {
          setColor(139, 0, 255);     // Purple
        } else {
          setColor(255, 0, 255);     // Magenta
        }
        break;
        
      case 1: // Warm mode
        if (dominantFrequency < 50) {
          setColor(255, 40, 0);      // Reddish
        } else if (dominantFrequency < 100) {
          setColor(255, 70, 0);      // Reddish-Orange
        } else if (dominantFrequency < 150) {
          setColor(255, 90, 0);      // Orange-Red
        } else if (dominantFrequency < 200) {
          setColor(255, 120, 0);     // Orange
        } else if (dominantFrequency < 250) {
          setColor(255, 150, 0);     // Orange-Yellow
        } else if (dominantFrequency < 300) {
          setColor(255, 180, 0);     // Yellowish
        } else if (dominantFrequency < 350) {
          setColor(255, 210, 0);     // Yellow
        } else if (dominantFrequency < 400) {
          setColor(255, 240, 0);     // Brighter Yellow
        } else if (dominantFrequency < 450) {
          setColor(230, 230, 20);    // Light Yellow-Green
        } else if (dominantFrequency < 500) {
          setColor(210, 240, 30);    // Greenish Yellow
        } else if (dominantFrequency < 550) {
          setColor(200, 230, 34);    // Light Yellow-Green
        } else if (dominantFrequency < 600) {
          setColor(185, 230, 36);    // Greenish Yellow
        } else if (dominantFrequency < 650) {
          setColor(174, 223, 38);    // Light Yellow
        } else if (dominantFrequency < 700) {
          setColor(160, 215, 40);    // Yellow-Green
        } else if (dominantFrequency < 750) {
          setColor(180, 200, 35);    // Light Green
        } else {
          setColor(174, 223, 38);    // Light Yellow-Green for high frequencies
        }
        break;

      case 2: // Cold mode
        if (dominantFrequency < 50) {
          setColor(150, 225, 245);   // Light Blue
        } else if (dominantFrequency < 100) {
          setColor(130, 210, 240);   // Softer Blue
        } else if (dominantFrequency < 150) {
          setColor(100, 180, 230);   // Lighter Blue
        } else if (dominantFrequency < 200) {
          setColor(75, 140, 220);    // Blue
        } else if (dominantFrequency < 250) {
          setColor(90, 120, 225);    // Light Violet
        } else if (dominantFrequency < 300) {
          setColor(120, 100, 222);   // Violet
        } else if (dominantFrequency < 350) {
          setColor(140, 80, 220);    // Purple
        } else if (dominantFrequency < 400) {
          setColor(160, 60, 218);    // Darker Violet
        } else if (dominantFrequency < 450) {
          setColor(175, 70, 210);    // Purple
        } else if (dominantFrequency < 500) {
          setColor(180, 60, 210);    // Deep Violet
        } else if (dominantFrequency < 550) {
          setColor(185, 50, 205);    // Light Purple
        } else if (dominantFrequency < 600) {
          setColor(190, 45, 200);    // Softer Purple
        } else if (dominantFrequency < 650) {
          setColor(140, 30, 200);    // Indigo
        } else if (dominantFrequency < 700) {
          setColor(150, 20, 190);    // Deep Indigo
        } else if (dominantFrequency < 750) {
          setColor(140, 0, 255);     // Dark Purple
        } else {
          setColor(120, 0, 240);     // Very Dark Purple for high frequencies
        }
        break;
    }
 
  delay(1000);
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

