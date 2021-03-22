
//uncomment to enable debug
//#define DEBUG

#define S0 A2
#define S1 A3
#define S2 A0
#define S3 A1
#define sensorOut 2
int frequency = 0;

//clickers
#define LCLICK 8
#define RCLICK 7

//motor pins
#define LMF 9
#define LMR 6
#define RMF 5
#define RMR 3

//error factor
#define ERRFACTOR 6

#define MPU_addr 0x68

//ultrasonic pins
#define LEFT 12
#define MIDDLE 11
#define RIGHT 10

//Wall distance
#define WALLDISTANCE 25

//define counter
#define MAXCOUNT 70

unsigned long previousMillis = 0;        // will store last time LED was updated

#define TILERUN 800

#define RAMPAMGLE 19

#define TILESIZE 30

//uncomment to enable back align
//#define BACKALIGN

//define
int accX;
int accY;
int accZ;
int GyX;
int GyY;
int GyZ;
int accCX;
int accCY;
int accCZ;
int CGyX;
int CGyY;
int CGyZ;
float pitch = 0;
float roll = 0;
float yaw = 0;
