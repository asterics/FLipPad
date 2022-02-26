# FLipPad

The FLipPad (a.k.a. Flipable Touchpad) is a replacement for a normal PC mouse / keyboard / joystick. 
Instead of moving the mouse/joystick with your hand or pressing keys on a keyboard, the FLipPad can be controlled by very small finger movements.
Unlike a normal Touchpad, the FLipPad can act as mouse, keyboard or joystick - and it can even control bluetooth devices.
For mouse cursor movements, the sensitivity and the operational mode of the FLipPad can be configured, so that finger movements and gestures in the range of millimeters are sufficient to control a computer, smart phone or a complex game.

The clicking functionality is accomplished by tapping, by sip- and puff-activities or via external switches.
All settings and functions of the FLipPad can be tailored to specific capabilities or needs. 
Multiple settings can be stored into the device and changed via finger gestures or other user interactions.
Additional features like built-in environmental control via infrared, optional bluetooth-add-on module for controlling smartphones/tablets or complete software-based control of all functions via serial command interface make the FlipPad one of the most flexible alternative input devices available today.

![FLipPad with 3d-printed enclosure - rendered image](/img/renderings/fp1.png)

The FLipPad was created in course of the [TOM Abraham Accords challenge 2022](https://www.tomchallenge.org/), together with a need-knower who requested and tested this design.
It is based upon the [FlipMouse](https://github.com/asterics/FLipMouse) - a mouth-controller which is also available as Open Source project / DIY-construction kit!



# Software (firmware and configuration manager)

The FLipPad firmware is based on the Arduino/Teensyduino framework. The firmware implements a composite USB HID device (mouse, keyboard, joystick and a serial port in one device).
The mouse and keyboard device classes are used to transmit different keys or mouse actions to the host device. The serial port is used configure the FLipPad (or even use it as a mouse simulator via AT commands).
Multiple configuration settings can be saved (stored in an EEPROM module) and changed via desired user actions.


## Configuration Manager

All settings of the FLipPad can be changed via the [FlipPad Configuration manager](https://flippad.asterics.eu)  
Using the configuration manager in the Chrome web browser, you can assign different actions to all user inputs of the FLipPad.
The Configuration mananger can also be used to update the firmware of the FlipPad and the optional Bluetooth module.

More Information can be found in the user manual: 
[user manual (english version)](https://github.com/asterics/FLipPad/blob/master/Documentation/UserManual/Markdown/FLipPadUserManual.md)


## Building the firmware
In order to build the firmware following prerequisites and dependencies must be installed:
* the [Arduino IDE](https://www.arduino.cc/en/software)
* the [Teensyduino](https://www.pjrc.com/teensy/td_download.html) add-on (must be compatible with the Arduino IDE version)
* the [SSD1306Ascii](https://github.com/greiman/SSD1306Ascii) library by Bill Greiman (can be installed using Arduino IDE's library manager)
* in the header file WireKinetis.h (Teensy Wire-library implemenation), uncomment [this line](https://github.com/PaulStoffregen/Wire/blob/2499ec67c2128629ee33697804f8650180293597/WireKinetis.h#L50), which is needed to implement the Wire1 interface. The file is located in an Arduino IDE installation subfolder, e.g.
`arduino-1.8.13/hardware/teensy/avr/libraries/Wire/WireKinetis.h` 
* select the *board* 'Teensy LC' in the Arduino IDE tools menu, and the *USB-type* "Serial+Keyboard+Mouse+Joystick"
* select the correct *Port* after connecting the TeensyLC to your system
* compile and upload the firmware 

![FLipPad opened - rendered image](/img/renderings/sc2.png)


# Links to related projects

Most of the work for the FLipPad has been accomplished at the UAS Technikum Wien, in course of research projects and together with students, see: 
([AsTeRICS Foundation homepage](https://www.asterics-foundation.org)).

