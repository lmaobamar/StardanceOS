#include "console.h"
#include "framebuffer.h"
#include "idt.h"
#include "io.h"
#include "serial.h"

void kernel_main(unsigned int multiboot_info_ptr) {
  init_serial();
  init_framebuffer(multiboot_info_ptr);
  init_console();
  kprintf("Starting StardanceOS...\n");
  init_idt();
  kprintf("Interrupt Descriptor Table loaded\n");

  outb(0x21, 0xFD);
  outb(0xA1, 0xFF);

  __asm__ volatile("sti");
  kprintf("Interrupts enabled globally\n");

  while (1) {
    __asm__ volatile("hlt");
  }
}