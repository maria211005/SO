#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSOS 100
#define MAX_NOME 10 

typedef struct{
    char nome[MAX_NOME]; 
    int tempo_execucao; 
    int tempo_chegada; 
    int tempo_restante; 
    int tempo_inicio; 
    int tempo_final; 
    int tempo_espera; 
    int tempo_resposta; 
    int iniciado; 
}Processo;

//função para ler o arquivo --> retorna a qtdd 
int lendoArq(const char *nomeArq, Processo processos[], int *quantidade){
    //abre o arquivo no modo read - apenas ler
    FILE *arquivo = fopen(nomeArq, "r");
    
    //verificando se nao deu erro na leitura
    if(arquivo == NULL){
        perror("erro ao abrir o arquivo!\n");
        return 0; 
    }

    char linha[1000]; 
    int i = 0; 

    //lendo a linha com os nomes 
    fgets(linha, sizeof(linha), arquivo); //pegando a linha
    linha[strcspn(linha, "\n")] = 0; //removendo o \n do final da string
    strtok(linha, " "); //função p/ quebrar a string em tokens 
    char *token = strtok(NULL, " "); //pega cada nome
    while(token != NULL){
        
        strcpy(processos[i].nome, token); //copiando o token pro nome do respectivo processo
        i++; 
        token = strtok(NULL, " "); //passando pro prox nome
    }
    //a gente descobre quantos processos tem por meio do contador (i++) e passa pra um ponteiro
    *quantidade = i;
    
    //lendo a linha com os tempos de execução 
    fgets(linha, sizeof(linha), arquivo); //pegando a linha
    strtok(linha, " "); //quebrando em tokens
    for(int j = 0; j < *quantidade; j++){
        token = strtok(NULL, " "); 
        //funcao atoi --> transforma string em int 
        processos[j].tempo_execucao = atoi(token); 
        processos[j].tempo_restante = processos[j].tempo_execucao;
    }

    //lendo a linha com os tempos de chegada 
    fgets(linha, sizeof(linha), arquivo); 
    strtok(linha, " "); //quebrando dnv em tokens
    for(int j = 0; j < *quantidade; j++){
        token = strtok(NULL, " "); 
        processos[j].tempo_chegada = atoi(token); 
        processos[j].tempo_inicio = 0; 
    }

    //inicializando as variaveis 
    for(int j = 0; j < *quantidade; j++){
        processos[j].iniciado = 0; 
        processos[j].tempo_espera = 0; 
        processos[j].tempo_resposta = 0; 
    }

    //como nao tem prioridade no SJF preemptivo, nao botei pra ler essa linha 
    fclose(arquivo); //fecha o arquivo
    return 1; 
}

//faz o sjf 
void SJF_preemptivo(Processo processos[], int quantidade){
    int tempo = 0; //tempo atual 
    int completados = 0; 
    int ultimo = -1; //ultimo processo que foi executado
    int totalEspera = 0; 
    int totalResposta = 0; 

    while(completados < quantidade){
        int menor = -1; 
        int menorTempo = 99999; 

        //escolhendo o processo com o menor tempo restante entre os que chegaram
        for(int i = 0; i < quantidade; i++){
            if(processos[i].tempo_chegada <= tempo && processos[i].tempo_restante > 0 && processos[i].tempo_restante < menorTempo){
                menorTempo = processos[i].tempo_restante; 
                menor = i; 
            }
        }

        //se o processo estiver pronto
        if(menor != -1){
            if(!processos[menor].iniciado){
                //marcando inicio
                processos[menor].tempo_inicio = tempo; 
                processos[menor].iniciado = 1; 
            }

            //executa uma unidade de tempo
            processos[menor].tempo_restante--; 

            //imprime mudança apenas se o processo for novo
            if(ultimo != menor){
                printf("|%d|---------%s---------", tempo, processos[menor].nome); 
                ultimo = menor; 
            }

            //se tiver acabado ele salva as infos
            if(processos[menor].tempo_restante == 0){
                processos[menor].tempo_final = tempo + 1; 
                processos[menor].tempo_resposta = processos[menor].tempo_inicio - processos[menor].tempo_chegada + processos[menor].tempo_execucao;
                processos[menor].tempo_espera = processos[menor].tempo_inicio - processos[menor].tempo_chegada;
                completados++; 
            }
        }else{
            //se nao tiver nenhum processo pronto ele add +1 no tempo
            if(ultimo != -1) 
            ultimo = -1; 
        }

        tempo++;
    }
    printf("|%d|\n", tempo); //imprime o fim do processo

    //impressao dos resultados
    //tempo de espera: 
    printf("\n\n\n\n\nTempo de espera pra cada processo:\n");
    for(int i = 0; i < quantidade; i++){
        printf("%3s ", processos[i].nome); 
    }
    printf("\n"); 
    for(int i = 0; i < quantidade; i++){
        printf("%3d ", processos[i].tempo_espera); 
        totalEspera += processos[i].tempo_espera;
    }
    printf("\n\n"); 

    //tempo de resposta:
    printf("Tempo de resposta de cada processo:\n"); 
    for(int i = 0; i < quantidade; i++){
        printf("%3s ", processos[i].nome); 
    }
    printf("\n"); 
    for(int i = 0; i < quantidade; i++){
        printf("%3d ", processos[i].tempo_resposta); 
        totalResposta += processos[i].tempo_resposta; 
    }

    //exibindo o tempo médio do TME E TMR 
    printf("\n\n\n\n\nTempo médio de espera: %.2f\n", (float)totalEspera / quantidade); 
    printf("Tempo médio de resposta: %.2f\n", (float)totalResposta / quantidade);

}

int main(){
    printf("SIMULADOR DE GERENCIADOR DE PROCESSOS\n");
    printf("Política SJF Preemptivo\n\n\n\n");

    Processo processos[MAX_PROCESSOS]; 
    int quantidade = 0; 

    //msg de erro caso o arquivo não abra 
    if(!lendoArq("entrada.txt", processos, &quantidade)){
        printf("erro ao abrir o arquivo!\n"); 
        return 1; 
    }
    //exibindo os dados lidos
    printf("Processos na fila do SJF:\n");
    for(int i = 0; i < quantidade; i++){
        printf("%s  ", processos[i].nome); 
    }
    printf("\n\nTempo de CPU requerida pelos processos:\n");
    for(int i = 0; i < quantidade; i++){
        printf("%d  ", processos[i].tempo_execucao); 
    }
    printf("\n\nTempo de chegada dos processos:\n");
    for(int i = 0; i < quantidade; i++){
        printf("%d  ", processos[i].tempo_chegada);
    }

    //SJF PREEMPTIVO 
    printf("\n\n\n\n\nLINHA DO TEMPO:\n\n");
    SJF_preemptivo(processos, quantidade); 

    return 0; 
}