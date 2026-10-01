import processing.serial.*;

Serial myPort;
PImage logo;
int bgcolor = 0;

void setup() {
  size(2400, 2401);
  colorMode(HSB, 255);

  logo = loadImage("https://cdn.freebiesupply.com/logos/large/2x/arduino-1-logo-png-transparent.png");

  myPort = new Serial(this, "/dev/ttyACM0", 9600);
}

void draw() {
  if (myPort.available() > 0) {
    bgcolor = myPort.read();
    println(bgcolor);
  }

  background(bgcolor, 255, 255);
  image(logo, 50, 50, 100, 100);
}
