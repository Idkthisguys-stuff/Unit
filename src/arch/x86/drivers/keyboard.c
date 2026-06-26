#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

unsigned char read_port(unsigned short port);
void write_port(unsigned short port, unsigned char data);

int shift_pressed = 0;

extern char *vidptr;
extern unsigned int current_loc;

unsigned char keyboard_map[128] = {
    0,   27,  '1',  '2',  '3',  '4', '5', '6',  '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
    'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
    'j', 'k', 'l',  ';',  '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm',  ',',  '.',  '/', 0,   '*',  0,   ' '};

unsigned char keyboard_map_shifted[128] = {
    0,   27,  '!',  '@',  '#',  '$', '%', '^',  '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q',  'W', 'E', 'R',  'T', 'Y', 'U', 'I',
    'O', 'P', '{',  '}',  '\n', 0,   'A', 'S',  'D', 'F', 'G', 'H',
    'J', 'K', 'L',  ':',  '"',  '~', 0,   '|',  'Z', 'X', 'C', 'V',
    'B', 'N', 'M',  '<',  '>',  '?', 0,   '*',  0,   ' '};

void keyboard_handler_main(void) {
  unsigned char status = read_port(KEYBOARD_STATUS_PORT);

  if (status & 0x01) {
    unsigned char scancode = read_port(KEYBOARD_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36) {
      shift_pressed = 1;
    } else if (scancode == (0x2A + 0x80) || scancode == (0x36 + 0x80)) {
      shift_pressed = 0;
    } else if (scancode < 0x80) {
      char keycode = shift_pressed ? keyboard_map_shifted[scancode] : keyboard_map[scancode];

      if (keycode >= 0) {
        vidptr[current_loc++] = keycode;
        vidptr[current_loc++] = 0x07;
      }
    }
  }
  write_port(0x20, 0x20);
}