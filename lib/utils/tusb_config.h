#ifndef TUSB_CONFIG_H_
#define TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

    // Board-specific
#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE)

    // RHPort max operational speed
#define CFG_TUD_MAX_SPEED       OPT_MODE_FULL_SPEED

    // Device mode - MCU type depends on chip
#if defined(PICO_RP2040)
#define CFG_TUSB_MCU        OPT_MCU_RP2040
#elif defined(PICO_RP2350)
#define CFG_TUSB_MCU        OPT_MCU_RP2350
#else
#error "Unknown Pico board type"
#endif


    // Enable device stack
#define CFG_TUD_ENABLED         1

    // CDC (Communications Device Class - virtual serial port)
#define CFG_TUD_CDC             1
#define CFG_TUD_CDC_RX_BUFSIZE  256
#define CFG_TUD_CDC_TX_BUFSIZE  256

    // USB Device descriptors
#define CFG_TUD_ENDPOINT0_SIZE  64

    // Other USB classes (disabled if you're only using CDC)
#define CFG_TUD_MSC             0  // Mass Storage
#define CFG_TUD_HID             0  // Human Interface Device
#define CFG_TUD_MIDI            0  // MIDI
#define CFG_TUD_VENDOR          0  // Vendor specific

#ifdef __cplusplus
}
#endif

#endif /* TUSB_CONFIG_H_ */
