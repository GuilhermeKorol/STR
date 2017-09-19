#include <stdio.h>
#include <vector>
#include <algorithm>

#define MAX_TAREFAS    26
#define MAX_GRADE      8192
#define MIN_PRIORIDADE 32
// #define DEBUG

typedef struct {
	char	   id;
	unsigned c;
	unsigned s;
	unsigned p;
	unsigned sched;
} TAREFA;

// Um vetor de TAREFAs para cada prioridade
typedef struct {
	// std::vector<std::vector<TAREFA> > tarefas(MIN_PRIORIDADE, vector<TAREFA>(MIN_PRIORIDADE));
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
	char		 pronto;

	/* Inicia leitura... */
	scanf("%u",&num_tarefas);
	while (num_tarefas > 0) {

		/* LEITURA */
		for (i=0;i<num_tarefas;++i) {
			TAREFA temp_t;
			temp_t.id = 'A'+i;
			scanf("%u %u %u %u",&(temp_t.c), &(temp_t.s),
						&(temp_t.p), &(temp_t.sched));

			if (temp_t.s == 0){
				t_set.tarefas[temp_t.p-1].push_back(temp_t);
			} else {
				espera.push_back(temp_t);
			}
		}

		// #ifdef DEBUG
		// for (i=0;i<MIN_PRIORIDADE-1;++i) {
		// 	if (t_set.tarefas[i].size() > 0) {
		// 		printf("\nt_set.tarefas[%d].size() = %ld\n", i, t_set.tarefas[i].size());
		// 		for (j=0;j<t_set.tarefas[i].size();++j) {
		// 				printf("t_set.tarefas[%d][%d].id = %c\n", i, j, t_set.tarefas[i][j].id);
		// 				printf("t_set.tarefas[%d][%d].c  = %d\n", i, j, t_set.tarefas[i][j].c);
		// 				printf("t_set.tarefas[%d][%d].s  = %d\n", i, j, t_set.tarefas[i][j].s);
		// 				printf("t_set.tarefas[%d][%d].p  = %d\n", i, j, t_set.tarefas[i][j].p);
		// 				printf("t_set.tarefas[%d][%d].sc = %d\n", i, j, t_set.tarefas[i][j].sched);
		// 				printf("tempo = %d\n", tempo);
		// 		}
		// 	}
		// }
		// printf("\nSTART\n");
		// #endif


		tempo = 0;
		tarefas_executando = num_tarefas;
		while (tarefas_executando > 0) {
			// Percorre prioridades da mais alta a mais baixa
			i = 0;
			while (i<MIN_PRIORIDADE-1) {
				for(j=0; j < espera.size(); ++j) {
					// Veriica se existe aluma tarefa para ser incluida na fila de prontos
					if (espera[j].s == tempo) {
						sp_temp = espera[j].p-1;
						t_set.tarefas[sp_temp].push_back(espera[j]);
						#ifdef DEBUG
							printf("tarefa %c entrou em %d\n", t_set.tarefas[sp_temp].end()->id, tempo);
						#endif
						espera[j].s = 8193;
						if (sp_temp < i) {
							// se a tarefa que chegou tem prioridade maior que a atual
							i = sp_temp;
						}
					}
				}

				if (t_set.tarefas[i].size() > 0) {
					// Se existe(m) tarefa(s) de prioridade i
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
								}
								// ASSUMINDO TIME SLICE = 1 UT
								else {
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
	}
	return 0;
}
