/*

Courtney Brown 
Oct. 2020
THis is a blob class that holds several features. It also contains info to destroy blob if it no longer exists. See aging.
*/

//NOTE: I suggest if you send velocity or other features that you modfiy these class to include them. You could also calculate features here.
//NOTE2: This whole setup assume that blobs will be born and die within a relatively short amt of time. With the current mocap code, this is a reasonable assumption IMO.
class Blob
{
  ArrayList<Float> allX = new ArrayList(); //the history of the x value, incl. current
  ArrayList<Float> allY = new ArrayList(); //the history of the y value, incl. current
  float blobsID = 0; //the id from C++ -- you don't really need this here bc its in a list in the main, BUT good to have for debugging just in case!!
  int blobsAlive = 0; //when was it last updated?
  
  //create with this info.
  Blob(float id, float x, float y)
  {
    update(id,x,y);
  }
  
  //putting in all at once to save time
  //also notes the blob is alive
  void update(float id, float x, float y)
  {
    blobsID = id; 
    allX.add(x); 
    allY.add(y);
    alive();
  }
  
  ArrayList<Float> x()
  {
    return allX; 
  }
  
    ArrayList<Float> y()
  {
    return allY; 
  }
  
  //draw the whole blob history
  void draw()
  {
    stroke(0); 
    fill(255);
    for(int j=0; j<x().size(); j++)
      circle(x().get(j), y().get(j), 50 ); 
  }
  
   //every time update gets called, we take away one from blobsAlive to note that it is a frame older
  //if it doesn't get updated & alive doesn't get called, it will continue to go down until we say
  //hey this blob doesn't exist any more.
  void age()
  {
    blobsAlive--;
  }
  
  //ok, now we check how much its been updated vs. how many frames its been alive
  boolean isDead()
  {
    return blobsAlive < WHEN_A_BLOB_IS_DEAD;
  }
  
  //this means it has been recently updated, so we know it was alive
  void alive()
  {
    blobsAlive=1;
  }
  

  



}
