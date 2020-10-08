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
ArrayList<Integer> count = new ArrayList<Integer>();
//where the mouse is on the screen in %, that is 0.-0.1
float x; 
float y; 



//the OSC addresses we are receiving
//MUST match what we are sending
final String DOWN_OSC_ADDRESS = "/MakeItArt/Down"; //is the mouse down? 0-no, 1-yes
final String WHERE_OSCADDRESS = "/MakeItArt/Where"; //where is the mouse down? x & y in 0.0-1.0 values
final String PIXELCOUNT_OSCADDRESS = "/MakeItArt/Pixel";
int N = 10;
int normalizedValue = 255;
//The port we are listening to... MUST match DESTPORT in the C++ example 
final int LISTENING_PORT = 8888;

  
  
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
  else if(addr.equals(PIXELCOUNT_OSCADDRESS)) //otherwise it indicates position, so set those
  {
      for(int i = 0; i < N*N; i++){
         count.add(msg.get(i).intValue());
       }
       
  }
  //println("downValue:" + downValue + " x:" + x + " y:" + y );
  //print the values
  
}

void draw()
{
//nothing here yet folks.
  textSize(12);
  fill(0, 102, 153, 51);
  text("Press a, b ,c to chnage the normalization value and transparency", 10, 60);
  text("Press d to activate color mode", 10, 90); 
  text("Press r to reset the program", 10, 120); 
  frameRate(30);
  
  display();
  count.clear();
  reset();
  
  
}

void reset(){
  setup();
  // Doesn't do anything yet.
}

void display(){
  int squareHeight = height/N;
  int squareWidth = width/N;
  // if pressed 'r', activate reset and clear the canva
  if(key == 'r'){
   reset();
   return;
  }
  
  for(int i = 0; i < count.size(); i++){
    int val = count.get(i);
    if(count.get(i) > 100 && count.get(i) < 8000){
      int row = i / N;
      int col = i % N;
      int x = row*squareWidth;
      int y = col*squareHeight;
      noStroke();
      int c =  int(val) % int(normalizedValue);
      int transpancy = val % 210;
      //println("color: "+ c + " val: "+ val + " normalized: " + normalizedValue );
      fill(c,c,c,transpancy);
      
      // if pressed 'd', show color
      if(key == 'd'){
        println("key d is pressed");
        fill(random(255),random(255),random(255),random(20));
      }
      rect(x, y, squareWidth, squareHeight);
    }
  }
}


// pressing different keys will change the normalization value
void keyPressed(){
  if (key == 'a'){
    println("key a is pressed");
    normalizedValue = 100;
  }
  if (key == 'b'){
    println("key b is pressed");
    normalizedValue = 150;
  }
  if (key == 'c'){
    println("key c is pressed");
    normalizedValue = 250;
  }
}
