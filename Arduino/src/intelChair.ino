#include <MMA7660.h>
#include <Wire.h>

#include <CurieIMU.h>
#include <MadgwickAHRS.h>
MMA7660 accelero;

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;
float angle;

void setup() {
  Serial.begin(9600);
  accelero.init();

  // start the IMU and filter
  CurieIMU.begin();
  CurieIMU.setGyroRate(15);
  CurieIMU.setAccelerometerRate(15);
  filter.begin(15);
  
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  // Set the gyroscope range to 250 degrees/second
  CurieIMU.setGyroRange(250);
  
  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 15;
  microsPrevious = micros();
}

void loop() {
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, back, heading;
  unsigned long microsNow;  

  float axx,ayy,azz;

  // check if it's time to read data and update the filter
  microsNow = micros();
  
  if (microsNow - microsPrevious >= microsPerReading) {
    // read raw data from CurieIMU
    CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);
    accelero.getAcceleration(&axx,&ayy,&azz);
    // convert from raw data to gravity and degrees/second units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);

    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);
    angle = atan2(axx,azz)*180/3.14;
    
    back = filter.getPitch();
    //Serial.print("arduinoAngle: ");
    //Serial.println(back);
    //Serial.print("accleroAngle: ");
    //Serial.println(angle);
    

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  }
  
  if ((abs(back) > 49 && abs(back) < 52) || ((abs(angle) > 87 && abs(angle) < 92))){
    Serial.println("v");
     //delay(1000);
  }

}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
