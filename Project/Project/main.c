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
	struct ing_node* next;
	struct ing_node* prec;
}INGREDIENT;

typedef struct rec_node {
	char name[LEN];
	INGREDIENT* ingredient;
	struct rec_node* next;
	struct rec_node* prec;
}RECIPE;

typedef struct store_node {
	char name[LEN];
	int quantity;
	int decay;
	struct store_node* next;
	struct store_node* prec;
}STORAGE;


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
	int rec_counter = 0;

	// storage list
	STORAGE* storage = NULL;
	STORAGE* new_prod = NULL;

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
		/*
		STORAGE* st_tmp = storage;
		
		if (st_tmp != NULL) {
			if (st_tmp->decay == cclock)
				st_tmp = st_tmp->next;
			while (st_tmp->next == NULL)
			{
				if (st_tmp->next->decay == cclock)
					st_tmp->next = st_tmp->next->next;
			}
		}
		
		
		st_tmp = storage;
		printf("\n");
		
		while (st_tmp != NULL) {

			printf("%d => %s %d %d\n",cclock, st_tmp->name, st_tmp->quantity, st_tmp->decay);
			st_tmp = st_tmp->next;
		}
		
		printf("\n");
		
		*/
		//chosing the option and considering the delivery
		if (cclock == delivery_clock) {
			//delivery
			cclock = 0;
		}
		
		
		
		// remove the command from the buffer
		sscanf(buffer, "%s", key_menu);

		if (buffer != NULL) {			
			char* new_buffer = buffer + strlen(key_menu) +1;			
			memmove(buffer, new_buffer, strlen(new_buffer) + 1);
		}
		//----

		if (strcmp(key_menu, "aggiungi_ricetta") == 0)      // aggiungi ricetta
		{
			
			sscanf(buffer, "%s", &name);
			

			*buffer = strstr(buffer, name);
			if (buffer != NULL) {
				char* new_buffer = buffer + strlen(name) + 1 ;
				memmove(buffer, new_buffer, strlen(new_buffer) + 1);
			}
			
			
			if (Find(name, recipes) == true)
				printf("ignorato\n");
			else {
				INGREDIENT* ing_head = NULL;
				INGREDIENT* new_ing = NULL;
				new_ing = NULL;


				new_recipe = (RECIPE*) malloc(sizeof(RECIPE));
				
				strcpy(new_recipe->name, name);
				
				while (buffer[0] != '\0')
				{
					
					new_ing = (INGREDIENT*)malloc(sizeof(INGREDIENT));
					sscanf(buffer, "%s %d", &new_ing->name, &new_ing->quantity);
					
					new_ing->next = ing_head;
					ing_head = new_ing;
					
					
					if (buffer != NULL) {
						int len = 0;
						int tmp = new_ing->quantity;
						do{
							len++;
							tmp= tmp / 10;
						} while (tmp > 0);

						int bebbo = strlen(new_ing->name) + 1 + len ;
						char* new_buffer = buffer + bebbo;
						if (buffer[0] == ' ')
							new_buffer =new_buffer + 1;
						memmove(buffer, new_buffer, strlen(new_buffer) + 1);
					}

				}
				new_recipe->ingredient = ing_head;
				new_recipe->next = recipes;
				recipes = new_recipe;

				
				
				printf("accettato\n");
				rec_counter++;
				
			}
		}
		else if (strcmp(key_menu, "rimuovi_ricetta") == 0)	// rimuovi ricetta
		{
			bool find = false;
			strcpy(name ,"");
			sscanf(buffer, "%s", name);
			int scroller = 1;

			if (recipes->next != NULL) {

				if (strcmp(recipes->name, name) == 0) {
					recipes->next->prec = recipes;
					recipes = recipes->next;
					find = true;
				}
				else {
					do {
						scroller++;
						if (strcmp(recipes->next->name, name) == 0) {
							recipes->next = recipes->next->next;
							find = true;
						}
						else {
							recipes->next->prec = recipes;
							recipes = recipes->next;
						}
					} while (recipes->next != NULL && find != true);
				}
			}
			else {
				if (strcmp(recipes->name, name) == 0) {
					recipes->prec->next = recipes;
					recipes = recipes->next;
					find = true;
				}
				else {
					do {
						scroller++;
						if (strcmp(recipes->prec->name, name) == 0) {
							recipes->prec = recipes->prec->prec;
							find = true;
						}
						else {
							recipes->prec->next = recipes;
							recipes = recipes->prec;
						}
					} while (recipes->prec != NULL && find != true);

				}
			}

			if (scroller < (rec_counter / 2)){
				while (recipes->prec != NULL)
					recipes = recipes->prec;
			}
			else {
				while (recipes->next != NULL)
					recipes = recipes->next;
			}


			if(find == true)
				printf("eliminato\n");
			else
				printf("non trovata\n");
		}
		else if (strcmp(key_menu, "rifornimento") == 0)		// rifornimento
		{

			while (buffer[0] != '\0')
			{
				new_prod = (STORAGE*)malloc(sizeof(STORAGE));

				sscanf(buffer,"%s %d %d", new_prod->name, &new_prod->quantity, &new_prod->decay);

				new_prod->next = storage;
				storage = new_prod;

				if (buffer != NULL) {
					int len = 0;
					int tmp = new_prod->quantity;
					do {
						len++;
						tmp = tmp / 10;
					} while (tmp > 0);

					tmp = new_prod->decay;

					do {
						len++;
						tmp = tmp / 10;
					} while (tmp > 0);


					int line_len = strlen(new_prod->name) + 1 + len + 1;
					char* new_buffer = buffer + line_len;
					if (buffer[0] == ' ')
						new_buffer = new_buffer + 1;
					memmove(buffer, new_buffer, strlen(new_buffer) + 1);

				}

			}
			printf("rifornito\n");
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
