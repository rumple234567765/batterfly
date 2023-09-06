#include "subs.h"
#include "math.h"

SBUS_CH_Struct SBUS_CH;
uint16_t pwm_input[3] = {0};

const int DEADZONE = 20;

const int PWM_CHANGE = 500;
const int PWM_MID = 1500;
const float DECAY_INPUT=0.1;

const int AMP_CUTOFF = 10;

const float WAVE_INT = 150.0; // Half period 
const float FREQ = PI/WAVE_INT;
//?sbus????????
uint16_t update_sbus(uint16_t *buf)
{
    int i;
    if (buf[23] == 0)
    {
        SBUS_CH.ConnectState = 1;
        SBUS_CH.CH[1] = ((uint16_t)buf[ 1] >> 0 | ((int16_t)buf[ 2] << 8 )) & 0x07FF;
        SBUS_CH.CH[2] = ((uint16_t)buf[ 2] >> 3 | ((int16_t)buf[ 3] << 5 )) & 0x07FF;
        SBUS_CH.CH[3] = ((uint16_t)buf[ 3] >> 6 | ((int16_t)buf[ 4] << 2 ) | (int16_t)buf[ 5] << 10 ) & 0x07FF;
        SBUS_CH.CH[4] = ((int16_t)buf[ 5] >> 1 | ((int16_t)buf[ 6] << 7 )) & 0x07FF;
        SBUS_CH.CH[5] = ((int16_t)buf[ 6] >> 4 | ((int16_t)buf[ 7] << 4 )) & 0x07FF;
        SBUS_CH.CH[6] = ((int16_t)buf[ 7] >> 7 | ((int16_t)buf[ 8] << 1 ) | (int16_t)buf[9] << 9 ) & 0x07FF;
        SBUS_CH.CH[7] = ((int16_t)buf[ 9] >> 2 | ((int16_t)buf[10] << 6 )) & 0x07FF;
        SBUS_CH.CH[8] = ((int16_t)buf[10] >> 5 | ((int16_t)buf[11] << 3 )) & 0x07FF;
        SBUS_CH.CH[9] = ((int16_t)buf[12] << 0 | ((int16_t)buf[13] << 8 )) & 0x07FF;
        SBUS_CH.CH[10] = ((int16_t)buf[13] >> 3 | ((int16_t)buf[14] << 5 )) & 0x07FF;
        SBUS_CH.CH[11] = ((int16_t)buf[14] >> 6 | ((int16_t)buf[15] << 2 ) | (int16_t)buf[16] << 10 ) & 0x07FF;
        SBUS_CH.CH[12] = ((int16_t)buf[16] >> 1 | ((int16_t)buf[17] << 7 )) & 0x07FF;
        SBUS_CH.CH[13] = ((int16_t)buf[17] >> 4 | ((int16_t)buf[18] << 4 )) & 0x07FF;
        SBUS_CH.CH[14] = ((int16_t)buf[18] >> 7 | ((int16_t)buf[19] << 1 ) | (int16_t)buf[20] << 9 ) & 0x07FF;
        SBUS_CH.CH[15] = ((int16_t)buf[20] >> 2 | ((int16_t)buf[21] << 6 )) & 0x07FF;
        SBUS_CH.CH[16] = ((int16_t)buf[21] >> 5 | ((int16_t)buf[22] << 3 )) & 0x07FF;
        return 1;
    }
    else 
    {
        SBUS_CH.ConnectState = 0;
        return 0;
    }
}

uint16_t sbus_to_pwm(uint16_t sbus_value)
{
    float pwm;
    pwm = (float)SBUS_TARGET_MIN + (float)(sbus_value - SBUS_RANGE_MIN) * SBUS_SCALE_FACTOR;
    //                1000                                   300              1000/1400
    if (pwm > 2000) pwm = 2000;
    if (pwm < 1000) pwm = 1000;
    return (uint16_t)pwm;
}

void input_filter( void ) {
  for( int index = 0; index < 3; index += 1 ) {
		if((SBUS_CH.CH[index]-pwm_input[index])<DEADZONE && (SBUS_CH.CH[index]-pwm_input[index])>-DEADZONE);
		else
			pwm_input[index] = SBUS_CH.CH[index]*DECAY_INPUT + pwm_input[index]*( 1 - DECAY_INPUT );
  }
}

int amp_func( int input ) {
  if( input <= AMP_CUTOFF ) {
    return 0;
  } else {
    return input - AMP_CUTOFF;
  }
}

int constrain( int a , int b , int c )
{
	if(a<b)	a=b;
	if(a>c) a=c;
	return a;
}

float sin_wave( void ) 
{
  return sin( cnt*FREQ );
}

int flap_func( int amplitude, int offset ) {
  if( amplitude == 0 ) {
    return constrain( offset , -PWM_CHANGE, PWM_CHANGE );
  } else {
    return constrain( offset + sin_wave()*amplitude , -PWM_CHANGE, PWM_CHANGE );
  }
}
