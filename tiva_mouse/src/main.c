#include "mouse.h"
#include "mouse_usb_config.h"

#include <stdint.h>

#include <stdbool.h>

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
#include "inc/hw_uart.h"

#include "debug_utils/swo_segger.h"
#include "utils/uartstdio.h"

uint32_t rxCallBack(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData) {
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

uint32_t txCallBack(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData) {
  switch (ui32Event) {
    case USB_EVENT_TX_COMPLETE:
      SWO_PrintString("Finished Sending\n");
      break;
  }

  SWO_PrintString("Tx event\n");
  return 0;
}

/* HID Device */
tUSBDHIDDevice mouseDevice = {.ui16VID                  = VENDOR_ID,
                              .ui16PID                  = PRODUCT_ID,
                              .ui16MaxPowermA           = MAX_PWR_MA,
                              .ui8PwrAttributes         = USB_CONF_ATTR_BUS_PWR,
                              .ui8Subclass              = USB_SUBCLASS_UNDEFINED,
                              .ui8Protocol              = USB_HID_PROTOCOL_MOUSE,
                              .ui8NumInputReports       = 1,
                              .psReportIdle             = g_psReportIdle,
                              .pfnRxCallback            = rxCallBack,
                              .pvRxCBData               = (void *)(&mouseDevice),
                              .pfnTxCallback            = txCallBack,
                              .pvTxCBData               = NULL,
                              .bUseOutEndpoint          = false,
                              .psHIDDescriptor          = hidDescriptorList,
                              .ppui8ClassDescriptors    = mouseClassDescriptor,
                              .ppui8StringDescriptors   = mouseStringDescriptorList,
                              .ui32NumStringDescriptors = TOTAL_STRING_DESCRIPTOR,
                              .ppsConfigDescriptor      = configHeaderList,
                              .sPrivateData             = 0};

void ConfigureUART(void) {
  //
  // Enable the GPIO Peripheral used by the UART.
  //
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  //
  // Enable UART0
  //
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

  //
  // Configure GPIO Pins for UART mode.
  //
  ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
  ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
  ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  //
  // Use the internal 16MHz oscillator as the UART clock source.
  //
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

  //
  // Initialize the UART for console I/O.
  //
  UARTStdioConfig(0, 115200, 16000000);
}

int main(void) {  //

  ROM_FPULazyStackingEnable();

  //
  // Set the clocking to run from the PLL at 50MHz
  //
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  // usb stuffs
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

  // onboard LED config
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2);

  // ConfigureUART();
  SWO_PrintString("Hello, world!\n");

  // usb interrupt register
  //   ROM_IntMasterEnable();
  //   USBIntEnableEndpoint(USB0_BASE, USB_INTEP_ALL);
  //   USBIntEnableControl(USB0_BASE, USB_INTCTRL_ALL);
  //   USBIntRegister(USB0_BASE, USB0DeviceIntHandler);

  if (NULL != USBDHIDInit(0, &mouseDevice)) {
    SWO_PrintString("Connection successful\n");
  } else {
    SWO_PrintString("Connection failed\n");
  }

  int8_t hidRep[HID_REPORT_BUF_LEN_BYTE] = {0};
  makeHidReport(hidRep);

  for (;;) {
    SWO_PrintString("Bru\n");
    ROM_IntMasterEnable();
    for (uint32_t counter = 0; counter < 500000; ++counter) {
      // wait
    }
    USBDHIDReportWrite(&mouseDevice, hidRep, HID_REPORT_BUF_LEN_BYTE, false);
  }
  return 0;
}
