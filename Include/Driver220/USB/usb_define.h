/**
    Header file for USB device

    This file is the header file for USB device

    @file       usb_define.h
    @ingroup    mIUSBDevice
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _USBDEFINE_H
#define _USBDEFINE_H

#include "Type.h"

/**
    @addtogroup mIUSBDevice
*/
//@{


/**
    @name   Basic parameters
*/
//@{
#define USB_EP_NUM                      8       ///< max number of endpoint
#define USB_NUM_CONFIG                  1       ///< number of configration
#define USB_NUM_INTERFACE               1       ///< number of interface
#define USB_NUM_STRING_DESC             4       ///< number of string descriptor
#define USB_CONFIG_DESC_TOTAL_SIZE      20      ///< configuration descriptor total size
#define EP0_PACKET_SIZE                 64      ///< EP0 : control packet size
#define USB_WAITBUFEMPTY_LOOPCNT        10000   ///< loop count for waiting buffer empty
//@}


/**
    @name   Block number
*/
//@{
#define SINGLE_BLK                      1       ///< Single block
#define DOUBLE_BLK                      2       ///< Double block
#define TRIPLE_BLK                      3       ///< triple block
//@}

/**
    @name   Endpoint/FIFO direction
*/
//@{
#define DIRECTION_IN                    0       ///< direction IN
#define DIRECTION_OUT                   1       ///< direction OUT
//@}

/**
    @name   USB Request Type
*/
//@{
#define USB_DEV_REQ_TYPE_STANDARD       0x00    ///< standard request
#define USB_DEV_REQ_TYPE_CLASS          0x01    ///< class specific request
#define USB_DEV_REQ_TYPE_VENDER         0x02    ///< vendor specific request
#define USB_DEV_REQ_TYPE_RESERVE        0x03    ///< reserved
#define USB_DEV_REQ_TYPE_UNSUPPORTED    0xff    ///< unsupported
//@}


/**
    @name   USB Standard Device Request
*/
//@{
#define USB_GET_STATUS                  0       ///< GetStatus request
#define USB_CLEAR_FEATURE               1       ///< ClearFeature request
#define USB_SET_FEATURE                 3       ///< SetFeature request
#define USB_SET_ADDRESS                 5       ///< SetAddress request
#define USB_GET_DESCRIPTOR              6       ///< GetDescriptor request
#define USB_SET_DESCRIPTOR              7       ///< SetDescriptor request
#define USB_GET_CONFIGURATION           8       ///< GetConfiguratoin request
#define USB_SET_CONFIGURATION           9       ///< SetConfiguratoin request
#define USB_GET_INTERFACE               10      ///< GetInterface request
#define USB_SET_INTERFACE               11      ///< SetInterface request
#define USB_SYNCH_FRAME                 12      ///< SynchFrame request
//@}


/**
    @name   device request
*/
//@{
#define USB_DEVICE_REQUEST_SIZE         8       ///< device request size
#define USB_DEVICE_TO_HOST              0x80    ///< device to host transfer
#define USB_HOST_TO_DEVICE              0x00    ///< host to device transfer
#define USB_DEVICE                      0x00    ///< request to device
#define USB_INTERFACE                   0x01    ///< request to interface
#define USB_ENDPOINT                    0x02    ///< request to endpoint
#define USB_CLASS                       0x20    ///< class request
#define USB_VENDOR                      0x40    ///< vendor request
#define USB_STANDARD                    0x00    ///< standard request
//@}


/**
    @name   descriptor size
*/
//@{
#define USB_DEV_LENGTH                  0x12    ///< device descriptor size
#define USB_CFG_LENGTH                  0x09    ///< config descriptor size
#define USB_IF_LENGTH                   0x09    ///< interface descriptor size
#define USB_BULK_EP_LENGTH              0x07    ///< bulk endpoint descriptor size
//#NT#2011/11/04#HH Chuang -begin
//#NT#Support HID class driver
#define USB_INT_EP_LENGTH               0x07    ///< interrupt endpoint descriptor size
//#NT#2011/11/04#HH Chuang -end
#define USB_AUDIO_AC_IF_LENGTH          0x09    ///< audio commander class specific interface descriptor size
#define USB_AUDIO_IT_IF_LENGTH          0x0C    ///< audio input terminal class specific interface descriptor size
#define USB_AUDIO_OT_IF_LENGTH          0x09    ///< audio output terminal class specific interface descriptor size
#define USB_AUDIO_AS_IF_LENGTH          0x07    ///< audio class specific AS interface descriptor size
#define USB_AUDIO_FT_IF_LENGTH          0x0B    ///< audio class specific format type interface descriptor size
#define USB_ISO_EP_LENGTH               0x09    ///< iso endpoint descriptor size
#define USB_AUDIO_CSEP_ISOC_LENGTH      0x07    ///< audio class specific endpoint descriptor size
#define USB_DEV_QUALI_LENGTH            0x0A    ///< device qualifier length */
//@}

/**
    @name   endpoint address
*/
//@{
#define USB_EP_IN_ADDRESS               0x80    ///< IN endpoint address
#define USB_EP_OUT_ADDRESS              0x00    ///< OUT endpoint address
#define USB_EP_EP1I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x01)
#define USB_EP_EP1O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x01)
#define USB_EP_EP2I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x02)
#define USB_EP_EP2O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x02)
#define USB_EP_EP3I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x03)
#define USB_EP_ADDRESS_MASK             (0x0F)
//@}

/**
    @name   endpoint attribute
*/
//@{
#define USB_EP_ATR_CONTROL              0x00    ///< transfer mode : control
#define USB_EP_ATR_ISO                  0x01    ///< transfer mode : isochronous
#define USB_EP_ATR_BULK                 0x02    ///< transfer mode : bulk
#define USB_EP_ATR_INTERRUPT            0x03    ///< transfer mode : interrupt
//@}

/**
    @name   bInterval value
*/
//@{
#define USB_EP_CONTROL_INTERVAL                 0x00    ///< control interval
#define USB_EP_BULK_INTERVAL                    0x00    ///< bulk interval
#define USB_EP_INT_INTERVAL                     0x01    ///< interrupt interval
#define USB_EP_ISO_INTERVAL                     0x01    ///< isochronous interval
#define USB_EP_INT_CFGA_IF0ALT0_INTERVAL        0x01    ///< interrupt CFGA_IF0ALT0 interval
#define USB_EP_INT_CFGA_IF0ALT1_INTERVAL        0x02    ///< interrupt CFGA_IF0ALT1 interval
#define USB_EP_INT_CFGA_IF0ALT2_INTERVAL        0x03    ///< interrupt CFGA_IF0ALT2 interval
#define USB_EP_INT_CFGA_IF0ALT3_INTERVAL        0x04    ///< interrupt CFGA_IF0ALT3 interval
#define USB_EP_INT_CFGB_IF1ALT0_INTERVAL        0x05    ///< interrupt CFGA_IF1ALT0 interval
//@}

/**
    @name   USB Feature Select
*/
//@{
#define USB_DEVICE_REMOTE_WAKEUP                1       ///< remote wake up
#define USB_ENDPOINT_STALL                      0       ///< endpoint stall
//@}

/**
    @name   USB descriptor type
*/
//@{
#define USB_DEVICE_DESCRIPTOR_TYPE              1       ///< device descriptor
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       2       ///< configuraton descriptor
#define USB_STRING_DESCRIPTOR_TYPE              3       ///< string descriptor
#define USB_INTERFACE_DESCRIPTOR_TYPE           4       ///< interface descriptor
#define USB_ENDPOINT_DESCRIPTOR_TYPE            5       ///< endpoint descriptor
#define USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE    6       ///< Device_Qualifier descriptor
#define USB_OTHER_SPEED_CONFIGURATION           7       ///< Other_Speed_Configuration
//#NT#2011/11/04#HH Chuang -begin
//#NT#Support HID class driver
#define USB_DESCRIPTOR_TYPE_HID                 0x21    ///< HID descriptor
#define USB_DESCRIPTOR_TYPE_HID_REPORT          0x22    ///< HID report descriptor
#define USB_DESCRIPTOR_TYPE_HID_PHYSICAL        0x23    ///< HID physical descriptor
//#NT#2011/11/04#HH Chuang -end
//@}

/**
    @name   config descriptor definitions
*/
//@{
#define USB_DEVDESC_ATB_BUS_POWER               0x80    ///< bus power
#define USB_DEVDESC_ATB_SELF_POWER              0x40    ///< self power
#define USB_DEVDESC_ATB_RMT_WAKEUP              0x20    ///< remote wake up
//@}

/**
    @name   USB request-bmRequestType : RECIPIENT type
*/
//@{
#define USB_RECIPIENT                           0x1F    ///< recipient mask
#define USB_RECIPIENT_DEVICE                    0x00    ///< recipient device
#define USB_RECIPIENT_INTERFACE                 0x01    ///< recipient interface
#define USB_RECIPIENT_ENDPOINT                  0x02    ///< recipient endpoint
//@}

/**
    @name   USB defined Feature select
*/
//@{
#define USB_FEATURE_ENDPOINT_STALL              0x0000  ///< feature select stall
#define USB_FEATURE_REMOTE_WAKEUP               0x0001  ///< feature select remote wakeup
#define USB_FEATURE_POWER_D0                    0x0002  ///< feature select POWER_D0
#define USB_FEATURE_POWER_D1                    0x0003  ///< feature select POWER_D1
#define USB_FEATURE_POWER_D2                    0x0004  ///< feature select POWER_D2
#define USB_FEATURE_POWER_D3                    0x0005  ///< feature select POWER_D3
//@}

/**
    @name   USB request and masks
*/
//@{
#define USB_REQUEST_TYPE_MASK                   0x60    ///< request type mask
#define USB_STANDARD_REQUEST                    0x00    ///< standard request
#define USB_CLASS_REQUEST                       0x20    ///< class request
#define USB_VENDOR_REQUEST                      0x40    ///< vendor request
#define USB_VENDOR_D2H_MASK                     0x80    ///< vendor d2h mask
#define USB_REQUEST_MASK                        0x0F    ///< request mask
#define DEVICE_ADDRESS_MASK                     0x7F    ///< device address mask
#define USB_ENDPOINT_DIRECTION_MASK             0x80    ///< endpoint direction mask
//@}

/**
    @name   Endpoint number
*/
//@{
#define EP0                             0x00    ///< Endpoint 0
#define EP1                             0x01    ///< Endpoint 1
#define EP2                             0x02    ///< Endpoint 2
#define EP3                             0x03    ///< Endpoint 3
#define EP4                             0x04    ///< Endpoint 4
#define EP5                             0x05    ///< Endpoint 5
#define EP6                             0x06    ///< Endpoint 6
#define EP7                             0x07    ///< Endpoint 7
#define EP8                             0x08    ///< Endpoint 8
//@}

/**
    @name   FIFO number
*/
//@{
#define FIFO0                           0x0     ///< FIFO 0
#define FIFO1                           0x1     ///< FIFO 1
#define FIFO2                           0x2     ///< FIFO 2
#define FIFO3                           0x3     ///< FIFO 3
//@}


/**
    @name   USB type
*/
//@{
#define USB_NONE                        0       ///< USB type not set
#define USB_MSD                         1       ///< USB type is MSDC
#define USB_SICD                        2       ///< USB type is SICD
#define USB_PCC                         3       ///< USB type is PCC
//@}


/**
    @name   USB event definition

    These parameters are used for fpEvent_CB()
*/
//@{
#define USB_EVENT_IN0                   0x00000001  ///< USB EP0 TX INT
#define USB_EVENT_OUT0                  0x00000002  ///< USB EP0 RX INT
#define USB_EVENT_SETUP                 0x00000004  ///< USB EP0 SETUP INT
#define USB_EVENT_BULKIN1               0x00000008  ///< USB EP1 TX INT
#define USB_EVENT_BULKOUT2              0x00000010  ///< USB EP2 RX INT
#define USB_EVENT_INT3                  0x00000020  ///< USB EP3 TX INT
#define USB_EVENT_ISOIN4                0x00000040  ///< USB EP4 RX INT
#define USB_EVENT_EP1TX0BYTE            0x00000080  ///< USB EP1 TX 0BYTE
#define USB_EVENT_EP3TX0BYTE            0x00100000  ///< USB EP3 TX 0BYTE
#define USB_EVENT_MSDCRESET             0x00010000  ///< USB MSDC Reset
#define USB_EVENT_CLRFEATURE            0x00040000  ///< USB Clear Feature
#define USB_EVENT_RESET                 0x00080000  ///< USB Reset
#define USB_EVENT_SICDSTOP              0x02000000  ///< USB ABORT Print for SICD
#define USB_EVENT_PCCVIDEOSTART         0x00000100  ///< USB PC-camera video start
#define USB_EVENT_PCCVIDEOSTOP          0x00000200  ///< USB PC-camera video stop
#define USB_EVENT_PCCVID_WAIT_CMD       0x00001000  ///< USB PC-camera can receive new usb command
#define USB_EVENT_PCC_CAP_IAMGE         0x00002000  ///< USB PC-camera, capture a high resolution image.
#define USB_EVENT_PCC_CAP_IMG_RDY       0x00004000  ///< USB PC-camera, capture a high resolution image.
#define USB_EVENT_PCC_FRAME_TIMER       0x00008000  ///< USB PC-camera,
#define USB_EVENT_TIMER                 0x00800000  ///< USB scan card detect timer
#define USB_EVENT_SETADDRESSCMD         0x04000000  ///< USB set address from host
#define USB_EVENT_CONNECT               0x08000000  ///< USB connect to host
#define USB_EVENT_DISCONNECT            0x10000000  ///< USB disconnect from host
#define USB_EVENT_SET_CONFIG            0x20000000  ///< USB set configuration
#define USB_EVENT_SUSPEND               0x40000000  ///< USB suspend event
#define USB_EVENT_RESUME                0x00000400  ///< USB resume event
#define USB_EVENT_SIG                   0x80000000  ///< USB signal
//@}


// For MSDC Multi-LUNs function
/**
    @name   MSDC Max LUN count
*/
//@{
#define MAX_LUN                         6       ///< Logical Unit Number
//@}

/**
    @name   SICD max packet size
*/
//@{
#define SICDFSEP1MAXPKTSIZE             64      ///< SICD FS EP1 max PACKET size
#define SICDHSEP1MAXPKTSIZE             512     ///< SICD HS EP1 max PACKET size
//@}

/**
    @name   USB max transfer length
*/
//@{
#define USB_MAX_DMA_LENGTH      0x1ffff         ///< 17 bits, 128KB-1 (unit: byte)
//@}

//@}

#endif /* _USBDEFINE_H */

