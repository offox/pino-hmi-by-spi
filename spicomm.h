#ifndef SPICOMM_H
#define SPICOMM_H

class SPIComm
{
public:
    SPIComm();

    bool openPort(int spi_device);
    bool closePort(int spi_device);
    int writeAndRead(int spi_device, unsigned char *data, int length);
};

#endif // SPICOMM_H
