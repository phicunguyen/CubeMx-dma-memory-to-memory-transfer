/*
 * dma_flash.c
 *
 *  Created on: Dec 10, 2019
 *      Author: pnguyen
 */


#include "dma_flash.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE			16

static bool flash2mem_done=false;

static uint8_t buffer[BUFFER_SIZE];
static uint8_t bufout[BUFFER_SIZE*3];

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;

/*
 * this callback will be called after the dma transfer up to BUFFER_SIZE
 * In this case, it's 16 bytes.
 *
 */
static void dma_callback(DMA_HandleTypeDef *hdma) {
	//could be use for trigger the main loop or a task that dma transfer is done.
	flash2mem_done = true;
}

/*
 * Call from the main loop that the DMA is done.
 * Ready for process the data.
 */
void dma_flash2mem_done(void) {
	//could be use for trigger the main loop or a task that dma transfer is done.
	uint8_t cnt=0;
	if (flash2mem_done) {
		for (int i=0; i < BUFFER_SIZE; i++) {
			cnt += sprintf((char *)&bufout[cnt], "%02X ", (uint8_t)buffer[i]);
		}
		bufout[cnt++] = '\r';
		bufout[cnt++] = '\n';
		CDC_Transmit_FS(bufout, (uint16_t)strlen((char *)bufout));
		//for debug purpose,
		//comment this out otherwise we could miss the printf.
		//when run the program and vcp was not open.
		//keep print out the buffer.
		//flash2mem_done = false;
	}
}

/*
 * Call this to start the DMA transfer.
 * dma_callback is a callback function and will be called when DMA transfer is done.
 */
void dma_flash2mem(void) {

	flash2mem_done = false;

	//register a callback when dma Full transfer is done
	HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, dma_callback);
	//start the dma interrupt.
	//source address is flash code
	//destination is the local buffer memory.
	HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, START_OF_FLASH, (uint32_t)&buffer, BUFFER_SIZE);
}
