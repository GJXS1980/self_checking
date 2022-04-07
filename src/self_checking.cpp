#include "ros/ros.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string> 
#include <iostream>
#include <std_msgs/Float64.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Int32.h>
#include <pthread.h>
#include <geometry_msgs/TwistStamped.h>
#include <mutex>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "std_msgs/String.h"

using namespace std;

string playradar_path;
string playprecamera_path;
string playpostcamera_path;
string playstm32_path;
string playultrasonic_path;
string plaympu_path;
string playpower_low_path;
string playpower_path;

int search_devfile(char *path,char *dev)
{
    struct dirent **name_list;
    int n = scandir(path,&name_list,0,alphasort);	
    if (n > 0)
    {
	int index = 0;
        while(index < n)
	{

	    if (strstr(dev,name_list[index]->d_name))
	    {
       		free(name_list[index++]);
                free(name_list);
		return 0;
            }
       		free(name_list[index++]);
	}
      
       free(name_list);
    }

   return 1;
}


struct Sensor
{
	float Ultrasonic_data[3];    
	float imu_sensor;
	float robot_power;
        
}Sensor_DATA;

void Ultrasonic_Data_Callback(const std_msgs::Float32MultiArray::ConstPtr& data)
{
	Sensor_DATA.Ultrasonic_data[0]  = data->data[0];
	Sensor_DATA.Ultrasonic_data[1]  = data->data[1];
}

void Back_Ultrasonic_Data(const std_msgs::Float32MultiArray::ConstPtr& data)
{

        Sensor_DATA.robot_power= data->data[0];
	Sensor_DATA.Ultrasonic_data[2]  = data->data[2];

}

void IMU_Data_Callback(const std_msgs::Float64::ConstPtr& data)
{
	Sensor_DATA.imu_sensor = data->data; 
}



// void  my_player(int num)
// {

//      const char *mplayer[] = {       "mplayer /home/hgrobot/arv_ws/src/self_checking/"," < /dev/null > /dev/null 2>check_erro.log "};
//      const char *voice_tip[]={       "voice/arv_radar.wav","voice/arv_precamera.wav","voice/arv_postcamera.wav",
//                                      "voice/arv_stm32.wav","voice/arv_ultrasonic.wav",
//                                      "voice/arv_mpu.wav","voice/power_low.wav","voice/power.wav"};
//      char check_tip[200] = {0};

//      strcpy(check_tip,mplayer[0]);
//      strcat(check_tip,voice_tip[num]);
//      strcat(check_tip,mplayer[1]);     
//      system(check_tip);
//      usleep(1000*500);

// }


// 语音播报函数
void  warning_player(int num)
{
	ros::NodeHandle nh("~");    //用于launch文件传递参数

   //  雷达
	nh.param("playradar_path", playradar_path, std::string("play ./arv_radar.wav"));    //从launch文件获取参数
	const char *playradar = playradar_path.data();

   //  前摄像头
	nh.param("playprecamera_path", playprecamera_path, std::string("play ./precamera.wav"));    //从launch文件获取参数
	const char *playprecamera = playprecamera_path.data();

   //  后摄像头
	nh.param("playpostcamera_path", playpostcamera_path, std::string("play ./postcamera.wav"));    //从launch文件获取参数
	const char *playpostcamera = playpostcamera_path.data();

   //  下位机
	nh.param("playstm32_path", playstm32_path, std::string("play ./stm32.wav"));    //从launch文件获取参数
	const char *playstm32 = playstm32_path.data();

   //  超声波
	nh.param("playultrasonic_path", playultrasonic_path, std::string("play ./ultrasonic.wav"));    //从launch文件获取参数
	const char *playultrasonic = playultrasonic_path.data();

   //  陀螺仪
	nh.param("plaympu_path", plaympu_path, std::string("play ./mpu.wav"));    //从launch文件获取参数
	const char *plaympu = plaympu_path.data();

   //  底电量
	nh.param("playpower_low_path", playpower_low_path, std::string("play ./power_low.wav"));    //从launch文件获取参数
	const char *playpower_low = playpower_low_path.data();

   //  电源
	nh.param("playpower_path", playpower_path, std::string("play ./power.wav"));    //从launch文件获取参数
	const char *playpower = playpower_path.data();

	switch(num)
	{
		case 0:
               	system(playradar);
	        	usleep(1000*1000);				
               	break;
		case 1:
               	system(playprecamera);
	            usleep(1000*1000);				
               	break;
		case 2:
               	system(playpostcamera);
	            usleep(1000*1000);				
               	break;
		case 3:
               	system(playstm32);
	            usleep(1000*1000);				
               	break;
		case 4:
               	system(playultrasonic);
	            usleep(1000*1000);				
               	break;
		case 5:
               	system(plaympu);
	            usleep(1000*1000);				
               	break;
		case 6:
               	system(playpower_low);
	            usleep(1000*1000);				
               	break;
		case 7:
               	system(playpower);
	            usleep(1000*1000);				
               	break;
		default:
				break;
	}
}


/*
1.检测激光雷达 
2.检测前后摄像头
3.检测传感器连接
 3.1 读取传感器数据

*/
void  read_sensor_data(void)
{
   if(search_devfile("/dev/","radar"))
   {
		warning_player(0);
   }	

   if(search_devfile("/dev/","prep_camera"))
   {
		warning_player(1);
   }	

   if(search_devfile("/dev/","post_camera"))
   {
		warning_player(2);
   }	

   if(search_devfile("/dev/","STM32_UART"))
   {
		warning_player(3);
   }
   else
   {
	   if(Sensor_DATA.Ultrasonic_data[0] == 6.5535f)
			warning_player(4);   	
		
		if(Sensor_DATA.Ultrasonic_data[1] == 6.5535f)
			warning_player(4);

		if(Sensor_DATA.Ultrasonic_data[2] == 0.0f)
			warning_player(4);

		if(Sensor_DATA.imu_sensor == 0.0f)
         	warning_player(5);
         
		if(Sensor_DATA.robot_power == 0.0f)
         	warning_player(7);

		else if(Sensor_DATA.robot_power < 45.0f)
			warning_player(6);   
   }		

}

void  claer_sensor_data(void)
{
	Sensor_DATA.Ultrasonic_data[0] = 0;
	Sensor_DATA.Ultrasonic_data[1] = 0;
	Sensor_DATA.Ultrasonic_data[2] = 0;

   	Sensor_DATA.robot_power = 0;
	Sensor_DATA.imu_sensor = 0;
}

 
int main(int argc,char **argv)
{
	ros::init(argc, argv, "SELF_CHECK_NODE");
	ros::NodeHandle n;		//创建句柄
	ros::NodeHandle nh("~");	

   	std::string playradar_path;
   	std::string playprecamera_path;
   	std::string playpostcamera_path;
   	std::string playstm32_path;
   	std::string playultrasonic_path;
   	std::string plaympu_path;
   	std::string playpower_low_path;
   	std::string playpower_path;

	ros::Subscriber Ultrasonic_Sub 	= n.subscribe("/Ult_Sensor_Data", 10, &Ultrasonic_Data_Callback);
	ros::Subscriber IMU_Sub 	= n.subscribe("/IMU_Sensor_Data", 10, &IMU_Data_Callback);
	ros::Subscriber back_Ultrasonic_Sub = n.subscribe("/Power_And_Distance", 10,&Back_Ultrasonic_Data);
   	printf("正在实时运行自检程序...\r\n");

	nh.param("playradar_path", playradar_path, std::string("play ./arv_radar.wav"));    //从launch文件获取参数
	nh.param("playprecamera_path", playprecamera_path, std::string("play ./precamera.wav"));    //从launch文件获取参数
	nh.param("playpostcamera_path", playpostcamera_path, std::string("play ./postcamera.wav"));    //从launch文件获取参数
	nh.param("playstm32_path", playstm32_path, std::string("play ./stm32.wav"));    //从launch文件获取参数
	nh.param("playultrasonic_path", playultrasonic_path, std::string("play ./ultrasonic.wav"));    //从launch文件获取参数
	nh.param("plaympu_path", plaympu_path, std::string("play ./mpu.wav"));    //从launch文件获取参数
	nh.param("playpower_low_path", playpower_low_path, std::string("play ./power_low.wav"));    //从launch文件获取参数
	nh.param("playpower_path", playpower_path, std::string("play ./power.wav"));    //从launch文件获取参数

   unsigned int conut = 50;   

   while(ros::ok())
   {
      if(conut % 100 == 0)
      {
         read_sensor_data();
         claer_sensor_data();
      }	
      conut++;
      ros::spinOnce();//循环等待回调函数
      usleep(1000*100);  //100 ms
   }
  
}





