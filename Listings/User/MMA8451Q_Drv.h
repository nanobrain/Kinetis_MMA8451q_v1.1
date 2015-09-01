/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define MMA8451_STATUS						0x00
#define MMA8451_OUT_X_MSB     		0x01
#define MMA8451_OUT_X_LSB     		0x02
#define MMA8451_OUT_Y_MSB     		0x03
#define MMA8451_OUT_Y_LSB     		0x04
#define MMA8451_OUT_Z_MSB     		0x05
#define MMA8451_OUT_Z_LSB     		0x06
//Reserved								0x07-0x08
#define MMA8451_F_SETUP						0x09
#define MMA8451_TRIG_CFG					0x0A
#define MMA8451_SYSMOD						0x0B
#define MMA8451_INT_SOURCE				0x0C
#define MMA8451_WHO_AM_I      		0x0D
#define MMA8451_XYZ_DATA_CFG			0x0E
#define MMA8451_HP_FILTER_CUTOFF	0x0F
#define MMA8451_PL_STATUS					0x10
#define MMA8451_PL_CFG						0x11
#define MMA8451_PL_COUNT					0x12
#define MMA8451_PL_BF_ZCOMP				0x13
#define MMA8451_P_L_THS_REG				0x14
#define MMA8451_FF_MT_CFG					0x15
#define MMA8451_FF_MTSRC					0x16
#define MMA8451_FF_MT_THS					0x17
#define MMA8451_FF_MT_COUNT				0x18
//Reserved								0x19-0x1C
#define MMA8451_TRANSIENT_CFG			0x1D
#define MMA8451_TRANSIENT_SCR			0x1E
#define MMA8451_CTRL_REG1     		0x2A
#define MMA8451_CTRL_REG2					0x2B
#define MMA8451_CTRL_REG3					0x2C
#define MMA8451_CTRL_REG4					0x2D
#define MMA8451_CTRL_REG5					0x2E
#define MMA8451_OFF_X							0x2F
#define MMA8451_OFF_Y							0x30
#define MMA8451_OFF_Z							0x31
//Reserved								0x40-0x7F
/////////////////////////////////////////////////////////
#define MMA8451Q_ADDR_WRITE   		0x3A //0x38
#define MMA8451Q_ADDR_READ    		0x3B //0x39
/////////////////////////////////////////////////////////
#define ACTIVE_MASK								0x01
#define FS_MASK										0x03
#define FS_4G_MASK								0x01
#define FS_8G_MASK								0x02
#define DR_MASK										0x03
#define I2C_ACCEL             I2C0
#define Accel_DR									0xBEEF
/////////////////////////////////////////////////////////
#define FRAC_2d1 									5000
#define FRAC_2d2 									2500
#define FRAC_2d3 									1250
#define FRAC_2d4 									625
#define FRAC_2d5 									313
#define FRAC_2d6 									156
#define FRAC_2d7 									78
#define FRAC_2d8 									39
#define FRAC_2d9 									20
#define FRAC_2d10 								10
#define FRAC_2d11 								5
#define FRAC_2d12 								2
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
typedef enum {
  X = 1,
  Y,
  Z
} Axis;

typedef enum {
	M_2G = 1,
	M_4G,
	M_8G
} Accel_Mode;

/*|	DR2	|	DR1	|	DR0	|  Rate   |
 *|  0  |  0  |  0  |	 800Hz  |
 *|  0  |  0  |  1  |  400Hz  |
 *|  0  |  1  |  0  |  200Hz  |
 *|  0  |  1  |  1  |  100Hz  |
 *|  1  |  0  |  0  |   50Hz  |
 *|  1  |  0  |  1  |  12.5Hz |
 *|  1  |  1  |  0  |  6.25Hz |
 *|  1  |  1  |  1  | 1.536Hz |
 */
typedef enum {
Hz800=0x00,
Hz400=0x10,
Hz200=0x20,
Hz100=0x30,
Hz50=0x40,
Hz12_5=0x50,
Hz6_25=0x60,
Hz1_536=0x70	
} Accel_dRate;

typedef enum {
	Active = 1,
	StandBy = 0
} Accel_Power;

typedef enum {
	Yes = 1,
	No = 0
} Accel_IRQ,Accel_LowNoise;

typedef struct
{
	Accel_Mode mode;					//2g,4g,8g
	Accel_dRate DataRate;			//Hz														//TODO: Fill up with all options
	Accel_Power Power;				//Active/StandBy								//TODO: Write init function
	Accel_IRQ IRQ;						//yes/no
	Accel_LowNoise LowNoise;	//yes/no
	//...
} Accel_Status;

#define mail_obj      16                                   // number of Message Queue Objects
typedef struct {                                           // object data type
  int16_t x_Value;
	int16_t y_Value;
	int16_t z_Value;
} mail_format;

uint8_t Accel_ReadReg(uint8_t registerAddr);
void Accel_WriteReg(uint8_t registerAddr, uint8_t registerData);
int16_t Accel_ReadAxis(Axis a);
void Accel_Stb(void);
void Accel_Actv(void);
void Accel_Set_Data_Rate(Accel_Status* Status);
void Accel_Actv_Mode(Accel_Status* Status);
char* Accel_s14FracOut(char* result, int16_t data);
