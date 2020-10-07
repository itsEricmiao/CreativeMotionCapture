//Programmer: Courtney Brown
//Purpose: just maps the index (eg. the y value) to the rhythm

import java.util.ArrayList;

class IndexToPitches
{
  ArrayList<Float> hz = new ArrayList(); 
  
  IndexToPitches()
  {
    hz.add(Frequency.ofPitch( "C4" ).asHz() );
    hz.add(Frequency.ofPitch( "B3" ).asHz() );
    hz.add(Frequency.ofPitch( "A3" ).asHz() );
    hz.add(Frequency.ofPitch( "G3" ).asHz() );
    hz.add(Frequency.ofPitch( "F3" ).asHz() );
    hz.add(Frequency.ofPitch( "E3" ).asHz() );
    hz.add(Frequency.ofPitch( "D3" ).asHz() );
    hz.add(Frequency.ofPitch( "C3" ).asHz() );
    hz.add(Frequency.ofPitch( "B2" ).asHz() );
    hz.add(Frequency.ofPitch( "A2" ).asHz() );
    hz.add(Frequency.ofPitch( "G2" ).asHz() );
    hz.add(Frequency.ofPitch( "F2" ).asHz() );
    hz.add(Frequency.ofPitch( "E2" ).asHz() );
    hz.add(Frequency.ofPitch( "D2" ).asHz() );
    hz.add(Frequency.ofPitch( "C2" ).asHz() );
  }
  
  float size()
  {
    return (float) hz.size(); 
  }
  
  float getPitch(int index)
  {
    if(index>=0 && index < hz.size())
      return hz.get(index).floatValue();
     else return hz.get(0).floatValue(); //silent fail oh well
  }
  
}
