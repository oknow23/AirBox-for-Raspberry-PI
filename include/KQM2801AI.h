#ifndef KQM2801_H_
#define KQM2801_H_

#define KQM2801_INTERFACE_ADDR       1
#define KQM2801_DEFAULT_ADDR         0x2f

typedef enum
{
  KQM2801_OK,
  KQM2801_CRC_CHECK_FAILED,
  KQM2801_BAD,
  KQM2801_READ_FAILED, 
  KQM2801_WRITE_FAILED
} kqm2801rtn;

int kqm2801_open(int i2c_address, uint8_t kqm2801_address);
int kqm2801_close(int file);

kqm2801rtn kqm2801_writeandread(int fd, uint8_t *buffer, int readsize);
kqm2801rtn getDensity(float *density,char *level);


#endif /* KQM2801_H_ */
