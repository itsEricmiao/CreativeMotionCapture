//Programmer: Eric Miao
//Date: Oct. 2020


/*I got inspired by the intro of Star Wars movies. 
  I decided to visualize all the blobs in a creative way. 
  The position each ‘star’ generated is based on the x, y positions of the blobs,but also I normalized both x,y values to help the visualization. 
  Besides, I set the color and transparency based on the speed of the blobs. It helps create a visual effect of twinkling. 
  In addition, the program uses the snap value to determine the rotation direction of the blobs. 
  If you quickly swing you arm, the program will capture that movement and send a signal to change the direction of the rotation. 
*/
import netP5.*;
import oscP5.*;
import ddf.minim.*;
import ddf.minim.ugens.*;
import processing.sound.*;
SoundFile file;

float speed = 0;
int mouse = 0;
int number_blobs = 0; // keep on track of the number of blobs on the screen
boolean direction = true;



Minim minim; //the minimum object
AudioOutput out; //our audio out
IndexToPitches pitches = new IndexToPitches(); //an array list of the hertz of pitches from C2-C4

OscP5 oscP5; //the object that will send OSC
NetAddress remoteLocation; //where we are receiving OSC from

float downValue; //is the mouse down?
int snap;

//where the mouse is on the screen in %, that is 0.-1.0
float x; 
float y; 
int size;

// define some color I will be using in the program
color c1 = color(204, 102, 0);
color c2 = color(204, 153, 0);
color c3 = color(153, 51, 0);
color c4 = color(153, 51, 0);

//the OSC addresses we are receiving
//MUST match what we are sending
final String DOWN_OSC_ADDRESS = "/MakeItArt/Down"; //is the mouse down? 0-no, 1-yes
final String WHERE_OSC_ADDRESS = "/MakeItArt/Where"; //where is the mouse down? x & y in 0.0-1.0 values
final String BLOB_OSC_ADDRESS = "/MakeItArt/Blobs"; 
final String SIZE_OSC_ADDRESS = "/MakeItArt/Size"; 
final String PREVB_OSC_ADDRESS = "/MakeItArt/PrevBlobs"; 
final String MAP_OSC_ADDRESS = "/MakeItArt/Map";
final String SPEED_OSC_ADDRESS = "/MakeItArt/Speed";
final String SNAP_OSC_ADDRESS = "/MakeItArt/Snap";
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
ArrayList<Blob> prevBlobs = new ArrayList();
ArrayList<Integer> map = new ArrayList();
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
    for(int i=prevBlobs.size()-1; i>=0; i--)
    {             
         prevBlobs.remove(i);    
    }
    
        for(int i=map.size()-1; i>=0; i--)
    {             
         map.remove(i);    
    }
}

//sets up OSC & the sound library Minim
void setup()
{
  size(640, 480); //NOTE: I made the size of the screen match the size of the C++ screen so I don't calculate anything extra. 
  fill(0); 
  smooth(10);
  file = new SoundFile(this, "music.mp3");
  file.play();
  //initialize OSC
  oscP5 = new OscP5(this, LISTENING_PORT); //listening for incoming!!
 
}

//this is the overloaded function to receive our OSC message 
void oscEvent(OscMessage msg)
{
  String addr = msg.addrPattern(); //get the address
  
  if(addr.equals(PREVB_OSC_ADDRESS)){
     float prev_X = msg.get(0).floatValue(); 
     float prev_Y = msg.get(1).floatValue(); 
     Blob b = new Blob(0,prev_X, prev_Y);
     prevBlobs.add(b);
  }
  
  else if(addr.equals(SIZE_OSC_ADDRESS)){
    size = msg.get(0).intValue();
  }
  
  else if(addr.equals(SNAP_OSC_ADDRESS)){
    snap = msg.get(0).intValue();
    println("snap = ", snap);
  }
  
  else if(addr.equals(MAP_OSC_ADDRESS)){
    for(int i=0; i<msg.arguments().length; i++){
       map.add(msg.get(i).intValue());
     }
  }
  else if(addr.equals(SPEED_OSC_ADDRESS)){
    speed = msg.get(0).floatValue();
  }
  
  else if(addr.equals(BLOB_OSC_ADDRESS) )
  {
    //for(int i=0; i<msg.arguments().length; i+=numberOfParamsPerMessage)
    //{
      float id = msg.get(0).floatValue(); 
      float x =  msg.get(1).floatValue(); 
      float y =  msg.get(2).floatValue();
      //your built-in sanity check. 
      addToBlobs(id, x, y);
  }
 
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
  background(51, 0, 0);
  translate(width/2, height/2);
  rotate(radians(number_blobs));
  
  if(mouse == 1) { direction = false; background(c1); fill(c3);}
  if(mouse == 0) { direction = false; background(c2); fill(c1); }
  if(snap == 1) { direction = true;}
  if(direction == true){
      number_blobs++; 
    }
  else{
    number_blobs--;
    }
  noStroke();
  drawBlobs();
  ageBlobs();
  killBlobs();
}

void mousePressed() {
  if (mouse == 0) {
    mouse = 1;
  } else {
    mouse = 0;
  }
}
