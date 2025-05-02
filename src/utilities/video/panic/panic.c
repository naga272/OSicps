#include "config.h"

#include "utilities/video/print/video.h"
#include "utilities/video/panic/panic.h"


extern void set_terminal_row(u16);
extern void set_terminal_column(u16);
extern void set_actual_color_terminal(char);

void terminal_initialize(char colore);
extern void print(const uchar* string);


/*
* Quando si entra in azione la procedura panic() l'interrupt 0x21 deve essere bloccato, l'user non deve poter scrivere.
* L'unica azione che e' concessa e' quella di riavviare tramite tasto della macchina
*/
extern void idt_set(u32 interrupt_no, void* address);
extern void no_interrupt_handler();


void panic(const uchar* msg_error)
{
    /*
    *   Funzione che viene applicata quando le cose stanno andando davvero male. 
    *   Obbliga l'utente a riavviare la macchina dal tasto fisico.
    */
    
    // devo disabilitare tutto PIC
    for (u8 i = 0x20; i != 0xA1; i++)
        idt_set(i, no_interrupt_handler);

    terminal_initialize(BG_BLU_C_WHITE);

    set_actual_color_terminal(BG_BLU_C_WHITE);
    set_terminal_row((u16) 0);
    set_terminal_column((u16) 0);

    print((const uchar*) panic_face);
    print(msg_error);
}
