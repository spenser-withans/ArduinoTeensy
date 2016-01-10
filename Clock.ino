/*
//  Name    : Shift Register Clock on a 8x8 LED Matrix                         
//  Author  : Spenser Johnson, Yishu Dai                        
//  Date    : 8 Dec, 2015                                      
//  Version : 1.0                                               
//  Notes   : Code to Display 8x8 Images to an LED Matrix
          Made for two 74HC595N shift registers and Teensy.
          Circuit: http://www.schematics.com/project/teensy-controlled-8x8-led-matrix-24953/
*/

//Pin connected to ST_CP of 74HC595
#define LATCH_PIN 8
//Pin connected to SH_CP of 74HC595
#define CLOCK_PIN 12
////Pin connected to DS of 74HC595
#define DATA_PIN 11

#include <array>
using std::array;

#define HWSERIAL Serial1
//////////////////PART OF THE TIME FUNCTIONING: TAKEN FROM THE TimeSerial EXAMPLE PROGRAM
#include <Time.h>  
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

/////////////////  A BUNCH OF DIFFERENT DISPLAYS THAT CAN BE DONE
int rowSelector[]={127,191,223,239,247,251,253,254};    //Controls the row that we are lighting up from top to bottom
array<int, 8> finalSumArray;                      //An 8 long array of integers that are read into the shift register. Changes based on what needs to be displayed

array<array<int, 8>, 8> largeArray = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> ONE = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 1, 1, 0, 0, 0}},
    {{0, 0, 0, 0, 1, 0, 0, 0}},
    {{0, 0, 0, 0, 1, 0, 0, 0}},
    {{0, 0, 0, 1, 1, 1, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> TWO = {{ 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 1, 0, 0}}, 
    {{0, 0, 0, 0, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 1, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> THREE = {{ 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 1, 0, 0}}, 
    {{0, 0, 0, 0, 0, 1, 0, 0}}, 
    {{0, 0, 1, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> FOUR = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 0, 0, 1, 1, 1, 0, 0}}, 
    {{0, 0, 0, 0, 0, 1, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> FIVE = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 0, 0, 0}}, 
    {{0, 0, 1, 1, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 1, 0, 0, 0}}, 
    {{0, 0, 1, 1, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> SIX = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 0, 0, 0}}, 
    {{0, 0, 1, 0, 1, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> SEVEN = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 1, 0, 0}}, 
    {{0, 0, 0, 0, 1, 0, 0, 0}}, 
    {{0, 0, 0, 0, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> EIGHT = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 1, 0, 0}}, 
    {{0, 0, 0, 1, 0, 1, 0, 0}}, 
    {{0, 0, 1, 0, 1, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> NINE = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 1, 0, 0}}, 
    {{0, 0, 0, 1, 0, 1, 0, 0}}, 
    {{0, 0, 0, 0, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> TEN = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 1, 0, 0, 0}}, 
    {{0, 0, 1, 1, 0, 1, 0, 0}}, 
    {{0, 0, 1, 1, 0, 1, 0, 0}}, 
    {{0, 0, 1, 0, 1, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> ELEVEN = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 1, 1, 0, 1, 1, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> TWELVE = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 1, 1, 0, 1, 1, 0, 0}}, 
    {{0, 0, 1, 0, 0, 0, 1, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> ZEROMIN = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> FIVEMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> TENMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> XVMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> XXMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}} 
}};

const array<array<int, 8>, 8> XXVMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 1, 1, 0}} 
}};
const array<array<int, 8>, 8> XXXMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 1, 1, 1, 1, 0}} 
}};
const array<array<int, 8>, 8> XXXVMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}} 
}};
const array<array<int, 8>, 8> XLMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}} 
}};
const array<array<int, 8>, 8> VLMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}} 
}};
const array<array<int, 8>, 8> LMIN = {{
    {{0, 0, 0, 0, 0, 1, 1, 0}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}} 
}};
const array<array<int, 8>, 8> LVMIN = {{
    {{0, 1, 1, 0, 0, 1, 1, 0}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{1, 0, 0, 0, 0, 0, 0, 1}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}} 
}};

////////////////BINARY 8X8 ARRAY ADDER/////////////////
//Takes two input arrays of 0s and 1s and Returns an 8x8 Array of 0s and 1s

array<array<int, 8>, 8> binArrayAdder(const array<array<int, 8>, 8> input1, const array<array<int, 8>, 8> input2){
  array<array<int, 8>, 8> output;
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      output[i][j] = input1[i][j] || input2[i][j];
    }
  }
  return output;
}

//////////////////////////////////////////////////////
////////////////8X8 BINARY ARRAY TO 8X1 DECIMAL ARRAY CONVERTER/////////////////////
//Converts a list of binary Arrays(varArray) into an array of Decimal sums(outArray).

array<int, 8> binArray2DecArray(const array<array<int, 8>, 8> varArray) { //Given an array 8x8
  array<int, 8> output;
  for(int i=0;i<8;i++){                    //For the row
    int total=0;                           //Start a running total
    for(int j=0;j<8;j++){                  //For each column
      total += varArray[i][j] * int(pow(2,7-j));//Calculate what should be the binary value for it the column position and add to total if 1
    }
    output[i]=total;                     //Add the total to the ith position of the output array
  }
  return output;
}
////////////////////FUNCTIONS TO SELECT THE APPROPRIATE HOUR AND MINUTE ARRAYS///////////////
array<array<int,8>,8> getHour(){
  int h=hourFormat12();
  if (h==1) return ONE;
  if (h==2) return TWO;
  if (h==3) return THREE;
  if (h==4) return FOUR;
  if (h==5) return FIVE;
  if (h==6) return SIX;
  if (h==7) return SEVEN;
  if (h==8) return EIGHT;
  if (h==9) return NINE;
  if (h==10) return TEN;
  if (h==11) return ELEVEN;
  if (h==12) return TWELVE;
}

array<array<int,8>,8> getMinute(){
  int m=minute();
  if (m< 5) return ZEROMIN;
  else if (m<10) return FIVEMIN;
  else if (m<15) return TENMIN;
  else if (m<20) return XVMIN;
  else if (m<25) return XXMIN;
  else if (m<30) return XXVMIN;
  else if (m<35) return XXXMIN;
  else if (m<40) return XXXVMIN;
  else if (m<45) return XLMIN;
  else if (m<50) return VLMIN;
  else if (m<55) return LMIN;
  else (m<60); return LVMIN;
}

//////////////////////CLOCK/TIME-FETCHING CODE TAKEN FROM TIMESERIAL EXAMPLE///////////////////
///////////////IT HAS BEEN CHANGED TO MEET OUR NEEDS//////////////////////////
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hourFormat12());
  printDigits(minute());
  Serial.println(); 
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}


void setup() {
  Serial.begin(9600);

  //set pins to output because they are addressed in the main loop
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
  
}

void loop() {
  if (Serial.available()) {
    processSyncMessage();
  }
//  if (timeStatus()!= timeNotSet) {
//    digitalClockDisplay();  
//  }
//////////////////FIGURES OUT WHAT TO DISPLAY HERE////////////////////////
  array<array<int, 8>, 8> hour = getHour();
  array<array<int, 8>, 8> minute = getMinute();
  largeArray = binArrayAdder(hour, minute);
  finalSumArray = binArray2DecArray(largeArray);

  ////////////////////DISPLAY FUNCTIONING/////////////////////////////
  for (int i=0; i< 8 ;i++){
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(LATCH_PIN, LOW);
    delay(1);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, rowSelector[i]);
    shiftOut(DATA_PIN,CLOCK_PIN,LSBFIRST, finalSumArray[i]);   
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(LATCH_PIN, HIGH);
  }
} 
