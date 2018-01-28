#include "CurieIMU.h"
#include <Wire.h>
#include <MMA7660.h>

MMA7660 accelemeter;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

  Serial.println("Initializing IMU device...");
  CurieIMU.begin();

  // Set accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);

  // Set up MMA accel
  accelemeter.init();
}

void loop() {
  // put your main code here, to run repeatedly

  // Scaled accelerometer values
  float ax, ay, az;

  // read measurement from device, scaled to configured range
  CurieIMU.readAccelerometerScaled(ax, ay, az);
  // Display values for x, y, z
  //Serial.println("a:\t");
  //Serial.print(ax);
  //Serial.print("\t");
  //Serial.print(ay);
  //Serial.print("\t");
  //Serial.print(az);
  //Serial.println();

  // a needs to be 1 +- .3

  int8_t x, y, z;
  float accel_x, accel_y, accel_z;
  accelemeter.getXYZ(&x, &y, &z);

 // Serial.print("x = ");
 // Serial.println(x + 20); 
 // Serial.print("y = ");
 // Serial.println(y);   
 // Serial.print("z = ");
 // Serial.println(z);

 // -15 < x < 25 
  
 // accelemeter.getAcceleration(&ax,&ay,&az);
 // Serial.println("accleration of X/Y/Z: ");
 // Serial.print(ax);
 // Serial.println(" g");
 // Serial.print(ay);
 // Serial.println(" g");
 // Serial.print(az);
 // Serial.println(" g");
 // Serial.println("*************");
 delay(500);
  x = x + 20;
  if(ax >= 0.5 && (x >= -3 && x <= 3 )){
    // Ergonomic
  } else {
    Serial.println("v");
  }
  
}
