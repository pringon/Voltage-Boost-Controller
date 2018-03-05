#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

typedef struct {
  float v_curr, v_target, duty_cycle;
  int adc;
} reading;

int fd1, fd2;
char buff[62], *buffer,*buffptr;
int wr,rd,nbytes,tries;
reading info;

void com_write(char *data);
void com_read(int bufferSize);
void com_end();
reading get_readings();

int main() {

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  fd1=open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd1 == -1 ) {

    perror("open_port: Unable to open /dev/ttyS0 – ");
  } else {

    fcntl(fd1, F_SETFL,0);
    printf("Port 1 has been sucessfully opened and %d is the file description\n",fd1);
  }

  com_read(60);
  char voltage[8];
  char voltage_setter[10];
  char cmd = 'a';
  while(cmd != 'g') {
    printf("Select a command: ");
    scanf("%c", &cmd);
    if(cmd == 'p') {
      info = get_readings();

      printf("Current voltage: %5.3f\n", info.v_curr);
      printf("Target voltage: %5.3f\n", info.v_target);
      printf("Duty cycle: %5.3f\n", info.duty_cycle);
      printf("ADC: %d\n", info.adc);
    } else if(cmd == 's')  {

      printf("Enter your target voltage: ");
      scanf("%s", voltage);
      voltage_setter[0] = 's';
      voltage_setter[1] = 0;
      strcat(voltage_setter, voltage);
      strcat(voltage_setter, "d");
      printf("%s", voltage_setter);
      com_write(voltage_setter);
    }
    printf("Press another key!\n");
  }
  com_end();

  return 0;
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
