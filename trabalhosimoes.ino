#include <LedControl.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

int DIN = 10;
int CS = 9;
int CLK = 8;
int ESQUERDA = 11;
int DIREITA = 12;

LedControl lc = LedControl(DIN, CLK, CS, 2);

int snake_len = 4;
byte screen[16][8] = {0};
int snake[128][2] = {{3,2}, {3,3}, {3,4}, {3,5}};
int direction = LEFT;
int apple[2];
bool ate_apple = false;

void displayMatrix(byte matrix[16][8]) {
  byte row, b;
  int i, j;
  for (i = 0; i < 16; i++) {
    b = 1;
    row = 0;
    for (j = 0; j < 8; j++) {
      row += b * matrix[i][j];
      b *= 2;
    }
    lc.setRow(i/8, i%8, row);
  }
}

void move_snake() {
  int i;
  int last_len = snake_len-1;
  if (!ate_apple) {
    for (i = 0; i < last_len; i++) {
      snake[i][0] = snake[i+1][0];
      snake[i][1] = snake[i+1][1];
    }
  } else {
    snake_len++;
    ate_apple = false;
  }
  
  switch (direction) {
    case UP:
      snake[snake_len-1][1] = snake[last_len][1]-1;
      snake[snake_len-1][0] = snake[last_len][0];
      break;
    case RIGHT:
      snake[snake_len-1][0] = snake[last_len][0]+1;
      snake[snake_len-1][1] = snake[last_len][1];
      break;
    case DOWN:
      snake[snake_len-1][1] = snake[last_len][1]+1;
      snake[snake_len-1][0] = snake[last_len][0];
      break;
    case LEFT:
      snake[snake_len-1][0] = snake[last_len][0]-1;
      snake[snake_len-1][1] = snake[last_len][1];
      break;
  }

  // loop pela tela
  if (snake[snake_len-1][0] == 16) {
    snake[snake_len-1][0] = 0;  
  }
  if (snake[snake_len-1][0] == -1) {
    snake[snake_len-1][0] = 15;
  }
  if (snake[snake_len-1][1] == 8) {
    snake[snake_len-1][1] = 0;  
  }
  if (snake[snake_len-1][1] == -1) {
    snake[snake_len-1][1] = 7;  
  }
  
  if (snake[snake_len-1][0] == apple[0] && snake[snake_len-1][1] == apple[1]) {
    eat_apple();
  }
}

bool is_snake(int pos[2]) {
  int i;
  bool is = false;
  for (i = 0; i < snake_len; i++) {
    if (snake[i][0] = pos[0] && snake[i][1] == pos[1]) {
      is = true;
    }
  }
  return is;
}

void eat_apple() {
  ate_apple = 1;
  apple[0] = random(16);
  apple[1] = random(8);
  while (is_snake(apple) && snake_len < 128) {
    apple[0] = random(16);
    apple[1] = random(8);
  }
}

void update_screen() {
  int i, j;
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 8; j++) {
      screen[i][j] = 0;
    }
  }
  for (i = 0; i < snake_len; i++) {
    screen[snake[i][0]][snake[i][1]] = 1;
  }

  screen[apple[0]][apple[1]] = 1;
}

void setup() {
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  
  analogWrite(A5, 0);
  analogWrite(A4, 0);
  analogWrite(A3, 255);
  analogWrite(A2, 255);
  
  randomSeed(0x512035);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  lc.shutdown(1, false);
  lc.setIntensity(1, 15);
  lc.clearDisplay(1);

  apple[0] = 1;
  apple[1] = 5;

  int i;
  for (i = 0; i < snake_len; i++) {
      screen[snake[i][0]][snake[i][1]] = 1;
  }
}

void loop() {
  int i = 0;
  while (1) {
    if (analogRead(A1) < 32) {
      direction = (direction + 1) % 4;
    }
    if (analogRead(A0) < 32) {
      direction = (direction + 3) % 4;
    }
    displayMatrix(screen);
    move_snake();
    update_screen();
    delay(500);
    i++;
  }
}
