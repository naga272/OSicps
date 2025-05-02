#ifndef IO_H
#define IO_H

/*
* Qui implemento funzioni di input/output (I/O) a basso livello per interagire con l'hardware del sistema, 
* come le porte di I/O (ad esempio, per comunicare con dispositivi come la tastiera, il mouse, il timer, ecc.).
* Funzioni:
*   insb: Legge un byte (8 bit) da una porta di I/O.
*   insw: Legge una word (16 bit) da una porta di I/O.
*   outb: Scrive un byte (8 bit) in una porta di I/O.
*   outw: Scrive una word (16 bit) in una porta di I/O.
*
*
*   Per porta intendo un canale per constire alla cpu di inviare e ricevere dati dai dispositivi.
*   Le porte di I/O sono degli indirizzi numerici (0x20, 0x21, ...) che il processore usa per comunicare
*   Ogni dispositivo hardware ha la sua porta per comunicare. 
*   L'accesso alle porte viene tramite l'uso delle istruzioni assembly in / out:
*   
*   mov dx, 0x60
*   in al, dx
*
*   Questo legge un byte dalla porta 0x60 (lo inserisce in al) 
*/

unsigned char insb(unsigned char port);
unsigned char insw(unsigned char port);

void outb(unsigned short port, unsigned char val);
void outw(unsigned short port, unsigned short val);

#endif