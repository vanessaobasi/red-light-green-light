#include "pitches.h"
#include "Non_blocking_music.h"
#include "victory_music1.h"
#include "Inputs.h"
#include "combined_header.h"
//PIN assignments
const int SPEAKER_PIN = 4;
const int PIR = A1;
const int RLED = 3;  //red LED
const int GLED = 9;  //green LED
const int BUTTON = 2;
//music variables
int degree;
extern bool isPlaying;
bool PLAYING = true;
//PIR variables
unsigned long detectionStartTime = 0;          // Time when motion detection starts
unsigned long cooldownStartTime = 0;           // Time when red light turns on
const unsigned long cooldownInterval = 1300;   // 1 second cooldown before detecting motion
const unsigned long detectionInterval = 3500;  // 5 second detection window
bool redLightOn = false;                       // Flag for red light state
bool detectingMotion = false;                  // Flag for whether we're in the motion detection window
bool motionDetected = false;                   // Flag for detecting motion
//LED variables
unsigned long LED_start_time = 0;
bool red_light_on = false;
bool turn_green_light_on = false;
//Game control variables
bool gameStart = false;
bool gameOver = false;



void startGame();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  gameStart = true;
}

void loop() {
  if (!gameOver) {
    startGame();  // Start the game if not started
  }

  if (!gameStart && !gameOver) {
    toggle_LEDS();
    playBGM(SPEAKER_PIN, !red_light_on);
    detectMotionDuringRedLight(SPEAKER_PIN, PIR, red_light_on);
  }

  // Check button to end the game
  if (checkButtonPressed(BUTTON)) {
    gameOver = true;
    Serial.println("GAME OVER");
    delay(300);
  }

  // Game over sequence
  if (gameOver) {
    gameOverLEDS(RLED, GLED);
    playVictoryMusic(SPEAKER_PIN);
  }
}
//Starts game, by David
void startGame() {
  while (!checkButtonPressed(BUTTON) && gameStart) {
    blinkAlternatingLEDS(RLED, GLED);
  }
  if (checkButtonPressed(BUTTON) && gameStart) {
    gameStart = false;
    setLEDS(RLED, false, GLED, true);
    flashLEDS(10, 400);
  }
}