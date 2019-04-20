#ifndef _GENERATOR_H
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;
#define LOGLINENUMBER 100;
class generator
{
  public:
    generator();
    ~generator();
    void generate(int, int interval = 1);

  private:
    string mod[10] = {"JTAG", "SSD", "RADAR", "UART", "CAM", "GPS", "LIDAR", "HDMI", "POWER", "ETH"};
    string error[10] = {"NET_ERRROR", "NET_OK", "GPS_ERROR", "GPS_OK", "IMU_ERROR", "IMU_OK", "SENSOR_ERROR", "SENSRO_OK", "CAN_ERROR", "CAN_OK"};
    ofstream outfile;
};
#endif // !_GENERATOR_H