#include <hf-risc.h>
#include "vga_drv.h"

#define NUM_INIMIGOS 12
#define MAX_TIROS 30
#define NUM_BARREIRAS 4

/* Cores definidas */
#define COR_FUNDO 0
#define COR_NAVE 7
#define COR_TIRO 7
#define COR_INIMIGO1 2
#define COR_INIMIGO2 6
#define COR_INIMIGO3 1
#define COR_BARREIRA 8
#define COR_TEXTO 15

/* Sprites refeitos com designs diferentes */
unsigned char inimigoTipo1A[8][10] = {
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0}, {0, 2, 2, 0, 2, 2, 0, 2, 2, 0},
    {2, 0, 0, 2, 2, 2, 2, 0, 0, 2}, {2, 0, 2, 0, 0, 0, 0, 2, 0, 2},
    {2, 0, 2, 2, 2, 2, 2, 2, 0, 2}, {0, 2, 0, 2, 2, 2, 2, 0, 2, 0},
    {0, 0, 2, 0, 0, 0, 0, 2, 0, 0}, {0, 2, 0, 0, 0, 0, 0, 0, 2, 0}};

unsigned char inimigoTipo1B[8][10] = {
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0}, {0, 2, 2, 0, 2, 2, 0, 2, 2, 0},
    {2, 0, 0, 2, 2, 2, 2, 0, 0, 2}, {2, 0, 2, 2, 2, 2, 2, 2, 0, 2},
    {2, 0, 0, 2, 2, 2, 2, 0, 0, 2}, {0, 2, 2, 0, 0, 0, 0, 2, 2, 0},
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0}, {0, 0, 2, 0, 0, 0, 0, 2, 0, 0}};

unsigned char inimigoTipo2A[8][9] = {
    {0, 0, 0, 6, 6, 6, 0, 0, 0}, {0, 0, 6, 6, 6, 6, 6, 0, 0},
    {0, 6, 6, 0, 6, 0, 6, 6, 0}, {6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 0, 6, 6, 6, 6, 6, 0, 6}, {6, 6, 0, 0, 0, 0, 0, 6, 6},
    {0, 6, 6, 6, 6, 6, 6, 6, 0}, {0, 0, 6, 0, 0, 0, 6, 0, 0}};

unsigned char inimigoTipo2B[8][9] = {
    {0, 0, 0, 6, 6, 6, 0, 0, 0}, {0, 0, 6, 6, 6, 6, 6, 0, 0},
    {0, 6, 6, 0, 6, 0, 6, 6, 0}, {6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 0, 0, 6, 6, 6, 0, 0, 6}, {0, 6, 6, 0, 0, 0, 6, 6, 0},
    {0, 0, 6, 6, 6, 6, 6, 0, 0}, {0, 6, 0, 0, 0, 0, 0, 6, 0}};

unsigned char inimigoTipo3A[8][11] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1}, {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}};

unsigned char inimigoTipo3B[8][11] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0}, {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0}};

unsigned char naveJogador[8][13] = {{0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 0},
                                    {0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0},
                                    {0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0},
                                    {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
                                    {0, 0, 7, 7, 0, 0, 0, 0, 0, 7, 7, 0, 0}};

unsigned char barreira[10][16] = {
    {0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0},
    {0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0},
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0},
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0},
    {8, 8, 8, 8, 0, 0, 8, 8, 0, 0, 8, 8, 8, 8, 0, 0},
    {8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0},
    {8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0}};

unsigned char projetilJogador[4][3] = {
    {0, 7, 0}, {7, 7, 7}, {0, 7, 0}, {0, 7, 0}};

unsigned char projetilInimigo[4][3] = {
    {0, 2, 0}, {0, 2, 0}, {2, 2, 2}, {0, 2, 0}};

/* Sistema de desenho otimizado */
void desenhar_sprite(unsigned int x, unsigned int y, unsigned char *sprite,
                     unsigned int largura, unsigned int altura, int cor) {
  unsigned int px, py;

  if (cor < 0) {
    for (py = 0; py < altura; py++) {
      for (px = 0; px < largura; px++) {
        unsigned char pixel = sprite[py * largura + px];
        if (pixel != 0) {
          display_pixel(x + px, y + py, pixel);
        }
      }
    }
  } else {
    for (py = 0; py < altura; py++) {
      for (px = 0; px < largura; px++) {
        if (sprite[py * largura + px] != 0) {
          display_pixel(x + px, y + py, cor & 0xf);
        }
      }
    }
  }
}

/* Estruturas do jogo */
struct Objeto {
  unsigned char *quadro_sprite[2];
  unsigned char largura, altura;
  unsigned char quadro_atual;
  unsigned int pos_x, pos_y;
  char dir_x, dir_y;
  unsigned char velocidade;
  unsigned char contador_velocidade;
  unsigned char ativo;
  unsigned char tipo;
};

struct Jogo {
  unsigned int pontuacao;
  unsigned char vidas;
  unsigned char nivel;
  unsigned char direcao_inimigos;
  unsigned char velocidade_inimigos;
  unsigned char inimigos_restantes;
  unsigned char contador_tiro;
};

/* Variáveis globais */
struct Objeto jogador;
struct Objeto inimigos[NUM_INIMIGOS];
struct Objeto tiros[MAX_TIROS];
struct Objeto barreiras[NUM_BARREIRAS];
struct Jogo estado_jogo;

unsigned char num_tiros = 0;
unsigned char contador_animacao = 0;

/* Inicialização de objetos */
void inicializar_objeto(struct Objeto *obj, unsigned char *sprite1,
                        unsigned char *sprite2, unsigned char largura,
                        unsigned char altura, unsigned int x, unsigned int y,
                        char dx, char dy, unsigned char vel,
                        unsigned char tipo) {
  obj->quadro_sprite[0] = sprite1;
  obj->quadro_sprite[1] = sprite2;
  obj->largura = largura;
  obj->altura = altura;
  obj->quadro_atual = 0;
  obj->pos_x = x;
  obj->pos_y = y;
  obj->dir_x = dx;
  obj->dir_y = dy;
  obj->velocidade = vel;
  obj->contador_velocidade = vel;
  obj->ativo = 1;
  obj->tipo = tipo;
}

void desenhar_objeto(struct Objeto *obj, unsigned char mudar_sprite, int cor) {
  if (mudar_sprite && obj->quadro_sprite[1] != 0) {
    obj->quadro_atual = !obj->quadro_atual;
  }

  desenhar_sprite(obj->pos_x, obj->pos_y, obj->quadro_sprite[obj->quadro_atual],
                  obj->largura, obj->altura, cor);
}

/* Sistema de movimento com formação */
void mover_inimigos_em_formacao() {
  unsigned char mudar_direcao = 0;
  unsigned int i;

  /* Verifica se algum inimigo ativo atingiu as bordas */
  for (i = 0; i < NUM_INIMIGOS; i++) {
    if (!inimigos[i].ativo)
      continue;

    if ((inimigos[i].pos_x <= 5 && estado_jogo.direcao_inimigos == -1) ||
        (inimigos[i].pos_x + inimigos[i].largura >= VGA_WIDTH - 5 &&
         estado_jogo.direcao_inimigos == 1)) {
      mudar_direcao = 1;
      break;
    }
  }

  /* Muda direção e desce */
  if (mudar_direcao) {
    estado_jogo.direcao_inimigos = -estado_jogo.direcao_inimigos;

    for (i = 0; i < NUM_INIMIGOS; i++) {
      if (inimigos[i].ativo) {
        inimigos[i].pos_y += 8; /* Desce uma linha */

        /* Game over se inimigos chegarem muito perto */
        if (inimigos[i].pos_y + inimigos[i].altura >= 180) {
          estado_jogo.vidas = 0;
        }
      }
    }
  }

  /* Move todos os inimigos na mesma direção */
  for (i = 0; i < NUM_INIMIGOS; i++) {
    if (inimigos[i].ativo) {
      inimigos[i].pos_x += estado_jogo.direcao_inimigos * 2;
    }
  }
}

/* Sistema de colisão corrigido */
unsigned char verificar_colisao(struct Objeto *obj1, struct Objeto *obj2) {
  return (obj1->pos_x < obj2->pos_x + obj2->largura &&
          obj1->pos_x + obj1->largura > obj2->pos_x &&
          obj1->pos_y < obj2->pos_y + obj2->altura &&
          obj1->pos_y + obj1->altura > obj2->pos_y);
}

/* Limpeza de tiros inativos */
void limpar_tiros_inativos() {
  unsigned char i, j;

  for (i = 0, j = 0; i < num_tiros; i++) {
    if (tiros[i].ativo) {
      /* Remove tiros que saíram da tela */
      if (tiros[i].pos_y <= 0 || tiros[i].pos_y >= VGA_HEIGHT) {
        desenhar_sprite(tiros[i].pos_x_anterior, tiros[i].pos_y_anterior, 
                       projetilJogador[0], 3, 4, COR_FUNDO);
		tiros[i].ativo = 0;
        desenhar_objeto(&tiros[i], 0, COR_FUNDO);  // TESTAR REMOVER ESSA LINHA POR CAUSA DO DESENHAR SPRITE ACIMA
      } else {
        /* Mantém tiro ativo reorganizando o array */
        if (i != j) {
          tiros[j] = tiros[i];
        }
        j++;
      }
    }
  }
  num_tiros = j;
}

/* Criação de tiros com limite */
void criar_tiro_jogador() {
  if (num_tiros < MAX_TIROS) {
    inicializar_objeto(&tiros[num_tiros], projetilJogador[0], 0, 3, 4,
                       jogador.pos_x + (jogador.largura - 3) / 2,
                       jogador.pos_y - 4, 0, -1, 2, 1);
    num_tiros++;
	estado_jogo.contador_tiro = 0;
  }
}

/* Sistema de pontuação */
void adicionar_pontuacao(unsigned char tipo_inimigo) {
  switch (tipo_inimigo) {
  case 1:
    estado_jogo.pontuacao += 100;
    break;
  case 2:
    estado_jogo.pontuacao += 200;
    break;
  case 3:
    estado_jogo.pontuacao += 300;
    break;
  }

  estado_jogo.inimigos_restantes--;

  /* Atualiza display da pontuação */
  char texto_pontuacao[20];
  sprintf(texto_pontuacao, "SCORE:%05d", estado_jogo.pontuacao);
  display_print("          ", 10, 10, 1, COR_FUNDO);
  display_print(texto_pontuacao, 10, 10, 1, COR_TEXTO);
}

/* Apaga objeto na posição anterior */
void apagar_objeto_anterior(struct Objeto *obj) {
  desenhar_sprite(obj->pos_x_anterior, obj->pos_y_anterior, 
                 obj->quadro_sprite[obj->quadro_atual],
                 obj->largura, obj->altura, COR_FUNDO);
}

/* Atualiza posição anterior */
void atualizar_posicao_anterior(struct Objeto *obj) {
  obj->pos_x_anterior = obj->pos_x;
  obj->pos_y_anterior = obj->pos_y;
}

/* Inicialização do jogo */
void inicializar_jogo() {
  unsigned char i;

  /* Estado do jogo */
  estado_jogo.pontuacao = 0;
  estado_jogo.vidas = 3;
  estado_jogo.nivel = 1;
  estado_jogo.direcao_inimigos = 1;
  estado_jogo.velocidade_inimigos = 20;
  estado_jogo.inimigos_restantes = NUM_INIMIGOS;
  estado_jogo.contador_tiro = 0;
	
  /* Jogador */
  inicializar_objeto(&jogador, naveJogador[0], naveJogador[0], 13, 8,
                     (VGA_WIDTH - 13) / 2, VGA_HEIGHT - 20, 0, 0, 3, 0);

  /* Inimigos */
  unsigned char q = NUM_INIMIGOS / 3;
  for (i = 0; i < q; i++) {
    inicializar_objeto(&inimigos[i], inimigoTipo1A[0], inimigoTipo1B[0], 10, 8,
                       30 + i * 25, 50, 0, 0, 30, 1);
    inicializar_objeto(&inimigos[q + i], inimigoTipo2A[0], inimigoTipo2B[0], 9,
                       8, 30 + i * 25, 80, 0, 0, 25, 2);
    inicializar_objeto(&inimigos[2 * q + i], inimigoTipo3A[0], inimigoTipo3B[0],
                       11, 8, 30 + i * 25, 110, 0, 0, 20, 3);
  }

  /* Barreiras */
  for (i = 0; i < NUM_BARREIRAS; i++) {
    inicializar_objeto(&barreiras[i], barreira[0], barreira[0], 16, 10,
                       50 + i * 70, VGA_HEIGHT - 60, 0, 0, 0, 4);
  }

  /* Limpa tiros */
  num_tiros = 0;
  for (i = 0; i < MAX_TIROS; i++) {
    tiros[i].ativo = 0;
  }

  /* Display inicial */
  display_background(COR_FUNDO);
  display_print("SPACE INVADERS", 80, 5, 1, COR_TEXTO);

  char texto_vidas[15];
  sprintf(texto_vidas, "VIDAS:%d", estado_jogo.vidas);
  display_print(texto_vidas, 250, 10, 1, COR_TEXTO);
}

/* Atualização principal do jogo */
void atualizar_jogo() {
  unsigned char i, j;

  /* Atualiza contador de tiro */
  estado_jogo.contador_tiro++;

  /* Movimento do jogador */
  if (--jogador.contador_velocidade == 0) {
    jogador.contador_velocidade = jogador.velocidade;

    /* Armazena posição anterior */
    atualizar_posicao_anterior(&jogador);

    /* Limites da tela */
    int nova_pos_x = jogador.pos_x + jogador.dir_x;
    if (nova_pos_x >= 0 && nova_pos_x + jogador.largura <= VGA_WIDTH) {
      jogador.pos_x = nova_pos_x;
    }

    /* Apaga na posição anterior e desenha na nova */
    apagar_objeto_anterior(&jogador);
    desenhar_objeto(&jogador, 0, COR_NAVE);
  }

  /* Movimento dos inimigos em formação - com eliminação de efeito fantasma */
  contador_animacao++;
  if (contador_animacao >= estado_jogo.velocidade_inimigos) {
    contador_animacao = 0;
    
    /* Primeiro apaga todos os inimigos nas posições anteriores */
    for (i = 0; i < NUM_INIMIGOS; i++) {
      if (inimigos[i].ativo) {
        apagar_objeto_anterior(&inimigos[i]);
        atualizar_posicao_anterior(&inimigos[i]);
      }
    }
    
    /* Move a formação */
    mover_inimigos_em_formacao();
    
    /* Desenha todos os inimigos nas novas posições */
    for (i = 0; i < NUM_INIMIGOS; i++) {
      if (inimigos[i].ativo) {
        desenhar_objeto(&inimigos[i], 1, -1); // Com animação
      }
    }
  }

  /* Atualiza e move tiros - com eliminação de efeito fantasma */
  for (i = 0; i < num_tiros; i++) {
    if (tiros[i].ativo) {
      /* Armazena posição anterior */
      atualizar_posicao_anterior(&tiros[i]);

      /* Move o tiro */
      tiros[i].pos_y += tiros[i].dir_y;

      /* Apaga na posição anterior */
      apagar_objeto_anterior(&tiros[i]);

      /* Verifica colisões com inimigos */
      for (j = 0; j < NUM_INIMIGOS; j++) {
        if (inimigos[j].ativo && verificar_colisao(&tiros[i], &inimigos[j])) {
          /* Colisão com inimigo */
          tiros[i].ativo = 0;
          inimigos[j].ativo = 0;
          adicionar_pontuacao(inimigos[j].tipo);
          // Apaga o inimigo da tela
          apagar_objeto_anterior(&inimigos[j]);
          break;
        }
      }

      /* Verifica colisão com barreiras */
      for (j = 0; j < NUM_BARREIRAS; j++) {
        if (barreiras[j].ativo && verificar_colisao(&tiros[i], &barreiras[j])) {
          tiros[i].ativo = 0;
          /* Danifica barreira - simplificado */
          desenhar_sprite(barreiras[j].pos_x, barreiras[j].pos_y, barreira[0],
                          16, 10, COR_FUNDO);
          break;
        }
      }

      /* Redesenha tiro na nova posição se ainda ativo */
      if (tiros[i].ativo) {
        desenhar_objeto(&tiros[i], 0, COR_TIRO);
      }
    }
  }

  /* Verifica colisão de inimigos com jogador */
  for (i = 0; i < NUM_INIMIGOS; i++) {
    if (inimigos[i].ativo && verificar_colisao(&inimigos[i], &jogador)) {
      estado_jogo.vidas--;
      // Atualiza display de vidas
      char texto_vidas[15];
      sprintf(texto_vidas, "VIDAS:%d", estado_jogo.vidas);
      display_print("          ", 250, 10, 1, COR_FUNDO);
      display_print(texto_vidas, 250, 10, 1, COR_TEXTO);
      
      // Reseta posição do jogador se ainda tem vidas
      if (estado_jogo.vidas > 0) {
        apagar_objeto_anterior(&jogador);
        jogador.pos_x = (VGA_WIDTH - 13) / 2;
        jogador.pos_y = VGA_HEIGHT - 20;
        desenhar_objeto(&jogador, 0, COR_NAVE);
      }
      break;
    }
  }

  /* Limpeza de tiros inativos */
  limpar_tiros_inativos();
}

/* Controles com debounce */
int ler_controles() {
  int teclas = 0;
  jogador.dir_x = 0;

  if (GPIOB->IN & MASK_P10) {
    teclas |= 0x04;
    jogador.dir_x = -1;
  }
  if (GPIOB->IN & MASK_P11) {
    teclas |= 0x08;
    jogador.dir_x = 1;
  }
  if (GPIOB->IN & MASK_P8) {
    teclas |= 0x01;
    criar_tiro_jogador();
  }

  return teclas;
}

/* display and input */
void init_display()
{
	display_background(BLACK);
}

enum {
	KEY_CENTER	= 0x01,
	KEY_UP		= 0x02,
	KEY_LEFT	= 0x04,
	KEY_RIGHT	= 0x08,
	KEY_DOWN	= 0x10
};

void init_input()
{
	/* configure GPIOB pins 8 .. 12 as inputs */
	GPIOB->DDR &= ~(MASK_P8 | MASK_P9 | MASK_P10 | MASK_P11 | MASK_P12);
}

int get_input()
{
	int keys = 0;
	
	if (GPIOB->IN & MASK_P8)	keys |= KEY_CENTER;
	if (GPIOB->IN & MASK_P9)	keys |= KEY_UP;
	if (GPIOB->IN & MASK_P10)	keys |= KEY_LEFT;
	if (GPIOB->IN & MASK_P11)	keys |= KEY_RIGHT;
	if (GPIOB->IN & MASK_P12)	keys |= KEY_DOWN;
		
	return keys;
}

/* Loop principal */
int main(void) {
  init_display();
  init_input();

  inicializar_jogo();

  while (1) {
    if (estado_jogo.vidas > 0 && estado_jogo.inimigos_restantes > 0) {
      ler_controles();
      atualizar_jogo();
      delay_ms(30);
    } else {
      /* Tela de game over */
      if (estado_jogo.inimigos_restantes == 0) {
        display_print("VITORIA!", 120, 100, 2, COR_TEXTO);
      } else {
        display_print("GAME OVER", 110, 100, 2, COR_TEXTO);
      }
      char texto_final[30];
      sprintf(texto_final, "PONTUACAO FINAL: %d", estado_jogo.pontuacao);
      display_print(texto_final, 80, 130, 1, COR_TEXTO);
      display_print("PRESSIONE CENTRO", 85, 160, 1, COR_TEXTO);

      /* Reinicia o jogo */
      if (GPIOB->IN & MASK_P8) {
        delay_ms(500);
        inicializar_jogo();
      }
    }
  }

  return 0;
}
