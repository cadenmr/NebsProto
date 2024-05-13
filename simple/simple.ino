#include <Adafruit_NeoPixel.h>
#include <esp_random.h>

// Neopixel Grid Settings
#define NP_PIN 9
#define CENTER_SIZE 256
#define EYE_SIZE 64
#define NUMPIXELS CENTER_SIZE + EYE_SIZE + EYE_SIZE 

// Timings
#define ANIM_BLINK_DURATION_MS 175
#define ANIM_BLINK_DELAY_MS 2200
#define ANIM_BLINK_DELAY_JITTER_MS 1000

#define ANIM_BLEPTRANSITION_DURATION_MS 270

// Speech
#define MICROPHONE_INPUT_PIN D2
#define ANIM_SPEAK_TIMEOUT_PERIOD_MILLIS 400
#define ANIM_SPEAK_DURATION_MS 150

// States
#define ST_NORM 0    // blink cycle + normal speak
#define ST_BLEPTRANSITION 1
#define ST_BLEPTRANSITION_R 2
#define ST_BLEP 3
#define ST_UWU 4    // UwU face, no anim

#define SPEAKST_NORM_IDLE 0
#define SPEAKST_NORM_FORWARD_ANIM 1
#define SPEAKST_NORM_REVERSE_ANIM 2
#define SPEAKST_NORM_HOLD_ON 3

// center_blank.png
// center
uint8_t center_blank[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,255,0,0,0,255,0,0,0,0,255,0,0,255,255,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,255,255,0,0,255,0,0,0,0,255,0,0,0,255,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_blep0.png
// center
uint8_t center_blep0[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,255,0,0,0,251,0,0,0,0,0,0,0,251,0,255,255,0,0,0,251,251,0,0,0,0,0,0,251,3,0,0,0,0,0,0,3,251,0,0,0,0,0,0,251,251,0,0,0,255,255,0,251,0,0,0,0,0,0,0,251,0,0,0,255,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
// center_blep1.png
// center
uint8_t center_blep1[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,255,0,0,0,251,0,0,0,0,0,0,0,251,0,255,255,0,0,0,251,251,0,0,0,0,0,251,74,251,0,0,0,0,0,0,251,74,251,0,0,0,0,0,251,251,0,0,0,255,255,0,251,0,0,0,0,0,0,0,251,0,0,0,255,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_blep2.png
// center
uint8_t center_blep2[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,255,0,0,0,251,0,0,0,0,0,0,0,251,0,255,255,0,0,0,251,251,251,0,0,0,251,74,74,251,0,0,0,0,0,0,251,74,74,251,0,0,0,251,251,251,0,0,0,255,255,0,251,0,0,0,0,0,0,0,251,0,0,0,255,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_blep3.png
// center
uint8_t center_blep3[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,255,0,0,0,251,0,0,0,0,0,0,0,251,0,255,255,0,0,0,251,251,251,251,0,251,74,74,74,251,0,0,0,0,0,0,251,74,74,74,251,0,251,251,251,251,0,0,0,255,255,0,251,0,0,0,0,0,0,0,251,0,0,0,255,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_blep3_speak.png
// center
uint8_t center_blep3_speak[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,251,0,0,0,0,0,0,0,251,251,0,0,0,251,251,0,0,0,0,0,0,0,0,0,0,0,251,251,0,251,251,0,0,0,0,0,255,0,0,0,251,251,0,0,0,0,0,251,251,0,255,255,0,0,0,251,251,251,251,0,251,74,74,74,251,0,0,0,0,0,0,251,74,74,74,251,0,251,251,251,251,0,0,0,255,255,0,251,251,0,0,0,0,0,251,251,0,0,0,255,0,0,0,0,0,251,251,0,251,251,0,0,0,0,0,0,0,0,0,0,0,251,251,0,0,0,251,251,0,0,0,0,0,0,0,251,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_norm.png
// center
uint8_t center_norm[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,255,0,0,0,0,0,0,255,0,0,255,0,0,0,0,0,255,0,0,255,255,0,0,0,0,0,255,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,255,0,0,0,0,0,255,255,0,0,255,0,0,0,0,0,255,0,0,255,0,0,0,0,0,0,255,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_speak1.png
// center
uint8_t center_speak1[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,255,255,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,0,255,255,0,0,0,0,0,0,255,0,0,255,255,0,0,0,255,255,0,0,255,255,0,0,0,0,0,255,255,0,0,0,0,255,255,0,0,0,0,0,0,0,0,255,255,0,0,0,0,255,255,0,0,0,0,0,255,255,0,0,255,255,0,0,0,255,255,0,0,255,0,0,0,0,0,0,255,255,0,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,255,255,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_speak2.png
// center
uint8_t center_speak2[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,255,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,255,255,255,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,255,0,0,255,255,255,255,0,255,255,0,0,255,255,0,0,0,0,0,255,255,255,0,0,255,255,255,0,0,0,0,0,0,0,0,255,255,255,0,0,255,255,255,0,0,0,0,0,255,255,0,0,255,255,0,255,255,255,255,0,0,255,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,255,255,255,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,255,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// center_squiggly.png
// center
uint8_t center_squiggly[256] = {0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,255,0,0,251,0,0,0,0,0,0,0,0,251,0,255,255,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,251,0,0,0,0,0,0,255,255,0,0,0,251,0,0,0,0,0,251,0,0,0,255,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,251,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0};

// center_uwu.png
// center
uint8_t center_uwu[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,251,251,0,0,0,0,0,0,0,251,251,251,0,0,0,0,0,251,251,251,0,0,0,251,251,251,0,0,0,0,0,0,0,0,0,251,251,251,251,251,0,0,0,0,0,0,0,0,0,0,0,0,251,251,251,251,251,0,0,0,0,0,255,0,0,0,251,251,251,0,0,0,251,251,251,0,255,255,0,0,0,251,251,0,0,0,0,0,0,251,251,0,0,0,0,0,0,251,251,0,0,0,0,0,0,251,251,0,0,0,255,255,0,251,251,251,0,0,0,251,251,251,0,0,0,255,0,0,0,0,0,251,251,251,251,251,0,0,0,0,0,0,0,0,0,0,0,0,251,251,251,251,251,0,0,0,0,0,0,0,0,0,251,251,251,0,0,0,251,251,251,0,0,0,0,0,251,251,251,0,0,0,0,0,0,0,251,251,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// eye_lefthand_blink1.png
// lefthand eye
uint8_t eye_lefthand_blink1[64] = {0,0,0,0,0,251,251,0,0,0,251,251,0,0,0,0,0,0,0,251,251,0,0,0,0,0,0,255,251,251,0,0,0,0,251,251,251,0,0,0,0,0,0,251,251,0,0,0,0,0,0,251,251,0,0,0,0,0,251,251,0,0,0,0};

// eye_lefthand_blink2.png
// lefthand eye
uint8_t eye_lefthand_blink2[64] = {0,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0};

// eye_lefthand_circle.png
// lefthand eye
uint8_t eye_lefthand_circle[64] = {0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,255,255,0,0,255,255,0,0,255,0,0,0,0,255,0,0,255,0,0,0,0,255,0,0,255,255,0,0,255,255,0,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0};

// eye_lefthand_happy.png
// lefthand eye
uint8_t eye_lefthand_happy[64] = {0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,255,255,0,0,255,255,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0};

// eye_lefthand_heart.png
// lefthand eye
uint8_t eye_lefthand_heart[64] = {0,0,251,251,251,0,0,0,0,0,251,0,0,0,251,0,0,251,0,0,0,0,251,0,251,0,0,0,0,251,0,0,0,0,251,0,0,0,0,251,0,251,0,0,0,0,251,0,0,251,0,0,0,251,0,0,0,0,0,251,251,251,0,0};

// eye_lefthand_norm.png
// lefthand eye
uint8_t eye_lefthand_norm[64] = {0,0,0,0,255,255,255,0,0,0,255,255,255,0,0,0,0,0,255,255,255,0,0,0,0,0,0,255,255,255,251,0,0,255,255,255,255,0,0,0,0,0,0,255,255,255,0,0,0,0,0,255,255,0,0,0,0,0,255,255,0,0,0,0};

// eye_lefthand_spiral.png
// lefthand eye
uint8_t eye_lefthand_spiral[64] = {0,0,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,251,251,0,0,251,0,0,251,0,251,0,0,251,0,0,251,0,251,251,0,251,0,0,251,0,0,0,0,251,0,0,0,251,251,251,251,0,0,0,0,0,0,0,0,0,0};

// eye_lefthand_squee.png
// lefthand eye
uint8_t eye_lefthand_squee[64] = {0,0,0,0,0,0,251,251,251,0,251,0,0,0,0,0,0,0,0,0,251,251,0,251,251,0,0,251,251,0,0,0,0,0,251,251,0,0,0,251,251,0,0,0,0,251,251,0,251,251,0,0,0,0,0,251,251,0,0,0,0,0,0,251};

// eye_lefthand_u.png
// lefthand eye
uint8_t eye_lefthand_u[64] = {0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,255,255,255,255,255,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,255,255,255,255,255,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0};

// eye_lefthand_x.png
// lefthand eye
uint8_t eye_lefthand_x[64] = {255,255,0,0,0,0,255,255,255,255,255,0,0,255,255,255,0,255,255,255,255,255,255,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,255,255,255,255,255,255,0,255,255,255,0,0,255,255,255,255,255,0,0,0,0,255,255};

// eye_righthand_blink1.png
// righthand eye
uint8_t eye_righthand_blink1[64] = {0,0,0,0,251,251,0,0,0,0,0,251,251,0,0,0,0,0,0,251,251,0,0,0,0,0,0,251,251,251,0,0,0,0,251,251,251,0,0,0,0,0,0,251,251,0,0,0,0,0,0,0,251,251,0,0,0,251,251,0,0,0,0,0};

// eye_righthand_blink2.png
// righthand eye
uint8_t eye_righthand_blink2[64] = {0,0,0,0,0,251,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,251,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,251,0,0,0,251,0,0,0,0,0,0};

// eye_righthand_circle.png
// righthand eye
uint8_t eye_righthand_circle[64] = {0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,255,255,0,0,255,255,0,0,255,0,0,0,0,255,0,0,255,0,0,0,0,255,0,0,255,255,0,0,255,255,0,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0};

// eye_righthand_happy.png
// righthand eye
uint8_t eye_righthand_happy[64] = {0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,255,255,0,0,255,255,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0};

// eye_righthand_heart.png
// righthand eye
uint8_t eye_righthand_heart[64] = {0,0,251,251,251,0,0,0,0,0,251,0,0,0,251,0,0,251,0,0,0,0,251,0,251,0,0,0,0,251,0,0,0,0,251,0,0,0,0,251,0,251,0,0,0,0,251,0,0,251,0,0,0,251,0,0,0,0,0,251,251,251,0,0};

// eye_righthand_norm.png
// righthand eye
uint8_t eye_righthand_norm[64] = {0,0,0,0,255,255,0,0,0,0,0,255,255,0,0,0,0,0,255,255,255,0,0,0,0,0,0,255,255,255,255,0,0,251,255,255,255,0,0,0,0,0,0,255,255,255,0,0,0,0,0,255,255,255,0,0,0,255,255,255,0,0,0,0};

// eye_righthand_spiral.png
// righthand eye
uint8_t eye_righthand_spiral[64] = {0,0,0,0,0,0,0,0,0,0,251,251,251,251,0,0,0,251,0,0,0,0,251,0,0,251,0,251,251,0,251,0,0,251,0,0,251,0,251,0,0,251,0,0,251,251,0,0,0,0,0,0,0,251,0,0,0,0,0,0,0,0,0,0};

// eye_righthand_squee.png
// righthand eye
uint8_t eye_righthand_squee[64] = {251,0,0,0,0,0,0,251,251,0,0,0,0,0,251,251,0,251,251,0,0,0,0,251,251,0,0,0,251,251,0,0,0,0,0,251,251,0,0,251,251,0,251,251,0,0,0,0,0,0,0,0,0,251,0,251,251,251,0,0,0,0,0,0};

// eye_righthand_u.png
// righthand eye
uint8_t eye_righthand_u[64] = {0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,255,255,255,255,255,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,255,255,255,255,255,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0};

// eye_righthand_x.png
// righthand eye
uint8_t eye_righthand_x[64] = {255,255,0,0,0,0,255,255,255,255,255,0,0,255,255,255,0,255,255,255,255,255,255,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,0,0,0,255,255,255,255,255,255,0,255,255,255,0,0,255,255,255,255,255,0,0,0,0,255,255};

Adafruit_NeoPixel pixels(NUMPIXELS, NP_PIN, NEO_GRB + NEO_KHZ800);

uint8_t state = ST_NORM;

// color array - TODO: Store me to EEPROM!
// state, screen (eye, center), rgb
double colors[][2][3] = { 

  {{ 0.3961, 0.1020, 0.6902 }, { 0.3961, 0.1020, 0.6902 }},   // NORM
  {{ 0.0157, 0.1922, 0.9765 }, { 0.0157, 0.1922, 0.9765 }},
  {{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }},
  {{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }} 

};

unsigned long switchMillis = 0;  // TODO: testing - remove me
bool updated = false; // TODO: testing - remove me
unsigned char switchCount = 0;  // TODO: testing - remove me

void setup() {

  Serial.begin(115200);

  state = ST_NORM;

  pixels.begin();
  pixels.clear();

  pixels.setBrightness(10);

  setAll_sprite(eye_lefthand_norm, center_blep3, eye_righthand_norm, colors, state);

  switchMillis = millis(); // TODO: testing - remove me

}

// RUNTIME VARS FOR ANIMATIONS:
// blinking
bool anim_BlinkEnabled = true;
unsigned long anim_BlinkEventNextStartingMillis = millis() + ANIM_BLINK_DELAY_MS;
bool anim_BlinkStagesStarted[] = {false, false, false};

// speaking
bool anim_SpeakNormEnabled = true;
bool anim_SpeakBlepEnabled = false;

unsigned long anim_SpeakLastActivationMillis = 0;
unsigned long anim_SpeakAnimStartedMillis = 0;
bool anim_SpeakTriggered = false;
unsigned char anim_SpeakState = 0;
bool anim_SpeakScreenUpdated = false;

// blep transition (forwards)
unsigned long anim_BlepTransitionStartingMillis = 0;
bool anim_BlepTransitionStagesStarted[] = {false, false, false};

void loop() {

  // TODO: testing - remove me
  // if (millis() > (switchMillis + 10000)) {

  //   if (state == ST_NORM && switchCount < 5) { state = ST_BLEPTRANSITION; }
  //   else if (state == ST_NORM && switchCount > 5) { switchCount = 0; state = ST_UWU; }
  //   else if (state == ST_UWU) { state = ST_NORM; }
  //   else if (state == ST_BLEP) { state = ST_BLEPTRANSITION_R; }

  //   switchMillis = millis();
  //   switchCount += 1;
  //   updated = false;
  // }

  switch(state) {

    default:

    case ST_NORM:

      if (!updated) { 

        setAll_sprite(eye_lefthand_norm, center_norm, eye_righthand_norm, colors, state); 

        anim_BlinkEnabled = true;
        anim_BlinkEventNextStartingMillis = millis() + ANIM_BLINK_DELAY_MS;

        anim_SpeakNormEnabled = true;
        
        updated = true; 

      }

      break;

    case ST_BLEPTRANSITION:

      anim_BlinkEnabled = false;
      anim_SpeakNormEnabled = false;

      if (!anim_BlepTransitionStagesStarted[0]) {

        anim_BlepTransitionStagesStarted[0] = true;
        anim_BlepTransitionStartingMillis = millis();

        setCenter_sprite(center_blep0, colors, ST_NORM);
        pixels.show();

      } else if (millis() > (anim_BlepTransitionStartingMillis + (ANIM_BLEPTRANSITION_DURATION_MS / 3)) && !anim_BlepTransitionStagesStarted[1]) {

        anim_BlepTransitionStagesStarted[1] = true;
        setAll_sprite(eye_lefthand_blink1, center_blep1, eye_righthand_blink1, colors, ST_NORM);

      } else if (millis() > (anim_BlepTransitionStartingMillis + ( 2 * (ANIM_BLEPTRANSITION_DURATION_MS / 3))) && !anim_BlepTransitionStagesStarted[2]) {

        anim_BlepTransitionStagesStarted[2] = true;
        setAll_sprite(eye_lefthand_blink1, center_blep2, eye_righthand_blink1, colors, ST_NORM);

      } else if (millis() > (anim_BlepTransitionStartingMillis + ANIM_BLEPTRANSITION_DURATION_MS)) {

        setAll_sprite(eye_lefthand_blink2, center_blep3, eye_righthand_blink2, colors, ST_NORM);

        anim_BlepTransitionStagesStarted[0] = false;
        anim_BlepTransitionStagesStarted[1] = false;
        anim_BlepTransitionStagesStarted[2] = false;

        state = ST_BLEP;

      }
      
      break;

    case ST_BLEPTRANSITION_R:

      anim_BlinkEnabled = false;
      anim_SpeakNormEnabled = false;

      if (!anim_BlepTransitionStagesStarted[0]) {

        anim_BlepTransitionStagesStarted[0] = true;
        anim_BlepTransitionStartingMillis = millis();

        setAll_sprite(eye_lefthand_blink2, center_blep3, eye_righthand_blink2, colors, ST_NORM);
        pixels.show();

      } else if (millis() > (anim_BlepTransitionStartingMillis + (ANIM_BLEPTRANSITION_DURATION_MS / 3)) && !anim_BlepTransitionStagesStarted[1]) {

        anim_BlepTransitionStagesStarted[1] = true;
        setAll_sprite(eye_lefthand_blink1, center_blep2, eye_righthand_blink1, colors, ST_NORM);

      } else if (millis() > (anim_BlepTransitionStartingMillis + ( 2 * (ANIM_BLEPTRANSITION_DURATION_MS / 3))) && !anim_BlepTransitionStagesStarted[2]) {

        anim_BlepTransitionStagesStarted[2] = true;
        setAll_sprite(eye_lefthand_blink1, center_blep1, eye_righthand_blink1, colors, ST_NORM);

      } else if (millis() > (anim_BlepTransitionStartingMillis + ANIM_BLEPTRANSITION_DURATION_MS)) {

        setAll_sprite(eye_lefthand_blink1, center_blep0, eye_righthand_blink1, colors, ST_NORM);

        anim_BlepTransitionStagesStarted[0] = false;
        anim_BlepTransitionStagesStarted[1] = false;
        anim_BlepTransitionStagesStarted[2] = false;

        state = ST_NORM;

      }
      
      break;

    case ST_BLEP:

      anim_BlinkEnabled = false;
      anim_SpeakNormEnabled = false;

      if (!updated) { setAll_sprite(eye_lefthand_blink2, center_blep3, eye_righthand_blink2, colors, ST_NORM); updated = true; }

      break;

    case ST_UWU:

      anim_BlinkEnabled = false;
      anim_SpeakNormEnabled = false;

      if (!updated) { setAll_sprite(eye_lefthand_u, center_uwu, eye_righthand_u, colors, 1); updated = true; }

  }

  // these will override anything manually set
  // disable this when using alternative sprites or manual pixel setting  

  // blinking routune
  if (anim_BlinkEnabled) {

    // frame 1
    if (millis() > anim_BlinkEventNextStartingMillis && !anim_BlinkStagesStarted[0]) {

      anim_BlinkStagesStarted[0] = true;

      setLefthandEye_sprite(eye_lefthand_blink1, colors, state);
      setRighthandEye_sprite(eye_righthand_blink1, colors, state);

      pixels.show();

    } else if ((millis() > (anim_BlinkEventNextStartingMillis + (ANIM_BLINK_DURATION_MS / 3))) && !anim_BlinkStagesStarted[1]) {  // blink frame 2

      anim_BlinkStagesStarted[1] = true;

      setLefthandEye_sprite(eye_lefthand_blink2, colors, state);
      setRighthandEye_sprite(eye_righthand_blink2, colors, state);

      pixels.show();

    } else if ((millis() > (anim_BlinkEventNextStartingMillis + (2 * (ANIM_BLINK_DURATION_MS / 3)))) && !anim_BlinkStagesStarted[2]) {  // blink frame 1

      anim_BlinkStagesStarted[2] = true;

      setLefthandEye_sprite(eye_lefthand_blink1, colors, state);
      setRighthandEye_sprite(eye_righthand_blink1, colors, state);

      pixels.show();

    } else if (millis() > ( anim_BlinkEventNextStartingMillis + ANIM_BLINK_DURATION_MS )) {  // end action

      double jitterAdder = esp_random() * ( (double)ANIM_BLINK_DELAY_JITTER_MS / (double)UINT32_MAX );
      anim_BlinkEventNextStartingMillis = millis() + ANIM_BLINK_DELAY_MS + (unsigned long)jitterAdder;

      anim_BlinkStagesStarted[0] = false;
      anim_BlinkStagesStarted[1] = false;
      anim_BlinkStagesStarted[2] = false;

      setLefthandEye_sprite(eye_lefthand_norm, colors, state);
      setRighthandEye_sprite(eye_righthand_norm, colors, state);

      pixels.show();

    }

  }

  // speaking routine
  if (anim_SpeakLastActivationMillis + millis() > ANIM_SPEAK_TIMEOUT_PERIOD_MILLIS) { anim_SpeakTriggered = false; }
  if (digitalRead(MICROPHONE_INPUT_PIN)) { anim_SpeakTriggered = true; anim_SpeakLastActivationMillis = millis(); }

  if (anim_SpeakNormEnabled && false) {

    switch (anim_SpeakState) {
      
      default:

      case SPEAKST_NORM_IDLE:

        if (!anim_SpeakScreenUpdated) {
          setCenter_sprite(center_norm, colors, ST_NORM);
          pixels.show();
          anim_SpeakScreenUpdated = true;
        }

        if (anim_SpeakTriggered) {
          anim_SpeakState = SPEAKST_NORM_FORWARD_ANIM;
          anim_SpeakScreenUpdated = false;
          anim_SpeakAnimStartedMillis = millis();
        }

        break;

      case SPEAKST_NORM_FORWARD_ANIM:


        if (!anim_SpeakScreenUpdated) {
          setCenter_sprite(center_speak1, colors, ST_NORM);
          pixels.show();
          anim_SpeakScreenUpdated = true;
        }

        if (millis() > anim_SpeakAnimStartedMillis + (ANIM_SPEAK_DURATION_MS / 2)) {

          setCenter_sprite(center_speak2, colors, ST_NORM);

          anim_SpeakScreenUpdated = false;
          anim_SpeakState = SPEAKST_NORM_HOLD_ON;

        }

        break;

      case SPEAKST_NORM_REVERSE_ANIM:

        break;

      case SPEAKST_NORM_HOLD_ON:

        if (!anim_SpeakTriggered) {
          anim_SpeakState = SPEAKST_NORM_IDLE;
          anim_SpeakScreenUpdated = false;
        }

        break;

    }

  }

}

void setRighthandEye_sprite(uint8_t sprite[64], double color[][2][3], uint8_t state) {
  
  for (int i = 0; i < EYE_SIZE; i++) {

    uint8_t r = sprite[i] * color[state][1][0];
    uint8_t g = sprite[i] * color[state][1][1];
    uint8_t b = sprite[i] * color[state][1][2];

    pixels.setPixelColor(i, pixels.Color(r, g, b));

  }

}

void setLefthandEye_sprite(uint8_t sprite[64], double color[][2][3], uint8_t state) {

    for (int i = 0; i < EYE_SIZE; i++) {

      uint8_t r = sprite[i] * color[state][1][0];
      uint8_t g = sprite[i] * color[state][1][1];
      uint8_t b = sprite[i] * color[state][1][2];

      pixels.setPixelColor(i + (EYE_SIZE + CENTER_SIZE), pixels.Color(r, g, b));

  }
  
}

void setCenter_sprite(uint8_t sprite[256], double color[][2][3], uint8_t state) {

  for (int i = 0; i < CENTER_SIZE; i++) {

    uint8_t r = sprite[i] * color[state][1][0];
    uint8_t g = sprite[i] * color[state][1][1];
    uint8_t b = sprite[i] * color[state][1][2];

    pixels.setPixelColor(i + EYE_SIZE, pixels.Color(r, g, b));

  }
  
}

void setAll_sprite(uint8_t spriteLefthandEye[64], uint8_t spriteCenter[256], uint8_t spriteRighthandEye[64], double color[][2][3], uint8_t state) {
  setRighthandEye_sprite(spriteRighthandEye, color, state);
  setCenter_sprite(spriteCenter, color, state);
  setLefthandEye_sprite(spriteLefthandEye, color, state);
  pixels.show();
}
