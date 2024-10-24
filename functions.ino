
// stepper motor driving function
void stepperDrive(int stepPin, int dirPin, int steps) {
  digitalWrite(dirPin, HIGH);
  digitalWrite(vibrator1, HIGH);
  digitalWrite(vibrator2, HIGH);

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }

  digitalWrite(vibrator1, LOW);
  digitalWrite(vibrator2, LOW);
}

// buzzer beepings
void BuzzerBeep(int buzzerTimes) {
  switch (buzzerTimes) {
    case 1:
      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
      }

    case 2:
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);

    default :
      return;
  }
}

// relay power up 
void relayOn (int relayPin) {
  digitalWrite(relayPin,HIGH);
}

void relayOff(int relayPin){
  digitalWrite(relayPin,LOW);
}







