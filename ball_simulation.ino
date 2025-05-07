#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUTTON_PIN 19  // GPIO19 (D19 on ESP32-WROOM-32)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPU6050 mpu;

const int MAX_BALLS = 15;
const float GRAVITY = 0.1;
const float BALL_RADIUS = 2;
const int BOX_MARGIN = 5;
float bounceFactor = 0.6;  // Lower = less bouncy (0.0 to 1.0)

struct Ball {
  float x, y;
  float vx, vy;
};

Ball balls[MAX_BALLS];
int activeBalls = 0;

void addBall() {
  if (activeBalls < MAX_BALLS) {
    balls[activeBalls].x = random(BOX_MARGIN + BALL_RADIUS, SCREEN_WIDTH - BOX_MARGIN - BALL_RADIUS);
    balls[activeBalls].y = random(BOX_MARGIN + BALL_RADIUS, SCREEN_HEIGHT - BOX_MARGIN - BALL_RADIUS);
    balls[activeBalls].vx = 0;
    balls[activeBalls].vy = 0;
    activeBalls++;
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }

  display.clearDisplay();
  display.display();

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  randomSeed(analogRead(0));
}

void loop() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    addBall();
    delay(200);  // debounce delay
  }
  lastButtonState = currentButtonState;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Adjust for rotated MPU6050 (mounted perpendicular to screen)
  float tiltX = -a.acceleration.y; // Flip L/R
  float tiltY = -a.acceleration.x; // Up/down stays same

  for (int i = 0; i < activeBalls; i++) {
    balls[i].vx += tiltX * 0.05;
    balls[i].vy += (GRAVITY + tiltY * 0.05);

    balls[i].x += balls[i].vx;
    balls[i].y += balls[i].vy;

    // Bounce off walls
    if (balls[i].x < BOX_MARGIN + BALL_RADIUS) {
      balls[i].x = BOX_MARGIN + BALL_RADIUS;
      balls[i].vx = -balls[i].vx * bounceFactor;
    }
    if (balls[i].x > SCREEN_WIDTH - BOX_MARGIN - BALL_RADIUS) {
      balls[i].x = SCREEN_WIDTH - BOX_MARGIN - BALL_RADIUS;
      balls[i].vx = -balls[i].vx * bounceFactor;
    }
    if (balls[i].y < BOX_MARGIN + BALL_RADIUS) {
      balls[i].y = BOX_MARGIN + BALL_RADIUS;
      balls[i].vy = -balls[i].vy * bounceFactor;
    }
    if (balls[i].y > SCREEN_HEIGHT - BOX_MARGIN - BALL_RADIUS) {
      balls[i].y = SCREEN_HEIGHT - BOX_MARGIN - BALL_RADIUS;
      balls[i].vy = -balls[i].vy * bounceFactor;
    }
  }

  // Ball-to-ball collision response
  for (int i = 0; i < activeBalls; i++) {
    for (int j = i + 1; j < activeBalls; j++) {
      float dx = balls[j].x - balls[i].x;
      float dy = balls[j].y - balls[i].y;
      float dist = sqrt(dx * dx + dy * dy);
      if (dist < BALL_RADIUS * 2) {
        float angle = atan2(dy, dx);
        float targetX = balls[i].x + cos(angle) * BALL_RADIUS * 2;
        float targetY = balls[i].y + sin(angle) * BALL_RADIUS * 2;
        float ax = (targetX - balls[j].x) * 0.5;
        float ay = (targetY - balls[j].y) * 0.5;

        balls[i].vx -= ax;
        balls[i].vy -= ay;
        balls[j].vx += ax;
        balls[j].vy += ay;
      }
    }
  }

  // Draw
  display.clearDisplay();
  display.drawRect(BOX_MARGIN, BOX_MARGIN, SCREEN_WIDTH - 2 * BOX_MARGIN, SCREEN_HEIGHT - 2 * BOX_MARGIN, SSD1306_WHITE);

  for (int i = 0; i < activeBalls; i++) {
    display.fillCircle((int)balls[i].x, (int)balls[i].y, BALL_RADIUS, SSD1306_WHITE);
  }

  display.display();
  delay(20);
}
