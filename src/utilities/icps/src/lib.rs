#![no_std]
#![no_main]


use core::panic::PanicInfo;


// Buffer per la tastiera (usando un array fisso)
static mut KB_BUFFER: [u8; 128] = [0; 128];  // Un buffer per i caratteri della tastiera
static mut KB_INDEX: usize = 0;  // Indice per la scrittura nel buffer


// La funzione di gestione del panico deve essere presente
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}


/* ENTRY POINT PER LA SHELL */
#[no_mangle]
pub extern "C" fn icps_init() {
    loop {}
}


#[no_mangle]
pub extern "C" fn kb_set_c(c: u8) {
    unsafe {
        KB_BUFFER[KB_INDEX] = c;
        KB_INDEX += 1;
    }

    return;
}