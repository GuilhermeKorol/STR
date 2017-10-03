#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string.h>

#define MAX_TAREFAS    26
#define MAX_GRADE      8192
#define MIN_PRIORIDADE 32
// #define DEBUG

// SCHED_FIFO = 1
// SCHED_RR   = 2

typedef struct {
	char	 id;
	unsigned c;
	unsigned s;
	unsigned p;
	unsigned sched;
} TAREFA;

// Um vetor de TAREFAs para cada prioridade
typedef struct {
	std::vector<TAREFA> tarefas[MIN_PRIORIDADE];
} P_TAREFAS;

int main() {
	P_TAREFAS t_set;
	std::vector<TAREFA> espera;
	unsigned num_tarefas;
	unsigned tarefas_executando;
	unsigned i,j;
	unsigned sp_temp;
	char     grade[MAX_GRADE+1] = {0};
  unsigned tempo = 0;

	/* Inicia leitura... */
	scanf("%u",&num_tarefas);
	while (num_tarefas > 0) {

    #ifdef DEBUG
		printf("\nNum_leituras %d\n", num_tarefas);
    #endif

		/* LEITURA */
		for (i=0;i<num_tarefas;++i) {
			TAREFA temp_t;
			temp_t.id = 'A'+i;
			scanf("%u %u %u %u",&(temp_t.c), &(temp_t.s),
						&(temp_t.p), &(temp_t.sched));
            //#ifdef DEBUG
            //  printf("t_set.tarefas[%d][%d].id = %c\n", i, j, temp_t.id);
						//  printf("t_set.tarefas[%d][%d].c  = %d\n", i, j, temp_t.c);
    				//	printf("t_set.tarefas[%d][%d].s  = %d\n", i, j, temp_t.s);
    				//	printf("t_set.tarefas[%d][%d].p  = %d\n", i, j, temp_t.p);
    				//	printf("t_set.tarefas[%d][%d].sc = %d\n", i, j, temp_t.sched);
    				//	printf("tempo = %d\n", tempo);
            //#endif

			if (temp_t.s == 0){
				t_set.tarefas[temp_t.p-1].push_back(temp_t);
			} else {
				espera.push_back(temp_t);
			}
		}

		#ifdef DEBUG
        printf("espera.size() = %ld\n", espera.size());
		for (i=0;i<MIN_PRIORIDADE-1;++i) {
            printf("t_set.tarefas[%d].size() = %ld\n", i, t_set.tarefas[i].size());
			if (t_set.tarefas[i].size() > 0) {
		 		for (j=0;j<t_set.tarefas[i].size();++j) {
		 				printf("\nt_set.tarefas[%d][%d].id = %c\n", i, j, t_set.tarefas[i][j].id);
		 				printf("t_set.tarefas[%d][%d].c  = %d\n", i, j, t_set.tarefas[i][j].c);
		 				printf("t_set.tarefas[%d][%d].s  = %d\n", i, j, t_set.tarefas[i][j].s);
		 				printf("t_set.tarefas[%d][%d].p  = %d\n", i, j, t_set.tarefas[i][j].p);
		 				printf("t_set.tarefas[%d][%d].sc = %d\n", i, j, t_set.tarefas[i][j].sched);
		 				printf("tempo = %d\n", tempo);
		 		}
			}
		}
		for (i = 0; i<espera.size(); i++) {
			printf("\nespera[%d].id = %c\n", i, espera[i].id);
			printf("espera[%d].c  = %d\n", i, espera[i].c);
			printf("espera[%d].s  = %d\n", i, espera[i].s);
			printf("espera[%d].p  = %d\n", i, espera[i].p);
			printf("espera[%d].sc = %d\n", i, espera[i].sched);
			printf("tempo = %d\n", tempo);
		}
		printf("\nSTART\n");
		#endif


		tempo = 0;
		tarefas_executando = num_tarefas;
		while (tarefas_executando > 0) {
			// Percorre prioridades da mais alta a mais baixa
			i = 0;
			while (i<MIN_PRIORIDADE-1) {
				#ifdef DEBUG
						printf("Grade [%d]: ",tempo);
						printf("%s\n",grade);
				#endif
        if (i == MIN_PRIORIDADE-2 && espera.size() > 0) {
				// Testa se chegamos ao final das prioridades com tarefas na espera
					#ifdef DEBUG
						printf("Incrementando tempo por falta de tarefas prontas\n");
						printf("i = %d | tempo = %d\n", i, tempo);
					#endif
          grade[tempo++] = '.';
        } else {
					#ifdef DEBUG
						printf("Nao incrementando tempo por falta de tarefas prontas\n");
						printf("i = %d | tempo = %d\n", i, tempo);
					#endif
				}
				for(j=0; j < espera.size(); ++j) {
				// Verifica se tem tarefas entrando
					if (tempo >= espera[j].s) {
						sp_temp = espera[j].p-1;
						// t_set.tarefas[sp_temp].push_back(espera[j]);
						if (espera[j].sched == 2 && t_set.tarefas[sp_temp].size() < 2) {
							// std::rotate(t_set.tarefas[sp_temp].begin(), t_set.tarefas[sp_temp].begin()+1, t_set.tarefas[sp_temp].end());
							t_set.tarefas[sp_temp].insert(t_set.tarefas[sp_temp].begin(), espera[j]);
						}
						// else if (espera[j].sched == 2 && t_set.tarefas[sp_temp].size() < 3) {
						// 	t_set.tarefas[sp_temp].insert(t_set.tarefas[sp_temp].begin()+1,espera[j]);
						// }
						else {
							t_set.tarefas[sp_temp].push_back(espera[j]);
						}
						#ifdef DEBUG
							printf("tarefa %c entrou em %d\n", t_set.tarefas[sp_temp].end()->id, tempo);
							printf("espara.size() = %ld\n", espera.size());
						#endif
            espera.erase(espera.begin()+j);
						if (sp_temp < i) {
							// se a tarefa que chegou tem prioridade maior que a atual
							i = sp_temp;
						}
					}
				}

				if (t_set.tarefas[i].size() > 0) {
					// Se existe tarefa de prioridade i
						if (t_set.tarefas[i][0].sched == 1) {
							// Se for sched_fifo
							if (t_set.tarefas[i][0].s <= tempo) {
								// Tarefa j ganha cpu
								grade[tempo++] = t_set.tarefas[i][0].id;
								// Computacao da tarefa j decrementa
								t_set.tarefas[i][0].c--;
								// Se acabou, deletar tarefa j
								if (t_set.tarefas[i][0].c == 0) {
									t_set.tarefas[i].erase(t_set.tarefas[i].begin());
									tarefas_executando--;
								}
							}
						}
						else {
							// Se for sched_rr
							if (t_set.tarefas[i][0].s <= tempo) {
								// Tarefa j ganha cpu
								grade[tempo++] = t_set.tarefas[i][0].id;
								// Computacao da tarefa j decrementa
								t_set.tarefas[i][0].c--;
								// Se acabou, deletar tarefa j
								if (t_set.tarefas[i][0].c == 0) {
									t_set.tarefas[i].erase(t_set.tarefas[i].begin());
									tarefas_executando--;
								} else { // ASSUMINDO TIME SLICE = 1 UT
									// Tarefa j para o final do vetor
									std::rotate(t_set.tarefas[i].begin(), t_set.tarefas[i].begin()+1, t_set.tarefas[i].end());
								}
							}
						}
						#ifdef DEBUG
							printf("t_set.tarefas[%d][%d].id = %c\n", i, 0, t_set.tarefas[i][0].id);
							printf("t_set.tarefas[%d][%d].c  = %d\n", i, 0, t_set.tarefas[i][0].c);
							printf("t_set.tarefas[%d][%d].s  = %d\n", i, 0, t_set.tarefas[i][0].s);
							printf("t_set.tarefas[%d][%d].p  = %d\n", i, 0, t_set.tarefas[i][0].p);
							printf("t_set.tarefas[%d][%d].sc = %d\n", i, 0, t_set.tarefas[i][0].sched);
							printf("Grade [%u]: ",tempo);
							printf("%s\n\n",grade);
						#endif
				}
				if (t_set.tarefas[i].size() <= 0) {
					// Se nao existem mais tarefas nessa prioridade, pular para proxima
					i++;
				}
			}
		}
		grade[tempo] = '\0';

		/* SAIDA */
#ifdef DEBUG
		printf("Grade [%u]: ",tempo);
#endif
		printf("%s\n\n",grade);

		/* Inicia nova leitura... */
		scanf("%u",&num_tarefas);

        // Limpa dados
        espera.clear();
        for (i=0;i<MIN_PRIORIDADE-1;++i) {
            t_set.tarefas[i].clear();
        }
        sp_temp = 0;
        memset(grade, 0, strlen(grade));
	}
	return 0;
}
