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

int main(void) {
  printf("hello\n");

  while (1) {
    /* realiza um polling do periférico a cada segundo e imprime */
    printf("sw val: %02x\n", sw_axi());
    delay_ms(1000);
  }

  return 0;
}
