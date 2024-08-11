#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>

//Globar variable

#define LEN 256
#define BLEN 100000

//structure declaring
//def of the recipes list

typedef struct ing_node {
	char name[LEN];
	int quantity;
	struct ing_node*next;
}INGREDIENT;

typedef struct rec_node {
	char name[LEN];
	INGREDIENT* ingredient;
	struct rec_node*next;
}RECIPE;



// Function declaring

bool Find(char* name[LEN], RECIPE* pointer) {
	bool find = false;

	while (pointer != NULL) {

		if (strcmp(pointer->name, name) == 0)
		{
			find = true;
			break;
		}
		pointer = pointer->next;
	}

	return find;
}

int Contacifre(int n) {
	int count = 0;
	while (n > 0) {
		n /= 10;
		count++;
	}
	return count;
}


int main() {
	
	// Inizia a misurare il tempo
	clock_t start_time = clock();



	// recipes list
	RECIPE* recipes = NULL;
	RECIPE* new_recipe = NULL;

	// storage list
	INGREDIENT* storage = NULL;
	INGREDIENT* new_ing = NULL;

	//def of the delivery truck

	int delivery_clock = 0;
	int delivery_dim = 0;

	scanf(" %d %d ", &delivery_clock, &delivery_dim);
	

	int cclock = 0; // is the real clock

	char buffer[BLEN];
	char name[LEN];
	char key_menu[LEN];

	//char* buffer;

	while ( gets(buffer) != NULL)
	{
		
		//chosing the option and considering the delivery
		if (cclock == delivery_clock) {
			//delivery
			cclock = 0;
		}


		sscanf(buffer, "%s", key_menu);


		//cutting the command
		//cut_menu = strstr(buffer , key_menu);
		*buffer = strstr(buffer, key_menu);

		if (buffer != NULL) {
			*buffer += strlen(key_menu) + 1;
		}
		

		if (strcmp(key_menu, "aggiungi_ricetta") == 0)      // aggiungi ricetta
		{
			
			
			sscanf(buffer, "%s", name);
			

			*buffer = strstr(buffer, name);
			if (buffer != NULL) {
				*buffer += strlen(name) + 1;
			}
			
			
			if (Find(name, recipes) == true)
				printf("ignorato\n");
			else {
				INGREDIENT* ing_head = NULL;
				new_ing = NULL;


				new_recipe = malloc(sizeof(RECIPE));
				strcpy(new_recipe->name, name);
				
				while (buffer[0] != '\0')
				{
					
					new_ing = malloc(sizeof(INGREDIENT));
					sscanf(buffer, "%s %d", &new_ing->name, &new_ing->quantity);
					
					new_ing->next = ing_head;
					ing_head = new_ing;
					
					if (buffer != NULL) {
						*buffer += strlen(new_ing->name) + 1 + Contacifre(new_ing->quantity);
						if (buffer[0] == ' ')
							*buffer += 1;
					}
				}
				new_recipe->ingredient = ing_head;
				new_recipe->next = recipes;
				recipes = new_recipe;

				
				
				printf("accettato\n");
				//===================
				printf("\n\n\n\n\n\n");

				while (recipes->next != NULL)
				{
					printf(" parola %s\n", recipes->name);
					recipes = recipes->next;
				}

				printf("\n\n\n\n\n\n");
				//=====================
			}
		}
		else if (strcmp(key_menu, "rimuovi_ricetta") == 0)	// rimuovi ricetta
		{
			bool find = false;
			sscanf(buffer, "&s", &name);
			if (strcmp(recipes->name, name) == 0) {
				recipes = recipes->next;
				find = true;
			}
			else {
				do {
					if (strcmp(recipes->next->name, name) == 0) {
						recipes->next = recipes->next->next;
						find = true;
					}

				} while (recipes->next != NULL || find == true);
			}
			if(find == true)
				printf("eliminato\n");
			else
				printf("non trovata\n");
		}
		else if (strcmp(key_menu, "rifornimento") == 0)		// rifornimento
		{





		}
		else if (strcmp(key_menu, "ordine") == 0)			// ordine
		{
		}

		cclock++;
		//free(buffer);
	}

	// Termina la misurazione del tempo
	clock_t end_time = clock();

	// Calcola il tempo trascorso in secondi
	double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("Runtime:: %f s.\n", time_taken);


	

	return 0;
}
