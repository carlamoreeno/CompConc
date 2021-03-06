// Módulo 2 - Laboratório: 5
// Aluna: Carla Moreno Barbosa
// DRE: 117250502
// Objetivo: Projetar e implementar um programa concorrente onde as
//           threads acessem um vetor de valores inteiros de forma
//           concorrente. Nesse arquivo, trataremos o problema de
//           forma que a prioridade esteja com os escritores.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TAMANHO_VETOR 5 // Tamanho do vetor de inteiro que usaremos
#define NTHREADS_LEITORA 2 // Threads leitoras
#define NTHREADS_ESCRITORA 2 // Threads escritoras

int *vetor;
int leitores = 0; // Contador das threads lendo
int escritores = 0; // Contador das threads escrevendo
int escritores_fila = 0; // Contador de escritores na fila de prioridade

pthread_mutex_t mutex;
pthread_cond_t cond_leitura, cond_escrita;

void inicio_leitura(int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);

    while(escritores > 0 || escritores_fila > 0) {
        printf("L[%d] bloqueou porque temos escritor esperando para escrever ou escrevendo\n", id);
        pthread_cond_wait(&cond_leitura, &mutex);
        printf("L[%d] desbloqueou\n", id);
    }

    leitores++;
    pthread_mutex_unlock(&mutex);
}

void fim_leitura(int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] terminou de ler\n", id);
    leitores--;
    if(leitores==0) pthread_cond_signal(&cond_escrita);
    pthread_mutex_unlock(&mutex);
}

void inicio_escrita(int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    escritores_fila++;

    while((leitores>0) || (escritores>0)) {
        printf("E[%d] bloqueou porque temos leitor lendo ou escritor escrevendo\n", id);
        pthread_cond_wait(&cond_escrita, &mutex);
        printf("E[%d] desbloqueou\n", id);
    }

    escritores_fila--;
    escritores++;
    pthread_mutex_unlock(&mutex);
}

void fim_escrita(int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] terminou de escrever\n", id);
    escritores--;
    pthread_cond_signal(&cond_escrita);
    if (escritores_fila == 0) pthread_cond_broadcast(&cond_leitura);
    pthread_mutex_unlock(&mutex);
}

void *leitor(void * arg) {
    int *id = (int *) arg;

    while(1) {
        inicio_leitura(*id);
        printf("Leitora %d está lendo\n", *id);
        double total = 0;
        char buffer[20 * (TAMANHO_VETOR + 2)] = "";
        char *ponteiro_buffer = buffer;
        sprintf(ponteiro_buffer, "L[%d] -> ", *id);
        for (int i=0; i < TAMANHO_VETOR; i++){
            sprintf(ponteiro_buffer, "%s%d ", buffer,vetor[i]);
            total += vetor[i];
        }
        printf("%s-> média dos valores: %.1f\n", buffer, total/TAMANHO_VETOR);

        fim_leitura(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

void *escritor(void *arg) {
    int *id = (int *) arg;

    while(1) {
        inicio_escrita(*id);
        printf("Escritora %d esta escrevendo\n", *id);

        vetor[0] = *id;
        vetor[TAMANHO_VETOR-1] = *id;
        for (int i=1; i < TAMANHO_VETOR-1; i++){
            vetor[i] = 2*(*id);
        }

        fim_escrita(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

int main(void) {
    // Identificadores das threads
    pthread_t tid[NTHREADS_LEITORA+NTHREADS_ESCRITORA];
    int id[NTHREADS_LEITORA+NTHREADS_ESCRITORA];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitura, NULL);
    pthread_cond_init(&cond_escrita, NULL);

    // Inicialização do vetor
    time_t t;
    srand((unsigned) time(&t));
    vetor = malloc(TAMANHO_VETOR * sizeof(int));
    if(!vetor){
        fprintf(stderr, "Não foi possível alocar memória para o vetor\n");
        exit(1);
    }
    for (int i = 0; i < TAMANHO_VETOR; i++)
    vetor[i] = rand() % 100;

    // Criando as threads leitoras
    for(int i=0; i<NTHREADS_LEITORA; i++) {
        id[i] = i + 1;
        if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
    }

    // Criando as threads escritoras
    for(int i=0; i<NTHREADS_ESCRITORA; i++) {
        id[i+NTHREADS_LEITORA] = i + 1;
        if(pthread_create(&tid[i+NTHREADS_LEITORA], NULL, escritor, (void *) &id[i+NTHREADS_LEITORA])) exit(-1);
    }

    pthread_exit(NULL);
    return 0;
}
