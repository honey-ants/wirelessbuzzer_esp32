# ESP32 Wireless Quiz Buzzer
![Buzzer Display](/images/buzzer%20display.png)


## Description
This repository acts as an archive for a personal project. We were hosting a trivia night event and thought having physical buzzer buttons would be really fun!

Each buzzer is designed to have an LED button and a speaker. All of them are controlled using a locally hosted captive portal webpage that can be accessed through phones or computers.


## Hardware
Each buzzer unit had the following components inside the enclosure:
- ESP32-S3-WROOM-1-N16R8 (https://github.com/vcc-gnd/YD-ESP32-S3)
- 12V LED game button
- MAX98357 amplifier + 3W speaker
- Battery

Additional wiring information and port assignments can be found inside of the code files. I may add a wiring diagram in the future for reference.


## Installation
This project was developed and designed to work with the PlatformIO plugin in VSCode. Please refer to the platformio.ini file to check the necessary configurations.

The main code is stored inside of the /codebase folder, please copy your code into /src/main.cpp before uploading to your ESP32.

NOTE: The names of the files and ESP32 MAC addresses inside each file is hard-coded for the board I am working with. Don't forget to customize them accordingly.


## Operation
The buzzer design is inspired by the ones used in the Jeopardy game show. Basically, the buzzer that is pressed first will emit a sound and disable other buzzers. The host will then "Reset" the buttons, disabling all of them and once the host is ready again, "Unlock"-ing the buttons will allow the buzzers to be buzzed again.

The buzzers broadcasts messages to one another using the ESP-NOW protocol. When the buzzers are in its "Unlocked" mode, their LED buttons will light up and the first button that is pressed will emit a sound and turn off the LEDs for all other buttons, so that the first pressed button will be the only one emitting light. When the buzzers are in its "Reset" mode, their LEDs will be turned off and any presses will not have any affect.

The buzzers are designed to have one parent buzzer (yellow, in this case) which hosts the captive portal webpage and broadcasts the WiFi network "BuzzerReset". Accessing the captive portal or 192.168.4.1 will display the following page to be used by the host:

![Webpage](/images/webpage.png)

The "Unlock" and "Reset" buttons will operate as described above. When the first button has been pressed, the color and ID name of the buzzer will be displayed at the top of the page for easy reference for the hose.


## Contact
If run into any problems while using this code, please feel free to contact me at anthony.sbudisuharto@gmail.com and I would be happy to help you out.
