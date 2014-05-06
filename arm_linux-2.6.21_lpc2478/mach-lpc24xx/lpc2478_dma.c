/*
 * lpc2478 dma
 */
#include <asm/dma.h>

const unsigned long MAX_DMA_ADDRESS = 0x03000000;
const unsigned int NO_DMA = 255;



#ifdef CONFIG_MAX_DMA_CHANNELS

#if CONFIG_MAX_DMA_CHANNELS > 0
const unsigned int MAX_DMA_CHANNELS = CONFIG_MAX_DMA_CHANNELS;
dma_t dma_chan[CONFIG_MAX_DMA_CHANNELS];
#endif

#endif