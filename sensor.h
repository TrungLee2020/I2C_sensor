#ifndef SENSOR_H
#define SENSOR_H

#define I2C_BUS "/dev/i2c-2"
#define SENSOR_ADDR 0x52 
#define XSHUT_PIN "/sys/class/gpio/gpio40/value"


struct sensor_config
{
    int i2c_bus = I2C_BUS;
    int sensor_addr = SENSOR_ADDR;
    char* xshut_pin = XSHUT_PIN;
} Sensor_Config;

// void set_Sensor(Sensor_Config *config, int i2c_bus, int sensor_addr, char* xshut_pin);
void active_sensor(Sensor_Config *config);
void read_data_sensor(Sensor_Config *config);

#endif 