import de.voidplus.leapmotion.*;
LeapMotion leap;

void setup(){
  size(800, 500);
  background(255);
  leap = new LeapMotion(this).allowGestures();  //Let leap motion detect all gestures
}

void draw(){
  
}