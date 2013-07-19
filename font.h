/* 
 * File:   font.h
 * Author: czoido
 *
 * Created on 28 de noviembre de 2012, 13:13
 */

#ifndef FONT_H
#define	FONT_H

#ifdef	__cplusplus
extern "C" {
#endif

static unsigned char font[] = {
 0x00, 0x00, 0x00, 0x00, 0x00,  /*   */
 0x20, 0x00, 0x20, 0x20, 0x20,  /* ! */
 0x00, 0x00, 0x00, 0x50, 0x50,  /* " */
 0x50, 0xF8, 0x50, 0xF8, 0x50,  /* # */
 0x20, 0x70, 0x20, 0x70, 0x20,  /* $ */
 0x88, 0x40, 0x20, 0x10, 0x88,  /* % */
 0x30, 0x58, 0x20, 0x50, 0x20,  /* & */
 0x00, 0x00, 0x00, 0x20, 0x20,  /* ' */
 0x20, 0x40, 0x40, 0x40, 0x20,  /* ( */
 0x20, 0x10, 0x10, 0x10, 0x20,  /* ) */
 0x88, 0x50, 0xA8, 0x50, 0x88,  /* * */
 0x20, 0x20, 0xF8, 0x20, 0x20,  /* + */
 0x80, 0x40, 0x00, 0x00, 0x00,  /* , */
 0x00, 0x00, 0x70, 0x00, 0x00,  /* - */
 0x40, 0x00, 0x00, 0x00, 0x00,  /* . */
 0x80, 0x40, 0x20, 0x10, 0x08,  /* / */
 0x70, 0x88, 0x88, 0x88, 0x70,  /* 0 */
 0x70, 0x20, 0x20, 0x60, 0x20,  /* 1 */
 0xF0, 0x40, 0x20, 0x90, 0x60,  /* 2 */
 0xE0, 0x10, 0x60, 0x10, 0xE0,  /* 3 */
 0x10, 0x10, 0xF0, 0x90, 0x90,  /* 4 */
 0x70, 0x08, 0xF0, 0x80, 0xF8,  /* 5 */
 0x70, 0x88, 0xF0, 0x80, 0x70,  /* 6 */
 0x40, 0x40, 0x20, 0x10, 0xF8,  /* 7 */
 0x70, 0x88, 0x70, 0x88, 0x70,  /* 8 */
 0x10, 0x08, 0x78, 0x88, 0x70,  /* 9 */
 0x40, 0x00, 0x00, 0x40, 0x00,  /* : */
 0x40, 0x20, 0x00, 0x20, 0x00,  /* ; */
 0x10, 0x20, 0x40, 0x20, 0x10,  /* < */
 0x00, 0x70, 0x00, 0x70, 0x00,  /* = */
 0x40, 0x20, 0x10, 0x20, 0x40,  /* > */
 0x10, 0x00, 0x10, 0x48, 0x30,  /* ? */
 0x70, 0x80, 0xB0, 0xB0, 0x60,  /* @ */
 0x88, 0xF8, 0x88, 0x50, 0x20,  /* A */
 0xF0, 0x88, 0xF0, 0x88, 0xF0,  /* B */
 0x78, 0x80, 0x80, 0x80, 0x78,  /* C */
 0xF0, 0x88, 0x88, 0x88, 0xF0,  /* D */
 0xF8, 0x80, 0xF0, 0x80, 0xF8,  /* E */
 0x80, 0x80, 0xF0, 0x80, 0xF0,  /* F */
 0x70, 0x88, 0x98, 0x80, 0x78,  /* G */
 0x88, 0x88, 0xF8, 0x88, 0x88,  /* H */
 0x70, 0x20, 0x20, 0x20, 0x70,  /* I */
 0x60, 0x90, 0x10, 0x10, 0x38,  /* J */
 0x88, 0x90, 0xE0, 0x90, 0x88,  /* K */
 0xF8, 0x80, 0x80, 0x80, 0x80,  /* L */
 0x88, 0xA8, 0xA8, 0xD8, 0x88,  /* M */
 0x88, 0x98, 0xA8, 0xC8, 0x88,  /* N */
 0x70, 0x88, 0x88, 0x88, 0x70,  /* O */
 0x80, 0x80, 0xF0, 0x88, 0xF0,  /* P */
 0x78, 0xA8, 0x88, 0x88, 0x70,  /* Q */
 0x90, 0xA0, 0xF0, 0x88, 0xF0,  /* R */
 0xF0, 0x08, 0x70, 0x80, 0x78,  /* S */
 0x20, 0x20, 0x20, 0x20, 0xF8,  /* T */
 0x70, 0x88, 0x88, 0x88, 0x88,  /* U */
 0x20, 0x50, 0x88, 0x88, 0x88,  /* V */
 0x88, 0xD8, 0xA8, 0xA8, 0x88,  /* W */
 0x88, 0x50, 0x20, 0x50, 0x88,  /* X */
 0x20, 0x20, 0x20, 0x50, 0x88,  /* Y */
 0xF8, 0x40, 0x20, 0x10, 0xF8,  /* Z */
 0x70, 0x40, 0x40, 0x40, 0x70,  /* [ */
 0x08, 0x10, 0x20, 0x40, 0x80,  /* \ */
 0x70, 0x10, 0x10, 0x10, 0x70,  /* ] */
 0x00, 0x00, 0x88, 0x50, 0x20,  /* ^ */
 0xF8, 0x00, 0x00, 0x00, 0x00,  /* _ */
 0x00, 0x00, 0x00, 0x20, 0x20,  /* ` */
 0x88, 0xF8, 0x88, 0x50, 0x20,  /* a */
 0xF0, 0x88, 0xF0, 0x88, 0xF0,  /* b */
 0x78, 0x80, 0x80, 0x80, 0x78,  /* c */
 0xF0, 0x88, 0x88, 0x88, 0xF0,  /* d */
 0xF8, 0x80, 0xF0, 0x80, 0xF8,  /* e */
 0x80, 0x80, 0xF0, 0x80, 0xF0,  /* f */
 0x70, 0x88, 0x98, 0x80, 0x78,  /* g */
 0x88, 0x88, 0xF8, 0x88, 0x88,  /* h */
 0x70, 0x20, 0x20, 0x20, 0x70,  /* i */
 0x60, 0x90, 0x10, 0x10, 0x38,  /* j */
 0x88, 0x90, 0xE0, 0x90, 0x88,  /* k */
 0xF8, 0x80, 0x80, 0x80, 0x80,  /* l */
 0x88, 0xA8, 0xA8, 0xD8, 0x88,  /* m */
 0x88, 0x98, 0xA8, 0xC8, 0x88,  /* n */
 0x70, 0x88, 0x88, 0x88, 0x70,  /* o */
 0x80, 0x80, 0xF0, 0x88, 0xF0,  /* p */
 0x78, 0xA8, 0x88, 0x88, 0x70,  /* q */
 0x90, 0xA0, 0xF0, 0x88, 0xF0,  /* r */
 0xF0, 0x08, 0x70, 0x80, 0x78,  /* s */
 0x20, 0x20, 0x20, 0x20, 0xF8,  /* t */
 0x70, 0x88, 0x88, 0x88, 0x88,  /* u */
 0x20, 0x50, 0x88, 0x88, 0x88,  /* v */
 0x88, 0xD8, 0xA8, 0xA8, 0x88,  /* w */
 0x88, 0x50, 0x20, 0x50, 0x88,  /* x */
 0x20, 0x20, 0x20, 0x50, 0x88,  /* y */
 0xF8, 0x40, 0x20, 0x10, 0xF8,  /* z */
 0x30, 0x20, 0x60, 0x20, 0x30,  /* { */
 0x20, 0x20, 0x00, 0x20, 0x20,  /* | */
 0x60, 0x20, 0x30, 0x20, 0x60,  /* } */
 0x00, 0x00, 0x90, 0x68, 0x00,  /* ~ */
};


#ifdef	__cplusplus
}
#endif

#endif	/* FONT_H */

