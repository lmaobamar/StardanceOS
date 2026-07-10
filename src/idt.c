#include "idt.h"
#include "io.h"
#include "keyboard.h"

static struct idt_entry idt[256];
static struct idt_ptr idtp;

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define ICW1_INIT    0x11

static void pic_remap(void) {
    unsigned char a1 = inb(PIC1_DATA);
    unsigned char a2 = inb(PIC2_DATA);
    outb(PIC1_COMMAND, ICW1_INIT);
    outb(PIC2_COMMAND, ICW1_INIT);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

static inline void lidt(const struct idt_ptr *ptr) {
  __asm__ volatile("lidt (%0)" : : "r"(ptr));
}

struct interrupt_frame {
  unsigned int ip;
  unsigned int cs;
  unsigned int flags;
  unsigned int sp;
  unsigned int ss;
};

__attribute__((interrupt)) void
default_interrupt_gate(struct interrupt_frame *frame) {
  (void)frame;
  outb(0x20, 0x20);
  outb(0xA0, 0x20);
}

__attribute__((interrupt)) void
keyboard_interrupt_gate(struct interrupt_frame *frame) {
  (void)frame;
  keyboard_handler();
  outb(0x20, 0x20);
}

static void set_idt_entry(int i, unsigned int base, unsigned short sel, unsigned char flags) {
  idt[i].base_low = base & 0xFFFF;
  idt[i].base_high = (base >> 16) & 0xFFFF;
  idt[i].selector = sel;
  idt[i].always0 = 0;
  idt[i].flags = flags;
}

void init_idt(void) {
  idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtp.base = (unsigned int)&idt;

  unsigned short sel;
  __asm__("mov %%cs, %0" : "=r"(sel));

  unsigned int default_base = (unsigned int)default_interrupt_gate;
  for (int i = 0; i < 256; i++) {
    set_idt_entry(i, default_base, sel, 0x8E);
  }

  pic_remap();

  unsigned int kbd_base = (unsigned int)keyboard_interrupt_gate;
  set_idt_entry(33, kbd_base, sel, 0x8E);

  lidt(&idtp);
}
