
////////////////////////////////////////////////////////////////////////////////
//////////SERVO/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <ESP32Servo.h>

#define SERVO_1_PIN 18
#define SERVO_2_PIN 19
#define SERVO_3_PIN 5

#define SERVO_ANGLE_MAX 180 // SG90:180 DM996:180
#define SERVO_PWM_MIN 500   // SG90:500 default:544
#define SERVO_PWM_MAX 2400  // SG90:2400 default:2400

Servo myservo_1;
Servo myservo_2;
Servo myservo_3;

long servo_timing = 5; // 5
long servo_previous_millis = 0;

int servo_flag_step = 0;
long servo_previous_step_millis = 0;

float servo_1_angle = 90;
float servo_2_angle = 90;
float servo_3_angle = 90;
float servo_1_angle_smooth = 90;
float servo_2_angle_smooth = 90;
float servo_3_angle_smooth = 90;
float servo_1_angle_smooth_prev = 90;
float servo_2_angle_smooth_prev = 90;
float servo_3_angle_smooth_prev = 90;

float smooth_a = 1; // smoothing value for servos 1 and 2
float smooth_b = 1; // smoothing value for servo 3
int step = 0; // 200
int change_b = 0;


#define SMOOTH_A_MIN 1
#define SMOOTH_A_MAX 3 + 1
#define SMOOTH_B_MIN 4
#define SMOOTH_B_MAX 5 + 1

#define ANGLE_A_MIN 30
#define ANGLE_A_MAX 150 + 1
#define ANGLE_B_MIN 70
#define ANGLE_B_MAX 110 + 1

#define STEP_MIN 500
#define STEP_MAX 4000

////////////////////////////////////////////////////////////////////////////////
//////////FUNCTIONS/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup_servo() {
  myservo_1.setPeriodHertz(50);
  myservo_2.setPeriodHertz(50);
  myservo_3.setPeriodHertz(50);
  myservo_1.attach(SERVO_1_PIN, SERVO_PWM_MIN, SERVO_PWM_MAX);
  myservo_2.attach(SERVO_2_PIN, SERVO_PWM_MIN, SERVO_PWM_MAX);
  myservo_3.attach(SERVO_3_PIN, SERVO_PWM_MIN, SERVO_PWM_MAX);
}

void turn(int servo_1_angle, int servo_2_angle, int servo_3_angle) {
  myservo_1.write(servo_1_angle);
  myservo_2.write(servo_2_angle); 
  myservo_3.write(servo_3_angle); 
}
