## Basic information
This configuration offers 5 modes (slots) for different mouse-pointer, keyboard and infrared actions.
The modes can be changed by triple taps or by pressing Button1/Button3.

## Slot1 - "StickMode"
- controls mouse cursor via USB interface
- uses joystick mode (return finger to first touch point to stop moving)

## Slot2 - "PadMode"
- controls mouse cursor via USB interface
- uses pad mode (finger movement = cursor movement)

## Slot3 - "Phone"
- controls mouse cursor via BT interface 
- uses pad mode (finger movement = cursor movement)
- another tap+down gesture or pressing external button3 creates the keyboard shortcut KEY_GUI + KEY_BACKSPACE which acctivates the Home/Back button in many Android phones
- note that the BT-module must be installed and paired with a BT-capable device

## Slot4 - "GameKey"
- finger movement creates cursor keys (up / down / left / right) via USB interface
- tap creates space key
- uses sticky pad mode (last finger movement determines key, keys remain sticky)

## Slot5 - "TVRemote"
- finger movement up/down: play ir commands "ChnUp" / "ChnDwn" for changing the TV channel 
- finger movement left/right: play ir commands "VolUp" / "VolDwn" for changing the TV volume 


## for all slots:
- Button1 (internal button), Button3 or TripleTap: load next slot
- Button2 or Sip: hold left mouse button (in Slot4: Space Key)
- Puff: click right mouse button (in Slot4: Enter Key)


## Please note:
- The position correction which is applied after taps and finger gestures only works correctly if the feature "enhance mouse pointer precision" is deactivated, please check the mouse pointer settings of your operating system. Also the overall mouse pointer speed may be adjusted there.
- The IR commands "ChnUp", "ChnDwn", "VolUp" and "VolDwn" must be recorded for your device via the WebGui before you can use the "TVremote" actions