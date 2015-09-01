void i2c_set_rx_mode(I2C_Type * p);
void i2c_set_tx_mode(I2C_Type * p);
void i2c_set_master_mode(I2C_Type * p);
void i2c_set_slave_mode(I2C_Type * p);
void i2c_start(I2C_Type * p);
void i2c_restart(I2C_Type * p);
void i2c_stop(I2C_Type * p);
void i2c_write_byte(I2C_Type * p, uint8_t data);
uint8_t i2c_read_byte(I2C_Type * p);
uint8_t i2c_wait(I2C_Type * p);
void i2c_set_rx_mode(I2C_Type * p);
void i2c_set_tx_mode(I2C_Type * p);
void i2c_disable_ack(I2C_Type * p);
void i2c_disable(I2C_Type * p);
void i2c_enable(I2C_Type * p);

void I2C0_Init(void);
void I2C1_Init(void);
