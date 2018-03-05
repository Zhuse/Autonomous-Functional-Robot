  //import processing.serial.*; //import serial reading
  import de.voidplus.leapmotion.*;

 // Serial myPort;        // The serial port
  float distance = 0;  //current distance reading.
  float temp = 0;      //current temp reading.
  float soundSpeed = 0; //current sound speed.
  float degree = PI/2; //current degree
  int time = 0;           //time labels for the x axis
  int direction = 2; //direction of the angle rotation.
  int mode = 0; //mode for the scale
  int currentDir = 0; //current direction
  int xPos = 1105; //x-beginning of the graph
  ArrayList<Line> lines = new ArrayList<Line>(); //contains previous lines for the radar plot
  ArrayList<Line> plot = new ArrayList<Line>(); //contains previous lines for the graph
  
  PImage up;
  PImage down;;
  PImage left;
  PImage right;
  PImage upU;
  PImage downU;
  PImage leftU;
  PImage rightU;
  PImage background;
  
  PFont futura;
  PFont xeno;
  
  int upUse = 0;
  int downUse = 0;
  int leftUse = 0;
  int rightUse = 0;
  int gear = 1;
  
  LeapMotion leap;
  
  //Object to store a line object's deltaX,deltaY,opacity,angle,distance, and x position values.
  //These objects are used for redrawing past lines.
  class Line{
    public float xPos = 0;
    public float distance = 0;
    public float x = 0;
    public float y = 0;
    public float opacity = 0;
    public float angle = 0;
    
    //Constructor
    Line(float x, float y, float opacity, float angle, float distance, float xPos){
      this.x = x;
      this.y = y;
      this.opacity = opacity;
      this.angle = angle;
      this.distance = distance;
      this.xPos = xPos;
    }
  }
  void setup () {
    // set the window size:
    size(1800, 600);
    up = loadImage("arrowUp.png");
    down = loadImage("arrowDown.png");
    left = loadImage("arrowLeft.png");
    right = loadImage("arrowRight.png");
    background = loadImage("background.jpg");
    
    futura = createFont("Futura Light Regular.otf", 32);
    xeno = createFont("Xenotron.ttf", 32);
    
    // Open the Serial port.
   // myPort = new Serial(this, "COM5", 9600);  //Use COM5

    // don't generate a serialEvent() unless you get a newline character:
    //myPort.bufferUntil('\n');
    for(int i = 0; i < 181; i++){
      Line l = new Line(0,0,0,0,0,0);
      lines.add(l);
    }
    // set initial background and graph plots:
    image(background, 0, 0, width, height);
    drawGrid();
    
    strokeWeight(3);
    
    //draw the rotation line at the default position (PI/2)
    line(width / 3.3, height, width / 3.3 + 400 * cos(-1 * degree), height + 400 * sin(-1 * degree));
    delay(500);
    
    leap = new LeapMotion(this).allowGestures();  // All gestures  
    textFont(futura);
  }
  
  void leapOnSwipeGesture(SwipeGesture g, int state){
  PVector direction        = g.getDirection();

  if(mode == 1){
  switch(state){
    case 1: // Start
      break;
    case 2: // Update
      break;
    case 3: // Stop
      if(direction.x > 0){
        if(gear < 4){
          gear++;
        }
      }
      if(direction.x <= 0){
        if(gear > 0){
          gear--;
        }
      }
      break;
  }
  }
  }
  void leapOnCircleGesture(CircleGesture g, int state){
  int     direction        = g.getDirection();

  if(mode == 1){
  switch(state){
    case 1: // Start
      break;
    case 2: // Update
      break;
    case 3: // Stop
      switch(direction){
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

  // ======================================================
// 3. Screen Tap Gesture

void leapOnScreenTapGesture(ScreenTapGesture g){
  
  if(mode == 1){
    if(upUse == 1 || downUse == 1){
      upUse = 0;
      downUse = 0;
    } else {
      upUse = 1;
    }
  }
}


// ======================================================
// 4. Key Tap Gesture

void leapOnKeyTapGesture(KeyTapGesture g){
  if(mode == 1){
    if(upUse == 1 || downUse == 1){
      upUse = 0;
      downUse = 0;
    } else {
      upUse = 1;
    }
  }
}
  //Draw the radar plot
  void drawGrid(){
    String distanceMark = ""; //labels for the distance markings
    int w = (int)(width / 3.3); //x position of the plot
    int h = height; //y position of the plot
    legend(); //draw the legend
    
    //Draw the radar lines.
    radarLine(w, h, 400, 4);
    strokeWeight(1);
    radarLine(w, h, 350, 1);
    radarLine(w, h, 300, 1);
    radarLine(w, h, 250, 1);
    radarLine(w, h, 200, 1);
    radarLine(w, h, 150, 1);
    radarLine(w, h, 100, 1);
    radarLine(w, h, 50, 1);
    textSize(14);
    fill(255,255,0);
    
    //Draw the labels for the distance markings depending on the mode
    for(int i = 8; i > 0; i--){
        distanceMark = 50*i + ""; 
      time++;
      text(distanceMark, width / 3.3 - 3, (8-i) * 50 + height / 2.78);
    }
    strokeWeight(2);
    
    //Draw the error bound lines
    for(int i = 0; i <= 6; i++){
      fill(75,160,255);
      line(width / 3.3 + 400 * cos(-1 * i * PI/6), height + 400 * sin(-1 * i * PI/6), width / 3.3 + 410 * cos(-1 * i * PI/6), height + 410 * sin(-1 * i * PI/6));
      text(i* 30 + "°", width / 3.3 + 425 * cos(-1 * i * PI/6) - 10, height + 418 * sin(-1 * i * PI/6));
    }
    strokeWeight(1);
  }
  
  //Redraw the canvas
  void draw () {
    int counter = 0; //index for line objects
    float x; 
    float y;
    float opacity;
    float angle;
    
    //Avoid out of bounds error
    if(degree >= PI){
      degree = PI;
    } else if(degree <= 0){
      degree = 0;
    } 
    //Rotate depending on direction.
    if(direction == 0){
      degree += (PI / 180);
    } else if (direction == 1){
      degree = degree - PI / 180;
    }
    
    //If a button is pressed, rotate the "zone" and redraw the canvas with previous lines
    if(direction != 2){
      image(background, 0, 0, width, height);
      drawGrid();
      strokeWeight(3);
      stroke(255,0,0,255);
      strokeWeight(10);
      counter = (int)(degree * 180 / PI);
      if(counter >= 179){
        counter = 179;
        direction = 2;
      } 
      if(counter <= 0){
        counter = 0;
        direction = 2;
      }
      title();
      //Redraw previous lines with reduced opacity
      lines.get(counter).distance = distance;
      lines.get(counter).x = distance * cos(-1 * degree) + width / 3.3;
      lines.get(counter).y = distance * sin(-1 * degree) + height;
      lines.get(counter).opacity = 255;
      lines.get(counter).angle = degree;
      for(int i = 1; i < 180; i++){
        x = lines.get(i).x;
        y = lines.get(i).y;
        angle = lines.get(i).angle;
        //System.out.println(x*x + y*y + "");
        lines.get(i).opacity -= 10;
        opacity = lines.get(i).opacity;
        stroke(255,0,0,opacity);
        line(x, y, width / 3.3 + 397 * cos(-1 * angle), height + 397 * sin(-1 * angle));
        stroke(255,0,0,opacity);
      }
      
      //Redraw error bound lines
      strokeWeight(1);
      line(width / 3.3, height, width / 3.3 + 397 * cos(-1 * degree), height + 397 * sin(-1 * degree));
      stroke(255,128,128,255);
      line(width / 3.3, height, width / 3.3 + 397 * cos(-1 * degree + PI/12), height + 397 * sin(-1 * degree + PI/12));
      line(width / 3.3, height, width / 3.3 + 397 * cos(-1 * degree - PI/12), height + 397 * sin(-1 * degree - PI/12));
      //myPort.write(direction); //Send the current direction of rotation to the Arduino for motor movement
      
       //If no button is pressed, do not rotate the "zone" and continue redrawing the canvas
    } else if (direction == 2){
      image(background, 0, 0, width, height);
      drawGrid();
      title();
      xAxis();
      yAxis();
      stroke(255,0,0,255);
      strokeWeight(10);
      
      //Redraw the previous lines with a reduced opacity
      for(int i = 1; i < 180; i++){
        x = lines.get(i).x;
        y = lines.get(i).y;
        angle = lines.get(i).angle;
        
        //If the distance error is out of scale, bring it back in.
        if(mode == 0 && x*x + y*y > 16000000){
          x = 400 * cos(angle);
          y = 400 * sin(angle);
        }
        lines.get(i).opacity -= 10;
        opacity = lines.get(i).opacity;
        stroke(255,0,0,opacity);
        
        //Draw the lines on the plot and graph
        line(x, y, width / 3.3 + 397 * cos(-1 * angle), height + 397 * sin(-1 * angle));
      }
      
      //Redraw the "zone" and the error bound lines
      strokeWeight(3);
      line(width / 3.3, height, width / 3.3 + 397 * cos(-1 * degree), height + 397 * sin(-1 * degree));
      strokeWeight(0.5);
      stroke(255,128,128,255);
      line(width / 3.3, height, width / 3.3 + 397 * cos(-1 * degree + PI/12), height + 397 * sin(-1 * degree + PI/12));
      line(width / 3.3, height, width / 3.3 + 397 * cos(-1 * degree - PI/12), height + 397 * sin(-1 * degree - PI/12));
      strokeWeight(1);
    }
    stroke(154,205,50);
    strokeWeight(10);
    
    //Continue drawing the lines on the plot.
    for(int i = 0;i < plot.size(); i++){
      line(plot.get(i).xPos,height - 60, plot.get(i).xPos, height - 60 - plot.get(i).distance);
    }
    
    //Clear the graph if the x position of the drawn lines has exceeded the end of the graph.
    if(xPos >= 1600){
      xPos = 1105;
      plot.clear();
    }
      System.out.println(upUse + "" + downUse + "" + leftUse + "" + rightUse + "" + gear);
      if(upUse == 1){
        //myPort.write("up");
      }
      if(mode == 1){
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
  
  //Detect for pressed keys
  void keyPressed() {
    if (key == CODED) {
      
      //Change the direction of rotation to counter-clockwise
      if (keyCode == LEFT) {
        currentDir = 0;
        direction = 0;
        
        //Change the direction of rotation to clockwise
      } else if (keyCode == RIGHT) {
        currentDir = 1;
        direction = 1;
        
        //Change the scale to 0 - 400 cm
      } 
    } else{
      if(mode == 0){
      if(key == 'w'){
        upUse = 1;
        if(downUse == 1){
          downUse = 0;
        }
      }
      if(key == 's'){
        downUse = 1;
        if(upUse == 1){
          upUse = 0;
        }
      }
      if(key == 'a'){
        leftUse = 1;
        if(rightUse == 1){
          rightUse = 0;
        }
      }
      if(key == 'd'){
        rightUse = 1;
        if(leftUse == 1){
          leftUse = 0;
        }
      }
      if(key == '1'){
        gear = 1;
      }
      if(key == '2'){
        gear = 2;
      }
      if(key == '3'){
        gear = 3;
      }
      if(key == '4'){
        gear = 4;
      }
      }
      if(key == '5'){
        mode = 0;
        upUse = 0;
        downUse = 0;
        leftUse = 0;
        rightUse = 0;
      }
      if(key == '6'){
        mode = 1;
        upUse = 0;
        downUse = 0;
        leftUse = 0;
        rightUse = 0;
      }
    }
    
    //Clear the lines if the direction is changed.
    if(direction != currentDir)
      reset(lines);
  }
  
  //Set all of an array's line objects to have a value of 0 for all their values
  void reset(ArrayList<Line> lines){
    for(int i = 0;i < 180; i++){
      lines.get(i).x = 0;
      lines.get(i).y = 0;
      lines.get(i).opacity = 0;
      lines.get(i).angle = 0;
    }
  }
  
  //Detect for any released keys and stop the rotation
  void keyReleased() {
    direction = 2;
    if(key == 'w'){
        upUse = 0;
      }
      if(key == 's'){
        downUse = 0;
      }
      if(key == 'a'){
        leftUse = 0;
      }
      if(key == 'd'){
        rightUse = 0;
      }
  }
  
  
  //Parse the data coming in from the serial port.
  /*void serialEvent (Serial myPort) {
    // get the ASCII string:
    try {
    String inString = myPort.readStringUntil('\n'); //Read the values from the sensors, which are split with a space
    if (inString != null) {
      inString = trim(inString); //remove whitespace from the end of the string
      float values[] = float(split(inString, " ")); //Split the string into sensor values
   
      //Parse the data
      distance = values[0];
      temp = values[1];
      soundSpeed = values[2];
      
      //Change the mapping scale depending on the mode.
      if(mode == 0){
        distance = map(distance, 0, 40, 0, 400);
      } if (mode == 1){
        distance = map(distance, 0, 400, 0, 400);
      }
    }
    
    //Catch any non-string inputs from the Arduino.
    } catch (Exception e){
    }
  }*/
  
  //Draw a green circle for the radar plot.
  void radarLine(int w, int h, int radius, int thickness){
    noFill();
    strokeWeight(thickness);
    stroke(255,255,255);
    ellipse(w, h, 2*radius, 2*radius);
  }
  
  //Draw the legend.
  void legend(){
    noFill();
    strokeWeight(2);
    stroke(255,255,255,255);
    rect(width*0.01, height * 0.05, 250, 100); //draw the rectangle box
    textSize(15); 
    
    //Fill the legend with the temperature value, speed of sound value, distance value and the scale
    fill(255, 255, 255);
    text("Temperature(Celsius): " + temp, width * 0.015, height * 0.1);
    text("Speed of Sound(m/s): " + soundSpeed, width * 0.015, height * 0.15);
    text("Current Distance(cm): " + distance, width * 0.015, height * 0.2);
    noFill();
    if(mode == 0){
    if(upUse == 1){
      image(up, 1260,150,130,130);
    } else {
      image(up, 1250,140,150,150);
    }
    if(downUse == 1){
      image(down, 1260,430,130,130);
    } else {
      image(down, 1250,420,150,150);
    }
    if(rightUse == 1){
      image(right, 1460,290,130,130);
    } else {
      image(right, 1450,280,150,150);
    }
    if(leftUse == 1){
      image(left, 1060,290,130,130);
    } else {
      image(left, 1050,280,150,150);
    }
    } else {
      textSize(20);
      fill(255, 255, 255);
      
      text("GESTURE DRIVING ENABLED", width * 0.67, height * 0.16);
    }
  }
  
  //Draw the title
  void title(){
    textSize(26); 
    fill(255, 255, 255);
    textFont(xeno);
    text("REMOTE CONTROLLER", width * 0.4, height * 0.08);
    textFont(futura);
  }
  
  //Draw the x-axis
  void xAxis(){
    
  }
  
  //Draw the y-axises
  void yAxis(){
   
  }