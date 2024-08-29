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

typedef struct item_queue_node {
	char name[LEN];
	ITEM* item;
	struct item_queue_node* next;
}ITEM_QUEUE;

typedef struct ing_pointer_node {
	ITEM_QUEUE* recipe_ingredients;
	struct ing_pointer_node* next;
}ING_POINTER;

typedef struct order_pointers_node {
	RECIPE* recipe;
	ING_POINTER* ing_pointer;
}ORDER_POINTERS;

typedef struct wait_list_node {
	ORDER_POINTERS* order_pointer;
	int time;
	int quantity;
	struct wait_list_node* next;
}WAIT_LIST;

typedef struct delivery_truck_node {
	char name[LEN];
	int time;
	int quantity;
	int weight;
	struct delivery_truck_node* next;
}DELIVERY_TRUCK;


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


ITEM* Add_item(ITEM_QUEUE* storage, ITEM* new_item) {
	if (storage->item->decay > new_item->decay) {
		new_item->next = storage->item;
		storage->item = new_item;
	}
	else {
		bool sort = false;
		ITEM* tmp = storage->item;
		while (sort == false) {
			//
			if (tmp->next != NULL && tmp->next->decay > new_item->decay) {
				new_item->next = tmp->next;
				tmp->next = new_item;
				sort = true;
			}
			else {

				if (tmp->next == NULL) {
					new_item->next = NULL;
					tmp->next = new_item;
					new_item = tmp;
					sort = true;
				}
				else
					tmp = tmp->next;
			}
		}
	}
	return storage->item;
}

ITEM_QUEUE* Check_if_somethink_is_rotten(ITEM_QUEUE* prod, int program_clock) {
	
	if (prod != NULL) {
		//ITEM_QUEUE* prod_next = prod->next;
		if (prod->next == NULL) { // if there is only 1 element in prod
			//ITEM* item_tmp = check_storage->item;
			while (prod->item != NULL && prod->item->decay <= program_clock)
			{
				ITEM* tmp = prod->item;
				prod->item = prod->item->next;
				free(tmp);
			}
			if (prod->item == NULL) {
				free(prod);
				prod = NULL;
			}
		}
		else if (prod->item == NULL) {

			while (prod != NULL && prod->item == NULL) {

				if (prod->item == NULL) {
					while (prod->item != NULL) {
						ITEM* tmp = prod->next->item;
						prod->next->item = prod->next->item->next;
						free(tmp);
					}
					if (prod->item == NULL) {
						ITEM_QUEUE* storage_tmp = prod;
						prod = prod->next;
						free(storage_tmp);
					}
				}
			}

		}
		else {
			ITEM_QUEUE* prod_tmp = prod;
			while (prod_tmp != NULL) {// if there at least 2 element in prod

				//ITEM* item_tmp = check_storage->next->item;
				if (prod_tmp->next != NULL) {
					while (prod_tmp->next->item != NULL && prod_tmp->next->item->decay <= program_clock)
					{
						ITEM* tmp = prod_tmp->next->item;
						prod_tmp->next->item = prod_tmp->next->item->next;
						free(tmp);
					}
					if (prod_tmp->next->item == NULL) {
						ITEM_QUEUE* storage_tmp = prod_tmp->next;
						prod_tmp->next = prod_tmp->next->next;
						prod_tmp = prod_tmp->next;
						free(storage_tmp);
					}
					else
						prod_tmp = prod_tmp->next;
				}
				else {
					prod_tmp = prod_tmp->next;
				}
			}
		}
	}

	return prod;
}


ING_POINTER* Delete_ingredient_from_research(ING_POINTER* order_ing_pointer) {
	while (order_ing_pointer != NULL) {
		ING_POINTER* tmp = order_ing_pointer;
		order_ing_pointer = order_ing_pointer->next;
		free(tmp);
	}
	return NULL;
}

ING_POINTER* Check_all_the_ing_after_refill(INGREDIENT* ing, ITEM_QUEUE* storage, int num) {

	ING_POINTER* order_ing_pointer = NULL;
	ING_POINTER* order_ing_pointer_tail = NULL;

	bool not_enough_item = false;

	while (ing != NULL && storage != NULL && not_enough_item != true) {
		bool find_ingredient = false;
		if (strcmp(ing->name, storage->name) == 0) { // check if all the ingredients are in and if so save the head
			find_ingredient = true;

			int tot = ing->quantity * num;

			if (storage->item != NULL) {
				if (tot > storage->item->quantity) {

					ITEM* item_list = storage->item;
					while (tot > 0 && item_list != NULL) {
						tot -= item_list->quantity;
						if (tot > 0) {
							item_list = item_list->next;
						}
					}
					if (item_list != NULL) {
						ING_POINTER* new_order_ing_pointer = (ING_POINTER*)malloc(sizeof(ING_POINTER));
						new_order_ing_pointer->recipe_ingredients = storage;
						if (order_ing_pointer == NULL) {
							new_order_ing_pointer->next = NULL;
							order_ing_pointer = new_order_ing_pointer;
							order_ing_pointer_tail = order_ing_pointer;
						}
						else {
							new_order_ing_pointer->next = NULL;
							order_ing_pointer_tail->next = new_order_ing_pointer;
							order_ing_pointer_tail = new_order_ing_pointer;
						}
					}
					else {
						order_ing_pointer = Delete_ingredient_from_research(order_ing_pointer);
						not_enough_item = true;
					}
				}
				else if (tot <= storage->item->quantity) {
					ING_POINTER* new_order_ing_pointer = (ING_POINTER*)malloc(sizeof(ING_POINTER));
					new_order_ing_pointer->recipe_ingredients = storage;

					if (order_ing_pointer == NULL) {
						new_order_ing_pointer->next = NULL;
						order_ing_pointer = new_order_ing_pointer;
						order_ing_pointer_tail = order_ing_pointer;
					}
					else {
						new_order_ing_pointer->next = NULL;
						order_ing_pointer_tail->next = new_order_ing_pointer;
						order_ing_pointer_tail = new_order_ing_pointer;
					}
				}
				storage = storage->next;
				ing = ing->next;
			}
			else {
				order_ing_pointer = Delete_ingredient_from_research(order_ing_pointer);
				not_enough_item = true;
			}
		}
		else {
			if (find_ingredient == false) {
				storage = storage->next;
				if (storage == NULL || strcmp(ing->name, storage->name) < 0) {
					order_ing_pointer = Delete_ingredient_from_research(order_ing_pointer);
					not_enough_item = true;
				}
			}
		}
	}

	return order_ing_pointer;

}


bool  Find_in_WaitList(WAIT_LIST* wait_list, char name[LEN]) {
	bool find = false;
	while (wait_list != NULL && find != true) {
		if (strcmp(wait_list->order_pointer->recipe->name, name) == 0)
			find = true;
		else
			wait_list = wait_list->next;
	}
	return find;
}

ORDER_POINTERS* Check_ing_for_order(RECIPE* recipe, ITEM_QUEUE* storage, char name[LEN], int num) {
	// check if the recipe and the ingredient are ok for making the order.

	ORDER_POINTERS* order_pointers = NULL;

	bool recipe_check = false;
	while (recipe != NULL && recipe_check != true) {
		if (strcmp(recipe->name, name) == 0) {
			order_pointers = (ORDER_POINTERS*)malloc(sizeof(ORDER_POINTERS));

			order_pointers->recipe = recipe;
			recipe_check = true;

			ING_POINTER* order_ing_pointer = NULL;
			if (storage != NULL)
				order_ing_pointer = Check_all_the_ing_after_refill(recipe->ingredient, storage, num);

			order_pointers->ing_pointer = order_ing_pointer;

		}
		else {
			recipe = recipe->next;
		}

	}

	return order_pointers;

}

DELIVERY_TRUCK* sort_the_new_order(DELIVERY_TRUCK* new_delivery, DELIVERY_TRUCK* delivery_truck) {
	if (delivery_truck == NULL) {
		delivery_truck = new_delivery;
	}
	else {
		DELIVERY_TRUCK* delivery_truck_tmp = delivery_truck;
		bool sorted = false;
		if (delivery_truck->next == NULL)
		{
			if (delivery_truck->time > new_delivery->time) {
				new_delivery->next = delivery_truck;
				delivery_truck = new_delivery;
			}
			else
				delivery_truck->next = new_delivery;
		}
		else {
			while (sorted != true) { //delivery_truck_tmp != NULL &&
				if (delivery_truck_tmp->next == NULL) {
					delivery_truck_tmp->next = new_delivery;
					sorted = true;
				}
				if (delivery_truck->next->time > new_delivery->time) {
					new_delivery->next = delivery_truck_tmp->next;
					delivery_truck_tmp->next = new_delivery;
					sorted = true;
				}
				else
					delivery_truck_tmp = delivery_truck_tmp->next;
			}
		}
	}

	return delivery_truck;
}

DELIVERY_TRUCK* Prepare_the_order(WAIT_LIST* order_list, DELIVERY_TRUCK* delivery_truck) {

	DELIVERY_TRUCK* new_delivery = (DELIVERY_TRUCK*)malloc(sizeof(DELIVERY_TRUCK));
	new_delivery->time = order_list->time;
	strcpy(new_delivery->name, order_list->order_pointer->recipe->name);
	new_delivery->quantity = order_list->quantity;
	new_delivery->weight = 0;

	new_delivery->next = NULL;

	INGREDIENT* ing = order_list->order_pointer->recipe->ingredient;
	ING_POINTER* ing_point = order_list->order_pointer->ing_pointer;
	while (ing != NULL) {
		ITEM_QUEUE* storage = ing_point->recipe_ingredients;
		int tot = ing->quantity * order_list->quantity;
		if (tot < storage->item->quantity) {
			storage->item->quantity = storage->item->quantity - tot;
		}
		else if (tot == storage->item->quantity) {
			ITEM* tmp = storage->item;
			storage->item = storage->item->next;
			free(tmp);
		}
		else {
			int tot_tmp = tot;
			while (tot_tmp > 0) {
				if (tot_tmp < storage->item->quantity) {
					storage->item->quantity = storage->item->quantity - tot_tmp;
					tot_tmp = 0;
				}
				else if (tot_tmp >= storage->item->quantity) {
					tot_tmp = tot_tmp - storage->item->quantity;
					ITEM* tmp = storage->item;
					storage->item = storage->item->next;
					free(tmp);
					//storage = tmp;
				}
			}

		}
		ing = ing->next;
		ing_point = ing_point->next;
		new_delivery->weight += tot;
	}

	delivery_truck = sort_the_new_order(new_delivery, delivery_truck);

	return delivery_truck;
}

DELIVERY_TRUCK* Copy_truck_value(DELIVERY_TRUCK* delivery_truck) {
	DELIVERY_TRUCK* loading_queue = (DELIVERY_TRUCK*)malloc(sizeof(DELIVERY_TRUCK));
	if (delivery_truck != NULL) {
		strcpy(loading_queue->name, delivery_truck->name);
		loading_queue->quantity = delivery_truck->quantity;
		loading_queue->time = delivery_truck->time;
		loading_queue->weight = delivery_truck->weight;
		loading_queue->next = NULL;
	}
	else
		loading_queue = NULL;
	return loading_queue;
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

	// order in wait list
	WAIT_LIST* wait_list = NULL;
	WAIT_LIST* wait_list_tail = NULL;

	// order ready for the DELIVERY_TRUCK
	DELIVERY_TRUCK* delivery_truck = NULL;

	//def of the delivery truck

	int delivery_clock = 0;
	int delivery_dim = 0;

	scanf(" %d %d ", &delivery_clock, &delivery_dim);

	//int program_clock = 2; //id the debugger clock
	int program_clock = 0; // is the real clock

	char buffer[BLEN];
	char name[LEN];
	char key_menu[LEN];


	while (gets(buffer) != NULL)
	{
		//printf("%d. ", program_clock);
		//chosing the option and considering the delivery
		if (program_clock != 0 && program_clock % delivery_clock == 0) {
			//delivery
			int load = 0;


			DELIVERY_TRUCK* new_loading_queue = NULL;
			DELIVERY_TRUCK* loading_queue = NULL;

			if (delivery_truck != NULL) {
				while (delivery_truck != NULL && load < delivery_dim) {

					load += delivery_truck->weight;
					if (load < delivery_dim) {
						new_loading_queue = Copy_truck_value(delivery_truck);

						if (loading_queue == NULL) {
							loading_queue = new_loading_queue;
						}

						else if (loading_queue->next == NULL) {
							if (loading_queue->weight < new_loading_queue->weight) {
								new_loading_queue->next = loading_queue;
								loading_queue = new_loading_queue;
							}
							else {
								loading_queue->next = new_loading_queue;
							}

						}
						else if (loading_queue->weight < new_loading_queue->weight) {
							new_loading_queue->next = loading_queue;
							loading_queue = new_loading_queue;
						}
						else {
							DELIVERY_TRUCK* loading_queue_tmp = loading_queue;
							bool sorted = false;

							while (loading_queue_tmp != NULL && sorted != true) {
								if (loading_queue_tmp->next == NULL) {
									loading_queue_tmp->next = new_loading_queue;
									sorted = true;
								}
								if (loading_queue_tmp->next->weight < new_loading_queue->weight) {
									new_loading_queue->next = loading_queue_tmp->next;
									loading_queue_tmp->next = new_loading_queue;
									sorted = true;
								}
								else
									loading_queue_tmp = loading_queue_tmp->next;
							}

						}
						DELIVERY_TRUCK* delivery_tmp = delivery_truck->next;
						free(delivery_truck);
						delivery_truck = delivery_tmp;
						//free(delivery_tmp);
					}

				}


				if (loading_queue == NULL) {
					printf("camioncino vuoto\n");
				}
				else {
					while (loading_queue != NULL) {
						DELIVERY_TRUCK* tmp = loading_queue;
						printf("%d %s %d\n", loading_queue->time, loading_queue->name, loading_queue->quantity);
						loading_queue = loading_queue->next;
						free(tmp);
					}
				}
			}
			else
				printf("camioncino vuoto\n");
		}



		// check if something went rotten

		prod = Check_if_somethink_is_rotten(prod, program_clock);



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

					if (ing_head == NULL) {
						new_ing->next = ing_head;
						ing_head = new_ing;
					}
					else if (ing_head != NULL && ing_head->next == NULL) {
						if (strcmp(new_ing->name, ing_head->name) < 0) {
							new_ing->next = ing_head;
							ing_head = new_ing;
						}
						else {
							ing_head->next = new_ing;
						}
					}
					else if (strcmp(new_ing->name, ing_head->name) < 0) {
						new_ing->next = ing_head;
						ing_head = new_ing;
					}
					else {
						bool sorted = false;
						INGREDIENT* ing_tmp = ing_head;
						while (sorted != true) {

							if (ing_tmp->next == NULL) {
								ing_tmp->next = new_ing;
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




				printf("aggiunta\n");
				rec_counter++;

			}
		}
		else if (strcmp(key_menu, "rimuovi_ricetta") == 0)	// rimuovi ricetta
		{
			bool find = false;
			strcpy(name, "");
			sscanf(buffer, "%s", name);
			RECIPE* find_recipe = recipes;


			bool find_in_ws = Find_in_WaitList(wait_list, name);
			if (find_in_ws != true) {
				if (strcmp(find_recipe->name, name) == 0) {
					RECIPE* tmp = find_recipe;
					recipes = find_recipe->next;
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
					printf("rimossa\n");
				else
					printf("non presente\n");
			}
			else {
				printf("ordini in sospeso\n");
			}



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


					if (strcmp(storage->name, tmp_name) == 0) {
						find = true;
						new_item = (ITEM*)malloc(sizeof(ITEM));
						new_item->next = NULL;
						sscanf(buffer, "%d %d", &new_item->quantity, &new_item->decay);

						bebbo = Chop_two_int(buffer, new_item->quantity, new_item->decay);
						if (bebbo == NULL)
							buffer[0] = '\0';
						else
							strcpy(buffer, bebbo);

						storage->item = Add_item(storage, new_item);

					}
					// item is alredy in ITEM_QUEUE------------------------------------
					if (storage->next != NULL) {
						while (find == false && storage->next != NULL)
						{
							if (strcmp(storage->name, tmp_name) == 0) {
								new_item = (ITEM*)malloc(sizeof(ITEM));
								new_item->next = NULL;

								sscanf(buffer, "%d %d", &new_item->quantity, &new_item->decay);

								bebbo = Chop_two_int(buffer, new_item->quantity, new_item->decay);
								if (bebbo == NULL)
									buffer[0] = '\0';
								else
									strcpy(buffer, bebbo);

								storage->item = Add_item(storage, new_item);

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
							prod = new_prod;
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
							else if (strcmp(new_prod->name, prod_tmp->name) < 0) {
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
			//prod;
			// check if now some order in wait list can be processed
			if (wait_list != NULL) {
				ING_POINTER* ing_pointers = NULL;

				do { // cycle untill all the order in wait_list in time order can be performed
					ing_pointers = NULL;
					INGREDIENT* ing = wait_list->order_pointer->recipe->ingredient;
					int num = wait_list->quantity;

					ing_pointers = Check_all_the_ing_after_refill(ing, prod, num);

					if (ing_pointers != NULL) {

						wait_list->order_pointer->ing_pointer = ing_pointers;
						delivery_truck = Prepare_the_order(wait_list, delivery_truck);

						WAIT_LIST* free_wait_list = wait_list;
						wait_list = wait_list->next;

						free(free_wait_list);

					}

				} while (wait_list != NULL && ing_pointers != NULL);
			}
			if (wait_list != NULL) {
				ING_POINTER* ing_pointers = NULL;
				WAIT_LIST* wait_list_tmp_next = wait_list->next;
				WAIT_LIST* wait_list_tmp = wait_list;
				prod;
				while (wait_list_tmp_next != NULL) {

					ing_pointers = NULL;
					INGREDIENT* ing = wait_list_tmp_next->order_pointer->recipe->ingredient;
					int num = wait_list_tmp_next->quantity;

					ing_pointers = Check_all_the_ing_after_refill(ing, prod, num);

					if (ing_pointers != NULL) {

						wait_list_tmp_next->order_pointer->ing_pointer = ing_pointers;
						delivery_truck = Prepare_the_order(wait_list_tmp_next, delivery_truck);

						WAIT_LIST* free_wait_list = wait_list_tmp_next;

						wait_list_tmp->next = wait_list_tmp_next->next;
						wait_list_tmp_next = wait_list_tmp_next->next;

						free(free_wait_list);

					}
					else {
						wait_list_tmp = wait_list_tmp_next;
						wait_list_tmp_next = wait_list_tmp_next->next;
					}


				}
			}


			printf("rifornito\n");
		}
		else if (strcmp(key_menu, "ordine") == 0)			// ordine
		{
			int number = 0;

			sscanf(buffer, "%s %d", &name, &number);

			// struct for saving pointers for the order
			ORDER_POINTERS* order_pointers = NULL;

			order_pointers = Check_ing_for_order(recipes, prod, name, number);

			
			if (order_pointers != NULL) { // if != NULL im shure that at least the Recipe exist


				WAIT_LIST* new_wait_list = (WAIT_LIST*)malloc(sizeof(WAIT_LIST));

				new_wait_list->order_pointer = order_pointers;
				new_wait_list->time = program_clock;
				new_wait_list->quantity = number;
				new_wait_list->next = NULL;
				if (order_pointers->ing_pointer != NULL) {
					// delivery_truck
					delivery_truck = Prepare_the_order(new_wait_list, delivery_truck);
					free(new_wait_list);//delete the temporari storing

				}
				else {
					if (wait_list == NULL) {
						wait_list = new_wait_list;
						wait_list_tail = wait_list;
					}
					else {
						// wait_list
						wait_list_tail->next = new_wait_list;
						wait_list_tail = new_wait_list;
					}
				}
				printf("accettato\n");
			}
			else
				printf("rifiutato\n");


		}

		program_clock++;

	}


	// Termina la misurazione del tempo
	clock_t end_time = clock();

	// Calcola il tempo trascorso in secondi
	double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("Runtime:: %f s.\n", time_taken);




	return 0;
}
