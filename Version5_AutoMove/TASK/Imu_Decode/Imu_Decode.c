#include "Imu_Decode.h" 
float EulerAngle_lastdata=0.0;
float Gyro_lastdata=0.0;
void IMU_Decode(u8 *pData)
{
    int i=0;
    for(i=0;i<78;i++)
    {
        imu_rawdata.Used_Data.byte[i]=pData[i+11];
    }
    Z_EulerAngle=imu_rawdata.Used_Data.IMU_Data[15];
    if(Z_EulerAngle>pi||Z_EulerAngle<-pi)
    {
        Z_EulerAngle=EulerAngle_lastdata;
    }
    Z_Gyro=imu_rawdata.Used_Data.IMU_Data[2];
    if(Z_Gyro>2000&&Z_Gyro<-2000)
    {
      
        Z_Gyro=Gyro_lastdata;
    }
    EulerAngle_lastdata=Z_EulerAngle;
    Gyro_lastdata=Z_Gyro;
}
