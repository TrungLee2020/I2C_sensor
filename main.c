#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "sensor.h"



void set_SensorConfig(SensorConfig *config, int i2c_bus, int sensor_addr, char* xshut_pin);
{
    config->i2c_bus = I2C_DEVICE;
    config->sensor_addr = I2C_ADDR;
    config->xshut_pin = XSHUT_PIN;
}
void active_sensor(){
    int fd = open(XSHUT_PIN, O_WRONLY);
    if (fd < 0)
    {
        perror("Failed to open GPIO Xshut pin.");
        exit(1);
        write(fd, "0", 1);
        usleep(10000);
        write(fd, "1", 1);
        usleep(10000);
        close(fd);
    }
}

void read_data_sensor(SensorConfig *config)
{
    unsigned char buf[2];
    buf[0] = 0x00;
    buf[1] = 0x01;

    // Mo I2C
    if((file = open(config->i2c_bus, O_RDWR)) < 0)
    {
        perror("Failed to open I2C bus");
        pthread_exit(NULL);
    }
    // Set dia chi
    if(ioctl(file, I2C_SLAVE, config->sensor_addr) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
        pthread_exit(NULL);
    }
    // doc du lieu
    while(1)
    {
        if(write(file, buf, 1) != 1)
        {
            perror("Failed to write to the i2c bus");
            pthread_exit(NULL);
        }
        usleep(10000); // Chờ 10ms
        if (read(file, buf, 2) != 2)
        {
            perror("Failed to read to the i2c bus");
            pthread_exit(NULL);
        }
        // int h = (buf[0] << 8) | buf[1];
        int h = (buf[0] << 8) + buf[1];
        print("Height: %d\n", h);
    }
    close(file);
}

int main()
{
    pthread thread;

    if(pthread_create(&thread, NULL, read_data_sensor, NULL) != 0)
    {
        perror("Failed to create thread");
        return 1;
    }
    pthread_join(thread, NULL);
    return 0;
}