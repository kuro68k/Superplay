/*
 * dfu_config.h
 *
 */

#ifndef DFU_CONFIG_H_
#define DFU_CONFIG_H_


/* Store the first page written in RAM until the last page is written and
 * the bootloader enters the manifest state. The first page will be erased.
 * If the firmware update is interrupted the bootloader can detect and empty
 * page zero and go back into DFU mode.
 */
#define	DELAYED_ZERO_PAGE


/* Verify writes and retry if they fail.
 */
#define VERIFY_WRITES


/* Enable upload (read firmware from device) support.
 */
#define	UPLOAD_SUPPORT


/* Return true if the DFU bootloader should be started. DFU can be started
 * by some condition (button pressed, flash memory empty etc.) or by the
 * application firmware.
 */
static inline bool CheckStartConditions(void)
{
	PORTB.DIRCLR = PIN7_bm;
	PORTB.PIN7CTRL = (PORTB.PIN7CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc;
	for (uint16_t i = 1000; i; i--);
	
	if ((*(uint32_t *)(INTERNAL_SRAM_START) == 0x4c4f4144) ||	// "LOAD"
		(*(const __flash uint16_t *)(0) == 0xFFFF) ||			// reset vector blank
		(!(PORTB.IN & PIN7_bm)))								// button 1
	{
		*(uint32_t *)(INTERNAL_SRAM_START) = 0;					// clear signature
		return true;
	}
	return false;
}


#endif /* DFU_CONFIG_H_ */
