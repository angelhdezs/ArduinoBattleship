#include <Adafruit_NeoPixel.h>

// ==============================================================================
// PARTE 1: CODIGO DE BUZZERS
// ==============================================================================

#define buzzer2acciones 12
#define buzzer1musica 10

#define NOTE_C3  131
#define NOTE_E3  165
#define NOTE_G3  196
#define NOTE_C4  262
#define NOTE_E4  330
#define NOTE_G4  392
#define NOTE_C5  523

#define A3_M  220 
#define C4_M  262
#define D4_M  294
#define E4_M  330
#define F4_M  349
#define G4_M  392
#define GS4_M 415 
#define a4_M  440
#define B4_M  494
#define C5_M  523 
#define D5_M  587
#define E5_M  659
#define F5_M  698
#define G5_M  784
#define R_M   0

const int Freq[] = {
  A3_M, A3_M, A3_M, A3_M, A3_M, A3_M, F4_M, E4_M,
  a4_M, B4_M, C5_M, D5_M, E5_M,
  F5_M, E5_M, D5_M, C5_M, B4_M,
  G4_M, GS4_M, a4_M, B4_M, C5_M, D5_M, E5_M,
  E5_M, F5_M, E5_M, D5_M, C5_M, B4_M, a4_M,
  E4_M, GS4_M
};

const int Time[] = {
  100, 100, 100, 100, 100, 100, 300, 300,
  300, 100, 100, 100, 600,
  100, 100, 100, 100, 600,
  100, 100, 100, 100, 100, 100, 300,
  100, 100, 100, 100, 300, 300, 300,
  100, 100
};

unsigned long lastMusicNoteTime = 0;
int musicIndex = 0;
int currentNoteDuration = 0;
const int musicLoopStartIndex = 8;
const int totalMusicNotes = 34;

int lastPosX_1 = 0; int lastPosY_1 = 0;
int lastPosX_2 = 0; int lastPosY_2 = 0;

void initializeAudioSystem() {
  pinMode(buzzer1musica, OUTPUT);
  pinMode(buzzer2acciones, OUTPUT);
  lastPosX_1 = analogRead(A0); lastPosY_1 = analogRead(A1); 
  lastPosX_2 = analogRead(A2); lastPosY_2 = analogRead(A3);
}

void resetBackgroundMusic() {
  musicIndex = 0;
  lastMusicNoteTime = millis();
}

void updateBackgroundMusic() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastMusicNoteTime >= currentNoteDuration + 50) {
    noTone(buzzer1musica);
    tone(buzzer1musica, Freq[musicIndex], Time[musicIndex]);
    lastMusicNoteTime = currentMillis;
    currentNoteDuration = Time[musicIndex];
    musicIndex++;
    if (musicIndex >= totalMusicNotes) {
      musicIndex = musicLoopStartIndex;
    }
  }
}

void stopBackgroundMusic() {
  noTone(buzzer1musica);
}

void handleJoystickSound(int joyX, int joyY, int* lastX, int* lastY) {
  if (abs(joyX - *lastX) > 100 || abs(joyY - *lastY) > 100) {
    tone(buzzer2acciones, 1047, 50);
    *lastX = joyX; *lastY = joyY;
  }
}

void soundPlaceShip() {
  tone(buzzer2acciones, 300, 100); 
}

void soundHit() {
  for (int freq = 1500; freq > 200; freq -= 100) { 
    tone(buzzer2acciones, freq, 15); delay(10); 
  }
  tone(buzzer2acciones, 100, 200); 
}

void soundWin() {
  stopBackgroundMusic(); 
  int melody[] = { NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_G3, NOTE_E3, NOTE_C3 };
  int noteDurations[] = { 4, 4, 4, 2, 4, 4, 1 };
  for (int i = 0; i < 7; i++) {
    int noteDuration = 1000 / noteDurations[i];
    tone(buzzer2acciones, melody[i], noteDuration);
    delay(noteDuration * 1.30);
    noTone(buzzer2acciones);
  }
  delay(500); 
}

// ==============================================================================
// PARTE 2: LOGICA PRINCIPAL DEL JUEGO
// ==============================================================================

// Pines
#define PIN_MATRIX_1   6
#define PIN_MATRIX_2   7
#define LED_COUNT      64
#define PIN_JOY_X_1    A0
#define PIN_JOY_Y_1    A1
#define PIN_JOY_SW_1   2
#define PIN_JOY_X_2    A2
#define PIN_JOY_Y_2    A3
#define PIN_JOY_SW_2   3

// Objetos LEDs
Adafruit_NeoPixel matrix1 = Adafruit_NeoPixel(LED_COUNT, PIN_MATRIX_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrix2 = Adafruit_NeoPixel(LED_COUNT, PIN_MATRIX_2, NEO_GRB + NEO_KHZ800);

// Variables del juego
int cursorX_1 = 0, cursorY_1 = 0; 
int cursorX_2 = 0, cursorY_2 = 0; 
unsigned long lastMoveTime_1 = 0;
unsigned long lastMoveTime_2 = 0;
int moveDelay = 150;
int gameState = 0; // Estado actual

#define SHIPS_TO_PLACE 20 // Cantidad de barcos

int shipCount_1 = 0; int shipCount_2 = 0;
int hits_by_P1 = 0; int hits_by_P2 = 0; 

// Memoria de tableros
int grid_1_ships[8][8];    
int grid_2_ships[8][8];   
int grid_1_guesses[8][8]; 
int grid_2_guesses[8][8]; 

// Configuracion inicial
void setup() {
  Serial.begin(9600);
  pinMode(PIN_JOY_SW_1, INPUT_PULLUP);
  pinMode(PIN_JOY_SW_2, INPUT_PULLUP);
  
  // Iniciar LEDs
  matrix1.begin(); matrix2.begin();
  matrix1.setBrightness(20); matrix2.setBrightness(20);

  // Iniciar audio
  initializeAudioSystem();
  
  // Empezar juego
  resetGame();
}

// Reiniciar todo
void resetGame() {
  resetBackgroundMusic(); // Reiniciar musica

  Serial.println("JUEGO NUEVO. Fase de Colocacion.");
  shipCount_1 = 0; shipCount_2 = 0;
  hits_by_P1 = 0; hits_by_P2 = 0;
  cursorX_1 = 0; cursorY_1 = 0;
  cursorX_2 = 0; cursorY_2 = 0;
  
  // Limpiar tableros
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      grid_1_ships[x][y] = 0; grid_2_ships[x][y] = 0;
      grid_1_guesses[x][y] = 0; grid_2_guesses[x][y] = 0;
    }
  }
  gameState = 0; // Ir a colocacion
  drawGrid_1(); drawGrid_2();
}

// Bucle principal
void loop() {
  // Mantener musica sonando si se esta jugando
  if (gameState != 3 && gameState != 4) {
      updateBackgroundMusic();
  }

  // Maquina de estados
  if (gameState == 0) handleSetupPhase();
  else if (gameState == 1) handleBattlePhase_P1();
  else if (gameState == 2) handleBattlePhase_P2();
  else if (gameState == 3) { // J1 Gana
    gameOverSequence(1);
  }
  else if (gameState == 4) { // J2 Gana
    gameOverSequence(2);
  }
}

// Fase de colocacion
void handleSetupPhase() {
  // Turno J1
  if (millis() - lastMoveTime_1 > moveDelay) {
    int joyX = analogRead(PIN_JOY_X_1); int joyY = analogRead(PIN_JOY_Y_1);
    handleJoystickSound(joyX, joyY, &lastPosX_1, &lastPosY_1); // Sonido
    if (moveCursor(joyX, joyY, &cursorX_1, &cursorY_1)) { drawGrid_1(); lastMoveTime_1 = millis(); }
  }
  if (digitalRead(PIN_JOY_SW_1) == LOW) {
    if (shipCount_1 < SHIPS_TO_PLACE && grid_1_ships[cursorX_1][cursorY_1] == 0) {
      grid_1_ships[cursorX_1][cursorY_1] = 1; shipCount_1++;
      soundPlaceShip(); // Sonido
      drawGrid_1();
    } delay(300); 
  }
  // Turno J2
  if (millis() - lastMoveTime_2 > moveDelay) {
    int joyX = analogRead(PIN_JOY_X_2); int joyY = analogRead(PIN_JOY_Y_2);
    handleJoystickSound(joyX, joyY, &lastPosX_2, &lastPosY_2); // Sonido
    if (moveCursor(joyX, joyY, &cursorX_2, &cursorY_2)) { drawGrid_2(); lastMoveTime_2 = millis(); }
  }
  if (digitalRead(PIN_JOY_SW_2) == LOW) {
    if (shipCount_2 < SHIPS_TO_PLACE && grid_2_ships[cursorX_2][cursorY_2] == 0) {
      grid_2_ships[cursorX_2][cursorY_2] = 1; shipCount_2++;
      soundPlaceShip(); // Sonido
      drawGrid_2();
    } delay(300); 
  }
  // Empezar batalla
  if (shipCount_1 >= SHIPS_TO_PLACE && shipCount_2 >= SHIPS_TO_PLACE && gameState == 0) {
    gameState = 1; drawGrid_1(); drawGrid_2(); delay(500); 
  }
}

// Turno de ataque J1
void handleBattlePhase_P1() {
  if (millis() - lastMoveTime_1 > moveDelay) {
    int joyX = analogRead(PIN_JOY_X_1); int joyY = analogRead(PIN_JOY_Y_1);
    handleJoystickSound(joyX, joyY, &lastPosX_1, &lastPosY_1); // Sonido
    if (moveCursor(joyX, joyY, &cursorX_1, &cursorY_1)) { drawGrid_1(); lastMoveTime_1 = millis(); }
  }
  if (digitalRead(PIN_JOY_SW_1) == LOW) handleFire_P1();
}

// Turno de ataque J2
void handleBattlePhase_P2() {
  if (millis() - lastMoveTime_2 > moveDelay) {
    int joyX = analogRead(PIN_JOY_X_2); int joyY = analogRead(PIN_JOY_Y_2);
    handleJoystickSound(joyX, joyY, &lastPosX_2, &lastPosY_2); // Sonido
    if (moveCursor(joyX, joyY, &cursorX_2, &cursorY_2)) { drawGrid_2(); lastMoveTime_2 = millis(); }
  }
  if (digitalRead(PIN_JOY_SW_2) == LOW) handleFire_P2();
}

// Disparo J1
void handleFire_P1() {
  if (grid_1_guesses[cursorX_1][cursorY_1] > 0) return; // Casilla ya usada
  if (grid_2_ships[cursorX_1][cursorY_1] == 1) {
    soundHit(); // Sonido bomba
    grid_1_guesses[cursorX_1][cursorY_1] = 2; hits_by_P1++;
    flashMatrix(matrix1, 0, 255, 0, 2); flashMatrix(matrix2, 255, 0, 0, 2); 
  } else { grid_1_guesses[cursorX_1][cursorY_1] = 1; } // Agua
  if (hits_by_P1 >= SHIPS_TO_PLACE) { gameState = 3; return; } // Ganó J1
  gameState = 2; drawGrid_1(); drawGrid_2(); delay(500); 
}

// Disparo J2
void handleFire_P2() {
  if (grid_2_guesses[cursorX_2][cursorY_2] > 0) return; // Casilla ya usada
  if (grid_1_ships[cursorX_2][cursorY_2] == 1) {
    soundHit(); // Sonido bomba
    grid_2_guesses[cursorX_2][cursorY_2] = 2; hits_by_P2++;
    flashMatrix(matrix2, 0, 255, 0, 2); flashMatrix(matrix1, 255, 0, 0, 2); 
  } else { grid_2_guesses[cursorX_2][cursorY_2] = 1; } // Agua
  if (hits_by_P2 >= SHIPS_TO_PLACE) { gameState = 4; return; } // Ganó J2
  gameState = 1; drawGrid_1(); drawGrid_2(); delay(500); 
}

// Secuencia de fin
void gameOverSequence(int winner) {
    if (winner == 1) { fillMatrix(matrix1, 0, 255, 0); fillMatrix(matrix2, 0, 0, 0); }
    else { fillMatrix(matrix1, 0, 0, 0); fillMatrix(matrix2, 0, 255, 0); }
    
    unsigned long start = millis();
    while(millis() - start < 4000) soundWin(); // Musica victoria
    resetGame();
}

// Mover cursor (Invertido)
bool moveCursor(int joyX, int joyY, int* cursorX, int* cursorY) {
  int newX = *cursorX; int newY = *cursorY; bool hasMoved = false;
  if (joyX < 200) { newY++; hasMoved = true; } else if (joyX > 800) { newY--; hasMoved = true; } 
  if (joyY < 200) { newX--; hasMoved = true; } else if (joyY > 800) { newX++; hasMoved = true; } 
  if (hasMoved) {
    newX = constrain(newX, 0, 7); newY = constrain(newY, 0, 7); 
    if (newX != *cursorX || newY != *cursorY) { *cursorX = newX; *cursorY = newY; return true; }
  } return false; 
}

// Dibujar matriz J1
void drawGrid_1() {
  if (gameState == 0 || gameState == 2) { // Ver mis barcos
    for (int y = 0; y < 8; y++) { for (int x = 0; x < 8; x++) {
        int idx = getPixelIndex(x, y); uint32_t color = 0;
        if (grid_1_ships[x][y] == 1) color = matrix1.Color(0, 0, 50); 
        if (grid_2_guesses[x][y] == 2) color = matrix1.Color(0, 50, 0); 
        else if (grid_2_guesses[x][y] == 1) color = matrix1.Color(50, 0, 0); 
        matrix1.setPixelColor(idx, color);
    }}
  } else { // Ver ataque
    for (int y = 0; y < 8; y++) { for (int x = 0; x < 8; x++) {
        int idx = getPixelIndex(x, y); uint32_t color = 0;
        if (grid_1_guesses[x][y] == 2) color = matrix1.Color(0, 50, 0); 
        else if (grid_1_guesses[x][y] == 1) color = matrix1.Color(50, 0, 0); 
        matrix1.setPixelColor(idx, color);
    }}
  }
  if (gameState == 0 || gameState == 1) matrix1.setPixelColor(getPixelIndex(cursorX_1, cursorY_1), 255, 255, 255); 
  matrix1.show();
}

// Dibujar matriz J2
void drawGrid_2() {
     if (gameState == 0 || gameState == 1) { // Ver mis barcos
    for (int y = 0; y < 8; y++) { for (int x = 0; x < 8; x++) {
        int idx = getPixelIndex(x, y); uint32_t color = 0;
        if (grid_2_ships[x][y] == 1) color = matrix2.Color(0, 0, 50); 
        if (grid_1_guesses[x][y] == 2) color = matrix2.Color(0, 50, 0); 
        else if (grid_1_guesses[x][y] == 1) color = matrix2.Color(50, 0, 0); 
        matrix2.setPixelColor(idx, color);
    }}
  } else { // Ver ataque
    for (int y = 0; y < 8; y++) { for (int x = 0; x < 8; x++) {
        int idx = getPixelIndex(x, y); uint32_t color = 0;
        if (grid_2_guesses[x][y] == 2) color = matrix2.Color(0, 50, 0); 
        else if (grid_2_guesses[x][y] == 1) color = matrix2.Color(50, 0, 0); 
        matrix2.setPixelColor(idx, color);
    }}
  }
  if (gameState == 0 || gameState == 2) matrix2.setPixelColor(getPixelIndex(cursorX_2, cursorY_2), 255, 255, 255); 
  matrix2.show();
}

// Parpadear matriz
void flashMatrix(Adafruit_NeoPixel& m, int r, int g, int b, int c) {
  for (int i = 0; i < c; i++) { fillMatrix(m, r, g, b); delay(100); fillMatrix(m, 0, 0, 0); delay(100); }
}
// Llenar matriz
void fillMatrix(Adafruit_NeoPixel& m, int r, int g, int b) {
  for (int j = 0; j < LED_COUNT; j++) m.setPixelColor(j, r, g, b); m.show();
}
// Obtener indice de led
int getPixelIndex(int x, int y) { return y * 8 + x; }