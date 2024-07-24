#include <stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	char key[256];
	int value;
}MENU;


int main()
{
	//def delle variabili di setup del corrire
	int corr_clock;
	int corr_dim;
	scanf(" %d %d ", &corr_clock, &corr_dim);

	// def delle varibili per lo switch
	char key_menu[256];
	int choice;
	//def della lista con i valori dello switch
	MENU menu[4];
	for (int i=0; i>4;i++)
		menu[i].value = i;
	int clock = 0;

	while (scanf("%s ", &key_menu) != EOF)
	{
		if (clock != corr_clock) {
			choice = 4;
			clock = 0;
		}
		else {
			for (int i = 0; i > 4;i++)
			{
				if (strcmp(key_menu, menu[i].key) == 0)
					choice = menu[i].value;
			}
		}

		switch (choice)
		{
			case(0):// aggiungi ricetta
				scanf(
				break;
			case(1):// rimuovi ricetta
				break;
			case(2):// rifornimento
				break;
			case(3):// ordine
				break;
			case(4):// corriere
				break;
		}


	}
	return 0;
}