import processing.serial.*; //import serial reading
import de.voidplus.leapmotion.*;

Serial myPort;        // The serial port
float distance = 0;  //current distance reading.
float temp = 0;      //current temp reading.
float soundSpeed = 0; //current sound speed.
float rotate = 0;  //rotation of the needle image
float degree = PI/2; //current degree
int direction = 2; //direction of the angle rotation.
int mode = 0; //mode for the controller

PImage up; //up arrow image
PImage down; //down arrow image
PImage left; //left arrow image
PImage right; //right arrow image
PImage background; //background image
PImage speed; //speedometer image
PImage needle; //needle image

PFont futura; //Futura font
PFont xeno; //Xenotron font

int upUse = 0; //up direction boolean
int downUse = 0; //down direction boolean
int leftUse = 0; //left direction boolean
int rightUse = 0; //right direction boolean
int gear = 1;

LeapMotion leap;
void setup () {
  // set the window size:
  size(1800, 600);
  
  //Load the images
  up = loadImage("arrowUp.png");
  down = loadImage("arrowDown.png");
  left = loadImage("arrowLeft.png");
  right = loadImage("arrowRight.png");
  background = loadImage("background.jpg");
  speed = loadImage("speed.png");
  needle = loadImage("needle.png");

  //load the fonts
  futura = createFont("Futura Light Regular.otf", 32);
  xeno = createFont("Xenotron.ttf", 32);

  // Open the Serial port.
  myPort = new Serial(this, "COM8", 9600);  //Use COM5

  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  // set initial background and graph plots:
  image(background, 0, 0, width, height);
  drawGrid();

  strokeWeight(3);
  delay(500);

  leap = new LeapMotion(this).allowGestures();  // All gestures  
  textFont(futura);
}

//Handles circle gestures
void leapOnCircleGesture(CircleGesture g, int state) {
  int     direction        = g.getDirection();

  if (mode == 1) {
    switch(state) {
    case 1: // Start
      break;
    case 2: // Update
      break;
    case 3: // Stop
      switch(direction) {
      case 0: // Anticlockwise/Left gesture
        leftUse = 1;
        rightUse = 0;
        break;
      case 1: // Clockwise/Right gesture
        rightUse = 1;
        leftUse = 0;
        break;
      }
      break;
    }
  }
}


// Handles keytap cases
void leapOnKeyTapGesture(KeyTapGesture g) {
  if (mode == 1) {
    if (upUse == 1 || downUse == 1) {
      if (rightUse == 1) {
        rightUse = 0;
      } else if (leftUse == 1) {
        leftUse = 0;
      } else {
        upUse = 0;
        downUse = 0;
      }
    } else if (rightUse == 1) {
      rightUse = 0;
    } else if (leftUse == 1) {
      leftUse = 0;
    } else {
      upUse = 1;
    }
  }
}

//Draw the radar plot
void drawGrid() {
  //Draw the RPM meter and the needle
  image(speed, 150, 200);
  pushMatrix();
  imageMode(CORNER);
  translate(540, height);
  rotate(PI + rotate);
  image(needle, -needle.width/2, -needle.height/2);
  popMatrix();
  imageMode(CORNER);
  strokeWeight(1);
  textSize(14);
  fill(255, 255, 0);
  strokeWeight(2);

  //Draw the axis markings in RPM
  for (int i = 0; i < 9; i++) {
    fill(255, 255, 255);
    textSize(30);
    text(((i* 100 - 900) * -1), width / 3.3 + 425 * cos(-1 * i * PI/9 - PI/20) - 20, height + 418 * sin(-1 * i * PI/9 - PI / 20));
  }
  strokeWeight(1);
}

//Redraw the canvas
void draw () {
  String txtIns = ""; //Current instruction for gesture controls
  image(background, 0, 0, width, height); //Redraw the background
  
  //Redraw the UI
  title();
  drawGrid();
  legend();
  
  
  //System.out.println(upUse + "" + downUse + "" + leftUse + "" + rightUse + "" + gear);
  myPort.write((int)(upUse * 10000 + downUse * 1000 + leftUse * 100 + rightUse * 10)); //Send the instruction to the Arduino
  txtIns = determineIns(); //Find the instruction
  
  //Draw the current instruction and the camera if in gesture controls
  if (mode == 1) {
    text(txtIns, width / 1.42, height / 1.15);
  }
  if (mode == 1) {
    fill(255, 255, 255, 150);
    stroke(255, 255, 255, 150);
    rect(990, 190, 660, 260);
    
    //Draw the camera image if leapMotion is connected
    if (leap.hasImages()) {
      for (Image camera : leap.getImages()) {
        if (camera.isLeft()) {
          // Left camera
          image(camera, 1000, 200);
        } else {
        }
      }
    }
  }
  delay(1);
}

//Determine which instruction is in use depending on direction booleans 
String determineIns () {
  String ins = "";
  if (upUse == 1) {
    if (leftUse == 1) {
      ins = "Forward-Left";
    } else if (rightUse == 1) {
      ins = "Forward-Right";
    } else {
      ins = "Forward";
    }
  } else if (downUse == 1) {
    if (leftUse == 1) {
      ins = "Backward-Left";
    } else if (rightUse == 1) {
      ins = "Backward-Right";
    } else {
      ins = "Forward";
    }
  } else if (rightUse == 1) {
    ins = "Rotate Right";
  } else if (leftUse == 1) {
    ins = "Rotate Left";
  } else {
    ins = "OFF";
  }
  return ins;
}

//Detect for pressed keys
void keyPressed() {
  if (key == CODED) {
  } else {
    
    //If in keyboard controls, parse input from the WASD keys
    if (mode == 0) {
      if (key == 'w') {
        upUse = 1;
        if (downUse == 1) {
          downUse = 0;
        }
      }
      if (key == 's') {
        downUse = 1;
        if (upUse == 1) {
          upUse = 0;
        }
      }
      if (key == 'a') {
        leftUse = 1;
        if (rightUse == 1) {
          rightUse = 0;
        }
      }
      if (key == 'd') {
        rightUse = 1;
        if (leftUse == 1) {
          leftUse = 0;
        }
      }
    }
    
    //Press 5 to change to keyboard controls
    if (key == '5') {
      mode = 0;
      upUse = 0;
      downUse = 0;
      leftUse = 0;
      rightUse = 0;
    }
    
    //Press 6 to change to gesture controls
    if (key == '6') {
      mode = 1;
      upUse = 0;
      downUse = 0;
      leftUse = 0;
      rightUse = 0;
    }
  }
}

//Detect for any released keys and stop the rotation
void keyReleased() {
  direction = 2;
  if (key == 'w') {
    upUse = 0;
  }
  if (key == 's') {
    downUse = 0;
  }
  if (key == 'a') {
    leftUse = 0;
  }
  if (key == 'd') {
    rightUse = 0;
  }
}


//Parse the data coming in from the serial port.
void serialEvent (Serial myPort) {
  // get the ASCII string:
  try {
    String inString = myPort.readStringUntil('\n'); //Read the values from the sensors, which are split with a space
    if (inString != null) {
      //Receive RPM from the Arduino and cast to a float
      rotate = float(inString);
      System.out.println(rotate);
    }
    //Calculate how much to rotate the needle image by
    rotate = map(rotate, 0, 12000, 0, PI);
    
    //Catch any non-string inputs from the Arduino.
  } 
  catch (Exception e) {
  }
}

//Draw the legend.
void legend() {
  noFill();
  strokeWeight(2);
  stroke(255, 255, 255, 255);
  noFill();
  
  //If in keyboard controls, draw the arrows
  if (mode == 0) {
    if (upUse == 1) {
      image(up, 1260, 150, 130, 130);
    } else {
      image(up, 1250, 140, 150, 150);
    }
    if (downUse == 1) {
      image(down, 1260, 430, 130, 130);
    } else {
      image(down, 1250, 420, 150, 150);
    }
    if (rightUse == 1) {
      image(right, 1460, 290, 130, 130);
    } else {
      image(right, 1450, 280, 150, 150);
    }
    if (leftUse == 1) {
      image(left, 1060, 290, 130, 130);
    } else {
      image(left, 1050, 280, 150, 150);
    }
  } else {
    textSize(20);
    fill(255, 255, 255);

    text("GESTURE DRIVING ENABLED", width * 0.67, height * 0.16);
  }
}

//Draw the title
void title() {
  textSize(26); 
  fill(255, 255, 255);
  textFont(xeno);
  text("REMOTE CONTROLLER", width * 0.4, height * 0.08);
  textFont(futura);
}