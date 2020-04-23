#ifndef __LGT8F690A_MISC__
#define __LGT8F690A_MISC__
// ============================================
// MISC helper function
// include AXU implementation
// ============================================
#include <stdint.h>
#include "lgt8f690a.h"

#define E2P		0
#define MTP		1

uint16_t axu_fmul8x8(uint8_t, uint8_t);
uint16_t axu_fmul16x8(uint16_t, uint8_t);
uint32_t axu_wmul16x8(uint16_t, uint8_t);
uint16_t axu_fdiv16d8(uint16_t, uint8_t);
void axu_div16d8(uint8_t);
uint8_t axu_fdaw(uint8_t);
uint8_t axu_fdsw(uint8_t);

char * misc_utoa(char *, unsigned, int);

uint16_t misc_eer(uint8_t, uint16_t);
uint8_t cfw_valid();

// read trimming for 16MHz HFINTOSC
#define	cfw_rctrim() misc_eer(1, 0x1007)

// read trimming for 1.5V IVREF
#define	cfw_v1trim() (misc_eer(1, 0x100d) & 0x3f)

// read trimming for 2.56V IVREF
#define cfw_v2trim() ((misc_eer(1, 0x100d) >> 6) & 0x3f)

#endif