#include <Servo.h>
#include "max6675.h"

/* pin froim 23 - 53 */
// Stepper motors
#define X_STEP 2
#define X_DIR 5

#define Y_STEP 3
#define Y_DIR 6

#define Z_STEP 4
#define Z_DIR 7

#define A_STEP 23
#define A_DIR 25

#define B_STEP 27
#define B_DIR 29

#define bucket 31  // Make sure this pin is not shared with vibrator1
#define oil_pump 33

#define buzzer 35
#define vibrator1 37  // Change this to a different pin
#define vibrator2 39  // Change this to a different pin

// defining buttons
#define C_BTN 24
#define E_BTN 26

#define H_BTN 28
#define F_BTN 30

// defining indicator light
#define C_LED 32
#define E_LED 34

#define H_LED 36
#define F_LED 38

int thermoDO = 41;
int thermoCS = 45;
int thermoCLK = 46;

const int STEPPER_DELAY = 1000;


MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


// change step delay to changing the ingredients bucket's time
const int MOTOR_X_FULL_STEPS = 9000;
const int MOTOR_Y_FULL_STEPS = 9000;
const int MOTOR_Z_FULL_STEPS = 9000;
const int MOTOR_A_FULL_STEPS = 9000;
const int MOTOR_B_FULL_STEPS = 9000;

const int MOTOR_X_HALF_STEPS = 9000;
const int MOTOR_Y_HALF_STEPS = 9000;
const int MOTOR_Z_HALF_STEPS = 9000;
const int MOTOR_A_HALF_STEPS = 9000;
const int MOTOR_B_HALF_STEPS = 9000;

const int STEP_DELAY = 1000;
// bucket heating time
const int BUCKET_HEAT_TIME_FULL = 30000;
const int BUCKET_HEAT_TIME_HALF = 30000;

// fire_servo angles
const int GAS_OPEN_ANGLE = 20;
const int GAS_INC = 40;
const int OIL_PUMP_TIME_FULL = 5000;
const int OIL_PUMP_TIME_HALF = 5000;

// const int BUCKET_HEAT_TIME_HALF = 4000;
// const int BUCKET_HEAT_TIME_FULL = 4000;

const int COOKING_TIME = 4000;

// initialize servos
Servo fire_servo;



void setup() {
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(Z_STEP, OUTPUT);
  pinMode(A_STEP, OUTPUT);
  pinMode(B_STEP, OUTPUT);

  pinMode(X_DIR, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Z_DIR, OUTPUT);
  pinMode(A_DIR, OUTPUT);
  pinMode(B_DIR, OUTPUT);

  pinMode(C_BTN, INPUT_PULLUP);
  pinMode(E_BTN, INPUT_PULLUP);
  pinMode(H_BTN, INPUT_PULLUP);
  pinMode(F_BTN, INPUT_PULLUP);

  pinMode(C_LED, OUTPUT);
  pinMode(E_LED, OUTPUT);
  pinMode(H_LED, OUTPUT);
  pinMode(F_LED, OUTPUT);

  Serial.begin(9600);

  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);

  // Servo motor attach for servo
  fire_servo.attach(44);
  fire_servo.write(0);
}

void loop() {

  bool C_BTN_STATE = digitalRead(C_BTN);
  bool E_BTN_STATE = digitalRead(H_BTN);
  bool H_BTN_STATE = digitalRead(H_BTN);
  bool F_BTN_STATE = digitalRead(F_BTN);

  // calculate temperature of the bucket
  Serial.print("C = "); 
  Serial.println(thermocouple.readCelsius());
  delay(1000);

  float temp = thermocouple.readCelsius();

  if (C_BTN_STATE == HIGH) {
    // turn on GAS MOTOR
    fire_servo.write(GAS_OPEN_ANGLE);
    BuzzerBeep(1);

    if (F_BTN_STATE == HIGH) {
      Serial.println("Chicken full Rice");
      BuzzerBeep(1);

      if (temp > 40) {
        // turn on oil pump
        relayOn(oil_pump);
        delay(OIL_PUMP_TIME_FULL);
        relayOff(oil_pump);
      }

      // check temperature
      stepperDrive(X_STEP, X_DIR, MOTOR_X_FULL_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Y_STEP, Y_DIR, MOTOR_Y_FULL_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Z_STEP, Z_DIR, MOTOR_Z_FULL_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(A_STEP, A_DIR, MOTOR_A_FULL_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(B_STEP, B_DIR, MOTOR_B_FULL_STEPS);
      delay(STEPPER_DELAY);

      fire_servo.write(GAS_INC);

      delay(COOKING_TIME);

      // TURN OFF LED
      Serial.println("Done ...!");
      // turn off gas
      fire_servo.write(0);

      C_BTN_STATE = LOW;
      F_BTN_STATE = LOW;

      digitalWrite(C_LED, LOW);
      digitalWrite(C_LED, HIGH);

      Serial.println("Stopped .... !");
    }

    if (H_BTN_STATE == HIGH) {
      Serial.println("Chicken half Rice");
      BuzzerBeep(1);

      if (temp > 40) {
        // turn on oil pump
        relayOn(oil_pump);
        delay(OIL_PUMP_TIME_HALF);
        relayOff(oil_pump);
      }

      // bucket on 
      relayOn(bucket);

      // check temperature
      stepperDrive(X_STEP, X_DIR, MOTOR_X_HALF_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Y_STEP, Y_DIR, MOTOR_Y_HALF_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Z_STEP, Z_DIR, MOTOR_Z_HALF_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(A_STEP, A_DIR, MOTOR_A_HALF_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(B_STEP, B_DIR, MOTOR_B_HALF_STEPS);
      delay(STEPPER_DELAY);

      fire_servo.write(GAS_INC);
      
      delay(COOKING_TIME);

      // TURN OFF LED
      Serial.println("Done ...!");
      // turn off gas
      fire_servo.write(0);

      C_BTN_STATE = LOW;
      F_BTN_STATE = LOW;

      digitalWrite(C_LED, LOW);
      digitalWrite(C_LED, HIGH);
      relayOff(bucket);

      Serial.println("Stopped .... !");
    }
  }

  if (E_BTN_STATE == HIGH) {
    // turn on GAS MOTOR
    fire_servo.write(GAS_OPEN_ANGLE);
    BuzzerBeep(1);


    if (H_BTN_STATE == HIGH) {
      Serial.println("Egg Half rice");
      BuzzerBeep(1);

      if (temp > 40) {
        // turn on oil pump
        relayOn(oil_pump);
        delay(OIL_PUMP_TIME_FULL);
        relayOff(oil_pump);
      }

      // check temperature
      stepperDrive(X_STEP, X_DIR, MOTOR_X_FULL_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Y_STEP, Y_DIR, MOTOR_Y_FULL_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Z_STEP, Z_DIR, MOTOR_Z_FULL_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(A_STEP, A_DIR, MOTOR_A_FULL_STEPS);
      delay(STEPPER_DELAY);
      

      fire_servo.write(GAS_INC);
      
      delay(COOKING_TIME);

      // TURN OFF LED
      Serial.println("Done ...!");
      // turn off gas
      fire_servo.write(0);

      C_BTN_STATE = LOW;
      F_BTN_STATE = LOW;

      digitalWrite(C_LED, LOW);
      digitalWrite(C_LED, HIGH);

      Serial.println("Stopped .... !");
    }

    if (F_BTN_STATE == HIGH) {
      Serial.println("Egg Full rice");
      BuzzerBeep(1);

      if (temp > 40) {
        // turn on oil pump
        relayOn(oil_pump);
        delay(OIL_PUMP_TIME_HALF);
        relayOff(oil_pump);
      }

      // BUCKET ON 
      relayOn(bucket);

      // check temperature
      stepperDrive(X_STEP, X_DIR, MOTOR_X_HALF_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Y_STEP, Y_DIR, MOTOR_Y_HALF_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(Z_STEP, Z_DIR, MOTOR_Z_HALF_STEPS);
      delay(STEPPER_DELAY);
      stepperDrive(A_STEP, A_DIR, MOTOR_A_HALF_STEPS);
      delay(STEPPER_DELAY);

      fire_servo.write(GAS_INC);
      
      delay(COOKING_TIME);

      // TURN OFF LED
      Serial.println("Done ...!");
      // turn off gas
      fire_servo.write(0);

      C_BTN_STATE = LOW;
      F_BTN_STATE = LOW;

      digitalWrite(C_LED, LOW);
      digitalWrite(C_LED, HIGH);
      relayOff(bucket);

      Serial.println("Stopped .... !");
    }
  }
}