#include <Arduino.h>
#include <BleGamepad.h>

//A,B,X,Y or S,X,C,T
#define X_BUTTON 15         // A
#define CIRCLE_BUTTON 17    // B
#define TRIANGLE_BUTTON 4  // Y
#define SQUARE_BUTTON 5    // X

//TRIGGERS R,L
#define R1_BUTTON 18
#define R2_BUTTON 19
#define L1_BUTTON 21
#define L2_BUTTON 22

//MENU 
#define START_BUTTON 23
#define SELECT_BUTTON 26
#define PS_BUTTON 25

//JOYSTICK BUTTONS
#define R3_BUTTON 33
#define L3_BUTTON 32

//JOYSTICKS AXIS
#define RIGHT_VRX_JOYSTICK 12
#define RIGHT_VRY_JOYSTICK 13
#define LEFT_VRX_JOYSTICK 14
#define LEFT_VRY_JOYSTICK 27      


#define NUM_OF_BUTTONS 13

int buttonsPins[NUM_OF_BUTTONS] = {X_BUTTON, CIRCLE_BUTTON, TRIANGLE_BUTTON, SQUARE_BUTTON,
                          R1_BUTTON, R2_BUTTON, L1_BUTTON, L2_BUTTON,
                          START_BUTTON, SELECT_BUTTON, PS_BUTTON,
                          R3_BUTTON, L3_BUTTON};

int androidGamepadButtons[NUM_OF_BUTTONS] = {1, 2, 3, 4, 8, 10, 7, 9, 12, 11, 13, 15, 14};
int PCGamepadButtons[NUM_OF_BUTTONS] = {1, 2, 4, 3, 6, 8, 5, 7, 10, 9, 0, 12, 11};


uint16_t leftVrxJoystickLecture, leftVryJoystickLecture, rightVrxJoystickLecture, rightVryJoystickLecture = 0;

uint16_t leftVrxJoystickValue, leftVryJoystickValue, rightVrxJoystickValue, rightVryJoystickValue = 0;


typedef enum{ANDROID, PC} GamepadModes;
GamepadModes gamepadMode = ANDROID;  // Change this to ANDROID or PC


BleGamepad bleGamepad("BLE Gamepad", "ESP");
BleGamepadConfiguration bleGamepadConfig;  


void setup() {
  delay(500);
  Serial.begin(115200);

  for(int i=0; i<NUM_OF_BUTTONS; i++){
    pinMode(buttonsPins[i], INPUT_PULLUP);
  }

  bleGamepadConfig.setAutoReport(false);
    // CONTROLLER_TYPE_JOYSTICK (for PC), CONTROLLER_TYPE_GAMEPAD (for Mobile/Android)
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setVid(0xe502);
  bleGamepadConfig.setPid(0xabcd);
  bleGamepadConfig.setHatSwitchCount(0);
  bleGamepad.begin(&bleGamepadConfig);
}

void loop() {
  if(bleGamepad.isConnected()){
    leftVrxJoystickLecture = analogRead(LEFT_VRX_JOYSTICK);
    leftVryJoystickLecture = analogRead(LEFT_VRY_JOYSTICK);
    rightVrxJoystickLecture = analogRead(RIGHT_VRX_JOYSTICK);
    rightVryJoystickLecture = analogRead(RIGHT_VRY_JOYSTICK);

    Serial.println(rightVrxJoystickLecture);
    leftVrxJoystickValue = map(leftVrxJoystickLecture, 4095, 0, 0, 32737);
    leftVryJoystickValue = map(leftVryJoystickLecture, 0, 4095, 0, 32737);
    rightVrxJoystickValue = map(rightVrxJoystickLecture, 4095, 0, 0, 32737);
    rightVryJoystickValue = map(rightVryJoystickLecture, 0, 4095, 0, 32737);

    
    switch(gamepadMode){
      case ANDROID:
        for(int i=0; i<NUM_OF_BUTTONS; i++){
          if(!digitalRead(buttonsPins[i])){
              bleGamepad.press(androidGamepadButtons[i]);   
          }
          else{
              bleGamepad.release(androidGamepadButtons[i]);     
          }
          bleGamepad.setLeftThumb(leftVrxJoystickValue, leftVryJoystickValue);
          bleGamepad.setRightThumb(rightVrxJoystickValue, rightVryJoystickValue);
        } 
        break; 

      case PC:
          for(int i=0; i<NUM_OF_BUTTONS; i++){
            if(!digitalRead(buttonsPins[i])){
              bleGamepad.press(PCGamepadButtons[i]);
            }
            else{
              bleGamepad.release(PCGamepadButtons[i]);
            }
            bleGamepad.setX(leftVrxJoystickValue);
            bleGamepad.setY(leftVryJoystickValue);
            bleGamepad.setZ(rightVrxJoystickValue);
            bleGamepad.setRX(rightVryJoystickValue);
          }
          break;
    }
    bleGamepad.sendReport();
  }
}

// By @Mobarak46 - Telegram
