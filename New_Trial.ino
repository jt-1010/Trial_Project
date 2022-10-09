#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
const int mpu = 0x68;
float ax,ay,az,xval,yval,zval,x2val,y2val,z2val;
int offset = 0;
float result,anglex,angley;
Servo s;
int servopin = 2;
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(mpu);
  Wire.write(0x6B);
  Wire.write(0);
  Serial.setTimeout(200);
  s.attach(servopin);
  Wire.endTransmission(true);
}

void loop() 
{
  Wire.beginTransmission(mpu);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(mpu,3*2,true);
  ax = Wire.read()<<8 | Wire.read(); 
  ay = Wire.read()<<8 | Wire.read(); 
  az = Wire.read()<<8 | Wire.read();
  Serial.print("    GX = "); Serial.print(az);
  Serial.print("    GY = "); Serial.print(ay);
  Serial.print("    GZ = "); Serial.print(az);
  xval = ax-16700;
  yval = ay-100;
  zval = az-16650;
  x2val = xval*xval;
  y2val = yval*yval;
  z2val = zval*zval;
  result = sqrt(x2val+z2val);
  result = yval/result;
  angley = atan(result);
  angley = angley*100;
  int temp = Serial.parseInt();
  if(temp > 0 && temp <= 180)
    offset = temp;
  if(temp == -1)
    offset = 0;
  Serial.print("    Angle = "); Serial.print(angley);
  Serial.print("    Offset = "); Serial.print(offset);
  if(angley < -10)
    if(angley > -70)
      s.write(100+(angley/1.55)+offset);
    else
      s.write(100+(angley/1.25)+offset);
  else if(angley > 10)
    if(angley < 35)
      s.write(100+(angley/3)+offset);
    else
      s.write(90+angley+offset);
  else if(angley < 10 && angley > -10)
  {
    s.write(100+offset);
  }
  Serial.println();
  delay(200);
}
