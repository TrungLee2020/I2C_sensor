#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>

#define I2C_DEVICE "/dev/i2c-2"
#define I2C_ADDR 0x52 


void read_data_sensor()
{
    int file;
    char buffer[2];

    // Mo I2C
    if((file = open(I2C_DEVICE, O_RDWR)) < 0)
    {
        perror("Failed to open I2C bus");
        pthread_exit(NULL);
    }
    // Set dia chi
    if(ioctl(file, I2C_SLAVE, I2C_ADDR) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
        pthread_exit(NULL);
    }
    // doc du lieu
    while(1)
    {
        if(write(file, buffer, 1) != 1)
        {
            perror("Failed to write to the i2c bus");
            pthread_exit(NULL);
        }
        usleep(10000); // Chờ 10ms
        if (read(file, buffer, 2) != 2)
        {
            perror("Failed to read to the i2c bus");
            pthread_exit(NULL);
        }
        int h = (buffer[0] << 8) | buffer[1];
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