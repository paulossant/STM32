#include "main.h"

void ad9833_init(void);
void ad9833_reset(uint8_t res);
void add9833_DACON(uint8_t state);
void ad9833_write(uint16_t value);

void ad9833_freq(uint32_t value);
void ad9833_phase(uint32_t value);
void ad9833_waveform(uint8_t wvf);
void ad9833_sweep(uint32_t fstart, uint32_t fstop, uint32_t fstep, uint32_t fdelay);
