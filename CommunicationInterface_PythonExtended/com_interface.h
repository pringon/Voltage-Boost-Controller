

typedef struct {
  float v_curr, v_target, duty_cycle;
  int adc;
} reading;

int fd1;
char buff[62], *buffer,*buffptr;
int wr,rd;
reading info;

void com_init();
void com_write(char *data);
void com_read(int bufferSize);
void com_end();
reading get_readings();
double read_voltage();
void write_voltage(double voltage);
