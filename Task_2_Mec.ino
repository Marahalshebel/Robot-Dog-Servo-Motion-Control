/*
  Robot Dog - 3 Motions
  Pins:
  FL = GPIO17
  FR = GPIO18
  RL = GPIO4
  RR = GPIO23

  Neutral:
  FL = 180
  FR = 36
  RL = 78
  RR = 176

  Commands:
  1 = Wave
  2 = Raise Hand
  3 = Sit & Stand
  4 = Stand
*/

const int PIN_FL = 17;
const int PIN_FR = 18;
const int PIN_RL = 4;
const int PIN_RR = 23;

const int SERVO_HZ = 50;
const int SERVO_BITS = 16;

const int NEUTRAL_FL = 180;
const int NEUTRAL_FR = 36;
const int NEUTRAL_RL = 78;
const int NEUTRAL_RR = 176;

// وضع الجلوس
const int SIT_FRONT = 78;
const int SIT_REAR  = 148;

const int MOVE_DELAY = 15;

int posFL = NEUTRAL_FL;
int posFR = NEUTRAL_FR;
int posRL = NEUTRAL_RL;
int posRR = NEUTRAL_RR;

bool okFL, okFR, okRL, okRR;


// =========================
// تحويل الزاوية إلى PWM
// =========================

uint32_t angleToDuty(int angle) {

  angle = constrain(angle, 0, 180);

  uint32_t pulseUs =
    map(angle, 0, 180, 500, 2500);

  return (pulseUs * ((1u << SERVO_BITS) - 1)) / 20000;
}


// =========================
// تحريك سيرفو
// =========================

void writeServo(int pin, int angle) {

  ledcWrite(pin, angleToDuty(angle));
}


// =========================
// Attach
// =========================

bool attachServo(int pin, const char* name) {

  bool ok = ledcAttach(pin, SERVO_HZ, SERVO_BITS);

  Serial.print("Attach ");
  Serial.print(name);
  Serial.print(" GPIO");
  Serial.print(pin);
  Serial.print(" -> ");

  if (ok)
    Serial.println("OK");
  else
    Serial.println("FAIL");

  delay(100);

  return ok;
}


// =========================
// حركة تدريجية
// =========================

void moveServo(
  int pin,
  int &current,
  int target
) {

  target = constrain(target, 0, 180);

  if (current < target) {

    for (int a = current; a <= target; a++) {

      writeServo(pin, a);
      delay(MOVE_DELAY);
    }

  } else {

    for (int a = current; a >= target; a--) {

      writeServo(pin, a);
      delay(MOVE_DELAY);
    }
  }

  current = target;
}


// =========================
// Stand
// =========================

void stand() {

  Serial.println("\n>>> STAND");

  moveServo(PIN_FL, posFL, NEUTRAL_FL);
  moveServo(PIN_FR, posFR, NEUTRAL_FR);
  moveServo(PIN_RL, posRL, NEUTRAL_RL);
  moveServo(PIN_RR, posRR, NEUTRAL_RR);

  Serial.println("Stand complete.");
}


// =========================
// حركة 1 - Wave
// =========================

void wave() {

  Serial.println("\n>>> WAVE");

  // رفع الرجل الأمامية اليسار
  moveServo(PIN_FL, posFL, 150);

  delay(300);

  // تلويح عدة مرات
  moveServo(PIN_FL, posFL, 165);
  delay(250);

  moveServo(PIN_FL, posFL, 145);
  delay(250);

  moveServo(PIN_FL, posFL, 165);
  delay(250);

  moveServo(PIN_FL, posFL, 145);
  delay(250);

  // رجوع للوضع الطبيعي
  moveServo(PIN_FL, posFL, NEUTRAL_FL);

  Serial.println("Wave complete.");
}


// =========================
// حركة 2 - Raise Hand
// =========================

void raiseHand() {

  Serial.println("\n>>> RAISE HAND");

  // رفع الرجل الأمامية اليسار
  moveServo(PIN_FL, posFL, 145);

  delay(1500);

  // رجوع للوضع الطبيعي
  moveServo(PIN_FL, posFL, NEUTRAL_FL);

  Serial.println("Raise Hand complete.");
}


// =========================
// حركة 3 - Sit & Stand
// =========================

void thirdMotion() {

  Serial.println("\n>>> BODY SHAKE");

  // حركة أولى
  moveServo(PIN_FL, posFL, 170);
  moveServo(PIN_FR, posFR, 46);
  delay(300);

  // حركة عكسية
  moveServo(PIN_FL, posFL, 160);
  moveServo(PIN_FR, posFR, 56);
  delay(300);

  // مرة ثانية
  moveServo(PIN_FL, posFL, 170);
  moveServo(PIN_FR, posFR, 46);
  delay(300);

  // الرجوع للوضع الطبيعي
  moveServo(PIN_FL, posFL, NEUTRAL_FL);
  moveServo(PIN_FR, posFR, NEUTRAL_FR);

  Serial.println("Body Shake complete.");
}


// =========================
// Setup
// =========================

void setup() {

  Serial.begin(115200);

  delay(1500);

  Serial.println("\n==========================");
  Serial.println("     ROBOT DOG MOTIONS");
  Serial.println("==========================");

  okFL = attachServo(PIN_FL, "FL");
  okFR = attachServo(PIN_FR, "FR");
  okRL = attachServo(PIN_RL, "RL");
  okRR = attachServo(PIN_RR, "RR");

  // إرسال الوضع الأساسي
  writeServo(PIN_FL, NEUTRAL_FL);
  writeServo(PIN_FR, NEUTRAL_FR);
  writeServo(PIN_RL, NEUTRAL_RL);
  writeServo(PIN_RR, NEUTRAL_RR);

  delay(1000);

  Serial.println("\nCommands:");
  Serial.println("1 = Wave");
  Serial.println("2 = Raise Hand");
  Serial.println("3 = BodyShake");
  Serial.println("4 = Stand");

  Serial.println("\nReady!");
}


// =========================
// Loop
// =========================

void loop() {

  if (Serial.available()) {

    char command = Serial.read();

    // تجاهل Enter
    while (Serial.available()) {
      Serial.read();
    }

    switch (command) {

      case '1':
        wave();
        break;

      case '2':
        raiseHand();
        break;

      case '3':
        thirdMotion();
        break;

      case '4':
        stand();
        break;

      default:
        break;
    }
  }
}
