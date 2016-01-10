 /*
//  Name    : Tracing Game                        
//  Author  : Spenser Johnson, Yishu Dai                        
//  Date    : 15 Dec, 2015                                      
//  Version : Alpha 1.0       
//  Circuit Diagram: http://www.schematics.com/project/teensy-controlled-8x8-led-matrix-24953/
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

//  A BUNCH OF DIFFERENT DISPLAYS THAT CAN BE DONE
int rowSelector[]={127,191,223,239,247,251,253,254};    //Controls the row that we are lighting up from top to bottom
int dot[]={32,0,0,0,0,0,0,0};
//int level1[]={32,32,60,4,4,4,124,64};

////////////////////GAME CONDITIONS THAT WE ARE CHANGING/////////////////
int gameLevel = 1;
int gameWinCondition = 0; //0 is game in play, 1 is game loss, 2 is game win

///////////////////ARRAYS/////////////////////////////////////////////
array<int, 8> finalSumArray;

array<array<int, 8>, 8> pathArray = {{ //records where we where and will be checked against the level
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

array<array<int, 8>, 8> currentLocation = {{ //shows where we are located on the board
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> nullArray = {{ // used to reset pathArray at the end of a game
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> startingPoint = {{
    {{0, 0, 1, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> LEVEL1 = {{
    {{0, 0, 1, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 1, 0, 0}}, 
    {{0, 0, 0, 0, 0, 1, 0, 0}},
    {{0, 0, 0, 0, 0, 1, 0, 0}},
    {{0, 0, 0, 0, 0, 1, 0, 0}}, 
    {{0, 1, 1, 1, 1, 1, 0, 0}},
    {{0, 1, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> LEVEL2 = {{
    {{0, 0, 1, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 1, 1, 1, 1}}, 
    {{0, 0, 1, 1, 1, 0, 0, 1}}, 
    {{0, 0, 0, 0, 0, 0, 1, 1}},
    {{0, 0, 0, 0, 0, 0, 1, 0}},
    {{0, 0, 0, 0, 0, 0, 1, 0}}, 
    {{0, 1, 1, 1, 0, 0, 1, 0}},
    {{0, 1, 0, 1, 1, 1, 1, 0}}
}};

const array<array<int, 8>, 8> LEVEL3 = {{
    {{0, 0, 1, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 1, 1, 1, 1}}, 
    {{1, 1, 1, 0, 1, 0, 0, 1}}, 
    {{1, 0, 0, 0, 1, 0, 1, 1}},
    {{1, 1, 1, 1, 1, 0, 1, 0}},
    {{0, 0, 0, 0, 0, 0, 1, 0}}, 
    {{0, 1, 1, 1, 0, 1, 1, 0}},
    {{0, 1, 0, 1, 1, 1, 0, 0}}
}};

const array<array<int, 8>, 8> LEVELtemplate = {{
    {{0, 0, 1, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> LEVEL4 = {{
    {{0, 0, 1, 1, 1, 1, 1, 1}}, 
    {{0, 0, 0, 0, 0, 1, 0, 1}}, 
    {{0, 0, 1, 1, 1, 1, 1, 1}}, 
    {{0, 0, 1, 1, 0, 0, 0, 0}},
    {{0, 0, 0, 1, 1, 0, 0, 0}},
    {{0, 0, 1, 1, 1, 0, 0, 0}}, 
    {{0, 0, 1, 1, 1, 1, 1, 1}},
    {{0, 0, 0, 0, 0, 0, 0, 1}} // eric's level
}};

const array<array<int, 8>, 8> happy = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 0, 0, 0, 0, 1, 0}},
    {{0, 1, 0, 0, 0, 0, 1, 0}}, 
    {{0, 1, 1, 1, 1, 1, 1, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> sad = {{
    {{0, 0, 0, 0, 0, 0, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 0, 1, 0, 0, 1, 0, 0}}, 
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 1, 1, 1, 1, 1, 0}},
    {{0, 1, 0, 0, 0, 0, 1, 0}}, 
    {{0, 1, 0, 0, 0, 0, 1, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

const array<array<int, 8>, 8> YouWin = {{
    {{0, 0, 0, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 1, 1, 0, 0, 0}}, 
    {{0, 0, 0, 1, 1, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0}}
}};

////////////////BINARY 8X8 ARRAY ADDER/////////////////
array<array<int, 8>, 8> binArrayAdder(const array<array<int, 8>, 8> input1, const array<array<int, 8>, 8> input2){
  array<array<int, 8>, 8> output;
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      output[i][j] = input1[i][j] || input2[i][j];
    }
  }
  return output;
}
/*HORIZONTAL AND VERTICAL SHIFTING FUNCTIONS FOR 1X8 ARRAY SIZES*/
///////////////////////////////////////////////////////////////////////
void shiftRight() {
    array<array<int,8>,8> tempArray;
    for (int i=0; i<8;i++){
      for (int j=0;j<8;j++){
        tempArray[i][j]=currentLocation[i][j];
      }
    }
    for (int i=0;i<8;i++){
      for (int j=0; j<8;j++){
        if (j == 0){
          currentLocation[i][0]=tempArray[i][7];
        }
        else{currentLocation[i][j] = tempArray[i][j-1];
        }
      }
    }
}

void shiftLeft() {
    array<array<int,8>,8> tempArray;
    for (int i=0; i<8;i++){
      for (int j=0;j<8;j++){
        tempArray[i][j]=currentLocation[i][j];
      }
    }
    for (int i=0;i<8;i++){
      for (int j=0; j<8;j++){
        if (j==7){
          currentLocation[i][7]=tempArray[i][0];
        }
        else{currentLocation[i][j] = tempArray[i][j+1];
        }
      }
    }
}

void shiftUp() {
  array<array<int,8>,8> tempArray;
  for (int i=0;i < 8;i++){
    tempArray[i]=currentLocation[i];
  }
  for (int i=0; i<8;i++){
    if (i==7){
      currentLocation[7]=tempArray[0];
    }
    else{currentLocation[i] = tempArray[i+1];
    }
  }
}

void shiftDown() {
  array< array<int,8>, 8> tempArray;
  for (int i=0;i < 8;i++){
    tempArray[i]=currentLocation[i];
  }
  for (int i=0; i<8;i++){
    if (i==0){
      currentLocation[0]=tempArray[7];
    }
    else{currentLocation[i] = tempArray[i-1];
    }
  }
}
//////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////
void flashLine(array<array<int,8>,8> someLine, int timeDisplay){
  array<int,8> simpleArray = binArray2DecArray(someLine);
  for (int t = 0;t<timeDisplay;t++){
    for (int i=0;i<8;i++){
      //ground latchPin and hold low for as long as you are transmitting
      digitalWrite(LATCH_PIN, LOW);
      delay(1);
      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, rowSelector[i]);
      shiftOut(DATA_PIN,CLOCK_PIN,LSBFIRST, simpleArray[i]);   
      //return the latch pin high to signal chip that it 
      //no longer needs to listen for information
      digitalWrite(LATCH_PIN, HIGH);
    }
  }
}
//////////////////////////////////////////////////////
void compare(array<array<int, 8>, 8> currentLevel, array<array<int, 8>, 8> pathSubmitted) { //compares path array and level map after a button press
  int errors = 0;
  for (int i=0;i<8;i++){
      for (int j=0; j<8;j++){
        if (currentLevel[i][j] != pathSubmitted[i][j]) {
          errors++;
          //display sad face for 5 seconds, then clear all
          //Change an error value to 1
        }
      }
  }
  if (errors>0) gameWinCondition = 1;
  if (errors==0) gameWinCondition = 2;
}
//////////////////////////////////////////////////////
void resGame(){
  currentLocation=startingPoint;
  pathArray=nullArray;
  gameWinCondition = 0;  
}
/////////////////////////////////////////////////////
array<array<int,8>,8> getLevel(){
  if (gameLevel == 1) return LEVEL1;
  if (gameLevel == 2) return LEVEL2;
  if (gameLevel == 3) return LEVEL3;
  if (gameLevel == 4) return LEVEL4;
  else return LEVEL1;
}
//////////////////////////////////////////////////////
void setup(){
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Started Up Connections");
  flashLine(LEVEL1,300);
  Serial.println("Displayed Level 1");
  currentLocation=startingPoint;
}
void loop() {
  if (gameWinCondition == 0){
    array<array<int,8>,8> currentLevel = getLevel();
    ////////////////UPDATING CURRENT LOCATION/////////////
    byte incomingByte = 0;
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      //Serial.print("USB received: ");
      //Serial.println(incomingByte,BIN);
    }
    if (incomingByte == B1101111){
      flashLine(sad, 1000);
      delay(1000);
      flashLine(currentLevel,300);
    }
    if (incomingByte == B1110111) shiftUp();
    if (incomingByte == B1110011) shiftDown();
    if (incomingByte == B1100100) shiftRight();
    if (incomingByte == B1100001) shiftLeft();
    if (incomingByte == B00100000) compare(currentLevel,pathArray);
    /////////////////UPDATING PATH TRAVELLED////////////////
    pathArray = binArrayAdder(pathArray,currentLocation);
    /////////////CONVERSION FOR DISPLAY PURPOSE////////////
    finalSumArray = binArray2DecArray(pathArray);
    /////////////////DISPLAYING PATH ARRAY/////////////////
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
  if (gameWinCondition == 1){
    flashLine(sad,100);
    resGame();
    gameLevel=1;
    array<array<int,8>,8> nextLevel = getLevel();
    flashLine(nextLevel,300);    
  }
  if (gameWinCondition == 2){  
    flashLine(happy,100);
    gameLevel++;
    if (gameLevel==5){
      flashLine(YouWin,500);
      gameLevel=1;
    }
    array<array<int,8>,8> nextLevel = getLevel();
    flashLine(nextLevel,300);
    resGame();     
  }
}
