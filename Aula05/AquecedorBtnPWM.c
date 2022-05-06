// Vinícius Meng - 0250583

/* ########################################################################

   PICsim - PIC simulator http://sourceforge.net/projects/picsim/

   ########################################################################

   Copyright (c) : 2015  Luis Claudio Gambôa Lopes

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
/* Template source file generated by piklab */

#include <xc.h>

#include "config_877A.h"
#include "atraso.h"
#include "lcd.h"
#include "display7s.h"
#include "i2c.h"
#include "serial.h"
#include "eeprom.h"
#include "eeprom_ext.h"
#include "adc.h"
#include "itoa.h"

#define _XTAL_FREQ 4000000UL

unsigned char cnt;
unsigned int t1cont;

void interrupt isrh(void);

char buffer[45];

bit btnRB0, btnRB1;

void main() {
    unsigned char tmp;
    unsigned int tmpi;

    char str[6];

    TRISA = 0xFF;
    TRISB = 0x00;
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;

    lcd_init();
    i2c_init();
    adc_init();

    ADCON1 = 0x02;
    T2CON = 0b00000101; // bit2 = liga [bit 1 bit 0]  preescaler = 4
    CMCON = 0X07; // desliga comparadores
    CCP1CON = 0b00111111; //veja pagina 57 pic16g628a
    PR2 = 255;
    

    TRISAbits.TRISA5 = 0;

    //testa chaves
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("RB0=+      RB1=-");

    tmp = 0;
    
    CCPR1L = 0;
    PORTCbits.RC2 = 1;

    while (1) {
        TRISB = 0x0F;

        if (PORTBbits.RB0 != 0) {
            btnRB0 = 1;
        }

        if (PORTBbits.RB1 != 0) {
            btnRB1 = 1;
        }
        
        if (PORTBbits.RB0 == 0 && btnRB0) {
            btnRB0 = 0;
            CCPR1L += 25;
        }
        
        if (PORTBbits.RB1 == 0 && btnRB0) {
            PORTCbits.RC2 = 0;
            btnRB1 = 0;
            CCPR1L -= 25;
        }

        tmpi = (((unsigned int) adc_amostra(0)*10) / 8) - 150;
        lcd_cmd(L_L2 + 5);
        itoa(tmpi, str);
        lcd_dat(str[2]);
        lcd_dat(str[3]);
        lcd_dat(',');
        lcd_dat(str[4]);
        lcd_dat('C');
    }
}

/**
 * High priority interrupt routine
 */
void interrupt isrh() {
    cnt--;
    if (!cnt) {
        //executada a cada 1 segundo
        T1CONbits.TMR1ON = 0;
        t1cont = (((unsigned int) TMR1H << 8) | (TMR1L)) / 7; //ventilador com 7 pï¿½s
        cnt = 125;
        TMR1H = 0;
        TMR1L = 0;
        T1CONbits.TMR1ON = 1;
    }
    INTCONbits.T0IF = 0;
    TMR0 = 6;
}