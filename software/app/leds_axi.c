#include <hf-risc.h>

/* endereço do periférico AXI */
#define LEDS_AXI_BASE 0xe4a90000

/* registradores STATUS e DATA, implementados no hardware */
#define LEDS_AXI_STATUS (*(volatile uint32_t *)(LEDS_AXI_BASE + 0x010))
#define LEDS_AXI_MDATA (*(volatile uint32_t *)(LEDS_AXI_BASE + 0x030))

/* máscada dos bits READY e VALID, presentes no registrador STATUS */
#define LEDS_AXI_MTREADY (1 << 2)
#define LEDS_AXI_MTVALID (1 << 3)

void leds_axi(uint8_t byte) {
  /* escrita no registrador DATA, ativa uma transferência para o periférico */
  LEDS_AXI_MDATA = byte;
  /* leitura do registrador STATUS e espera ocupada, até que o periférico tenha
   * aceito o dado */
  while (!(LEDS_AXI_STATUS & LEDS_AXI_MTREADY))
    ;
}

int main(void) {
  uint8_t data = 0x01;

  while (1) {
    leds_axi(data);
    printf("LEDs value: %02x\n", LEDS_AXI_MDATA);
    data <<= 1;
    if (!data)
      data = 0x01;
    delay_ms(1000);
  }

  return 0;
}
