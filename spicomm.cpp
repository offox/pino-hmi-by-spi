#include <fcntl.h>				
#include <sys/ioctl.h>			
#include <linux/spi/spidev.h>	
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

int spi_cs0_fd;
int spi_cs1_fd;

void SPIComm::SPIComm(int device)
{
    if (device)
    {
        spi_cs_fd = &spi_cs1_fd;
    }
    else
    {
        spi_cs_fd = &spi_cs0_fd;
    }
}

bool SPIComm::openPort()
{
	int status_value = -1;
    int *spi_cs_fd;

    /* Set SPI mode
     * SPI_MODE_0 (0,0) - CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
     * SPI_MODE_1 (0,1) - CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
     * SPI_MODE_2 (1,0) - CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
     * SPI_MODE_3 (1,1) - CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
     */
    mode = SPI_MODE_0;

    bitsPerWord = 8;
    speed = 1000000;		//1000000 = 1MHz (1uS per bit)

    if (device)
	{
    	*spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    }
	else 
	{
    	*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);
	}

    if (*spi_cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
        return false;
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        return false;
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status_value < 0)
    {
		perror("Could not set SPIMode (RD)...ioctl fail");
		return false;
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
		perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
		return false;
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
		perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
		return false;
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
		perror("Could not set SPI speed (WR)...ioctl fail");
		return false;
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
		perror("Could not set SPI speed (RD)...ioctl fail");
		return false;
    }
    return(status_value);
}

bool SPIComm::closePort()
{
	int status_value = -1;
    int *spi_cs_fd;

    if (spi_device) 
	{
    	spi_cs_fd = &spi_cs1_fd;
	}
    else
	{
    	spi_cs_fd = &spi_cs0_fd;
	}

    status_value = close(*spi_cs_fd);
    if(status_value < 0)
    {
    	perror("Error - Could not close SPI device");
    	return false;
    }
    return(status_value);
}

int SPIComm::writeAndRead(unsigned char *data, int length)
{
	struct spi_ioc_transfer spi[length];
	int i = 0;
	int retVal = -1;
    int *spi_cs_fd;

    if (spi_device)
	{
    	spi_cs_fd = &spi_cs1_fd;
	}
    else
	{
		spi_cs_fd = &spi_cs0_fd;
	}

	//one spi transfer for each byte

	for (i = 0 ; i < length ; i++)
	{
		spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
		spi[i].len           = sizeof(*(data + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = spi_speed ;
		spi[i].bits_per_word = spi_bitsPerWord ;
		spi[i].cs_change = 0;
	}

	retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;

	if(retVal < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;
}
