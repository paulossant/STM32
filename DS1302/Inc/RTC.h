#include "main.h"

void rtc_init(void);

int rtc_read(unsigned char  adress);
void rtc_write(unsigned char adress, unsigned char buffer);

int rtc_sec(void);
int rtc_min(void);
int rtc_hor(void);
int rtc_day(void);
int rtc_mon(void);
int rtc_yer(void);
int bcd2dec(uint16_t val);
