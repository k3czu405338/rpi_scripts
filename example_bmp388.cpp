#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

#define BMP388_I2C_ADDRESS 0x77
#define BMP388_REG_ID         0x00  // BMP388 Chip ID register
#define BMP388_REG_STATUS     0x03  // Status register
#define BMP388_REG_CTRL_MEAS  0x1F  // Control measurement register
#define BMP388_REG_PRESS      0x04  // Pressure data
#define BMP388_REG_TEMP       0x07  // Temperature data

int readI2CRegister(int fd, uint8_t reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

int initBMP388() {
    int fd = wiringPiI2CSetup(BMP388_I2C_ADDRESS);
    if (fd == -1) {
        std::cerr << "Failed to connect to BMP388!" << std::endl;
        return -1;
    }

    uint8_t chip_id = readI2CRegister(fd, BMP388_REG_ID);
    if (chip_id != 0x50) {  // 0x50 is the expected chip ID for BMP388
        std::cerr << "BMP388 chip ID mismatch!" << std::endl;
        return -1;
    }

    wiringPiI2CWriteReg8(fd, BMP388_REG_CTRL_MEAS, 0x24); // Example: setup for normal mode

    return fd;
}

float readTemperature(int fd) {
    uint8_t tempData[3];
    tempData[0] = readI2CRegister(fd, BMP388_REG_TEMP);
    tempData[1] = readI2CRegister(fd, BMP388_REG_TEMP + 1);
    tempData[2] = readI2CRegister(fd, BMP388_REG_TEMP + 2);

    int32_t tempRaw = (tempData[0] << 12) | (tempData[1] << 4) | (tempData[2] >> 4);
    return (float)tempRaw / 100.0f;  // Placeholder conversion, check datasheet for proper compensation
}

float readPressure(int fd) {
    uint8_t pressData[3];
    pressData[0] = readI2CRegister(fd, BMP388_REG_PRESS);
    pressData[1] = readI2CRegister(fd, BMP388_REG_PRESS + 1);
    pressData[2] = readI2CRegister(fd, BMP388_REG_PRESS + 2);

    int32_t pressRaw = (pressData[0] << 12) | (pressData[1] << 4) | (pressData[2] >> 4);
    return (float)pressRaw / 100.0f;  
}

int main() {
    int fd = initBMP388();
    if (fd == -1) {
        return -1;  
    }

    while (true) {
        float temperature = readTemperature(fd);
        float pressure = readPressure(fd);

        std::cout << "Temperature: " << temperature << " °C" << std::endl;
        std::cout << "Pressure: " << pressure << " Pa" << std::endl;

        sleep(1);
    }

    return 0;
}
