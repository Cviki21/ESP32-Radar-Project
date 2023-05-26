ESP32 Radar Project
This repository contains the Arduino code for an ESP32-based radar system. The radar utilizes an ultrasonic sensor (HC-SR04), a micro servo motor (SG90), and a 1.77-inch TFT display to detect and visualize objects in real-time.

Components
To replicate this project, you will need the following components:

ESP32 D1 R32
Micro Servo SG90
Ultrasonic Sensor HC-SR04
1.77-inch TFT Display
Pin Configuration
Connect the components to the ESP32 board using the following pin configuration:

Ultrasonic Sensor:
Trig Pin: 13
Echo Pin: 14
Micro Servo Motor: 16
TFT Display: 
Installation
To set up the ESP32 radar project, follow these steps:

Install the required libraries:

TFT_eSPI
ESP32Servo
Download the .ino file

Open the Arduino IDE and upload the code to your ESP32 board.

Make the necessary connections according to the pin configuration.

Power on the system and observe the radar visualization on the TFT display.

Usage
Once the project is set up and running, the radar system will detect objects using the ultrasonic sensor and display them on the TFT display. The servo motor enables the radar to scan the surrounding area, providing a real-time visualization of objects.

Feel free to modify the code to customize the radar's behavior or enhance its features.

Contributing
Contributions to the ESP32 Radar project are welcome! If you have any ideas, improvements, or bug fixes, please submit a pull request.
