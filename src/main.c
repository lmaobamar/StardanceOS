void kernel_main(void) {
  volatile char *vga_buffer =
      (volatile char *)0xB8000; // this is a pointer to it
  const char *message = "Hello Stardance!";

  /* * upper 4 bits, "0" (black background)
   * lower 4 bits, "F" (bright white text) */
  char color_attribute = 0x0F;

  int string_index = 0;
  int vga_memory_index = 0;
  while (message[string_index] !=
         '\0') { // ! this failed as "\0" and not '\0'. the compiler sees '' as
                 // char literals
    // * put the char byte at the current EVEN memory address
    vga_buffer[vga_memory_index] = message[string_index];
    // * put the colour byte immediately after it at the ODD memory address
    vga_buffer[vga_memory_index + 1] = color_attribute;
    // advance
    string_index++;
    // say we have added 2 bytes onto memory index basically
    vga_memory_index = vga_memory_index + 2;
  }
}