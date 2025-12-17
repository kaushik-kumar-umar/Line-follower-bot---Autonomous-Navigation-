// ---------- PIN DEFINITIONS ----------
int sensorPins[5] = {2, 3, 4, 5, 6};

// Motor pins
int ENA = 9;
int IN1 = 7;
int IN2 = 8;

int ENB = 10;
int IN3 = 11;
int IN4 = 12;

// ---------- VARIABLES ----------
int baseSpeed = 120;   // Adjust for your N20 motors
int turnSpeed = 80;

int weights[5] = {-4, -2, 0, 2, 4};

// ---------- SETUP ----------
void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

// ---------- MAIN LOOP ----------
void loop() {
  int sensorValue[5];
  int error = 0;
  int activeSensors = 0;

  // Read sensors
  for (int i = 0; i < 5; i++) {
    sensorValue[i] = digitalRead(sensorPins[i]);
    if (sensorValue[i] == 1) {
      error += weights[i];
      activeSensors++;
    }
  }

  // ---------- JUNCTION HANDLING ----------
  if (activeSensors >= 4) {
    // Junction detected (T or +)
    moveForward(baseSpeed);
    delay(80);
    return;
  }

  // ---------- LINE LOST ----------
  if (activeSensors == 0) {
    stopMotors();
    return;
  }

  // ---------- SENSOR WEIGHT CONTROL ----------
  int leftSpeed = baseSpeed - (error * 15);
  int rightSpeed = baseSpeed + (error * 15);

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  setMotorSpeed(leftSpeed, rightSpeed);
}

// ---------- MOTOR FUNCTIONS ----------
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, leftSpeed);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, rightSpeed);
}

void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
