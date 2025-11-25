#include <hf-risc.h>

/* endereço do periférico AXI */
#define SW_AXI_BASE 0xe4a90000

/* registradores STATUS e DATA, implementados no hardware */
#define SW_AXI_STATUS (*(volatile uint32_t *)(SW_AXI_BASE + 0x010))
#define SW_AXI_SDATA (*(volatile uint32_t *)(SW_AXI_BASE + 0x020))

/* máscada dos bits READY e VALID, presentes no registrador STATUS */
#define SW_AXI_STREADY (1 << 0)
#define SW_AXI_STVALID (1 << 1)

uint8_t sw_axi() {
  uint8_t data;

  /* leitura 'fake' do registrador DATA, ativa uma transferência do periférico
   */
  data = SW_AXI_SDATA;
  /* leitura do registrador STATUS e espera ocupada, até que o periférico tenha
   * um dado válido */
  while (!(SW_AXI_STATUS & SW_AXI_STVALID))
    ;
  /* leitura 'real' do registrador DATA, contendo os dados transferidos do
   * periférico */
  data = SW_AXI_SDATA;

  return data;
}

void print_key_name(int scan_code) {
  // We use a switch statement to map the hexadecimal scan code value to the key
  // name.

  switch (scan_code) {
  // --- Letters ---
  case 0x1C:
    printf("A");
    break;
  case 0x32:
    printf("B");
    break;
  case 0x21:
    printf("C");
    break;
  case 0x23:
    printf("D");
    break;
  case 0x24:
    printf("E");
    break;
  case 0x2B:
    printf("F");
    break;
  case 0x34:
    printf("G");
    break;
  case 0x33:
    printf("H");
    break;
  case 0x43:
    printf("I");
    break;
  case 0x3B:
    printf("J");
    break;
  case 0x42:
    printf("K");
    break;
  case 0x4B:
    printf("L");
    break;
  case 0x3A:
    printf("M");
    break;
  case 0x31:
    printf("N");
    break;
  case 0x44:
    printf("O");
    break;
  case 0x4D:
    printf("P");
    break;
  case 0x15:
    printf("Q");
    break;
  case 0x2D:
    printf("R");
    break;
  case 0x1B:
    printf("S");
    break;
  case 0x2C:
    printf("T");
    break;
  case 0x3C:
    printf("U");
    break;
  case 0x2A:
    printf("V");
    break;
  case 0x1D:
    printf("W");
    break;
  case 0x22:
    printf("X");
    break;
  case 0x35:
    printf("Y");
    break;
  case 0x1A:
    printf("Z");
    break;

  // --- Numbers & Symbols ---
  case 0x45:
    printf("0");
    break;
  case 0x16:
    printf("1");
    break;
  case 0x1E:
    printf("2");
    break;
  case 0x26:
    printf("3");
    break;
  case 0x25:
    printf("4");
    break;
  case 0x2E:
    printf("5");
    break;
  case 0x36:
    printf("6");
    break;
  case 0x3D:
    printf("7");
    break;
  case 0x3E:
    printf("8");
    break;
  case 0x46:
    printf("9");
    break;
  case 0x0E:
    printf("-");
    break;
  case 0x55:
    printf("=");
    break;
  case 0x54:
    printf("[");
    break;
  case 0x5B:
    printf("]");
    break;
  case 0x4C:
    printf("'");
    break;
  case 0x52:
    printf("\\");
    break;
  case 0x41:
    printf("~");
    break; // Tilde/Grave Accent
  case 0x49:
    printf(",");
    break;
  case 0x4A:
    printf(".");
    break;

  // --- Punctuation/Misc (Single Byte) ---
  case 0x4E:
    printf("[Missing Key 4E]");
    break;
  case 0x5D:
    printf("[Missing Key 5D]");
    break;

  // --- Control Keys (Single Byte) ---
  case 0x66:
    printf("BKSP");
    break;
  case 0x29:
    printf("SPACE");
    break;
  case 0x0D:
    printf("TAB");
    break;
  case 0x58:
    printf("CAPS");
    break;
  case 0x12:
    printf("L SHFT");
    break;
  case 0x14:
    printf("L CTRL");
    break;
  case 0x11:
    printf("L ALT");
    break;
  case 0x59:
    printf("R SHFT");
    break;
  case 0x5A:
    printf("ENTER");
    break;
  case 0x76:
    printf("ESC");
    break;
  case 0x7E:
    printf("SCROLL");
    break;
  case 0x77:
    printf("NUM");
    break; // Num Lock

  // --- Function Keys (Single Byte) ---
  case 0x05:
    printf("F1");
    break;
  case 0x06:
    printf("F2");
    break;
  case 0x04:
    printf("F3");
    break;
  case 0x0C:
    printf("F4");
    break;
  case 0x03:
    printf("F5");
    break;
  case 0x0B:
    printf("F6");
    break;
  case 0x83:
    printf("F7");
    break; // Note: 0x83 is typically F7/F8 in different sets/modes
  case 0x0A:
    printf("F8");
    break;
  case 0x01:
    printf("F9");
    break;
  case 0x09:
    printf("F10");
    break;
  case 0x78:
    printf("F11");
    break;
  case 0x07:
    printf("F12");
    break;

  // --- Numeric Keypad Keys (Single Byte) ---
  case 0x7C:
    printf("KP *");
    break;
  case 0x7B:
    printf("KP -");
    break;
  case 0x79:
    printf("KP +");
    break;
  case 0x71:
    printf("KP .");
    break;
  case 0x70:
    printf("KP 0");
    break;
  case 0x69:
    printf("KP 1");
    break;
  case 0x72:
    printf("KP 2");
    break;
  case 0x7A:
    printf("KP 3");
    break;
  case 0x6B:
    printf("KP 4");
    break;
  case 0x73:
    printf("KP 5");
    break;
  case 0x74:
    printf("KP 6");
    break;
  case 0x6C:
    printf("KP 7");
    break;
  case 0x75:
    printf("KP 8");
    break;
  case 0x7D:
    printf("KP 9");
    break;

  // --- Multi-byte codes start with E0 or E1. The following case only tells us
  // we saw the start.
  case 0xE0:
    printf("START OF E0 EXTENDED CODE (e.g., Arrows, Del, PrintScreen)");
    break;
  case 0xE1:
    printf("START OF E1 EXTENDED CODE (e.g., Pause)");
    break;

  default:
    printf("Unknown Key");
    break;
  }
  printf("\n");
}

int main(void) {
  printf("hello\n");
  uint8_t x;
  uint8_t key_press = 0;
  while (1) {
    x = sw_axi();
    if (x == 240) {
      key_press = 1;
    } else if (key_press) {
      print_key_name(x);
      key_press = 0;
    }
  }

  return 0;
}
