import processing.serial.*;

Serial myPort;

int SteveX = 500;
int SteveY = 500;
int SteveWidth = 30;
int SteveHeight = 30;

int foodWidth = 10;
int foodHeight = 10;
float foodX = random(1000 - foodWidth);
float foodY = random(1000 - foodHeight);

int inputVal = 0;
int speed = 5;
int score = 0;

boolean resetHeld = false;

color bgcolor = color(255, 204, 100);

void setup() {
  size(1000, 1000);
  strokeWeight(4);

  myPort = new Serial(this, "/dev/ttyACM0", 9600);
}

void draw() {
  background(bgcolor);

  if (myPort.available() > 0) {
    inputVal = myPort.read();
  }

  // Reset once per button press
  if (inputVal == 5 && !resetHeld) {
    resetGame();
  }
  resetHeld = (inputVal == 5);

  if (inputVal == 1) {
    SteveX -= speed;
  }
  else if (inputVal == 2) {
    SteveY -= speed;
  }
  else if (inputVal == 3) {
    SteveX += speed;
  }
  else if (inputVal == 4) {
    SteveY += speed;
  }

  // Keep Steve inside the game window
  SteveX = constrain(SteveX, 0, width - SteveWidth);
  SteveY = constrain(SteveY, 0, height - SteveHeight);

  // Draw Steve
  fill(255);
  rect(SteveX, SteveY, SteveWidth, SteveHeight);

  // Draw food
  fill(255, 0, 0);
  rect(foodX, foodY, foodWidth, foodHeight);

  // Check whether Steve eats the food
  if ((foodX > SteveX - foodWidth) &&
      (foodX < SteveX + SteveWidth) &&
      (foodY < SteveY + SteveHeight) &&
      (foodY > SteveY - foodHeight)) {

    score++;
    foodX = random(width - foodWidth);
    foodY = random(height - foodHeight);
  }

  // Display score
  fill(0);
  textSize(28);
  text("Score: " + score, 20, 35);
}

void resetGame() {
  score = 0;
  SteveX = width / 2;
  SteveY = height / 2;
  foodX = random(width - foodWidth);
  foodY = random(height - foodHeight);
}
