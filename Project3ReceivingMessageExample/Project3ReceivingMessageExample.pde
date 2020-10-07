//Programmer: Courtney Brown
//Desc: Demonstrates receiving OSC with no clutter of the fancy example.
//Prints values receives from another program. 

//Go to the top menu Sketch->Import Library
//then search for oscP5 and import that
import netP5.*;
import oscP5.*;

OscP5 oscP5; //the object that will send OSC
NetAddress remoteLocation; //where we are receiving OSC from

float downValue; //is the mouse down?

//where the mouse is on the screen in %, that is 0.-0.1
float x; 
float y; 
float count;

//the OSC addresses we are receiving
//MUST match what we are sending
final String DOWN_OSC_ADDRESS = "/MakeItArt/Down"; //is the mouse down? 0-no, 1-yes
final String WHERE_OSCADDRESS = "/MakeItArt/Where"; //where is the mouse down? x & y in 0.0-1.0 values
final String PIXELCOUNT_OSCADDRESS = "/MakeItArt/Pixel";

//The port we are listening to... MUST match DESTPORT in the C++ example 
final int LISTENING_PORT = 8888;

void setup()
{
  size(500, 500); 
  background(255);  
  
  //
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
  else if(addr.equals(PIXELCOUNT_OSCADDRESS)) //otherwise it indicates position, so set those
  {
     count = msg.get(0).floatValue(); 
  }
  
  //print the values
  println("downValue:" + downValue + " x:" + x + " y:" + y + " count:"+ count );
}

void draw()
{
//nothing here yet folks.
}
