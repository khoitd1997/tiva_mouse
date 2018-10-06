#include "mouse.h"

#include <stdint.h>

void makeHidReport(int8_t hidBuf[HID_REPORT_BUF_LEN_BYTE]) {
  for (uint32_t hidBufIndex = 0; hidBufIndex < HID_REPORT_BUF_LEN_BYTE; ++hidBufIndex) {
    hidBuf[hidBufIndex] = 0;
  }

  hidBuf[HID_REPORT_X_INDEX] = 120;
  hidBuf[HID_REPORT_Y_INDEX] = 120;
}