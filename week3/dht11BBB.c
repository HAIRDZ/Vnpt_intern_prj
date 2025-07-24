#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#define PIN_NUM        "60"
#define GPIO_EXPORT    "/sys/class/gpio/export"
#define GPIO_DIR_PATH  "/sys/class/gpio/gpio60/direction"
#define GPIO_VAL_PATH  "/sys/class/gpio/gpio60/value"
#define TIMEOUT_US     100000  // 100 ms

// doc trang thai gpio
static int readGpioVal(void) {
    FILE *f = fopen(GPIO_VAL_PATH, "r");
    if (!f) return -1;
    int c = fgetc(f);
    fclose(f);
    return (c == '1');
}

void readDht11(void) {
    uint8_t d[5];

    // Export chi 1 lan truoc vong lap
    system("echo " PIN_NUM " > " GPIO_EXPORT);
    usleep(100000);

    for (int attempt = 0; attempt < 5; attempt++) { // attempt 5 lan
        memset(d, 0, sizeof(d)); // ham gan gia tri 0 vao 5 phan tu cua d de reset

        // khoi tao tin hieu cho dht11
        system("echo out > " GPIO_DIR_PATH);   // gpio 60 = outout
        system("echo 0 > " GPIO_VAL_PATH);     // keo chan data xuong muc 0
        usleep(18000);                         // datasheet
        system("echo 1 > " GPIO_VAL_PATH);
        usleep(40);
        system("echo in > " GPIO_DIR_PATH);

        struct timeval t0, t1;
        // ghi lai moc thoi gian bat dau muc low
        gettimeofday(&t0, NULL);
        while (readGpioVal() != 0) { // doc gia tri tu gpio
            gettimeofday(&t1, NULL); // moc t1
            long dt = (t1.tv_sec - t0.tv_sec) * 1000000L
                    + (t1.tv_usec - t0.tv_usec);
            // neu dt > TIMEOUT_US => retry
            if (dt > TIMEOUT_US) goto retry;
        }
        gettimeofday(&t0, NULL);

        while (readGpioVal() != 1) {
            gettimeofday(&t1, NULL);
            long dt = (t1.tv_sec - t0.tv_sec) * 1000000L
                    + (t1.tv_usec - t0.tv_usec);
            if (dt > TIMEOUT_US) goto retry;
        }

        // Vong lap de doc data
        for (int i = 0; i < 40; i++) // Data=8bitintegraRH 8bitdecimalRH 8bitintegralT 8bitdecimalT 8bitchecksum
        {
            gettimeofday(&t0, NULL);

            while (readGpioVal() == 1) {
                gettimeofday(&t1, NULL);
                long dt = (t1.tv_sec - t0.tv_sec) * 1000000L
                        + (t1.tv_usec - t0.tv_usec);
                if (dt > TIMEOUT_US) goto retry;
            }
            gettimeofday(&t0, NULL);

            while (readGpioVal() == 1)
                gettimeofday(&t1, NULL);
            long pulse = (t1.tv_sec - t0.tv_sec) * 1000000L
                       + (t1.tv_usec - t0.tv_usec);
            d[i/8] = (d[i/8] << 1) | (pulse < 50); // pulse <50 => dich bit1 else dich bit0
        }

        // checksum
        if ((((int)d[0] + d[1] + d[2] + d[3]) & 0xFF) == d[4]) // so sanh 4 bit voi bit checksum
        {
            printf("Temperature: %d.%d °C, Humidity: %d.%d %%\n",
                   d[2], d[3], d[0], d[1]);
            return;
        }
    }

retry:
    usleep(1000000);
}

int main(void) {
    while (1) {
        readDht11();
        sleep(2);
    }
    return 0;
}
