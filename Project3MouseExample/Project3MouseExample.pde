//Programmer: Courtney Brown
//Desc: Demonstrates something cool to do with Project 3's concept using processing.
//Desc: Receives OSC and uses value to play a musical instrument.

//Go to the top menu Sketch->Import Library
//then search for oscP5 and import that
import netP5.*;
import oscP5.*;

//Go to the top menu Sketch->Import Library
//then search for Minim and import that
import ddf.minim.*;
import ddf.minim.ugens.*;

Minim minim; //the minimum object
AudioOutput out; //our audio out
IndexToPitches pitches = new IndexToPitches(); //an array list of the hertz of pitches from C2-C4

OscP5 oscP5; //the object that will send OSC
NetAddress remoteLocation; //where we are receiving OSC from

float downValue; //is the mouse down?

//where the mouse is on the screen in %, that is 0.-0.1
float x; 
float y; 

//the OSC addresses we are receiving
//MUST match what we are sending
final String DOWN_OSC_ADDRESS = "/MakeItArt/Down"; //is the mouse down? 0-no, 1-yes
final String WHERE_OSCADDRESS = "/MakeItArt/Where"; //where is the mouse down? x & y in 0.0-1.0 values

//The port we are listening to... MUST match DESTPORT in the C++ example 
final int LISTENING_PORT = 8888;

//how many milliseconds in a second?
final float HALF_SECOND = 500;
final float QUARTER_SECOND = 500;

//the last time we played a note... let's not play too many at once and overload the system
float lastPlayed = 0; 

void setup()
{
  size(500, 500); 
  background(255);  
  
  //initialize OSC
  oscP5 = new OscP5(this, LISTENING_PORT); //listening for incoming!!
  
  //init the minim
  minim = new Minim(this);
  
  
  //copied from Minin example -- see the Sine Instrument tab for details
  // use the getLineOut method of the Minim object to get an AudioOutput object
  out = minim.getLineOut();
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
    
}

void draw()
{
  float timeBetween =  millis()-lastPlayed; //time between last time it was played
  
  //if the timebetween is longer than a 0.25 sec, then play
  //so pitches is an array list of pitches
  //so we multiply the y value times the # of pitches to get which pitch to select
  //since y is 0.0 to 1.0 it will give us the index into pitches. higher y - higher pitch. (see IndexToPitches class)
  if(timeBetween>=QUARTER_SECOND) 
    if(downValue >= 1.0)
      out.playNote( 0.0, 0.9, new ToneInstrument( pitches.getPitch(round(y*(pitches.size()-1)) ) , 0.2f ));

  //NOTE I have haven't mapped the x values yet
}
