//Programmer: Eric Miao
//Desc: 
//Draw squares to the window based on motion detected by user

//Go to the top menu Sketch->Import Library
//then search for oscP5 and import that
import netP5.*;
import oscP5.*;
import ddf.minim.*;
import ddf.minim.ugens.*;

Minim minim; //the minimum object
AudioOutput out; //our audio out


OscP5 oscP5; //the object that will send OSC
NetAddress remoteLocation; //where we are receiving OSC from

float downValue; //is the mouse down?
ArrayList<Integer> count = new ArrayList<Integer>(); //this is for storing all features from opencv
//where the mouse is on the screen in %, that is 0.-0.1
float x; 
float y; 

//the OSC addresses we are receiving
//MUST match what we are sending
final String DOWN_OSC_ADDRESS = "/MakeItArt/Down"; //is the mouse down? 0-no, 1-yes
final String WHERE_OSCADDRESS = "/MakeItArt/Where"; //where is the mouse down? x & y in 0.0-1.0 values
final String PIXELCOUNT_OSCADDRESS = "/MakeItArt/Pixel";

int N = 10; // represent the number of squares on the windows
int normalizedValue = 750; // this is for changing color/transparency

//The port we are listening to... MUST match DESTPORT in the C++ example 
final int LISTENING_PORT = 8888;

// For playing note
final float HALF_SECOND = 500;
final float QUARTER_SECOND = 500;

//the last time we played a note... let's not play too many at once and overload the system
float lastPlayed = 0; 
  
void setup()
{
  size(640, 480); 
  background(255);  
  oscP5 = new OscP5(this, LISTENING_PORT); //listening for incoming!!
}



//this is the overloaded function to receive our OSC message 
void oscEvent(OscMessage msg)
{
  String addr = msg.addrPattern(); //get the address
  
  if(addr.equals(DOWN_OSC_ADDRESS)) //if the address is mouse down, then set downValue
  {
    downValue = msg.get(0).floatValue();   
  }
  else if(addr.equals(WHERE_OSCADDRESS)) //otherwise it indicates position, so set those
  {
     x = msg.get(0).floatValue(); 
     y = msg.get(1).floatValue(); 
  }
  
  // read in all the features from opencv program
  else if(addr.equals(PIXELCOUNT_OSCADDRESS)) //otherwise it indicates position, so set those
  {
      for(int i = 0; i < N*N; i++){
         count.add(msg.get(i).intValue());
       }
  }
}

void draw()
{
  frameRate(30);
  showInstruction();
  display();
  count.clear();
}


// display user instructions
void showInstruction(){
  textSize(12);
  fill(0, 102, 153, 51);
  text("Press a, b ,c to chnage the normalization value and transparency", 10, 60);
  text("Press d to activate color mode", 10, 90); 
  text("Press r to reset the program", 10, 120); 
}


void reset(){
  setup();
  // Doesn't do anything yet.
}

void display(){
  float timeBetween =  millis()-lastPlayed; //time between last time it was played
  
  int squareHeight = height/N; // width of square height
  int squareWidth = width/N; // width of square width
  
  
  // if pressed 'r', activate reset and clear the canva
  if(key == 'r'){
   reset();
   return;
  }
  
  for(int i = 0; i < count.size(); i++){
    int val = count.get(i);
    if(count.get(i) > 100 && count.get(i) < 8000){
      int row = i / N; // which row the square gonna be at
      int col = i % N; // which column the square gonna be at
      int x = row*squareWidth; //x position of the square
      int y = col*squareHeight; // y position of the square
      noStroke();
      int c =  int(val) % int(normalizedValue); // color and it is being normalized based on mode selected by user
      
      int transpancy = val % 210; // transparency value that is normalized
      fill(c,c,c,transpancy);
      
      // if pressed 'd', show color
      if(key == 'd'){
        println("key d is pressed");
        fill(random(255),random(255),random(255),random(20));
      }
      
      rect(x, y, squareWidth, squareHeight);
      
      // playing note based on # of features in the square
      if(timeBetween>=QUARTER_SECOND){
       //out.playNote( 0.0, 0.5, new ToneInstrument( float(count.get(0) % 100.0), 0.2f ));
       println("playing notes based on normalization valued: ", count.get(i) % 100.0);
      }
    }
  }
}



void playNotes(int value){
  float timeBetween =  millis()-lastPlayed; //time between last time it was played
  //if the timebetween is longer than a 0.25 sec, then play
  //so pitches is an array list of pitches
  //so we multiply the y value times the # of pitches to get which pitch to select
  //since y is 0.0 to 1.0 it will give us the index into pitches. higher y - higher pitch. (see IndexToPitches class)
  
}



// pressing different keys will change the normalization value
void keyPressed(){
  if (key == 'a'){
    println("key a is pressed");
    normalizedValue = 1000;
  }
  if (key == 'b'){
    println("key b is pressed");
    normalizedValue = 1200;
  }
  if (key == 'c'){
    println("key c is pressed");
    normalizedValue = 1500;
  }
    if (key == 'e'){
    println("key e is pressed");
    normalizedValue = 250;
  }
}
