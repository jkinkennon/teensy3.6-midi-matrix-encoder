// Name:    MIDI_Matrix_Allen
// Created: 11/30/2019
// Author:  John Kinkennon

#include <arduino.h>
#include "User.h"
#include "Midi.h"

const int column1 = 0;
const int column2 = 1;
const int column3 = 2;
const int column4 = 3;
const int column5 = 4;
const int column6 = 5;
const int column7 = 6;
const int column8 = 7;
const int column9 = 8;
const int column10 = 9;
const int column11 = 10;
const int column12 = 11;
const int column13 = 12;
const int column14 = 24;
const int column15 = 25;
const int column16 = 26;
const int column17 = 27;
const int column18 = 28;
const int column19 = 29;
const int column20 = 30;
const int column21 = 31;
const int column22 = 32;

const int row1 = 16;
const int row2 = 17;
const int row3 = 18;
const int row4 = 19;
const int row5 = 20;
const int row6 = 21;
const int row7 = 22;
const int row8 = 23;
const int row9 = 15;
const int row10 = 14;
const int row11 = 13;
const int row12 = 37;
const int row13 = 36;
const int row14 = 35;
const int row15 = 34;
const int row16 = 33;

keyTable8_t keyTable;
keyTableB_t keyBit;           // true for keys which are depressed
keyTableB_t keyOn;            // true for keys which are on after debounce

elapsedMicros sinceScan;      // micro seconds since previous scan
bool scanTime;

//void myNoteOff(byte, byte, byte);
//void myNoteOn(byte, byte, byte);

void setup() {
  int i;
  pinMode(row1, INPUT);
  pinMode(row2, INPUT);
  pinMode(row3, INPUT);
  pinMode(row4, INPUT);
  pinMode(row5, INPUT);
  pinMode(row6, INPUT);
  pinMode(row7, INPUT);
  pinMode(row8, INPUT);
  pinMode(row9, INPUT);
  pinMode(row10, INPUT);
  pinMode(row11, INPUT);
  pinMode(row12, INPUT);
  pinMode(row13, INPUT);
  pinMode(row14, INPUT);
  pinMode(row15, INPUT);
  pinMode(row16, INPUT);

  pinMode(column1, OUTPUT);   digitalWrite(column1, LOW);
  pinMode(column2, OUTPUT);   digitalWrite(column2, HIGH);
  pinMode(column3, OUTPUT);   digitalWrite(column3, HIGH);
  pinMode(column4, OUTPUT);   digitalWrite(column4, HIGH);
  pinMode(column5, OUTPUT);   digitalWrite(column5, HIGH);
  pinMode(column6, OUTPUT);   digitalWrite(column6, HIGH);
  pinMode(column7, OUTPUT);   digitalWrite(column7, HIGH);
  pinMode(column8, OUTPUT);   digitalWrite(column8, HIGH);
  pinMode(column9, OUTPUT);   digitalWrite(column9, HIGH);
  pinMode(column10, OUTPUT);  digitalWrite(column10, HIGH);
  pinMode(column11, OUTPUT);  digitalWrite(column11, HIGH);
  pinMode(column12, OUTPUT);  digitalWrite(column12, HIGH);
  pinMode(column13, OUTPUT);  digitalWrite(column13, HIGH);
  pinMode(column14, OUTPUT);  digitalWrite(column14, HIGH);
  pinMode(column15, OUTPUT);  digitalWrite(column15, HIGH);
  pinMode(column16, OUTPUT);  digitalWrite(column16, HIGH);
  pinMode(column17, OUTPUT);  digitalWrite(column17, HIGH);
  pinMode(column18, OUTPUT);  digitalWrite(column18, HIGH);
  pinMode(column19, OUTPUT);  digitalWrite(column19, HIGH);
  pinMode(column20, OUTPUT);  digitalWrite(column20, HIGH);
  pinMode(column21, OUTPUT);  digitalWrite(column21, HIGH);
  pinMode(column22, OUTPUT);  digitalWrite(column22, HIGH);
  
  for (i = 0; i < NUM_SWITCHES; i++) {
    keyTable.O[i] = 0;
    keyBit.O[i] = false;
    keyOn.O[i] = false;
  }

//  usbMIDI.setHandleNoteOff(myNoteOff);
//  usbMIDI.setHandleNoteOn(myNoteOn);
}

void loop() {

  while (scanTime == false) {       // for now just delay here
    if (sinceScan >= 330) {
      scanTime = true;
      sinceScan = 0;
    }
  }
  scanTime = false;

  int matrix_pair;
  for (matrix_pair = 0; matrix_pair < 2; matrix_pair++) {
    scan(matrix_pair);  // 8 or 11 columns
  }
  usbMIDI.read();       // prime USB to receive any MIDI packet
}

void scan(int matrix_pair) {
  // scan two of four matrix manuals
  int col = 0;
  if (matrix_pair == 1) col = 11;
  for (int column = col; column < (col + NUM_COLUMNS); column++) {
    if (NUM_COLUMNS == 8) readSixteenRows(matrix_pair, column);
    if (NUM_COLUMNS == 11) readTwelveRows(matrix_pair, column);
    setColumn(column + 1);
  }
} 
 
void readSixteenRows(int matrix_pair, int column) {
  // read the rows and write to keyBit[ch][key]
  int matrix = 0;
  int key1 = column*8;
  if(matrix_pair == 1) {
    matrix = 2;
    key1 = (column - NUM_COLUMNS) * NUM_ROWS;
  }
  int key = key1;
  keyBit.C[matrix][key++] = !digitalRead(row1);
  keyBit.C[matrix][key++] = !digitalRead(row2);
  keyBit.C[matrix][key++] = !digitalRead(row3);
  keyBit.C[matrix][key++] = !digitalRead(row4);
  keyBit.C[matrix][key++] = !digitalRead(row5);
  keyBit.C[matrix][key++] = !digitalRead(row6);
  keyBit.C[matrix][key++] = !digitalRead(row7);
  keyBit.C[matrix][key++] = !digitalRead(row8);
  updateKeyTable(matrix);
  matrix++;
  key = key1;
  keyBit.C[matrix][key++] = !digitalRead(row9);
  keyBit.C[matrix][key++] = !digitalRead(row10);
  keyBit.C[matrix][key++] = !digitalRead(row11);
  keyBit.C[matrix][key++] = !digitalRead(row12);
  keyBit.C[matrix][key++] = !digitalRead(row13);
  keyBit.C[matrix][key++] = !digitalRead(row14);
  keyBit.C[matrix][key++] = !digitalRead(row15);
  keyBit.C[matrix][key++] = !digitalRead(row16);
  updateKeyTable(matrix);
}

void readTwelveRows(int matrix_pair, int column) {
  // read the rows and write to keyBit[ch][key]
  int matrix = 0;
  int key1 = (column - 1)*NUM_ROWS + 1;;
  if (matrix_pair == 1) {
    matrix = 2;
    key1 = (column - 12)*NUM_ROWS + 1;
  }
  if(column == 0) {
    keyBit.C[0][0] = !digitalRead(row6);
    keyBit.C[1][0] = !digitalRead(row14);
  }
  else if(column == 11) {
    keyBit.C[2][0] = !digitalRead(row6);
    keyBit.C[3][0] = !digitalRead(row14);
  }
  else {
    int key = key1;
    keyBit.C[matrix][key++] = !digitalRead(row1);
    keyBit.C[matrix][key++] = !digitalRead(row2);
    keyBit.C[matrix][key++] = !digitalRead(row3);
    keyBit.C[matrix][key++] = !digitalRead(row4);
    keyBit.C[matrix][key++] = !digitalRead(row5);
    keyBit.C[matrix][key++] = !digitalRead(row6);
    updateKeyTable(matrix);
    matrix++;
    key = key1;
    keyBit.C[matrix][key++] = !digitalRead(row9);
    keyBit.C[matrix][key++] = !digitalRead(row10);
    keyBit.C[matrix][key++] = !digitalRead(row11);
    keyBit.C[matrix][key++] = !digitalRead(row12);
    keyBit.C[matrix][key++] = !digitalRead(row13);
    keyBit.C[matrix][key++] = !digitalRead(row14);
    updateKeyTable(matrix);
  }
}

void setColumn(int column) {
  if (column >= NUM_COLUMNS*2) column = 0;
  switch(column) {
    case 0:
      digitalWrite(column16, HIGH);
      digitalWrite(column22, HIGH);
      digitalWrite(column1, LOW);
      break;
    case 1:
      digitalWrite(column1, HIGH);
      digitalWrite(column2, LOW);
      break;
    case 2:
      digitalWrite(column2, HIGH);
      digitalWrite(column3, LOW);
      break;
    case 3:
      digitalWrite(column3, HIGH);
      digitalWrite(column4, LOW);
      break;
    case 4:
      digitalWrite(column4, HIGH);
      digitalWrite(column5, LOW);
      break;
    case 5:
      digitalWrite(column5, HIGH);
      digitalWrite(column6, LOW);
      break;
    case 6:
      digitalWrite(column6, HIGH);
      digitalWrite(column7, LOW);
      break;
    case 7:
      digitalWrite(column7, HIGH);
      digitalWrite(column8, LOW);
      break;
    case 8:
      digitalWrite(column8, HIGH);
      digitalWrite(column9, LOW);
      break;
    case 9:
      digitalWrite(column9, HIGH);
      digitalWrite(column10, LOW);
      break;
    case 10:
      digitalWrite(column10, HIGH);
      digitalWrite(column11, LOW);
      break;
    case 11:
      digitalWrite(column11, HIGH);
      digitalWrite(column12, LOW);
      break;
    case 12:
      digitalWrite(column12, HIGH);
      digitalWrite(column13, LOW);
      break;
    case 13:
      digitalWrite(column13, HIGH);
      digitalWrite(column14, LOW);
      break;
    case 14:
      digitalWrite(column14, HIGH);
      digitalWrite(column15, LOW);
      break;
    case 15:
      digitalWrite(column15, HIGH);
      digitalWrite(column16, LOW);
      break;
    case 16:
      digitalWrite(column16, HIGH);
      digitalWrite(column17, LOW);
      break;
    case 17:
      digitalWrite(column17, HIGH);
      digitalWrite(column18, LOW);
      break;
    case 18:
      digitalWrite(column18, HIGH);
      digitalWrite(column19, LOW);
      break;
    case 19:
      digitalWrite(column19, HIGH);
      digitalWrite(column20, LOW);
      break;
    case 20:
      digitalWrite(column20, HIGH);
      digitalWrite(column21, LOW);
      break;
    case 21:
      digitalWrite(column21, HIGH);
      digitalWrite(column22, LOW);
      break;
    default:
      break;
  }
}

void updateKeyTable(int ch) {
  
  for (int key = 0; key < 64; key++) {
    uint8_t prevData = keyTable.C[ch][key];
    bool newKey = keyBit.C[ch][key];  // is keyDown now?
    bool keyDown = (prevData & 0x80); // was keyDown true on previous scan?
    prevData <<= 1;                   // shift the data left one bit
    if (newKey) prevData++;           // set newest bit
    prevData &= 0x0f;                 // strip any high bits

    if (!keyDown) { // if note is off in keyTable see if it should be on
      //if (prevData == 0b00000011) {   // if 3 consecutive keyDown
      if (prevData > 0) {           // this is a fast noteOn for testing
        keyDown = true;               // turn key on in keyTable
        keyOn.C[ch][key] = true;
        usbMIDI.sendNoteOn(key + 0x24, M_VELOCITY_ON, ch + 1);
      }
    }
    else if (keyDown) { // if note is on in keyTable see if it should be off
      if (prevData == 0b00000000) {   // if 4 consecutive keyUp
        keyDown = false;              // turn key off in keyTable
        keyOn.C[ch][key] = false;
        usbMIDI.sendNoteOff(key + 0x24, M_VELOCITY_OFF, ch + 1);
      }
    }
    if (keyDown) prevData |= 0x80;    // store the data
      keyTable.C[ch][key] = prevData;
  }
}
