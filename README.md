# FLipPad

The FLipPad (a.k.a. Flipable Touchpad) is a replacement for a normal PC mouse / keyboard / joystick. 
Instead of moving the mouse/joystick with your hand or pressing keys on a keyboard, the FLipPad can be controlled by very small finger movements.
Unlike a normal Touchpad, the FLipPad can act as mouse, keyboard or joystick - and it can even control bluetooth devices.
For mouse cursor movements, the sensitivity and the operational mode of the FLipPad can be configured, so that finger movements and gestures in the range of millimeters are sufficient to control a computer, smart phone or a complex game.

The clicking functionality is accomplished by tapping, by sip- and puff-activities or via external switches.
All settings and functions of the FLipPad can be tailored to specific capabilities or needs. 
Multiple settings can be stored into the device and changed via finger gestures or other user interactions.
Additional features like built-in environmental control via infrared, optional bluetooth-add-on module for controlling smartphones/tablets or complete software-based control of all functions via serial command interface make the FlipPad one of the most flexible alternative input devices available today.

![FLipPad opened - rendered image](/img/renderings/sc2.png)

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
* [user manual (english version)](https://github.com/asterics/FLipPad/blob/main/Documentation/UserManual/FLipPadUserManual.md)
* [user manual (german version)](https://github.com/asterics/FLipPad/blob/main/Documentation/UserManual/FLipPadAnwendungsanleitung.md)


## Building the firmware
In order to build the firmware following prerequisites and dependencies must be installed:
* the [Arduino IDE](https://www.arduino.cc/en/software) - use either IDE1 version 1.8.x or IDE2 verison 2.0.x
* the [Teensyduino](https://www.pjrc.com/teensy/td_download.html) add-on (must be compatible with the Arduino IDE version)
* the [SSD1306Ascii](https://github.com/greiman/SSD1306Ascii) library by Bill Greiman (can be installed using Arduino IDE's library manager)
* **important:** in the header file WireKinetis.h (Teensy Wire-library implemenation), *uncomment* the line `//#define WIRE_IMPLEMENT_WIRE1` - should be line number 50, see [here](https://github.com/PaulStoffregen/Wire/blob/2499ec67c2128629ee33697804f8650180293597/WireKinetis.h#L50). 
(This is needed to implement the Wire1 interface, which is disabled by default for the TeensyLC microcontroller in order to save memory.) 
  * If you are using a 1.8.x IDE version under Windows, the file is located inside a subfolder of the Arduino IDE installation, e.g.
`C:/Program Files (x86)/Arduino-1.8.19/hardware/teensy/avr/libraries/Wire/WireKinetis.h` 
  * If you are using a 2.0.x IDE version under Windows, the file is located inside a subfolder of Application Data, e.g.
`C:/Users/<YourName>/AppData/Local/Arduino15/packages/teensy/hardware/avr/1.58.0/libraries/Wire/WireKinetis.h`
* select the *board* 'Teensy LC' in the Arduino IDE tools menu
* select the *USB-type* 'Serial+Keyboard+Mouse+Joystick'  in the Arduino IDE tools menu
* select the correct *Port* after connecting the TeensyLC to your system
* compile and upload the firmware 

![FLipPad with 3d-printed enclosure - rendered image](/img/renderings/fp1.png)



# Links and Credits

Most of the work for the FLipPad has been accomplished at the UAS Technikum Wien in course of the R&D-project *WBT* (MA23 project 26-02), which has been supported by the [City of Vienna](https://www.wien.gv.at/kontakte/ma23/index.html),
see: [Webpage WBT project](https://wbt.wien).


Have a look at the [AsTeRICS Foundation homepage](https://www.asterics-foundation.org) and our other Open Source AT projects:

* AsTeRICS: [AsTeRICS framework homepage](http://www.asterics.eu), [AsTeRICS framework GitHub](https://github.com/asterics/AsTeRICS): The AsTeRICS framework provides a much higher flexibility for building assistive solutions. 
The FLipMouse is also AsTeRICS compatible, so it is possible to use the raw input data for a different assistive solution.

* FLipMouse: [The FLipMouse controller](https://github.com/asterics/FLipMouse): an open source alternative input device for controlling computers and mobile devices with minimal muscle movement.

* FABI: [FABI: Flexible Assistive Button Interface GitHub](https://github.com/asterics/FABI): The Flexible Assistive Button Interface (FABI) provides basically the same control methods (mouse, clicking, keyboard,...), but the input
is limited to simple buttons.

* AsTeRICS Grid: [Asterics Grid AAC Web-App](https://grid.asterics.eu): an open source, cross plattform communicator / talker for Augmented and Alternative Communication (AAC).



# Support us
Please support the development of FLipPad by donating to the AsTeRICS Foundation:

<div>
<a title="Donate with PayPal" href="https://www.paypal.com/donate/?hosted_button_id=38AJJNS427MJ2" target="_blank" style="margin-right:3em">
<img src="https://github.com/asterics/AsTeRICS-Grid/raw/master/app/img/donate-paypal.png" width=300/></a>
<span>&nbsp;&nbsp;&nbsp;</span>
<a title="Donate at opencollective.com" href="https://opencollective.com/asterics-foundation" target="_blank">
<img src="https://github.com/asterics/AsTeRICS-Grid/raw/master/app/img/donate-open-collective.png" width=300/></a>
</div>

