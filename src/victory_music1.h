/* Program plays Take on Me. Found online 
and modified to use non blocking polling

By Matthew

*/
int ending_melody[] = {

  // Take on me, by A-ha
  // Score available at https://musescore.com/user/27103612/scores/4834399
  // Arranged by Edward Truong

  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  
};
// Variables to track ending_melody playback
int ending_tempo = 140;

int noteIndex = 0;
unsigned long startNoteTime = 0;
bool notePlaying = false;
unsigned long noteWaitTime = 0;

int notes = sizeof(ending_melody) / sizeof(ending_melody[0]) / 2;
int wholenote = (60000 * 4) / ending_tempo;

void playVictoryMusic(int SPEAKER) {
  // Check if it's time to play the next note
  if (millis() - startNoteTime >= noteWaitTime && !notePlaying) {
    if (noteIndex < notes * 2) {
      int divider = ending_melody[noteIndex + 1];
      int noteDuration = (divider > 0) ? (wholenote / divider) : (wholenote / abs(divider)) * 1.5;
      noteWaitTime = noteDuration; // Store how long to wait for the next note
      startNoteTime = millis(); // Track when this note starts
      
      if (ending_melody[noteIndex] != REST) {
        tone(SPEAKER, ending_melody[noteIndex], noteDuration * 0.9); // Play note for 90% of duration
      }
      noteIndex += 2; // Move to the next note in the ending_melody
    } else {
      noTone(SPEAKER); // Stop sound after ending_melody completes
      noteIndex = 0; // Reset for next loop
    }
    notePlaying = true; // Indicate the note is playing
  }

  // Stop the note once its duration has passed
  if (notePlaying && millis() - startNoteTime >= noteWaitTime * 0.9) {
    noTone(SPEAKER);
    notePlaying = false;
  }
}
