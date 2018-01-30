/* =============================================================
 * Copyright 2018 The MITRE Corporation.  All rights reserved.
 * =============================================================
 */
#include <project.h>
#include <stdio.h>
#include <strong-arm/sha256.h>
#include <strong-arm/aes.h>

uint8_t hex2byte(char upper_digit, char lower_digit)
{
    uint8_t value = 0;
    
    // Lower nybble.
    if(lower_digit >= '0' && lower_digit <= '9')
    {
        value = lower_digit - '0';
    }
    else if(lower_digit >= 'a' && lower_digit <= 'f')
    {
        value = lower_digit - 'a' + 0xA;
    }
    else if(lower_digit >= 'A' && lower_digit <= 'F')
    {
        value = lower_digit - 'A' + 0xA;
    }
    
    // Upper nybble.
    if(upper_digit >= '0' && upper_digit <= '9')
    {
        value |= (upper_digit - '0') << 4;
    }
    else if(upper_digit >= 'a' && upper_digit <= 'f')
    {
        value |= (upper_digit - 'a' + 0xA) << 4;
    }
    else if(upper_digit >= 'A' && upper_digit <= 'F')
    {
        value |= (upper_digit - 'A' + 0xA) << 4;
    }
    
    // Return result.
    return value;
}

void generate_unsecure_key(uint8_t key[32])
{
    /* NOTE: You can reverse engineer this function if you insist, but it won't help you win... */
    uint32_t short_key[4] = {0x1337EC7F,0x1337EC7F,0x1337EC7F,0x1337EC7F};
    short_key[0] ^= (*(reg32 *)CYREG_HSIOM_PORT_SEL0) ^ CYREG_HSIOM_PORT_SEL1 ^
                    (*(reg32 *)CYREG_HSIOM_PORT_SEL2) ^ (*(reg32 *)CYREG_PRT4_PC);
    short_key[1] ^= USB_UART_CTRL_REG ^ USB_UART_I2C_SLAVE_CMD_REG ^ USB_UART_TX_FIFO_CTRL_REG ^
                    short_key[0];
    short_key[2] ^= Clock_1MHz_DIV_REG ^ Clock_1MHz_ENABLE_REG ^ short_key[1];
    short_key[3] ^= ADC_0_INTCFG ^ ADC_0_INTSTAT ^ ADC_0_PS ^ short_key[2];

    SHA256(key, (uint8_t*)short_key, 16);
}

void wait_for_user_keypress()
{
    while(!USB_UART_UartGetChar())
    { /* Wait for user to hit key. */ }
}

int main()
{
    uint8_t key[32];
    
    /* Enable global interrupts */
    CyGlobalIntEnable;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USB_UART_Start();
    
    generate_unsecure_key(key);
    
    // Part 1: Getting Started with Serial.
    uint8_t ciphertext[16];
    char hex_ciphertext[32] = "aaa79a1c1214c0a8a10aa8b4174ff639";
    char text[64] = "";
    uint8_t plaintext[17];
    for(int i = 0; i < 16; ++i)
    {
        ciphertext[i] = hex2byte(hex_ciphertext[2*i], hex_ciphertext[2*i + 1]);
    }
    aes256_encrypt_block(plaintext, key, ciphertext);
    plaintext[16] = '\0';
    USB_UART_UartPutString("Here's your \"Getting Started with Serial\" flag:\n");
    USB_UART_UartPutString("ectf{");
    USB_UART_UartPutString((char*)plaintext);
    USB_UART_UartPutString("}");
    USB_UART_UartPutString("\nNow fire up that debugger and hit any key to continue...\n");
    wait_for_user_keypress();

    // Part 2: Getting Started with the Debugger.
    char in_memory_flag[32] = "";
    strncpy(hex_ciphertext, "a64ca53aa366fe5232547efe0c7e523b", 32);
    for(int i = 0; i < 16; ++i)
    {
        ciphertext[i] = hex2byte(hex_ciphertext[2*i], hex_ciphertext[2*i + 1]);
    }
    aes256_encrypt_block(plaintext, key, ciphertext);
    plaintext[16] = '\0';
    sprintf(in_memory_flag, "ectf{%s}", plaintext);
    USB_UART_UartPutString("Here's the address of your \"Getting Started with the Debugger\" flag:\n");
    sprintf(text, "%p\n", in_memory_flag);
    USB_UART_UartPutString(text);
    USB_UART_UartPutString("\nNow fire up socat and connect to the server...\n");
        
    // Part 3: Using socat.
    // Get ciphertext.
    char temp;
    for(int i = 0; i < 32; ++i)
    {
        do
        {
            temp = USB_UART_UartGetChar();
            if((temp >= '0' && temp <= '9') || (temp >= 'a' && temp <= 'f') || (temp >= 'A' && temp <= 'F'))
            {   
                hex_ciphertext[i] = temp;
            } 
            else
            {
                hex_ciphertext[i] = 0;   
            }
            
        }
        while(hex_ciphertext[i] == 0);
    }
    for(int i = 0; i < 16; ++i)
    {
        ciphertext[i] = hex2byte(hex_ciphertext[2*i], hex_ciphertext[2*i + 1]);
    }
    aes256_encrypt_block(plaintext, key, ciphertext);
    uint16_t *plaintext16 = (uint16_t*)plaintext;
    sprintf(text, "%04x%04x%04x%04x", plaintext16[0], plaintext16[1], plaintext16[2], plaintext16[3]);
    USB_UART_UartPutString(text);
    sprintf(text, "%04x%04x%04x%04x\n", plaintext16[4], plaintext16[5], plaintext16[6], plaintext16[7]);
    USB_UART_UartPutString(text);
    
    for(int i = 0; i < 64; ++i)
    {
        do
        {
            text[i] = USB_UART_UartGetChar();
        }
        while(text[i] == 0);
    }
    text[63] = '\0';

    for(;;)
    {
        wait_for_user_keypress();
        USB_UART_UartPutString("Here's what we got from the Server: ");
        USB_UART_UartPutString(text);
        USB_UART_UartPutChar('\n');
    }
}
