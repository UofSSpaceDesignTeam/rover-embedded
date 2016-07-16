#ifndef ARM2016_SERDBG
#define ARM2016_SERDBG
#include <stdlib.h>
#include <string.h>
#include <cstdlib>

#define ALL_MOTORS 10

enum ECommand { SET, MOVE, STOP, ERROR, READ };

void ReadSerialCommand(char command_cstr[]) {
	ECommand command;
	char* word;
	int motor_id = -1;
	char delim[] = " ";
	word = strtok(command_cstr, delim);
	// Begin parsing the command
	if 		(strcmp(word, "set") == 0) 	command = SET;
	else if (strcmp(word, "move") == 0) 	command = MOVE;
	else if (strcmp(word, "stop") == 0) 	command = STOP;
	else if (strcmp(word, "r") == 0)		command = READ;
	else 								command = ERROR;
	// Parse which motor
	word = strtok(NULL, " ");
	if (word != NULL) {
		if 		(strcmp(word, "0")  == 0) 	motor_id = 0;
		else if (strcmp(word, "1")  == 0) 	motor_id = 1;
		else if (strcmp(word, "2")  == 0) 	motor_id = 2;
		else if (strcmp(word, "3")  == 0) 	motor_id = 3;
		else if (strcmp(word, "4")  == 0) 	motor_id = 4;
		else if (strcmp(word, "5")  == 0) 	motor_id = 5;
		else if (strcmp(word, "all") == 0 && command == STOP) {
			motor_id = ALL_MOTORS;
		}
		else command = ERROR;
	} else if(command == STOP) {
		motor_id = ALL_MOTORS;
	} else command = ERROR;
	// Parse additional argument
	word = strtok(NULL, " ");
	double arg = 0;
	if (word != NULL) {
		arg = strtod(word, NULL);
	}
	// Carry out the commands
	switch (command) {
	case SET:
		g_ramping_enabled = false;
		g_duty_cycle[motor_id] = arg;
		Serial.print("Setting duty-cycle of m");
		Serial.print(motor_id);
		Serial.print(": value=");
		Serial.println(arg);
		break;
	case READ:
		Serial.println((*g_position)[motor_id]);
		break;
	case MOVE:
		smartMove(motor_id, arg);
    DCManager_init(motor_id);
		Serial.print("Initializing movement of m");
		Serial.print(motor_id);
		Serial.print(": destination=");
		Serial.println(arg);
		break;
	case ERROR:
		Serial.print("got ");
		Serial.println(String(command_cstr));
		Serial.println("ERROR: There was an error in your arguments!");
		motor_id = ALL_MOTORS;
	case STOP:
		if(motor_id != ALL_MOTORS) {
			g_duty_cycle[motor_id] = 0;
      g_ramping_enabled = false;
			Serial.print("Stopping m");
			Serial.println(motor_id);
		} else {
      g_ramping_enabled = false;
			g_duty_cycle[0] = 0;
			g_duty_cycle[1] = 0;
			g_duty_cycle[2] = 0;
			g_duty_cycle[3] = 0;
			g_duty_cycle[4] = 0;
			g_duty_cycle[5] = 0;
      DCM_stages[0] = DONE;
      DCM_stages[1] = DONE;
      DCM_stages[2] = DONE;
      DCM_stages[3] = DONE;
      DCM_stages[4] = DONE;
      DCM_stages[5] = DONE;
			Serial.println("Stopping all motors");
		}
		break;
	}

}

#endif
