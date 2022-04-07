/* Vinicius Meng - 0250583 */

/* ########################################################################

   PICsim - PIC simulator http://sourceforge.net/projects/picsim/

   ########################################################################

   Copyright (c) : 2015  Luis Claudio Gamb�a Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

/* ----------------------------------------------------------------------- */

// PIC16F628A Configuration Bit Settings

#include "config_628A.h"
#include "atraso.h"
#include "display7s.h"
#include "eeprom.h"

/*
    1 ciclo de maquina = 1/(clock/4)
    1 ciclo de maquina = 1/(4Mhz/4) = 1us
    timer0 = [0 255] = 256
    prescaler = 1:4 
    T = 1 ciclo de maquina * prescaler * 256
    T = 1us * 4 * 256 = 1024 us
    tempo de troca de RB0 desejado 500ms
    tempo de troca de RB0 = T * 500 = 0.512 s
    inicializar timer0 = 6
    tempo de troca de RB0 = 1us * 4 * 250 * 500 = 0.500 s
 */

int counter1 = 0x00; // Vari�vel de contagem auxiliar

void __interrupt() IRS(void) // Vetor de interrup��o 
{
    if (INTCONbits.T0IF == 1) // Houve o estouro do TIMER0?
    {
        counter1++; // Incrementa o counter
        PORTBbits.RB1 = ~PORTBbits.RB1;
        TMR0 = 0x00; // Reinicia o registrador TMR0

        T0IF = 0x00; // Limpa a flag para a pr�xima interrup��o
    }
} //end interrupt

void main() {
    unsigned char tmp;
    unsigned char i;
    
    tmp = 0;
    PORTB = display7s(tmp) | 0x10;

    CMCON = 0x07;
    TRISA = 0xFE;
    TRISB = 0X00;

    // tmp = e2prom_r(0x10);
    // e2prom_w(0x10,tmp+1);

    while (1) {
        //testa display 7s;
        /*for (tmp = 0; tmp < 16; tmp++) {
            PORTB = display7s(tmp) | 0x10;
            atraso_ms(200);
        }

        for (tmp = 0; tmp < 16; tmp++) {
            PORTB = display7s(tmp);
            atraso_ms(200);
        }

        for (tmp = 0; tmp < 16; tmp++) {
            for (i = 0; i < 10; i++) {
                PORTB = display7s(tmp);
                atraso_ms(10);
                PORTB = display7s(tmp) | 0x10;
                atraso_ms(10);
            }
        }

        PORTB = 0;

        //testa RA0
        for (i = 0; i < 4; i++) {
            RA0 ^= 1;
            atraso_ms(500);
        }

        //testa LEDs
        for (tmp = 0; tmp < 3; tmp++) {
            for (i = 1; i > 0; i = i * 2) {
                PORTB = i;
                atraso_ms(200);
            }
        }

        PORTB = 0;

        for (i = 0; i < 4; i++) {
            PORTB ^= 0xFF;
            atraso_ms(200);
        } */

        //testa chaves
        
        if (RA1 == 0) {
            if (tmp < 99) tmp++;
            while (RA1 == 0);
        }
        
        if (RA2 == 0) {
            if (tmp > 0) tmp--;
            while (RA2 == 0);
        }
        
        if (tmp < 10) {
            PORTB = display7s(0);
            i = 0;
        } else if (tmp >= 10 && tmp < 20) {
            PORTB = display7s(1);
            i = 10;
        } else if (tmp >= 20 && tmp < 30) {
            PORTB = display7s(2);
            i = 20;
        } else if (tmp >= 30 && tmp < 40) {
            PORTB = display7s(3);
            i = 30;
        } else if (tmp >= 40 && tmp < 50) {
            PORTB = display7s(4);
            i = 40;
        } else if (tmp >= 50 && tmp < 60) {
            PORTB = display7s(5);
            i = 50;
        } else if (tmp >= 60 && tmp < 70) {
            PORTB = display7s(6);
            i = 60;
        } else if (tmp >= 70 && tmp < 80) {
            PORTB = display7s(7);
            i = 70;
        } else if (tmp >= 80 && tmp < 90) {
            PORTB = display7s(8);
            i = 80;
        } else if (tmp >= 90) {
            PORTB = display7s(9);
            i = 90;
        }
        atraso_ms(5);
        
        PORTB = display7s(tmp - i) | 0x10;
        atraso_ms(5);
        
        /*while ((PORTB & 0x0F) != 0x0F) {
            if (RA1 == 0) {
                PORTB |= 0x01;
                while (RA1 == 0);
            }
            if (RA2 == 0) {
                PORTB |= 0x02;
                while (RA2 == 0);
            }
            if (RA3 == 0) {
                PORTB |= 0x04;
                while (RA3 == 0);
            }
            if (RA4 == 0) {
                PORTB |= 0x08;
                while (RA4 == 0);
            }
        }*/
        //PORTB = 0;

        //testa LEDS2
        /*TRISA = 0xE0;
        PORTA = 0;
        for (i = 0; i < 4; i++) {
            RA1 ^= 1;
            atraso_ms(200);
            RA2 ^= 1;
            atraso_ms(200);
            RA3 ^= 1;
            atraso_ms(200);
            RA4 ^= 1;
            atraso_ms(200);
        }
        
        TRISA = 0xFE;*/
        
        //fim
        //while (1);
    }
}
