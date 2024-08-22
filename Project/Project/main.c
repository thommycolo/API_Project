#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>

//Globar variable

#define LEN 256
#define BLEN 100000
//
//structure declaring
//def of the recipes list

typedef struct ing_node {
	char name[LEN];
	int quantity;
	struct ing_node* next;
}INGREDIENT;

typedef struct rec_node {
	char name[LEN];
	INGREDIENT* ingredient;
	struct rec_node* next;
}RECIPE;

typedef struct item_node {
	int quantity;
	int decay;
	struct item_node* next;
}ITEM;


typedef struct queue_node {
	char name[LEN];
	ITEM* item;
	struct queue_node* next;
}ITEM_QUEUE;





// Function declaring

bool Find(char* name, RECIPE* pointer) {
	bool find = false;
	if (pointer->next != NULL) {
		while (pointer->next != NULL) {

			if (strcmp(pointer->name, name) == 0)
			{
				find = true;
				break;
			}
			pointer = pointer->next;
		}
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

char* Chop_word(char* phrase, char* word) {

	phrase = strstr(phrase, word);
	if (phrase != NULL) {
		char* new_buffer = phrase + strlen(word) + 1;
		memmove(phrase, new_buffer, strlen(new_buffer) + 1);
	}
	if (strcmp(phrase, "") == 0)
		return '\0';
	else
		return phrase;
}

char* Chop_word_int(char* phrase, char* word, int num) {

	phrase = strstr(phrase, word);

	if (phrase != NULL) {
		int len = 0;
		int tmp = num;
		do {
			len++;
			tmp = tmp / 10;
		} while (tmp > 0);

		char* new_buffer = phrase + strlen(word) + 1 + len;
		if (phrase[0] == ' ')
			new_buffer = new_buffer + 1;
		memmove(phrase, new_buffer, strlen(new_buffer) + 1);
	}
	if (strcmp(phrase, "") == 0)
		return '\0';
	else
		return phrase;
}

char* Chop_two_int(char* phrase, int num1, int num2) {

	if (phrase != NULL) {
		int len = 0;
		int tmp = num1;
		do {
			len++;
			tmp = tmp / 10;
		} while (tmp > 0);
		tmp = num2;
		do {
			len++;
			tmp = tmp / 10;
		} while (tmp > 0);
		char* new_buffer = phrase + 1 + len;
		if (phrase[0] == ' ')
			new_buffer = new_buffer + 1;
		memmove(phrase, new_buffer, strlen(new_buffer) + 1);
	}
	if (strcmp(phrase, "") == 0)
		return '\0';
	else
		return phrase;
}

int Check_ing_for_order(RECIPE* recipe, ITEM_QUEUE* storage, char name[LEN], int num) {
	bool recipe_check = false;
	bool ingredient_check = false;
	while (recipe != NULL || recipe_check != true) {
		if (strcmp(recipe->name, name) == 0) {
			recipe_check = true;
			INGREDIENT* ing = recipe->ingredient;
			
			while (ing != NULL || storage != NULL) { // check if all the ingredient are in enough quantity in storage
				bool ing_check_tmp = false;
				while (storage != NULL || ing_check_tmp != true) { // check if every single ingredient have enough quantity in storage
					if (strcmp(ing, storage->name) == 0) {
						ITEM* item_tmp = storage->item;
						if ((ing->quantity * num) <= storage->item->quantity) {
							ing_check_tmp = true;
							//code here to remove ing from storage when is all in one place
						}
						else if ((ing->quantity * num) > storage->item->quantity && storage->item->next != NULL) {
							ITEM* item = storage->item;
							int tot = ing->quantity * num;
							while (item != NULL || tot != 0) {

								if (tot > item->quantity && item->next != NULL) {
									tot = tot - item->quantity;
									//code here to remove ing from storage when is in more place
								}
								item = item->next;
							}
							if (tot == 0) // if ok, check = true
								ing_check_tmp = true;
						}

					}
					if (ing_check_tmp != true) // if ok, check = true
						storage = storage->next;
					else
						ing = ing->next;
				}
				if (ing == NULL) // if ok, check = true
					ingredient_check = true;

			}
		}
	}

	if (recipe_check == true && ingredient_check == true)
		return 1; // order is ready to be done
	else if (recipe_check == true && ingredient_check == false)
		return 2; // order need to be set in queue 
	else
		return 0; // order cannot be accepted


}



int main() {

	// Inizia a misurare il tempo
	clock_t start_time = clock();



	// recipes list
	RECIPE* recipes = NULL;
	RECIPE* new_recipe = NULL;
	int rec_counter = 0;

	// ITEM_QUEUE list
	ITEM_QUEUE* prod = NULL;
	ITEM_QUEUE* new_prod = NULL;

	//def of the delivery truck

	int delivery_clock = 0;
	int delivery_dim = 0;

	scanf(" %d %d ", &delivery_clock, &delivery_dim);


	int program_clock = 0; // is the real clock

	char buffer[BLEN];
	char name[LEN];
	char key_menu[LEN];

	//char* buffer;

	while (gets(buffer) != NULL)
	{

		//chosing the option and considering the delivery
		if (program_clock % delivery_clock == 0) {
			//delivery
			
		}



		// check if something went rotten
		if (prod != NULL) {
			ITEM_QUEUE* check_storage = prod;
			if (check_storage->next == NULL) {
				ITEM* item_tmp = check_storage->item;
				while (item_tmp != NULL && item_tmp->decay <= program_clock)
				{
					ITEM* tmp = item_tmp;
					item_tmp = item_tmp->next;
					free(tmp);
				}
				if (item_tmp == NULL) {
					ITEM_QUEUE* storage_tmp = check_storage;
					check_storage = check_storage->next;
					free(storage_tmp);
				}
			}
			else {
				while (check_storage->next != NULL) {

					ITEM* item_tmp = check_storage->next->item;
					while (item_tmp != NULL && item_tmp->decay <= program_clock)
					{
						ITEM* tmp = item_tmp;
						item_tmp = item_tmp->next;
						free(tmp);
					}
					if (item_tmp == NULL) {
						ITEM_QUEUE* storage_tmp = check_storage->next;
						check_storage->next = check_storage->next->next;
						free(storage_tmp);
					}
					else
						check_storage = check_storage->next;
				}
			}
		}



		// remove the command from the buffer
		sscanf(buffer, "%s", key_menu);
		char* bebbo = Chop_word(buffer, key_menu);

		strcpy(buffer, bebbo);
		//----

		if (strcmp(key_menu, "aggiungi_ricetta") == 0)      // aggiungi ricetta
		{
			sscanf(buffer, "%s", &name);

			bebbo = Chop_word(buffer, name);
			//*buffer = Chop_word(buffer, name);
			strcpy(buffer, bebbo);


			if (recipes != NULL && Find(name, recipes) == true)
				printf("ignorato\n");
			else {
				INGREDIENT* ing_head = NULL;
				INGREDIENT* new_ing = NULL;
				new_ing = NULL;


				new_recipe = (RECIPE*)malloc(sizeof(RECIPE));

				strcpy(new_recipe->name, name);

				while (buffer[0] != '\0')
				{

					new_ing = (INGREDIENT*)malloc(sizeof(INGREDIENT));
					new_ing->next = NULL;
					sscanf(buffer, "%s %d", new_ing->name, &new_ing->quantity);

					if (ing_head == NULL)
					{
						new_ing->next = ing_head;
						ing_head = new_ing;
					}
					else if (ing_head->next == NULL) {
						if (strcmp(new_ing->name, ing_head->name) < 0) {
							new_ing->next = ing_head;
						}
						else {
							ing_head->next = new_ing;
						}
					}
					else {
						bool sorted = false;
						INGREDIENT* ing_tmp = ing_head;
						while (sorted != true) {
							
							if (ing_tmp->next == NULL) {
								ing_tmp->next = new_ing;
								sorted = true;
							}
							else if (strcmp(new_ing->name, ing_tmp->name) < 0){
								new_ing->next = ing_head;
								ing_head = new_ing;
								sorted = true;
							}
							else {
								if (strcmp(new_ing->name, ing_tmp->next->name) < 0) {
									new_ing->next = ing_tmp->next;
									ing_tmp->next = new_ing;
									sorted = true;
								}
								else
									ing_tmp = ing_tmp->next;
							}


						}
					}

					bebbo = Chop_word_int(buffer, new_ing->name, new_ing->quantity);
					if (bebbo != NULL)
						strcpy(buffer, bebbo);
					else
						buffer[0] = '\0';
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
			strcpy(name, "");
			sscanf(buffer, "%s", name);
			RECIPE* find_recipe = recipes;

			if (strcmp(find_recipe->name, name) == 0) {
				RECIPE* tmp = find_recipe;
				find_recipe = find_recipe->next;
				free(tmp);
				find = true;
			}
			else if (find_recipe->next != NULL) {
				while (find_recipe->next != NULL) {
					if (strcmp(find_recipe->next->name, name) == 0) {
						RECIPE* tmp = find_recipe->next;
						find_recipe->next = find_recipe->next->next;
						free(tmp);
						find = true;
						break;
					}
					else {
						find_recipe = find_recipe->next;
					}
				}
			}

			if (find == true)
				printf("eliminato\n");
			else
				printf("non trovata\n");

		}
		else if (strcmp(key_menu, "rifornimento") == 0)		// rifornimento
		{

			while (buffer[0] != '\0')
			{


				char tmp_name[LEN];

				//ingredient name
				sscanf(buffer, "%s", tmp_name);
				bebbo = Chop_word(buffer, tmp_name);
				if (bebbo == NULL)
					buffer[0] = '\0';
				else
					strcpy(buffer, bebbo);
				ITEM_QUEUE* storage = prod;
				bool find = false;
				// ITEM_QUEUE is empty------------------------------------
				if (storage != NULL)
				{
					ITEM* new_item = NULL;
					

					if (strcmp(storage->name, tmp_name) == 0)
						find = true;

					// item is alredy in ITEM_QUEUE------------------------------------
					if (storage->next != NULL) {
						while (find == false && storage->next != NULL)
						{
							if (strcmp(storage->name, tmp_name) == 0) {
								new_item = (ITEM*)malloc(sizeof(ITEM));
								new_item->next = NULL;

								sscanf(buffer, "%d %d", &new_item->quantity, &new_item->decay);

								bebbo = Chop_two_int(buffer, new_item->quantity, new_item->decay);
								strcpy(buffer, bebbo);

								if (storage->item->decay > new_item->decay) {
									new_item->next = storage->item;
									storage->item = new_item;
								}
								else {
									bool sort = false;
									ITEM* tmp = storage->item;
									while (sort == false) {
										//
										if (tmp->next != NULL &&  tmp->next->decay > new_item->decay) {
											new_item->next = tmp->next;
											tmp->next = new_item;
											sort =true;
										}
										else {
											
											if (tmp->next == NULL) {
												new_item->next = NULL;
												tmp->next = new_item;
												sort = true;
											}
											else
												tmp = tmp->next;
										}
									}
								}


								find = true;
							}
							else {
								if (storage->next != NULL)
									storage = storage->next;

							}
						}

					}

				}
				// -------------------------------------------------------------
				if (find == false) {

					// item is new in the ITEM_QUEUE or ITEM_QUEUE is NULL-----------------------------------
					ITEM* new_item = NULL;

					new_prod = (ITEM_QUEUE*)malloc(sizeof(ITEM_QUEUE));
					strcpy(new_prod->name, tmp_name);
					new_prod->item = NULL;
					new_prod->next = NULL;


					new_item = (ITEM*)malloc(sizeof(ITEM));
					new_item->next = NULL;
					sscanf(buffer, "%d %d", &new_item->quantity, &new_item->decay);
					
					new_prod->item = new_item;

				
					if (prod == NULL)
					{
						new_prod->next = prod;
						prod = new_prod;
					}
					else if (prod->next == NULL) {
						if (strcmp(new_prod->name, prod->name) < 0) {
							new_prod->next = prod;
						}
						else {
							prod->next = new_prod;
						}
					}
					else {
						bool sorted = false;
						ITEM_QUEUE* prod_tmp = prod;
						while (sorted != true) {
							
							if (prod_tmp->next == NULL) {
								prod_tmp->next = new_prod;
								sorted = true;
							}
							else if (strcmp(new_prod->name, prod_tmp->name) < 0){
								new_prod->next = prod;
								prod = new_prod;
								sorted = true;
							}
							else {
								if (strcmp(new_prod->name, prod_tmp->next->name) < 0) {
									new_prod->next = prod_tmp->next;
									prod_tmp->next = new_prod;
									sorted = true;
								}
								else
									prod_tmp = prod_tmp->next;
							}


						}
					}
					

					bebbo = Chop_two_int(buffer, new_item->quantity, new_item->decay);
					if (bebbo != NULL)
						strcpy(buffer, bebbo);
					else
						buffer[0] = '\0';
					
				}

			}

		
			printf("rifornito\n");
		}
		else if (strcmp(key_menu, "ordine") == 0)			// ordine
		{
			int number = 0;

			sscanf(buffer, "%s %d", &name,&number);

			
			int order = Check_ing_for_order(recipes, prod, name, number);
			
			if (order == 1) {
				// go 
			}
			else if (order == 2) {
				// queue
			}
			else {
				//not accepted
			}



		}

		program_clock++;
		//free(buffer);
	}


	// Termina la misurazione del tempo
	clock_t end_time = clock();

	// Calcola il tempo trascorso in secondi
	double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("Runtime:: %f s.\n", time_taken);




	return 0;
}

