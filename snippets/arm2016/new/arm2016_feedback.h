#ifndef ARM2016_FEEDBACK
#define ARM2016_FEEDBACK

#include "arm2016_vars.h"

int readPosition(int id);
int median(int arset[], int n);
double calculateVelocity(int id);

void updateFeedback() {
	// update g_position pointer
	--g_position;
	if (g_position < g_position_log) {
		g_position = g_position_log + POSITION_LOG_DEPTH - 1;
	}
	// update the g_position and velocity of each of the motors
	for(int i = 0; i < NUM_MOCS; ++i) {
		if(i == BASE_MOC) {
			(*g_position)[i] += g_base_counter;
			g_base_counter = 0;
		}
		if(PINS_AI[i]) { // if we have feedback for this motor
			// Read g_position
			(*g_position)[i] = readPosition(i);
			// Calculate velocity
			g_velocity[i] = calculateVelocity(i);
		}
	}
}


int readPosition(int motor_id) {
	for(int i = 0; i < ANALOG_READ_NSAMPLES; ++i){
		analog_read_samples[i] = analogRead(PINS_AI[motor_id]);
	}
 // normalize position
 int pos = FB_NORM_SCALE[motor_id] * (median(analog_read_samples, ANALOG_READ_NSAMPLES) - FB_MIN_POS[motor_id]);
	return pos;
}

double calculateVelocity(int id) {
	int pos_idx = (g_position - g_position_log);
	int log_idx;
	double velo = 0;
	for(int d = 0; d < POSITION_LOG_DEPTH; ++d) {
		log_idx = (pos_idx + d) % POSITION_LOG_DEPTH;
		velo += term_coeffs[d] * g_position_log[log_idx][id];
	}
	return leading_coeff * velo;
}


int median(int arset[], int n)
{
	int i, j, l, m, x, t, k;
	k = n / 2;
	l = 0; m = n - 1;
	while (l<m) {
		x = arset[k];
		i = l;
		j = m;
		do {
			while (arset[i]<x) i++;
			while (x<arset[j]) j--;
			if (i <= j) {
				t = arset[i];
				arset[i] = arset[j];
				arset[j] = t;
				i++;
				j--;
			}
		} while (i <= j);
		if (j<k) l = i;
		if (k<i) m = j;
	}
	return arset[k];
}


void baseCounterInterupt() {
	if(g_duty_cycle[BASE_MOC] > 0) {
		++g_base_counter;
	} else if(g_duty_cycle[BASE_MOC] < 0){
		--g_base_counter;
	}
}

#endif
