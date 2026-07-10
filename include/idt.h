#ifndef IDT_H
#define IDT_H

// a single idt entry's structure
struct idt_entry {
  unsigned short base_low;  // lower 16 bits of our handler function address
  unsigned short selector;  // kernel code segment selector (usually 0x08?)
  unsigned char always0;    // always set to 0
  unsigned char flags;      // access flags (0x8E for a 32 bit interrupt gate)
  unsigned short base_high; // upper 16 bits of our handler function address
} __attribute__((packed));

// the pointer structure we pass to the 'lidt' instruction
struct idt_ptr {
  unsigned short limit; // size of the idt array - 1
  unsigned int base;    // linear addr of the idt array
} __attribute__((packed));

void init_idt(void);

#endif