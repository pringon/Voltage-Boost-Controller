#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "com_interface.h"

void com_init() {
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  fd1=open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd1 == -1 ) {

    perror("open_port: Unable to open /dev/ttyUSB0 – ");
  } else {

    fcntl(fd1, F_SETFL,0);
    printf("Port 1 has been sucessfully opened and %d is the file description\n",fd1);
  }

  com_read(60);
}

void com_write(char *data) {
  wr = write(fd1, data, strlen(data));
}

void com_read(int bufferSize) {
  rd = read(fd1, buff, (size_t)bufferSize);
}

void com_end() {
  close(fd1);
}

reading get_readings() {
  com_read(60);
  buffer = strchr(buff, 'b');

  buffptr = strtok(buffer+1, "\t");
  info.adc = atoi(buffptr);

  buffptr = strtok(NULL, "\t");
  info.duty_cycle = atof(buffptr);

  buffptr = strtok(NULL, "\t");
  info.v_curr = atof(buffptr);

  buffptr = strtok(NULL, "\t");
  info.v_target = atof(buffptr);

  return info;
}

double read_voltage() {
  com_read(60);
  buffer = strchr(buff, 'b');

  buffptr = strtok(buffer+1, "\t");
  buffptr = strtok(NULL, "\t");
  buffptr = strtok(NULL, "\t");

  return atof(buffptr);
}

void write_voltage(double voltage) {
  char string_voltage[8];
  sprintf(string_voltage, "s%2.1fd", voltage);
  com_write(string_voltage);
}
