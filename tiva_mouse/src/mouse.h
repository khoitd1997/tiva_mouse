#ifndef _MOUSE_H
#define _MOUSE_H

#include <stdint.h>

#include "mma8451.h"

#define HID_REPORT_BUF_LEN_BYTE 3
#define HID_REPORT_BUTTON_INDEX 0
#define HID_REPORT_X_INDEX 1
#define HID_REPORT_Y_INDEX 2

uint32_t mouseRxCallBack(void*    pvCBData,
                         uint32_t ui32Event,
                         uint32_t ui32MsgParam,
                         void*    pvMsgData);
uint32_t mouseTxCallBack(void*    pvCBData,
                         uint32_t ui32Event,
                         uint32_t ui32MsgParam,
                         void*    pvMsgData);

void mouseInit(void);
void mouseEnable(void);

void makeHidReport(uint8_t hidBuf[HID_REPORT_BUF_LEN_BYTE], int32_t accelData[ACCEL_TOTAL_AXIS]);

#endif