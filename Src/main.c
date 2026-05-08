/*
 * Bare Metal LED Blink - STM32F767ZI
 *
 * 1. Find base addresses from reference manual
 * 2. Calculate final address = base + offset
 * 3. Cast address as volatile pointer to access register
 * 4. Enable peripheral clock via RCC
 * 5. Configure pin as output via MODER register
 * 6. Control pin state via ODR register
 * 7. Use XOR to toggle LED on and off
 * 8. Use software delay to control blink speed
 *
 * Key registers:
 * RCC_AHB1EN_R = 0x40023830  enable peripheral clock
 * GPIOB_MODE_R = 0x40020400  configure pin direction
 * GPIOB_OD_R   = 0x40020414  control pin output
 *
 * Key operations:
 * |=   set bit ON
 * &=~  clear bit OFF
 * ^=   toggle bit
 */



#include <stdint.h>

//  1: Define base address for peripherals
#define PERIPH_BASE        (0x40000000UL)
//  2: Offset for AHB1 peripheral bus
#define AHB1PERIPH_OFFSET  (0x00020000UL)
//  3: Base address for AHB1 peripherals
#define AHB1PERIPH_BASE    (PERIPH_BASE + AHB1PERIPH_OFFSET)
//  4: Offset for GPIOB
#define GPIOB_OFFSET       (0x0400UL)
//  5: Base address for GPIOB
#define GPIOB_BASE         (AHB1PERIPH_BASE + GPIOB_OFFSET)
//  6: Offset for RCC
#define RCC_OFFSET         (0x3800UL)
//  7: Base address for RCC
#define RCC_BASE           (AHB1PERIPH_BASE + RCC_OFFSET)
//  8: Offset for AHB1EN register
#define AHB1EN_R_OFFSET    (0x30UL)
//  9: Address of AHB1EN register
#define RCC_AHB1EN_R       (*(volatile unsigned int *)(RCC_BASE + AHB1EN_R_OFFSET))
//  10: Offset for mode register
#define MODE_R_OFFSET      (0x00UL)
//  11: Address of GPIOB mode register
#define GPIOB_MODE_R       (*(volatile unsigned int *)(GPIOB_BASE + MODE_R_OFFSET))
//  12: Offset for output data register
#define OD_R_OFFSET        (0x14UL)
//  13: Address of GPIOB output data register
#define GPIOB_OD_R         (*(volatile unsigned int *)(GPIOB_BASE + OD_R_OFFSET))
//  14: Bit mask for enabling GPIOB (bit 1)
#define GPIOBEN            (1U << 1)
//  15: Bit mask for GPIOB pin 0
#define PIN0               (1U << 0)
//  16: Alias for PIN0 representing LED pin
#define LED_PIN            PIN0


void delay(volatile uint32_t count){
    while(count--);
}


//  17: Start of main function
int main(void)
{
    //  18: Enable clock access to GPIOB
    RCC_AHB1EN_R |= GPIOBEN;
    //  19: Set PB0 as output
    GPIOB_MODE_R |= (1U << 0);
    //  20: Clear bit 1
    GPIOB_MODE_R &= ~(1U << 1);
    //  21: Start of infinite loop
    while(1)
    {
        // mask to turn off then on


    	GPIOB_OD_R ^= (1 << 0);
        delay(1000000);

    }  //  23: End of infinite loop

}  //  24: End of main function
