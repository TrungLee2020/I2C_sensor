#ifndef SENSOR_H
#define SENSOR_H

#define I2C_DEVICE "/dev/i2c-2"
#define I2C_ADDR 0x52 
#define XSHUT_PIN "/sys/class/gpio/gpio42/value"


struct sensor_config
{
    /* data */
    int i2c_bus;
    int sensor_addr;
    char* xshut_pin;
} SensorConfig;

void set_SensorConfig(SensorConfig *config, int i2c_bus, int sensor_addr, char* xshut_pin);
void active_sensor();
void read_data_sensor(SensorConfig *config);

#endif 