#include "fsl_device_registers.h"
#include "I2C.h"

void i2c_start(I2C_Type * p){
  p->C1 |= I2C_C1_TX_MASK;
  p->C1 |= I2C_C1_MST_MASK;
}

void i2c_restart(I2C_Type * p){
  p->C1 |= I2C_C1_RSTA_MASK;
}

void i2c_stop(I2C_Type * p){
  p->C1 &= ~I2C_C1_MST_MASK;
  p->C1 &= ~I2C_C1_TX_MASK;
}

void i2c_write_byte(I2C_Type * p, uint8_t data){
  p->D = data;                            // Zapis danych do wyslania
}

uint8_t i2c_wait(I2C_Type * p){
  while( (p->S & I2C_S_IICIF_MASK) == 0 ){}

  // Check if ACK received, if not return 0
  if( (p->S & I2C_S_RXAK_MASK) != 0 ){
    return 0;                             // Odebrano NACK
  }

  p->S |= I2C_S_IICIF_MASK;               // Wyzerowanie flagi IICIF
  return 1;
}

uint8_t i2c_read_byte(I2C_Type * p){
  return p->D;                            // Pobranie przyjetych danych
}

void i2c_enable(I2C_Type * p){
  p->C1 |= I2C_C1_IICEN_MASK;             // Wlaczenie I2C
}

void i2c_disable(I2C_Type * p){
  p->C1 &= ~I2C_C1_IICEN_MASK;            // Wylaczenie I2C
}

void i2c_set_master_mode(I2C_Type * p){
  p->C1 |= I2C_C1_MST_MASK;               // I2C w tryb Master
}

void i2c_set_slave_mode(I2C_Type * p){
  p->C1 &= ~I2C_C1_MST_MASK;              // I2C w tryb Slave
}

void i2c_set_rx_mode(I2C_Type * p){
  p->C1 &= ~I2C_C1_TX_MASK;               // I2C w tryb odbioru
}

void i2c_set_tx_mode(I2C_Type * p){
  p->C1 |= I2C_C1_TX_MASK;                // I2C w tryb nadawania
}

void i2c_disable_ack(I2C_Type * p){
  p->C1 |= I2C_C1_TXAK_MASK;              // Jezeli modul jest w trybie Rx: zostanie wyslany NACK
}

//--------------------------------------------------------------------------

void I2C0_Init(void){
  SIM->SCGC5  |= SIM_SCGC5_PORTE_MASK;  // Wlaczenie sygnalu taktujacego dla portu E
  SIM->SCGC4  |= SIM_SCGC4_I2C0_MASK;   // Wlaczenie sygnalu taktujacego dla modulu I2C0
  
  PORTE->PCR[24] = PORT_PCR_MUX(5) |    // Funkcja alternatywna: 5
                   PORT_PCR_DSE_MASK;   // PTE24 (I2C0_SCL)
  PORTE->PCR[25] = PORT_PCR_MUX(5) |    // Funkcja alternatywna: 5
                   PORT_PCR_DSE_MASK;   // PTE25 (I2C0_SDA)

  I2C0->F   = 0x14;                     // baudrate: 300kHz
  //I2C0->C1 |= I2C_C1_IICEN_MASK;        // Wlaczenie I2C0
}

void I2C1_Init(void){
  SIM->SCGC5  |= SIM_SCGC5_PORTE_MASK;  // Wlaczenie zegara dla portu E
  SIM->SCGC4  |= SIM_SCGC4_I2C1_MASK;   // Wlaczenie zegara dla modulu I2C1
  
  PORTE->PCR[0] = PORT_PCR_MUX(6) |     // Funkcja alternatywna: 6
                  PORT_PCR_DSE_MASK;    // PTE0 (I2C1_SDA)
  PORTE->PCR[1] = PORT_PCR_MUX(6) |     // Funkcja alternatywna: 6
                  PORT_PCR_DSE_MASK;    // PTE1 (I2C1_SCL)

  I2C1->F   = 0x14;                     // baudrate: 300kHz
  //I2C1->C1 |= I2C_C1_IICEN_MASK;        // Wlaczenie I2C1
}
