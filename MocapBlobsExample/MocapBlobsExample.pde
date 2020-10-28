//Programmer: Courtney Brown
//Desc: Demonstrates receiving blob features
//Desc: Receives OSC and uses value to play a musical instrument & draw a blob.
//Date: Oct. 2020

import netP5.*;
import oscP5.*;
import ddf.minim.*;
import ddf.minim.ugens.*;

Minim minim; //the minimum object
AudioOutput out; //our audio out
IndexToPitches pitches = new IndexToPitches(); //an array list of the hertz of pitches from C2-C4

OscP5 oscP5; //the object that will send OSC
NetAddress remoteLocation; //where we are receiving OSC from

float downValue; //is the mouse down?

//where the mouse is on the screen in %, that is 0.-1.0
float x; 
float y; 

//the OSC addresses we are receiving
//MUST match what we are sending
final String DOWN_OSC_ADDRESS = "/MakeItArt/Down"; //is the mouse down? 0-no, 1-yes
final String WHERE_OSC_ADDRESS = "/MakeItArt/Where"; //where is the mouse down? x & y in 0.0-1.0 values
final String BLOB_OSCADDRESS = "/MakeItArt/Blobs"; 

//The port we are listening to... MUST match DESTPORT in the C++ example 
final int LISTENING_PORT = 8888;

//how many milliseconds in a second?
final float HALF_SECOND = 500;
final float QUARTER_SECOND = 500;

//the last time we played a note... let's not play too many at once and overload the system
float lastPlayed = 0; 

//my homemade Dictionary object -- feel free to sub if you are familiar & want to but if not, can leave as is
ArrayList<Blob> blobs = new ArrayList(); //all the blobs I found
ArrayList<Float> blobsID = new ArrayList(); //a quick hack so I don't have to write a find method

int newBlobs = 0; //how many new blobs this frame? 

int WHEN_A_BLOB_IS_DEAD = -100; 

//age all the blobs (note that a frame has gone by -- one frame older and closer to death!)
void ageBlobs()
{
    for(int i=0; i<blobs.size(); i++)
    {
      blobs.get(i).age(); 
    }
}

//if a blob is too old -- that is, it's been around awhile w/o updating -- BAH-lete
void killBlobs()
{
    //note how I'm going backwards through the ArrayList
    //always go backwards through an dynamic array to delete because the indices won't change no matter what you delete.
    for(int i=blobs.size()-1; i>=0; i--)
    {             
      if(blobs.get(i).isDead())
      {
         blobs.remove(i);    
         blobsID.remove(i);
      }
    }
}

//sets up OSC & the sound library Minim
void setup()
{
  size(640, 480); //NOTE: I made the size of the screen match the size of the C++ screen so I don't calculate anything extra. 
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
  
  int numberOfParamsPerMessage = 4; //change this number depending on how many features you send per blob
  if( addr.equals(DOWN_OSC_ADDRESS)){
     downValue = msg.get(0).floatValue();   
  }
  
  else if(addr.equals(WHERE_OSC_ADDRESS)){
     x = msg.get(0).floatValue(); 
     y = msg.get(1).floatValue(); 
  }
  
  
  
  //if( addr.equals(BLOB_OSCADDRESS) )
  //{
  //  for(int i=0; i<msg.arguments().length; i+=numberOfParamsPerMessage)
  //  {
  //    float id = msg.get(i).floatValue(); 
  //    float x =  msg.get(i+1).floatValue(); 
  //    float y =  msg.get(i+2).floatValue();
  //    //your built-in sanity check. 
  //    println(id+"," +"x" +"'"+ y);
  //    addToBlobs(id, x, y);
  //  }
  //  println("**************");
  //}
}

//add to blobs or update existing blobs if they are found
void addToBlobs(float id, float x, float y)
{
  int index = blobsID.indexOf(id); //find the current blob via id
  
  if(index == -1) //if the id isn't found, add
  {
    blobsID.add(id);
    blobs.add(new Blob(id, x, y)); 
    newBlobs++;
  }
  else //if it is found, update
  {
    blobs.get(index).update(id, x, y);
  }
}

void drawBlobs()
{
  for(int i=0; i<blobsID.size(); i++)
  {
    blobs.get(i).draw(); 
  }
}

//this will just play a note for every blob that is found.
void playBlobs(float timeBetween)
{
  if(timeBetween>=QUARTER_SECOND) 
  {
     for(int i=0; i<newBlobs; i++)
     {
          float r = random(0, 1); //random pitch from "IndexToPitches" class -- a selection of the A minor key
          out.playNote( 0.0, 0.9, new ToneInstrument( pitches.getPitch(round(r*(pitches.size()-1)) ) , 0.1f ));
     }
     newBlobs=0; //after the notes are played, reset new blobs to 0
  }
}


//this runs once per every frame. it draws blobs and plays notes if there are new blobs
//also deletes old blobs that haven't been updated in a while (killBlobs)
void draw()
{
  background(255); 
  float timeBetween =  millis()-lastPlayed; //time between last time it was played, 
  //basically quantizing everything to a quarter second beat, since I only play every 1/4 second
  //if tempo is 120bpm, then it is playing every 16th note if there are new blobs.
  playBlobs(timeBetween);
  println("Position: "+x+ "|",y);
  println("Down Value: " + downValue);
  println("**************");
  
  drawBlobs();
  ageBlobs();
  killBlobs();
}
