#include <PID_v1.h>

double pot_esq = 0, pot_dir = 0;
double gyro = 0;
double Kep = 1, Kei = 0.01, Ked = 0.4;
double Kdp = 1, Kdi = 0.01, Kdd = 0.4;
double setpoint = 0;

PID PID_ESQ(&gyro, &pot_esq, &setpoint, Kep, Kei, Ked, REVERSE);
PID PID_DIR(&gyro, &pot_dir, &setpoint, Kdp, Kdi, Kdd, DIRECT);

void Setup_PID()
{
  update_gyro();
  setpoint = get_gyro();
  PID_ESQ.SetOutputLimits(MIN_POT_PID, MAX_POT_PID);
  PID_DIR.SetOutputLimits(MIN_POT_PID, MAX_POT_PID);
  PID_ESQ.SetSampleTime(100);
  PID_DIR.SetSampleTime(100);
  PID_ESQ.SetMode(AUTOMATIC);
  PID_DIR.SetMode(AUTOMATIC);
}

void turn_right () {
  setpoint += 45;
}
void turn_left () {
  setpoint -= 45;
}

void update_pid()
{
  update_gyro();
  gyro = get_gyro();
  PID_ESQ.Compute();
  update_gyro();
  PID_DIR.Compute();
  update_gyro();
}

double potesq()
{
  return pot_esq;
}

double potdir()
{
  return pot_dir;
}
