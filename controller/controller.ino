#include "DFRobotDFPlayerMini.h"

HardwareSerial serialPortPlayer(2);
DFRobotDFPlayerMini player;

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 33;  // the number of the pushbutton pin
const int busyPin = 32;
const int ledPin1 = 23;  // the number of the LED pin
const int ledPin2 = 22;  // the number of the LED pin
const int ledPin3 = 27;  // the number of the LED pin
const int ledPin4 = 26;  // the number of the LED pin

int counter = 0;

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
int isBusy = 0;
int isReset = 1;

void setup() {
  Serial.begin(115200);

  Serial.println("Setting everything up");

  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(busyPin, INPUT);

  // Serial player setup
  serialPortPlayer.begin(9600, SERIAL_8N1, 16, 17);  // Speed, Type, RX, TX

  if (!player.begin(serialPortPlayer)) {
    Serial.println(player.readType(), HEX);
    Serial.println("Error connecting to player");

    // When powering on for the first time the board may fail connect. In this case we
    // will reboot and try again, after which it will work.
    ESP.restart();
  }
  Serial.println("Player connected");

  player.setTimeOut(500);
  player.volume(30);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  isBusy = digitalRead(busyPin);
  int pauseAfterSeconds = 5 * 60;  // Pause after 5 minutes (300 seconds)

  if (isBusy == LOW) {
    counter = counter + 1;
    if (counter >= pauseAfterSeconds * 10) {
      player.pause();
      counter = 0;
    }
  }

  Serial.println(counter);

  // check if the pushbutton is pressed. If it is, the buttonState is LOW:
  if (buttonState == LOW && isReset == 1) {
    // turn LED on:
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);

    if (isBusy == HIGH) {
      // If a track isn't currently playing, then play one
      isReset = 0;
      int trackToPlay = random(12) + 1;
      Serial.print("Playing track ");
      Serial.println(trackToPlay);
      player.play(trackToPlay);
      delay(1000);
    }
  } else if (buttonState == HIGH) {
    // turn LED off:
    isReset = 1;
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    player.pause();
  }
  delay(100);
}
