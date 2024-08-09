#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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



int main() {
	
	// recipes list
	RECIPE* rec_head = NULL;
	RECIPE* new_recipe = NULL;


	//def of the delivery truck

	int delivery_clock = 0;
	int delivery_dim = 0;

	scanf(" %d %d ", &delivery_clock, &delivery_dim);
	printf(" %d %d \n", delivery_clock, delivery_dim);

	int cclock = 0; // is the real clock

	char buffer[BLEN];
	

	char key_menu[LEN];


	while ( gets(buffer) != EOF)
	{
		
		//chosing the option and considering the delivery
		if (cclock == delivery_clock) {
			//delivery
			cclock = 0;
		}


		sscanf(buffer, "%s", key_menu);

		char* cut_menu;
		//cutting the command
		cut_menu = strstr(buffer , key_menu);

		if (cut_menu != NULL) {
			cut_menu += strlen(key_menu) + 1;
		}
		printf("%s\n", key_menu);



		if (strcmp(key_menu, "aggiungi_ricetta") == 0)      // aggiungi ricetta
		{
			
			char name[LEN];
			char* cut_ing;
			sscanf(cut_menu, "%s", name);
			printf("%s\n", name);
			cut_ing = strstr(buffer, name);
			if (cut_ing != NULL) {
				cut_ing += strlen(name)+1;
				
			}
		
			if (Find(name, rec_head) == true)
				printf("ignorato\n");
			else {
				INGREDIENT* ing_head = NULL;
				INGREDIENT* new_ing = NULL;
				;
				char ing[LEN];
				int qty = 0;

				new_recipe = malloc(sizeof(RECIPE));
				strcpy(new_recipe->name, name);
				
				while (buffer != NULL)
				{
					sscanf(buffer, "%s %d %s", ing, qty, buffer);
					//printf("%s\n", buffer);
					new_ing = malloc(sizeof(INGREDIENT));
					strcpy(new_ing->name, ing);
					new_ing->quantity = qty;
					new_ing->next = ing_head;
					ing_head = new_ing;
				}
				new_recipe->ingredient = ing_head;
				new_recipe->next = rec_head;
				rec_head = new_recipe;

				printf("accettato\n");
			}
		}
		else if (strcmp(key_menu, "rimuovi_ricetta") == 0)	// rimuovi ricetta
		{
		}
		else if (strcmp(key_menu, "rifornimento") == 0)		// rifornimento
		{
		}
		else if (strcmp(key_menu, "ordine") == 0)			// ordine
		{
		}

		cclock++;
	}


	return 0;
}
