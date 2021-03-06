#include "mouse.h"
#include "mma8451.h"
#include "mouse_usb_config.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "inc/hw_types.h"

#include "usblib/usblib.h"

#include "usblib/usbhid.h"

#include "usblib/usb-ids.h"

#include "usblib/device/usbdevice.h"

#include "usblib/device/usbdcomp.h"

#include "usblib/device/usbdhid.h"

#include "usblib/device/usbdhidgamepad.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/usb.h"

#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include "debug_utils/swo_segger.h"

#define MAX_MOUSE_DATA 127
#define MIN_MOUSE_DATA -127

#define MAX_ACCEL_DATA 8191
#define MIN_ACCEL_DATA -8191

uint32_t mouseRxCallBack(void*    pvCBData,
                         uint32_t ui32Event,
                         uint32_t ui32MsgParam,
                         void*    pvMsgData) {
  switch (ui32Event) {
    case USB_EVENT_CONNECTED:
      SWO_PrintString("Host connected\n");
      break;
    case USB_EVENT_DISCONNECTED:
      SWO_PrintString("Host disconnected\n");
      break;
    case USB_EVENT_RX_AVAILABLE:
      break;
    case USB_EVENT_ERROR:
      SWO_PrintString("Communication error\n");
      break;
    case USB_EVENT_SUSPEND:
      SWO_PrintString("Host suspended\n");
      break;
    case USB_EVENT_RESUME:
      SWO_PrintString("\nBus Resume\n");
      break;
    case USBD_HID_EVENT_IDLE_TIMEOUT:
      // must send report now
      break;
    case USBD_HID_EVENT_GET_REPORT_BUFFER:
      break;
    case USBD_HID_EVENT_GET_REPORT:
      // host requesting report
      break;
    case USBD_HID_EVENT_SET_PROTOCOL:
      break;
    case USBD_HID_EVENT_GET_PROTOCOL:
      break;
    case USBD_HID_EVENT_SET_REPORT:
      break;
    case USBD_HID_EVENT_REPORT_SENT:
      break;
    default:
      break;
  }

  // USBDHIDReportWrite();
  SWO_PrintString("Received something\n");

  return 0;
}

uint32_t mouseTxCallBack(void*    pvCBData,
                         uint32_t ui32Event,
                         uint32_t ui32MsgParam,
                         void*    pvMsgData) {
  switch (ui32Event) {
    case USB_EVENT_TX_COMPLETE:
      SWO_PrintString("Finished Sending\n");
      break;
  }

  SWO_PrintString("Tx event\n");
  return 0;
}

void mouseInit(void) {
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) {
    // wait for clock to be ready
  }

  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_USB0)) {
    // wait for clock to be ready
  }

  ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);
  USBStackModeSet(0, eUSBModeForceDevice, 0);
}

void mouseEnable(void) {
  if (NULL != USBDHIDInit(0, &mouseDevice)) {
    SWO_PrintString("Connection successful\n");
  } else {
    SWO_PrintString("Connection failed\n");
  }
}

static void convAccelToMouse(const int32_t accelData, uint8_t* mouseData) {
  *mouseData = (uint8_t)(accelData * MAX_MOUSE_DATA * 2 / ((float)MAX_ACCEL_DATA));
}

void makeHidReport(uint8_t hidBuf[HID_REPORT_BUF_LEN_BYTE], int32_t accelData[ACCEL_TOTAL_AXIS]) {
  static int32_t prevAccelData[ACCEL_TOTAL_AXIS] = {0};

  assert(hidBuf);
  assert(accelData);

  convAccelToMouse(accelData[0], &hidBuf[HID_REPORT_X_INDEX]);
  convAccelToMouse(accelData[1], &hidBuf[HID_REPORT_Y_INDEX]);

  for (uint32_t dataIndex = 0; dataIndex < ACCEL_TOTAL_AXIS; ++dataIndex) {
    prevAccelData[dataIndex] = accelData[dataIndex];
  }

  // hidBuf[HID_REPORT_X_INDEX] = 120;
  // hidBuf[HID_REPORT_Y_INDEX] = 120;
}