#include <stdint.h>

/* External symbols from linker script */
extern uint32_t _estack;
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Function prototypes */
void Reset_Handler(void);
int  main(void);

/* Weak aliases - all point to Default_Handler unless overridden */
void NMI_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)           __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)          __attribute__((weak, alias("Default_Handler")));

/* Vector table */
uint32_t vector_tbl[] __attribute__((section(".isr_vector_tbl"))) = {
    (uint32_t)&_estack,           /* Initial stack pointer */
    (uint32_t)&Reset_Handler,     /* Reset handler */
    (uint32_t)&NMI_Handler,       /* NMI handler */
    (uint32_t)&HardFault_Handler, /* Hard fault handler */
    (uint32_t)&MemManage_Handler, /* Memory management fault */
    (uint32_t)&BusFault_Handler,  /* Bus fault */
    (uint32_t)&UsageFault_Handler,/* Usage fault */
    0,                            /* Reserved */
    0,                            /* Reserved */
    0,                            /* Reserved */
    0,                            /* Reserved */
    (uint32_t)&SVC_Handler,       /* SVCall */
    (uint32_t)&DebugMon_Handler,  /* Debug monitor */
    0,                            /* Reserved */
    (uint32_t)&PendSV_Handler,    /* PendSV */
    (uint32_t)&SysTick_Handler,   /* SysTick */
};

/* Default handler - infinite loop for unhandled interrupts */
void Default_Handler(void) {
    while(1) {
    }
}

/* Reset handler - runs before main() */
void Reset_Handler(void) {
    /* Calculate sizes of .data and .bss sections */
    uint32_t data_mem_size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint32_t bss_mem_size  = (uint32_t)&_ebss  - (uint32_t)&_sbss;

    /* Copy .data from Flash to SRAM */
    uint32_t *p_src_mem  = (uint32_t *)&_etext;
    uint32_t *p_dest_mem = (uint32_t *)&_sdata;

    for(uint32_t i = 0; i < data_mem_size; i++) {
        *p_dest_mem++ = *p_src_mem++;
    }

    /* Zero out .bss in SRAM */
    p_dest_mem = (uint32_t *)&_sbss;

    for(uint32_t i = 0; i < bss_mem_size; i++) {
        *p_dest_mem++ = 0;
    }

    /* Call main application */
    main();
}