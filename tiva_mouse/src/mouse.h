#ifndef _MOUSE_H
#define _MOUSE_H

#include <stdint.h>

#define HID_REPORT_BUF_LEN_BYTE 3
#define HID_REPORT_BUTTON_INDEX 0
#define HID_REPORT_X_INDEX 1
#define HID_REPORT_Y_INDEX 2

void makeHidReport(int8_t hidBuf[HID_REPORT_BUF_LEN_BYTE]);

#endif