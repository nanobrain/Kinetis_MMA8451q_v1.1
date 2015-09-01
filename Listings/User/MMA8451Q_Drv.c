#include "fsl_device_registers.h"
#include "I2C.h"
#include "MMA8451Q_Drv.h"
#include "Low_Level_Init.h"
																																									//TODO: Write default init function ( faster )
void Init_MMA8451q(Accel_Status* Status)
{
	//|////////////////////0x2A CTRL_REG1 Register (Read/Write)////////////////////|//
	//|////////////////////////////////////////////////////////////////////////////|//
	//|		 Bit 7	 | 		Bit 6 	| Bit 5 |  Bit 4 | Bit 3 |  Bit 2 | Bit 1 | Bit 0  |//
	//|	ASLP_RATE1 | ASLP_RATE0 |  DR2  |   DR1  |  DR0  | LNOISE | FREAD | ACTIVE |//
	//|////////////////////////////////////////////////////////////////////////////|//
	
	//|////////////////////0x2E XYZ_DATA_CFG Register (Read/Write)/////////////////|//
	//|////////////////////////////////////////////////////////////////////////////|//
	//| 	 Bit 7  |  	Bit 6  |  Bit 5  |  Bit 4  | Bit 3 |  Bit 2 | Bit 1 |  Bit 0 |//
	//| 	------  |    00    |    0    | HPF_OUT |   0   |   0    | FS1   |  FS0   |//
	//|////////////////////////////////////////////////////////////////////////////|//
	Accel_Set_Data_Rate(Status);
	Accel_Noise(Status);
	Accel_Actv_Mode(Status);
}

uint8_t Accel_ReadReg(uint8_t registerAddr){
  uint8_t data;

  i2c_enable(I2C_ACCEL);

  i2c_start(I2C_ACCEL);                           // S
  i2c_write_byte(I2C_ACCEL, MMA8451Q_ADDR_WRITE); // Slave address + W
  i2c_wait(I2C_ACCEL);
  i2c_write_byte(I2C_ACCEL, registerAddr);        // Select register to read
  i2c_wait(I2C_ACCEL);
  i2c_restart(I2C_ACCEL);                         // RS
  i2c_write_byte(I2C_ACCEL, MMA8451Q_ADDR_READ);  // Slave address + R
  i2c_wait(I2C_ACCEL);
  i2c_set_rx_mode(I2C_ACCEL);                     // Mode: Rx
  i2c_disable_ack(I2C_ACCEL);                     // NACK
  data = i2c_read_byte(I2C_ACCEL);                // Clear Data register, also initiate read sequence
  i2c_wait(I2C_ACCEL);
  i2c_stop(I2C_ACCEL);                            // P
  data = i2c_read_byte(I2C_ACCEL);                // Get value

  i2c_disable(I2C_ACCEL);

  return data;
}

void Accel_WriteReg(uint8_t registerAddr, uint8_t registerData){
  i2c_enable(I2C_ACCEL);

  i2c_start(I2C_ACCEL);                           // S
  i2c_write_byte(I2C_ACCEL, MMA8451Q_ADDR_WRITE); // Slave address + W
  i2c_wait(I2C_ACCEL);
  i2c_write_byte(I2C_ACCEL, registerAddr);        // Select register for write
  i2c_wait(I2C_ACCEL);
  i2c_write_byte(I2C_ACCEL, registerData);        // Data
  i2c_wait(I2C_ACCEL);
  i2c_stop(I2C_ACCEL);                            // P

  i2c_disable(I2C_ACCEL);
}

int16_t Accel_ReadAxis(Axis a) {
  uint8_t regAddr;
  int16_t value;

  switch(a){
    case X:{
      regAddr = MMA8451_OUT_X_MSB;
      break;
    }
    case Y:{
      regAddr = MMA8451_OUT_Y_MSB;
      break;
    }
    case Z:{
      regAddr = MMA8451_OUT_Z_MSB;
      break;
    }
    default:
      break;
  }

  value = (Accel_ReadReg(regAddr) << 8);
  value +=(Accel_ReadReg(regAddr+1));
  value <<= 2;

  return value;
}


void Accel_Noise(Accel_Status* Status){
	uint8_t reg;
	Accel_Stb(Status);
	
	reg = Accel_ReadReg(MMA8451_CTRL_REG1);
	if(Status->LowNoise) Accel_WriteReg(MMA8451_CTRL_REG1, reg & LOW_NOISE_MASK);
	else Accel_WriteReg(MMA8451_CTRL_REG1,~reg | LOW_NOISE_MASK);
	
	Accel_Actv(Status);
}

void Accel_Stb(Accel_Status* Status){
	uint8_t reg;
	Status->Power = StandBy;
	reg = Accel_ReadReg(MMA8451_CTRL_REG1);
	Accel_WriteReg(MMA8451_CTRL_REG1,~reg | ACTIVE_MASK);
}

void Accel_Actv(Accel_Status* Status){
	uint8_t reg;
	Status->Power = Active;
	reg = Accel_ReadReg(MMA8451_CTRL_REG1);
	Accel_WriteReg(MMA8451_CTRL_REG1,reg & ACTIVE_MASK);
}

void Accel_Actv_Mode(Accel_Status* Status){
	uint8_t reg;
	
	Accel_Stb(Status);

	reg = Accel_ReadReg(MMA8451_XYZ_DATA_CFG);
	
	switch(Status->mode)
	{
		case M_2G:
		{
			Accel_WriteReg(MMA8451_XYZ_DATA_CFG, (reg & ~FS_MASK));
		} break;
		case M_4G:
		{
			Accel_WriteReg(MMA8451_XYZ_DATA_CFG, (reg & ~FS_MASK));
			Accel_WriteReg(MMA8451_XYZ_DATA_CFG, (reg | FS_4G_MASK));
		} break;
		case M_8G:
		{
			Accel_WriteReg(MMA8451_XYZ_DATA_CFG, (reg & ~FS_MASK));
			Accel_WriteReg(MMA8451_XYZ_DATA_CFG, (reg | FS_8G_MASK));
		} break;
		default: break;
	}
	
	Accel_Actv(Status);
}
void Accel_Set_Data_Rate(Accel_Status* Status){
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
	uint8_t reg;
	Accel_Stb(Status);																						// IMPORTANT:
	reg = Accel_ReadReg(MMA8451_CTRL_REG1);									// TODO: <-Check if that works at all
	Accel_WriteReg(MMA8451_CTRL_REG1,~reg & DR_MASK);				//
	if(Status->DataRate) Accel_WriteReg(MMA8451_CTRL_REG1,reg | Status->DataRate);
	Accel_Actv(Status);
}

																													//TODO: write other functions returning different types of result

char* Accel_s14FracOut(Accel_Status* Status, char* result, int16_t data)
{
	int32_t mantisa;
	uint8_t data_hi = data & 0xFF00;
	uint8_t data_lo = data & 0x00FF;
	
	if(data_hi > 0x7F)
	{
		result[0] = '-';
		data &= 0xFFFC;
		data = ~data + 1;
	}
	else result[0] = '+';
	
	
	switch(Status->mode)
	{
		////////////////////
		case M_2G:
		{
			result[1] = ((data & 0x4000) >> 6) + 48;
			data >>=2;
			if(data & 0x0001) mantisa +=FRAC_2d12;
		} break;
		////////////////////
		case M_4G:
		{
			result[1] = ((data & 0x6000) >> 5) + 48;
			data >>=3;
		} break;
		////////////////////
		case M_8G:
		{
			result[1] = ((data & 0x7000) >> 4) + 48;
			data >>=4;
			if(data & 0x0002) mantisa +=FRAC_2d11;
		} break;
		////////////////////
		default:{} break;
		////////////////////
	}
	
	result[2] = '.';
	if(data & 0x0800) mantisa +=FRAC_2d1;								// TODO: This can be faster
	if(data & 0x0400) mantisa +=FRAC_2d2;
	if(data & 0x0200) mantisa +=FRAC_2d3;
	if(data & 0x0100) mantisa +=FRAC_2d4;
	if(data & 0x0080) mantisa +=FRAC_2d5;
	if(data & 0x0040) mantisa +=FRAC_2d6;
	if(data & 0x0020) mantisa +=FRAC_2d7;
	if(data & 0x0010) mantisa +=FRAC_2d8;
	if(data & 0x0008) mantisa +=FRAC_2d9;
	if(data & 0x0004) mantisa +=FRAC_2d10;
	
	sShort2St(result+3, mantisa/10);
	
	return result;
	
}
