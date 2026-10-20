#ifndef INPUTS_H
#define INPUTS_H

bool checkButtonPressed(const int);

//Checks button state: By David
bool checkButtonPressed(const int BUTTON)
{
  if (digitalRead(BUTTON) == HIGH) return true;

  return false;
}


#endif