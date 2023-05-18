#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h> /* for open */
#include <unistd.h> /* for close */
#include <string.h>
#include <errno.h> /* for errno */
#include <sys/ioctl.h> /* for ioctl */
#include <linux/i2c.h>
#include <unistd.h> /* for usleep */
#include "amg883x.h"

#define AMG8833_DEV "/dev/amg883x"

void data_dump_byte(const char *desc, uint8_t *buf, uint16_t size)
{
    int i = 0;
    printf("\n%s", desc);
    for (i = 0; i < size; i++) {
        if ((i % 16) == 0) printf("\n");
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

void data_dump_word(const char *desc, uint16_t *buf, uint16_t size)
{
    int i = 0;
    printf("\n%s", desc);
    for (i = 0; i < size; i++) {
        if ((i % 16) == 0) printf("\n");
        printf("%04X ", buf[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    int ret;
    int fd;
    int temp;
    struct amg883x_read_data *rdBuf;
    struct amg883x_write_data *wrBuf;

    rdBuf = (struct amg883x_read_data *)malloc(sizeof(struct amg883x_read_data));
    wrBuf = (struct amg883x_write_data *)malloc(sizeof(struct amg883x_write_data));

    fd = open(AMG8833_DEV, O_RDWR);
    if (fd < 0) {
        printf("amg8833 open failed\n");
    }

    read(fd, rdBuf, sizeof(struct amg883x_read_data));
    printf("amg883x i2c temperature:%04x\n", rdBuf->thermistor);
    printf("amg883x int union:%d, %d, %d\n", rdBuf->int_control, rdBuf->int_control_reg.intEN, 
                                           rdBuf->int_control_reg.intMode);

    printf("amg883x status union:%d, %d, %d, %d\n", rdBuf->status, rdBuf->status_reg.intf, 
                                              rdBuf->status_reg.ir_over, 
                                              rdBuf->status_reg.temper_over);

    data_dump_byte("int flag table:", rdBuf->int_pixel_table, 8);
    data_dump_word("pixel table:", rdBuf->pixel_value_table, 64);

#if 0
    uint16_t tempU16;
    ret = read(fd, &tempU16, 2);
    ret = read(fd, rdBuf, 2);
    printf("temperature: %d * 0.0625°C\n", tempU16);
    if (ret > 0) {
        uint16_t tmp = rdBuf[0] + (rdBuf[1] << 8);
        float tempeature = tmp * 0.0625f;
        printf("temperature: %.4f°C\n", tempeature);
    }
    usleep(10000);
#endif
    
    wrBuf->wr_flag |= AMG883X_WR_FLAG_CLEAR;
    wrBuf->clear_reg = 0;
    wrBuf->clear_reg_bit.int_clear = 1;
    wrBuf->wr_flag |= AMG883X_WR_FLAG_FPS;
    wrBuf->fps = 1;
    wrBuf->wr_flag |= AMG883X_WR_FLAG_INTC;
    wrBuf->int_ctrl_reg = 0;
    wrBuf->int_ctrl_bit.int_en = 1;

    ret = write(fd, wrBuf, sizeof(struct amg883x_write_data));
    if (ret < 0)
        printf("Error: write return error %d", ret);

    ret = ioctl(fd, AMG_CMD_PW_ON);
    ret = ioctl(fd, AMG_CMD_PW_OFF);
    ret = ioctl(fd, AMG_CMD_RD_TEMP, &temp);
    if (ret == 0)
        printf("ioctl get temperature %0.3f°C", temp*0.0625f);

    free(rdBuf);
    free(wrBuf);
    close(fd);

    return 0;
}