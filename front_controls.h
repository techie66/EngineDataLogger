#ifndef FR_CONT_H
#define FR_CONT_H

#include "definitions.h"
#include "error_handling.h"
#include "serial.h"

int readFC(int& fd_front_controls, fc_data& fcData);
int writeFC(int fd_front_controls, fc_data& fcData);
char *exCmd_bin(uint8_t cmd);

#endif
