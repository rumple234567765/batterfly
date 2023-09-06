#ifndef __subs_h
#define __subs_h

#include <stdint.h>
#define PI 3.1415926
#define SBUS_TARGET_MIN 1000
#define SBUS_RANGE_MIN 300
#define SBUS_SCALE_FACTOR 1000/1400

extern int cnt;

typedef struct
{
	uint16_t CH[20];//??1??
	uint8_t ConnectState;//??????????? 0=???,1=????
}SBUS_CH_Struct;

uint16_t update_sbus(uint16_t *buf);
uint16_t sbus_to_pwm(uint16_t sbus_value);
void input_filter( void );
int amp_func( int input );
int constrain( int a , int b , int c );
int flap_func( int amplitude, int offset );

#endif