#include <PID_v1.h>

double pot_esq = 0, pot_dir = 0;
double gyro = 0;
double Kep = 1, Kei = 0, Ked = 0;
double Kdp = 1, Kdi = 0, Kdd = 0;
double setpoint = 0;

PID PID_ESQ(&gyro, &pot_esq, &setpoint, Kep, Kei, Ked, REVERSE);
PID PID_DIR(&gyro, &pot_dir, &setpoint, Kdp, Kdi, Kdd, DIRECT);

void Setup_PID()
{
  update_gyro();
  setpoint = get_gyro();
  PID_ESQ.SetOutputLimits(MIN_POT_PID, MAX_POT_PID);
  PID_DIR.SetOutputLimits(MIN_POT_PID, MAX_POT_PID);
  PID_ESQ.SetSampleTime(90);
  PID_DIR.SetSampleTime(90);
  PID_ESQ.SetMode(AUTOMATIC);
  PID_DIR.SetMode(AUTOMATIC);
}

void update_pid()
{
  update_gyro();
  gyro = get_gyro();
  PID_ESQ.Compute();
  PID_DIR.Compute();
}

double potesq()
{
  return pot_esq;
}

double potdir()
{
  return pot_dir;
}