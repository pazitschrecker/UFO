# Unidentified Fallen Art

# Description
This project is meant to provide users with a fun and whimsical learning experience that is difficult to experience due to the covid-19 pandemic. With many museums closed or open with new sanitary regulations, finding interactive exhibits and activities can be more difficult. "Unidentified Fallen Art" is an interactive project through which a user explores an object believed to be an extraterrestrial spaceship. Their job is to determine where the extraterrestrials (ETs) that dropped the object came from through light and textual clues (Hint: it's in our solar system).

Watch a video demo: https://www.youtube.com/watch?v=5KBab6er2QM

# Dependencies
This project requires the Arduino IDE to run. You can download it here: https://www.arduino.cc/en/software
Make sure to select the correct download based on your machine (I used a MacBook Pro).
In order to run the project, you will also need to install the follow arduino IDE libraries:

`Freenove_WS2812_Lib_for_ESP32.h`

`LiquidCrystal_I2C.h`

Both libraries can be installed by going to Arduino IDE --> "Tools" --> "Manage Libraries" --> Search for the name of the library and install it. 

This project also requires Pygame. I installed this with: `pip3 install pygame`

# Physical Setup

This project uses a few physical components, all connected to ESP32 via a breadboard. First, set up the ESP32 by plugging it into the GPIO Extender and then into a breadboard.

Plug the infrared sensor into the breadboard via 3 M-F wires. Connect the power port to 5V on the breadboard, the ground to GND, and the last wire to GPIO port 14.
Plug the LED ring power and ground pins to 3.3V and GND on the breadboard using 2 M-F wires. Connect a third wire to 'S' on the LED ring to GPIO port 2 on the breadboard. 
Lastly, connect the I2C Liquid Crystal Display power pin to 5V on the breadboard via a M-F wire. Connect the ground pin to GND on the breadboard via another M-F wire. Then, connect the SDA pin to GPIO port 21 and the SCL pin to GPIO port 22, using M-F wires for both.


The Arduino sketch was uploaded from my laptop to the ESP32. Once uploaded, I unplugged the ESP32 from my laptop and powered it with a 9V battery.

The ESP32 is connected to wifi via station mode. To run the project, I connected both the ESP32 and my laptop to a mobile hotspot set up on an iPhone. Change the "GUEST" wifi and corresponding "guest_pw" to the wifi network and password that you will connect to. Both the ESP32 and computer must be connected to the same network to send messages.

# A note on music
This project plays music and therefore requires downloaded .wav files. I have not included these in the repository because they are copyrighted materials. In order for your songs to work with the code, do the following: Create a folder in the same location or directory as the python script and call this "songs". Within the songs directory, add the corresponding songs as 3 .wav files with the following names:

- "imperial_march.wav" aka The Imperial March/Darth Vader's Theme (Star Wars: The Empire Strikes Back)
- "starwars.wav" aka Star Wars Main Theme (Star Wars: A New Hope)
- "et.wav" aka Flying Theme (E.T. the Extra-Terrestrial)


