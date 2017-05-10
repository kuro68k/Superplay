/*
 * hw_nvm.S
 * Author:	kuro68k
 * Created: 31/10/2011 16:52:59
 *
 * Most of this is adapted from app note AVR1316 sp_driver.S. NVM operations need things
 * in specific registers so it is easier to do them in assembler. The original app-note
 * contained functions that needed to be in the bootloader section (see the NVM section of
 * the XMEGA manual) which interfered with our bootloader, so the useful parts were copied
 * into this file. Some of the functions have been updated to use the ASF code, which has
 * a few minor fixes.
 */

#include <avr\io.h>

/* Defines not yet included in header file. */
#define NVM_CMD_NO_OPERATION_gc (0x00<<0)	// Noop/Ordinary LPM
#define NVM_CMD_READ_USER_SIG_ROW_gc (0x01<<0)	// Read user signature row
#define NVM_CMD_READ_CALIB_ROW_gc (0x02<<0)	// Read calibration row
#define NVM_CMD_READ_EEPROM_gc (0x06<<0)	// Read EEPROM
#define NVM_CMD_READ_FUSES_gc (0x07<<0)	// Read fuse byte
#define NVM_CMD_WRITE_LOCK_BITS_gc (0x08<<0)	// Write lock bits
#define NVM_CMD_ERASE_USER_SIG_ROW_gc (0x18<<0)	// Erase user signature row
#define NVM_CMD_WRITE_USER_SIG_ROW_gc (0x1A<<0)	// Write user signature row
#define NVM_CMD_ERASE_APP_gc (0x20<<0)	// Erase Application Section
#define NVM_CMD_ERASE_APP_PAGE_gc (0x22<<0)	// Erase Application Section page
#define NVM_CMD_LOAD_FLASH_BUFFER_gc (0x23<<0)	// Load Flash page buffer
#define NVM_CMD_WRITE_APP_PAGE_gc (0x24<<0)	// Write Application Section page
#define NVM_CMD_ERASE_WRITE_APP_PAGE_gc (0x25<<0)	// Erase-and-write Application Section page
#define NVM_CMD_ERASE_FLASH_BUFFER_gc (0x26<<0)	// Erase/flush Flash page buffer
#define NVM_CMD_ERASE_BOOT_PAGE_gc (0x2A<<0)	// Erase Boot Section page
#define NVM_CMD_WRITE_BOOT_PAGE_gc (0x2C<<0)	// Write Boot Section page
#define NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc (0x2D<<0)	// Erase-and-write Boot Section page
#define NVM_CMD_ERASE_EEPROM_gc (0x30<<0)	// Erase EEPROM
#define NVM_CMD_ERASE_EEPROM_PAGE_gc (0x32<<0)	// Erase EEPROM page
#define NVM_CMD_LOAD_EEPROM_BUFFER_gc (0x33<<0)	// Load EEPROM page buffer
#define NVM_CMD_WRITE_EEPROM_PAGE_gc (0x34<<0)	// Write EEPROM page
#define NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc (0x35<<0)	// Erase-and-write EEPROM page
#define NVM_CMD_ERASE_EEPROM_BUFFER_gc (0x36<<0)	// Erase/flush EEPROM page buffer
#define NVM_CMD_APP_CRC_gc (0x38<<0)	// Generate Application section CRC
#define NVM_CMD_BOOT_CRC_gc (0x39<<0)	// Generate Boot Section CRC
#define NVM_CMD_FLASH_RANGE_CRC_gc (0x3A<<0)	// Generate Flash Range CRC
#define CCP_SPM_gc (0x9D<<0)	// SPM Instruction Protection
#define CCP_IOREG_gc (0xD8<<0)	// IO Register Protection

.section .text
.global HW_read_fuse_byte
.global HW_read_signature_byte
.global HW_read_user_signature_byte
.global HW_WaitForSPM


HW_read_fuse_byte:
	sts		NVM_ADDR0, r24              ; Load fuse byte index into NVM Address Register 0.
	clr		r25                         ; Prepare a zero.
	sts		NVM_ADDR1, r24              ; Load zero into NVM Address Register 1.
	sts		NVM_ADDR2, r24              ; Load zero into NVM Address Register 2.
	ldi		r20, NVM_CMD_READ_FUSES_gc  ; Prepare NVM command in R20.
	rcall	hw_nv_command				; Jump to common NVM Action code.
	movw	r24, r22					; Move low byte to 1 byte return address.
	ret

HW_read_signature_byte:
	ldi		r20, NVM_CMD_READ_CALIB_ROW_gc    ; Prepare NVM command in R20.
	clr		r25
	rjmp	hw_common_lpm                     ; Jump to common LPM code.

HW_read_user_signature_byte:
	ldi		r20, NVM_CMD_READ_USER_SIG_ROW_gc  ; Prepare NVM command in R20.
	rjmp	hw_common_lpm                      ; Jump to common LPM code.

hw_nv_command:
	sts		NVM_CMD, r20        ; Load command into NVM Command register.
	ldi		r18, CCP_IOREG_gc   ; Prepare Protect IO-register signature in R18.
	ldi		r19, NVM_CMDEX_bm   ; Prepare bitmask for setting NVM Command Execute bit into R19.
	sts		CCP, r18            ; Enable IO-register operation (this disables interrupts for 4 cycles).
	sts		NVM_CTRLA, r19      ; Load bitmask into NVM Control Register A, which executes the command.
	lds		r22, NVM_DATA0      ; Load NVM Data Register 0 into R22.
	lds		r23, NVM_DATA1      ; Load NVM Data Register 1 into R23.
	lds		r24, NVM_DATA2      ; Load NVM Data Register 2 into R24.
	clr		r25                 ; Clear R25 in order to return a clean 32-bit value.
	sts		NVM_CMD, r1			; clean up
	ret

hw_common_lpm:
	movw	ZL, r24             ; Load index into Z.
	sts		NVM_CMD, r20        ; Load prepared command into NVM Command register.
	lpm		r24, Z
	sts		NVM_CMD, r1			; clean up
	ret

HW_WaitForSPM:
	lds		r18, NVM_STATUS     ; Load the NVM Status register.
	sbrc	r18, NVM_NVMBUSY_bp ; Check if bit is cleared.
	rjmp	HW_WaitForSPM       ; Repeat check if bit is not cleared.
	clr		r18
	sts		NVM_CMD, r18        ; Clear up command register to NO_OPERATION.
	ret


/* ----------------------------------------------------------------------------------------------------- */

.global HW_application_crc
.global HW_boot_crc

HW_application_crc:
	ldi		r20, NVM_CMD_APP_CRC_gc		; Prepare NVM command in R20.
	rjmp	hw_common_cmd				; Jump to common NVM Action code.

HW_boot_crc:
	ldi		r20, NVM_CMD_BOOT_CRC_gc	; Prepare NVM command in R20.
	rjmp	hw_common_cmd				; Jump to common NVM Action code.

hw_common_cmd:
	sts	NVM_CMD, r20        ; Load command into NVM Command register.
	ldi	r18, CCP_IOREG_gc   ; Prepare Protect IO-register signature in R18.
	ldi	r19, NVM_CMDEX_bm   ; Prepare bitmask for setting NVM Command Execute bit into R19.
	sts	CCP, r18            ; Enable IO-register operation (this disables interrupts for 4 cycles).
	sts	NVM_CTRLA, r19      ; Load bitmask into NVM Control Register A, which executes the command.
	lds	r22, NVM_DATA0      ; Load NVM Data Register 0 into R22.
	lds	r23, NVM_DATA1      ; Load NVM Data Register 1 into R23.
	lds	r24, NVM_DATA2      ; Load NVM Data Register 2 into R24.
	ldi	r25, NVM_CMD_NO_OPERATION_gc
	sts	NVM_CMD, r25
	clr	r25                 ; Clear R25 in order to return a clean 32-bit value.
	ret
