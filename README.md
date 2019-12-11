# CubeMx-dma-memory-to-memory-transfer (stm32f407-disc)
This code is using CubeMx to enable dma to transfer data from memory (flash) to memory (local buffer). 

It's quite easy using CubeMx. 
  
    1. Go to "System Core->DMA->MemToMem" click on "Add" Button.
    2. Go to "System Core->NIC" check the "DMA2 stream0 global interrupt"

That's all you have to do. (For this project, I enable the VCP for print out the data from flash but you don't have to do it)

What this project does is copy 16 bytes from flash code address start at 0x8000000 to a local buffer.

The function dma_flash2mem is called from main. It called HAL_DMA_RegisterCallback to register a call back (dma_callback), and 
passing in the enum HAL_DMA_XFER_CPLT_CB_ID. it means when the full transfer is done generate an interrupt. 

The dma_flash2mem also call the HAL_DMA_Start_IT which need to pass in three parameters.
      
      1. hdma_memtomem_dma2_stream0 is generated from CubeMx when you select the DMA MEMToMEM,
      2. START_OF_FLASH is the start address of your firmware (0x8000000).
      3. buffer is a local buffer to this application
      

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

This is the call back that register with HAL_DMA_RegisterCallback function. It set the flash2mem_done to true to tell the main
loop data is ready to process. 

      /*
       * this callback will be called after the dma transfer up to BUFFER_SIZE
       * In this case, it's 16 bytes.
       *
       */
      static void dma_callback(DMA_HandleTypeDef *hdma) {
        //could be use for trigger the main loop or a task that dma transfer is done.
        flash2mem_done = true;
      }

This function is called from main loop and print out the data.

     /*
     * Call from the main loop that the DMA is done.
     * Ready for process the data.
     */
    void dma_flash2mem_done(void) {
      uint8_t cnt=0;
      
      //flash2mem_done is set to true when the DMA transfer is done.
      if (flash2mem_done) {
        //convert the data to ascii hex to print out on the VCP.
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
    
