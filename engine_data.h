#ifndef ENGINE_DATA_H
#define ENGINE_DATA_H

#include "definitions.h"
#include "error_handling.h"
#include "serial.h"
#include "I2Cdev.h"

int readED(int fd_front_controls, en_data& enData);

#endif

