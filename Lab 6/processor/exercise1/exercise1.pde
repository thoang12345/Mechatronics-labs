import processing.serial.*;
Serial myPort;
PImage bg;

void setup() {
  size(2300, 1533);

  bg = loadImage("https://daylightcompany.com/cdn/shop/files/U15800-US-DaylightBulb_blackBG_Low_Res.jpg?v=1772707449");

  myPort = new Serial(this, "/dev/ttyACM0", 9600);
}

void draw() {
  background(bg);

  if (mousePressed && mouseButton == LEFT) {
    myPort.write("1");
  }

  if (mousePressed && mouseButton == RIGHT) {
    myPort.write("0");
  }
}
