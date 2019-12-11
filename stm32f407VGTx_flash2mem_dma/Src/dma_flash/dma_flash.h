/*
 * dma_flash.h
 *
 *  Created on: Dec 10, 2019
 *      Author: pnguyen
 */

#ifndef DMA_FLASH_DMA_FLASH_H_
#define DMA_FLASH_DMA_FLASH_H_

#include "main.h"

// Number OF 32-bit words of code flash
#define FLASH_SIZE 					((1024U*102U)/4U)

//flash code start address
#define START_OF_FLASH 				0x08000000U

//in millisecond ticks
#define ONE_SECOND					1000U


/***** global functions ******************************/
void dma_flash2mem(void);
void dma_flash2mem_done(void) ;

#endif /* DMA_FLASH_DMA_FLASH_H_ */
