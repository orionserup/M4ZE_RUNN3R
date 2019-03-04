
/*********************
    Orion Serup
    CS M10A
    Dr. Alnaji
    Moorpark College
    Spring 2019
 *********************/

#include <SPI.h> // comm lib
#include <EEPROM.h> // rom storage
#include <TFT_ILI9163C.h> // disp lib
#include <Adafruit_GFX.h> // part of disp lib

#define up 2
#define down 3   // directional button pins
#define left 4
#define right 5

#define offsetx 4
#define offsety -8

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define stdspeed 50

#define reset -1
//reset pin is the arduino reset pin
const static bool maze1[31][31] =
{
  {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0},
  {1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
  {0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
  {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1},
  {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0},
  {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1},
  {1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1},
  {0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
  {0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1}
}; // maze map in binary from paint

const static bool player[4][4] =
{
  {0, 0, 0, 0},
  {0, 1, 1, 0},
  {0, 1, 1, 0},
  {0, 0, 0, 0}
}; // character looks like a square

bool gameover = false;

uint8_t px, py, playerx, playery;  // all variables previous and current position coords

TFT_ILI9163C disp = TFT_ILI9163C(9, 10); // initiate the disp with dimensions, protocol and reset pin

uint16_t finalscore = 1200, counter = 0; // score subtracts from this 20 times a second

unsigned char char1 = 0 , char2 = 0;  // name chars

bool wall(uint8_t x, uint8_t y);
void drawmenu();
void about();
void drawmap();
void drawplayer(uint8_t x, uint8_t y);
void animate(char axis, uint8_t dir);
void eraseprev();

void setup() { // runs once

  pinMode(up, INPUT);   // the buttons are for reading
  pinMode(down, INPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);

  Serial.begin(9600); // start serial communication
  Serial.println("Serial is to simply give feedback to the computer");  // begin Serial as debugger

  disp.begin();
  disp.setRotation(2);

  disp.clearScreen();
  delay(500);

  drawmenu();

  Serial.print("Menu Ready");

  disp.clearScreen();
  drawmap();   // draw map function
  delay(10 * stdspeed);

  drawplayer(4, 128); // draw player at the bottom left corner
  delay(10);

}

void loop() {  // while(1);

  if (gameover == false) { // while the player is still playing

    if (digitalRead(up)) {
     // if ((playery <= 128) && (playery >= 4)) {
        if (!wall(playerx, playery - 4))
          animate('y', 1);

     }

      if (digitalRead(down)) {
        if ((playery <= 128) && (playery >= 4)) {
          if ((!wall(playerx, playery + 1)) && (!wall(playerx + 3, playery + 1)))
            animate('y', -1);
        }
      }

      if (digitalRead(left)) {
        if ((playerx <= 124) && (playerx >= 0)) {
          if ((!wall(playerx - 1, playery)) && (!wall(playerx - 1, playery + 3)))
            animate('x', -1);
        }
      }

      if (digitalRead(right)) {
        if ((playerx <= 124) && (playerx >= 0)) {
          if ((!wall(playerx + 4, playery)) && (!wall(playerx + 4, playery - 3)))
            animate('x', 1);
        }
      }

      if ( (playerx == 123) && (playery == 3)) {

        disp.setTextSize(2);
        disp.setCursor(20, 20);
        disp.clearScreen();
        delay(10 * stdspeed);
        disp.println("You Won");
        disp.print("NAME: ");

        gameover = true;
      }
    }

    else if (gameover == true) {

      uint8_t chrnum = 0;

      if (digitalRead(right))
        chrnum += 1;
      if (digitalRead(left))
        chrnum -= 1;

      if (chrnum <= 0) {
        if (digitalRead(up))
          char1 += 1;
        else if (digitalRead(down))
          char1 -= 1;
        // add set cursor to overide and update the value of the chars
        disp.print(char1);
      }
      else  if (chrnum == 1) {

        if (digitalRead(up))
          char2 += 1;
        if (digitalRead(down))
          char2 -= 1;

        disp.println(char2);
      }
      else if (chrnum == 2)
        disp.println("Press Right to Confirm");
      else if (chrnum == 3)

        counter++;
      delay(stdspeed);  // speed (lower the delay the faster the game)
      finalscore--; // take away from the score
    }
  }


void drawmap() { // scales the 32 x 16 maze to 128 x 64

  Serial.println("Printing Map");
  disp.clearScreen();

  bool pixel;
  uint8_t counterx, countery;
  uint8_t realx, realy;

  disp.display(false);

  for (countery = 0; countery < 32; countery++) {
    for (counterx = 0; counterx < 31; counterx++) {

      realx = 4 * counterx;
      realy = 4 * countery;

      pixel = maze1[countery][counterx];

      if (pixel == 1) {

        for (int i = 0; i < 4; i++) {
          for (int j = 0; j < 4; j++) {
            disp.drawPixel(realx + i + offsetx, realy - j - offsety, WHITE);
          }
        }
      }
      /* else if (maze1[countery][counterx] == 0) {
         for (uint8_t f = 0; f < 3; f++) {
           for (uint8_t g = 0; g < 3; g++) {

             xx = (4 * counterx) + f + offsetx;
             yy = (4 * countery) - g - offsety;

             disp.drawPixel(xx, yy, BLUE);
             delay(1);
           }
         }
        }
      */
    }
  }
  disp.display(true);
}

bool wall(uint8_t x, uint8_t y) { //if there is a block in the way return true
  x = x / 4 ;
  y = y / 4;
  if (maze1[y][x])
    return true;
  else
    return false;
}


void drawplayer(uint8_t x, uint8_t y) {  //draws the player at a point with (x,y)

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (player[i][j] == 1)
        disp.drawPixel(x + i, y - j, RED);
    }
  }

  px = playerx; // save pos as old
  py = playery;

  playerx = x;  // update position
  playery = y;

  delay(5); // wait
  // update screen
}

/*
  void eraseprev() { // clears the trail that occurs
   for (uint8_t i = 0; i < 4; i++) {
     for (uint8_t j = 0; j < 4; j++) {
       disp.drawPixel(px + i, py - j, 0); // prints black where the charcter was
     }
   }
   delay(5); // wait
   // update screen
  }
*/

void animate(char axis, uint8_t dir) { // move character 
  if ((playerx < 124) && (playerx > 0)) { // bounds check
      px = playerx; // saves position as previous position
      // deletes the player
      delay(5);
      drawplayer(playerx + dir , playery); // draws player 1 away
      playerx = playerx + dir; // saves position as new position
      delay(5);
      // disp changes
    }
  else {// bounds check
      py = playery;  //saves position as prev
      // erases character
      delay(stdspeed / 10);
      drawplayer(playerx, playery - 1 * dir); // moves character
      playery = playery - dir;  // saves new position
      delay(stdspeed / 5);
    
  }
}

void drawmenu() { // menu
  disp.clearScreen(); // clear screen
  disp.cp437(true);   // enables all characters
  disp.setTextSize(1); // makes bigger text
  disp.setCursor(4, 3);  // move in and down 20
  disp.println("M4Z3 RUNN3R");  // title  // set text back to normal
  disp.println();  // new line
  disp.println("Press Up for Game"); // prompt game
  disp.println("Press Down for About"); // go to about
  delay(500); // wait a half second

  while (1) { // loop
    delay(stdspeed); // wait
    if (digitalRead(down)) // pushing down?
      about();             // if so , go to about screen
    else if (digitalRead(up))  // pushing up?
      break;                  // leave loop
  }
  return; // leave
}

void about() {    // disps general information about me and the game

  disp.clearScreen();
  disp.setCursor(1, 1);
  disp.println("Made By Orion Serup CS M10A Moorpark College Spring 2019 \n");
  disp.println("HOW TO:");
  disp.println("Use Buttons to Move");
  disp.println("You Have 1 Minute");
  disp.println("Go Fast \n");
  disp.println("Press UP to return \n");

  while (1) {  // loop forever
    if (digitalRead(up)) { // pushing up?
      drawmenu();  // disp menu
      delay(2 * stdspeed); // wait
      break; // leave loop
    }
  }
  return;  // leave function
}
