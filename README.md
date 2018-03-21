# Autonomous Robot

This is a repository containing the code for an autonomous robot, capable of three primary functions:
  -  The robot moves at maximum speed straight forward until it detects an object < 50cm away, in which it gradually slows down and continues to approach the object, stopping right before it can hit the object.
  -  The robot uses reflective optical sensors to follow a track on the ground, made of black electrical tape.
  -  The robot is capable of being wirelessly (Bluetooth) controlled through a software controller, which incorporates LeapMotion and keyboard for movement controls. 

Primarily, the hardware is developed in Arduino, and the software controller is developed through Processing 3 (Java).
