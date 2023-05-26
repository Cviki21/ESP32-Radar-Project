#include <Wire.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ESP32Servo.h>

#define TRIG_PIN 13
#define ECHO_PIN 14
#define MAX_DISTANCE 100
#define OBJECT_THRESHOLD 50 // Adjust this value based on what you consider a new object
#define SERVO_PIN 16
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

Servo servo;
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3); // Rotate the screen 180 degrees
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo.attach(SERVO_PIN);
  servo.write(0);
}

void loop() {
  // Initialize object count and distance variables
  int objectCount = 0;
  long minDist = MAX_DISTANCE;
  long maxDist = 0;
  long totalDist = 0;
  long readings = 0;
  
  // Clear the screen
  tft.fillScreen(TFT_BLACK);

   // Draw the reference lines
  for (int r = 0; r <= SCREEN_WIDTH / 2; r += SCREEN_WIDTH / 2 / 4) {
    for (int i = 0; i <= 180; i++) { // Adjust the display to show only half of the circle.
      int x = SCREEN_WIDTH / 2 + r * cos(i * PI / 180.0);
      int y = SCREEN_HEIGHT - r * sin(i * PI / 180.0); // Changed the y calculation to invert the display
      tft.drawPixel(x, y, TFT_RED);
    }
  }

  // Calculate the end points of the 90 degree line
  int x90 = SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2 * cos(90 * PI / 180.0);
  int y90 = SCREEN_HEIGHT - SCREEN_WIDTH / 2 * sin(90 * PI / 180.0); // Changed the y calculation to invert the display

  // Draw 90 degree line
  tft.drawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT, x90, y90, TFT_WHITE); // Changed the starting point of the line to invert the display

  // Calculate the end points of the 45 degree lines
  int x45 = SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2 * cos(45 * PI / 180.0);
  int y45 = SCREEN_HEIGHT - SCREEN_WIDTH / 2 * sin(45 * PI / 180.0); // Changed the y calculation to invert the display

  // Draw 45 degree lines
  tft.drawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT, x45, y45, TFT_WHITE); // Changed the starting point of the line to invert the display
  tft.drawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH - x45, y45, TFT_WHITE); // Changed the starting point of the line to invert the display


  for (int cycle = 0; cycle < 2; cycle++) {
    long previousDist = MAX_DISTANCE;

    for (int j = 0; j < 2; j++) {
      int start = (j == 0) ? 0 : 180;
      int end = (j == 0) ? 180 : 0;
      int step = (j == 0) ? 1 : -1;

      for (int i = start; i != end; i += step) {
        servo.write(i);
        delay(40);

        long duration;
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        duration = pulseIn(ECHO_PIN, HIGH);
        long dist = (duration / 2) / 29.1;

        if (dist > MAX_DISTANCE) {
          dist = MAX_DISTANCE;
        }

        // Check if the distance indicates a new object
        if (abs(dist - previousDist) > OBJECT_THRESHOLD) {
          objectCount++;
        }
        previousDist = dist;

        // Update min, max, total, and readings for distance statistics
        minDist = min(minDist, dist);
        maxDist = max(maxDist, dist);
        totalDist += dist;
        readings++;

        // Convert polar coordinates (angle, distance) to Cartesian coordinates (x, y)
        int x = SCREEN_WIDTH / 2 + (dist * (SCREEN_WIDTH / 2) / MAX_DISTANCE) * cos(i * PI / 180.0);
        int y = SCREEN_HEIGHT - (dist * (SCREEN_WIDTH / 2) / MAX_DISTANCE) * sin(i * PI / 180.0);

        // Draw a line from the center of the screen to the calculated point
        tft.drawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT, x, y, TFT_GREEN);
      }
    }
    
    // Display the object count and distance data after every second full sweep
    if (cycle == 1) {
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(0, 0);
      tft.setTextSize(1);
      tft.println("Objects: " + String(objectCount));
      tft.println("Min Dist: " + String(minDist) + " cm");
      tft.println("Max Dist: " + String(maxDist) + " cm");
      tft.println("Avg Dist: " + String(totalDist / readings) + " cm");
    }

    delay(200);
  }

  // 5 seconds pause before refreshing the screen and repeating everything again
  delay(5000);
}
