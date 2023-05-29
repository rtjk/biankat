/////////////////////////
//////////MARCO//////////
/////////////////////////

#include "Arrays.h"
#include "Comment.h"
#include "Command.h"
#include "Matrix.h"
//#include "Network.h"
#include "Servo.h"
#include "Sound.h"

unsigned long current_millis;
bool in_idle = false;

////////////////////////////////////////////////////////////////////////////////
//////////SETUP/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);

  setup_matrix();
  setup_servo();
  setup_sound();
  setup_network();

  setup_idle();
  
}

////////////////////////////////////////////////////////////////////////////////
//////////LOOP//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void loop() {

  current_millis = millis();

  //check_connection();

  if (pause_reception_flag == 1 && current_millis - pause_reception_previous_millis > pause_reception_delay) {
    pause_reception_flag = 0;
  }

  if(pause_reception_flag == 0) {
    pause_reception_previous_millis = current_millis;
    receive_message();
  }

  if(!performing_emotion && !in_idle) {
    idle();
    facing = 6;
    in_idle = true;
    performing_emotion = true;
  }
  else if(!performing_emotion || performing_emotion && in_idle && next_emotion != "") {
    start_new_emotion();
    facing = facing_next;
    in_idle = false;    
    performing_emotion = true;
  }
  
  if(servo_sequence_step == servo_sequence_length) {
    performing_emotion = false;
    facing = 6;
    if(reading_flag == 1) {
      performing_emotion = true;
      in_idle = true;
      next_emotion = emotion_after_reading;
      reading_flag = 0;
      send_message("GG");
    }
  }

  // MATRIX
  
  if (matrix_flag_wait == 1 && current_millis - matrix_previous_millis > matrix_delay_betweeen_cycles) {
    matrix_previous_millis = current_millis;
    matrix_flag_wait = 0;
  } else if (matrix_flag_wait == 0 && current_millis - matrix_previous_millis > matrix_refreh) {
    matrix_previous_millis = current_millis;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = pgm_read_dword(&(led_array[i + NUM_LEDS * current_frame]));
    }
    FastLED.show();
    FastLED.clear();
    current_frame++;
    if (current_frame == matrix_cycle_length) {
      current_frame = 0;
      matrix_flag_wait = 1;
    }
  }

  // SOUND

  if (current_millis - player_previous_step_millis > playlist[playlist_step][1] && playlist_step < playlist_length) {
    player.playMp3Folder(playlist[playlist_step][0]);
    playlist_step++;
    player_previous_step_millis = current_millis;
  }

  // SERVO

  if (current_millis - servo_previous_millis >= servo_timing) {
    servo_previous_millis = current_millis;
    if (current_millis - servo_previous_step_millis > servo_sequence[servo_sequence_step][3] && servo_sequence_step < servo_sequence_length) {
      servo_1_angle = servo_sequence[servo_sequence_step][0];
      servo_2_angle = servo_sequence[servo_sequence_step][1];
      servo_3_angle = servo_sequence[servo_sequence_step][2] - 90 + ys[facing-1];
      smooth_a = float(servo_sequence[servo_sequence_step][4]) / 100;
      smooth_b = float(servo_sequence[servo_sequence_step][5]) / 100;
      servo_sequence_step++;
      servo_previous_step_millis = current_millis;
    }
    servo_1_angle_smooth = (servo_1_angle * smooth_a) + (servo_1_angle_smooth_prev * (1 - smooth_a));
    servo_2_angle_smooth = (servo_2_angle * smooth_a) + (servo_2_angle_smooth_prev * (1 - smooth_a));
    servo_3_angle_smooth = (servo_3_angle * smooth_b) + (servo_3_angle_smooth_prev * (1 - smooth_b));
    servo_1_angle_smooth_prev = servo_1_angle_smooth;
    servo_2_angle_smooth_prev = servo_2_angle_smooth;
    servo_3_angle_smooth_prev = servo_3_angle_smooth;
    turn(servo_1_angle_smooth, servo_2_angle_smooth, servo_3_angle_smooth);
  }
}

////////////////////////////////////////////////////////////////////////////////
//////////EMOTIONS//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void stop() {
  set_matrix_cycle(led_array_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence(sequence_stop_2, 2);
  set_playlist(playlist_stop_1, 1);
}
void idle() {
  set_matrix_cycle(led_array_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence_idle(0);
  set_playlist(playlist_stop_1, 1);
}
void angry() {
  send_message("6C03");
  set_matrix_cycle(led_array_angry_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence(sequence_angry_10, 10);
  set_playlist(playlist_angry_6, 6);
}
void embarrassed() {
  send_message("6K03");
  set_matrix_cycle(led_array_blush_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence(sequence_embarrassed_4, 4);
  set_playlist(playlist_embarrassed_4, 4);
}
void happy() {
  send_message("6B03");
  set_matrix_cycle(led_array_laugh_r65_d5250_21, 21, 5250, 65);
  set_servo_sequence(sequence_happy_19, 19);
  set_playlist(playlist_happy_5, 5);
}
void sad() {
  send_message("6E03");
  set_matrix_cycle(led_array_sad_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence(sequence_sad_8, 8);
  set_playlist(playlist_sad_2, 2);
}
void face() {
  set_matrix_cycle(led_array_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence(sequence_face_2, 2);
  set_playlist(playlist_face_1, 1);
  pause_reception(10000);
}
void smsa() {
  set_matrix_cycle(led_array_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence(sequence_smsa_4, 4);
  set_playlist(playlist_smsa_5, 5);
  pause_reception(10000);
  emotion_after_reading = "embarrassed";
  reading_flag = 1;
}
void smsb() {
  set_matrix_cycle(led_array_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence(sequence_smsb_4, 4);
  set_playlist(playlist_smsb_5, 5);
  pause_reception(25000);
  emotion_after_reading = "happy";
  reading_flag = 1;
  send_message_toward = 7;
}

////////////////////////////////////////////////////////////////////////////////
//////////FUNCTIONS/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void start_new_emotion() {
  if(next_emotion == "stop") stop();
  if(next_emotion == "angry") angry();
  if(next_emotion == "embarrassed") embarrassed();
  if(next_emotion == "happy") happy();
  if(next_emotion == "sad") sad();
  if(next_emotion == "face") face();
  if(next_emotion == "smsa") smsa();
  if(next_emotion == "smsb") smsb();
  next_emotion = "";
}

void setup_idle() {
  set_matrix_cycle(led_array_blink_r50_d4000_9, 9, 4000, 50);
  set_servo_sequence_idle(0);
}

void set_servo_sequence_idle(int after) {
  for (int i = 1; i < SEQUENCE_ROWS; i++) {
    change_b = random(0, 3);
    servo_sequence[i][0] = random(ANGLE_A_MIN, ANGLE_A_MAX);
    servo_sequence[i][1] = random(ANGLE_A_MIN, ANGLE_A_MAX);
    if (change_b) servo_sequence[i][2] = random(ANGLE_B_MIN, ANGLE_B_MAX);
    else servo_sequence[i][2] = servo_sequence[i - 1][2];
    servo_sequence[i][3] = random(STEP_MIN, STEP_MAX);
    servo_sequence[i][4] = random(SMOOTH_A_MIN, SMOOTH_A_MAX);
    servo_sequence[i][5] = random(SMOOTH_B_MIN, SMOOTH_B_MAX);
  }
  servo_sequence[0][0] = 90;
  servo_sequence[0][1] = 90;
  servo_sequence[0][2] = 90;
  servo_sequence[0][3] = after;
  servo_sequence[0][4] = 1;
  servo_sequence[0][5] = 1;
  servo_sequence_length = SEQUENCE_ROWS;
  servo_sequence_step = 0;
  set_matrix_cycle(led_array_blink_r50_d4000_9, 9, 4000, 50);
}

void set_matrix_cycle(long cycle[], int cycle_length, int new_delay, int new_matrix_refresh) {
  matrix_refreh = new_matrix_refresh;
  matrix_delay_betweeen_cycles = new_delay;
  led_array = cycle;
  matrix_cycle_length = cycle_length;
  matrix_flag_wait = 0;
  current_frame = 0;
}

void set_servo_sequence(int sequence[][SEQUENCE_COLUMNS], int sequence_length) {
  servo_sequence_length = sequence_length;
  memcpy(servo_sequence, sequence, SEQUENCE_COLUMNS * servo_sequence_length * sizeof(int));
  servo_sequence_step = 0;
}

void set_playlist(int new_playlist[PLAYLIST_ROWS][PLAYLIST_COLUMNS], int new_playlist_length) {
  playlist_length = new_playlist_length;
  memcpy(playlist, new_playlist, PLAYLIST_COLUMNS * playlist_length * sizeof(int));
  playlist_step = 0;
}


