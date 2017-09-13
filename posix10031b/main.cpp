#include <stdio.h>
#include <vector>
#include <algorithm>

#define MAX_TAREFAS    26
#define MAX_GRADE      8192
#define MIN_PRIORIDADE 32
#define DEBUG

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
	//TAREFA tarefas[MAX_TAREFAS];
	P_TAREFAS t_set;
	unsigned num_tarefas;
	unsigned tarefas_executando;
	unsigned i,j;
	char     grade[MAX_GRADE+1];
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

			t_set.tarefas[temp_t.p-1].push_back(temp_t);
		}

		// for (i=0;i<MIN_PRIORIDADE-1;++i) {
		// 	printf("t_set.tarefas[%d].size() = %ld\n", i, t_set.tarefas[i].size());
		// }

		tempo = 0;
		tarefas_executando = num_tarefas;
		while (tarefas_executando > 0) {
			// Percorre prioridades da mais alta a mais baixa
			for (i=0;i<MIN_PRIORIDADE-1;++i) {
				// Se existe(m) tarefa(s) de prioridade i
				if (t_set.tarefas[i].size() > 0) {
					// Percorre tarefas
					for (j=0;j<t_set.tarefas[i].size();++j) {
						#ifdef DEBUG
							// printf("t_set.tarefas[%d].size() = %ld\n", i, t_set.tarefas[i].size());
							printf("t_set.tarefas[%d][%d].id = %c\n", i, j, t_set.tarefas[i][j].id);
							printf("t_set.tarefas[%d][%d].c  = %d\n", i, j, t_set.tarefas[i][j].c);
							printf("t_set.tarefas[%d][%d].s  = %d\n\n", i, j, t_set.tarefas[i][j].s);
							printf("tempo = %d\n", tempo);
						#endif
					  // Se for sched_fifo
						if (t_set.tarefas[i][j].sched == 1) {
							if (t_set.tarefas[i][j].s <= tempo) {
								// Tarefa j ganha cpu
								grade[tempo++] = t_set.tarefas[i][j].id;
								// Computacao da tarefa j decrementa
								t_set.tarefas[i][j].c--;
								// Se acabou, deletar tarefa j
								if (t_set.tarefas[i][j].c == 0) {
									t_set.tarefas[i].erase(t_set.tarefas[i].begin()+j);
									tarefas_executando--;
								}
							}
						} // Se for sched_rr
						else {
							if (t_set.tarefas[i][j].s <= tempo) {
								// Tarefa j ganha cpu
								grade[tempo++] = t_set.tarefas[i][j].id;
								// Computacao da tarefa j decrementa
								t_set.tarefas[i][j].c--;
								// Se acabou, deletar tarefa j
								if (t_set.tarefas[i][j].c == 0) {
									t_set.tarefas[i].erase(t_set.tarefas[i].begin()+j);
									tarefas_executando--;
								}
								// ASSUMINDO TIME SLICE = 1 UT
								else {
									// Tarefa j para o final do vetor
									std::rotate(t_set.tarefas[i].begin(), t_set.tarefas[i].begin()+j+1, t_set.tarefas[i].end());
								}
							}
						}
					}
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
