// Módulo 2 - Laboratório: 4
// Aluna: Carla Moreno Barbosa
// DRE: 117250502
// Objetivo: Projetar e implementar um programa concorrente onde a
//           ordem de execução das threads é controlada no programa.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variáveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *A (void *t) {
  printf("tudo bem?\n");

  pthread_mutex_lock(&x_mutex); // Entrando na seção crítica
  x++;
  if (x==2) {
      pthread_cond_broadcast(&x_cond); // Desbloqueia as threads na fila caso thread A e thread B tenham rodado
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
  printf("bom dia\n");

  pthread_mutex_lock(&x_mutex); // Entrando na seção crítica
  x++;
  if (x==2) {
      pthread_cond_broadcast(&x_cond); // Desbloqueia as threads na fila caso thread A e thread B tenham rodado
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {
  pthread_mutex_lock(&x_mutex); // Entrando na seção crítica
  if (x != 2) { // Caso x seja diferente de 2, uma das threads iniciais (A ou B) ainda não rodou, então thread C é bloqueada
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex);
  printf("até mais!\n");
  pthread_exit(NULL);
}

/* Thread D */
void *D (void *t) {
  pthread_mutex_lock(&x_mutex); // Entrando na seção crítica
  if (x != 2) { // Caso x seja diferente de 2, uma das threads iniciais (A ou B) ainda não rodou, então thread D é bloqueada
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex);
  printf("boa tarde\n");
  pthread_exit(NULL);
}

/* Função principal */
int main(int argc, char *argv[]) {
  int i;
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusão mútua) e a variável de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[1], NULL, B, NULL);
  pthread_create(&threads[2], NULL, C, NULL);
  pthread_create(&threads[3], NULL, D, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variáveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
