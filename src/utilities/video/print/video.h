
#ifndef VIDEO_H
#define VIDEO_H

#include "config.h"
#include <stddef.h>

/* STANDARD COLOR */
#define NERO            0
#define BLU             1
#define VERDE           2
#define CIANO           3
#define ROSSO           4
#define VIOLA           5
#define MARRONE         6
#define GRIGIO          7
#define GRIGIO_SCURO    8
#define BLU_CHIARO      9
#define VERDE_CHIARO    10
#define CIANO_CHIARO    11
#define ROSSO_CHIARO    12
#define VIOLA_CHIARO    13
#define GIALLO          14
#define BIANCO          15

/* SET BACKBGROUND CON COLORE TESTO*/
#define BG_NERO_C_VERDE     (NERO << 4)     | VERDE
#define BG_BLU_C_WHITE      (BLU << 4)      | BIANCO
#define BG_BIANCO_C_NERO    (BIANCO << 4)   | NERO


#ifdef settings_video

// default colore dei caratteri del terminale (in futuro potrebbe cambiare il colore in base alle situazioni)
char actual_color_terminal = VERDE_CHIARO; 
u16* video_mem      = 0;
u16 terminal_row    = 0;  // tiene traccia a quale riga del monitor sto scrivendo (max value VGA_HEIGHT)
u16 terminal_col    = 0;  // tiene traccia della colonna del monitor da dove sto scrivendo (max value VGA_WIDTH)
u8 console_loglevel = 7;  // default value start kernel
#endif


#define VGA_WIDTH   80
#define VGA_HEIGHT  100


#ifdef prototype_fun_video_h

u16 set_char_terminal(char c, char colour);
void terminal_put_char(u32 x, u32 y, char c, char colour);
void terminal_writechar(char c, char colour);
void print(const uchar* string);
void terminal_initialize();
void printk(const char*);

u16 get_terminal_row();
u16 get_terminal_column();
u16 get_actual_color_terminal();

u16 set_terminal_row();
u16 set_terminal_column();
u16 set_actual_color_terminal();


#undef prototype_fun_video_h
#endif

#endif
