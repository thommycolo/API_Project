#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#include <time.h>

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

typedef struct recipe_tree_node {
	char name[LEN];
	INGREDIENT* ingredient;
	bool is_existing;
	struct recipe_tree_node* left;
	struct recipe_tree_node* right;
}RECIPE_TREE_NODE;

typedef struct item_node {
	int quantity;
	int decay;
	struct item_node* next;
}ITEM;

typedef struct prod_tree_node {
	char name[LEN];
	int quantity_tot;
	ITEM* item;
	struct prod_tree_node* left; // left if prod->name is smaller than the child
	struct prod_tree_node* right;// left if prod->name is bigger than the child
}PROD_TREE_NODE;



typedef struct ing_pointer_node {
	PROD_TREE_NODE* prod_node;
	struct ing_pointer_node* next;
}PROD_POINTER;

typedef struct order_pointers_node {
	RECIPE_TREE_NODE* recipe;
	PROD_POINTER* prod_pointer;
}ORDER_POINTERS;

typedef struct wait_list_node {
	ORDER_POINTERS* order_pointer;
	int time;
	int quantity;
	struct wait_list_node* next;
}WAIT_LIST;

typedef struct order_ready_node {
	char name[LEN];
	int time;
	int quantity;
	int weight;
	struct order_ready_node* next;
}ORDER_READY;

typedef struct delivery_truck_node {
	char name[LEN];
	int quantity;
	int time;
	int weight;
	struct delivery_truck_node* next;
}DELIVERY_TRUCK;

// Function declaring

// ============================ PROD AND ITEM FILLING CREATION =========================

void My_free(void* ptr) {

	if (ptr != NULL)
		free(ptr);

}


void Check_if_some_item_is_decayed(PROD_TREE_NODE* prod, int program_clock) {
	while (prod->item != NULL && program_clock >= prod->item->decay) {
		ITEM* tmp = prod->item;
		prod->quantity_tot -= prod->item->quantity;
		prod->item = prod->item->next;
		My_free(tmp);
	}

}
PROD_TREE_NODE* Create_prod_child_node(char* name, ITEM* item) {

	PROD_TREE_NODE* new_node = (PROD_TREE_NODE*)malloc(sizeof(PROD_TREE_NODE));
	if (new_node != NULL) {

		strcpy(new_node->name, name);
		new_node->item = item;
		new_node->quantity_tot = item->quantity;
		new_node->left = NULL;
		new_node->right = NULL;
	}
	return new_node;
}
ITEM* Sort_the_item(ITEM* item, ITEM* new_item) {

	ITEM* item_iterator = item;
	bool sorted = false;
	if (item != NULL) {
		if (item->decay > new_item->decay) { // switching the first position if necessary
			new_item->next = item;
			item = new_item;
			sorted = true;
		}
		while (sorted != true) {

			if (item_iterator->next == NULL) { // adding in the right position the new item
				new_item->next = NULL;
				item_iterator->next = new_item;
				sorted = true;
			}
			else if (item_iterator->next->decay > new_item->decay) {
				new_item->next = item_iterator->next;
				item_iterator->next = new_item;
				sorted = true;
			}
			else
				item_iterator = item_iterator->next;
		}
	}
	else
		item = new_item;
	return item;
}
void Sort_new_prod(PROD_TREE_NODE* prod_root, char* name, ITEM* new_item, int program_clock) {
	bool sorted = false;
	//PROD_TREE_NODE* treenode = prod_root;

	while (sorted != true) {

		if (strcmp(prod_root->name, name) == 0) { // new_prod is alredy existing in prood_root
			Check_if_some_item_is_decayed(prod_root, program_clock);
			prod_root->quantity_tot += new_item->quantity;
			prod_root->item = Sort_the_item(prod_root->item, new_item);
			sorted = true;
		}
		else {
			if (strcmp(prod_root->name, name) < 0) { // tmp_name is bigger than the father, so i check on his right
				if (prod_root->right == NULL) {
					PROD_TREE_NODE* new_node = Create_prod_child_node(name, new_item);// haven't found new_prod in the prod_root so i create a new node
					prod_root->right = new_node;
					sorted = true;
				}
				else
					prod_root = prod_root->right;
			}
			else { // tmp_name is smaller than the father, so i check on his left
				if (prod_root->left == NULL) {
					PROD_TREE_NODE* new_node = Create_prod_child_node(name, new_item);// haven't found new_prod in the prod_root so i create a new node
					prod_root->left = new_node;
					sorted = true;
				}
				else
					prod_root = prod_root->left;
			}

		}


	}

}

// ============================ RECIPE CHILD CREATION ==================================
/*bool Find_Recipe(RECIPE_TREE_NODE* root, char* name) {

	while (root != NULL)
	{
		if (root == NULL)
			return false;
		int ris = strcmp(root->name, name);
		if (ris == 0)
			return true;
		if (ris < 0)
		{
			if (root->right == NULL)
				return false;
			else
				root = root->right;
		}
		else
		{
			if (root->left == NULL)
				return false;
			else
				root = root->left;
		}
	}
	return false;
}
/*
bool Sort_new_recipe(RECIPE_TREE_NODE* recipe_root, RECIPE_TREE_NODE* recipe_tmp) {

	RECIPE_TREE_NODE* treenode = recipe_root;
	bool isPresent = false;
	while (true)
	{
		if (strcmp(treenode->name, recipe_tmp->name) == 0)
		{
			while (recipe_tmp->ingredient != NULL) {
				INGREDIENT* tmp = recipe_tmp->ingredient;
				recipe_tmp->ingredient = recipe_tmp->ingredient->next;
				My_free(tmp);
			}

			My_free(recipe_tmp);
			isPresent = true;
			break;// recipe was already existing
		}
		else
		{// recipe_tmp is bigger than treenode
			if (strcmp(treenode->name, recipe_tmp->name) < 0)
			{
				if (treenode->right == NULL)
				{
					treenode->right = recipe_tmp;
					isPresent = false;
					break; // recipe wasn't already existing
				}
				else
					treenode = treenode->right;
			}
			else
			{ // recipe_tmp is smaller than treenode

				if (treenode->left == NULL)
				{
					treenode->left = recipe_tmp;
					isPresent = false;
					break; // recipe wasn't already existing
				}
				else
					treenode = treenode->left;

			}

		}

	}

	return isPresent;
}
*/

void Add_new_recipe(RECIPE_TREE_NODE* recipe_root, RECIPE_TREE_NODE* recipe_tmp) {

	RECIPE_TREE_NODE* treenode = recipe_root;
	bool isPresent = false;
	while (true)
	{
		if (strcmp(treenode->name, recipe_tmp->name) == 0)
		{
			treenode->is_existing = true;
			treenode->ingredient = recipe_tmp->ingredient;
			break;// recipe was already existing
		}
		else
		{// recipe_tmp is bigger than treenode
			if (strcmp(treenode->name, recipe_tmp->name) < 0)
			{
				if (treenode->right == NULL)
				{
					treenode->right = recipe_tmp;
					break; // recipe wasn't already existing
				}
				else
					treenode = treenode->right;
			}
			else
			{ // recipe_tmp is smaller than treenode

				if (treenode->left == NULL)
				{
					treenode->left = recipe_tmp;
					break; // recipe wasn't already existing
				}
				else
					treenode = treenode->left;

			}

		}

	}

}

RECIPE_TREE_NODE* Find_Recipe(RECIPE_TREE_NODE* recipe_root, char* name) {
	if (recipe_root == NULL)
		return NULL;

	while (true) {

		if (strcmp(recipe_root->name, name) == 0)
		{
			return recipe_root;
		}
		if (strcmp(name, recipe_root->name) < 0)
		{
			if (recipe_root->left == NULL)
				return NULL;
			else
				recipe_root = recipe_root->left;
		}
		else
		{
			if (recipe_root->right == NULL)
				return NULL;
			else
				recipe_root = recipe_root->right;
		}

	}
	return recipe_root;
}

INGREDIENT* Create_new_ingredient(char* name, int quantity) {
	INGREDIENT* ing = (INGREDIENT*)malloc(sizeof(INGREDIENT));
	strcpy(ing->name, name);
	ing->quantity = quantity;
	ing->next = NULL;
	return ing;
}
RECIPE_TREE_NODE* Create_recipe_child_node(char* name, INGREDIENT* ingredient) {
	RECIPE_TREE_NODE* new_node = (RECIPE_TREE_NODE*)malloc(sizeof(RECIPE_TREE_NODE));
	if (new_node != NULL) {

		strcpy(new_node->name, name);
		new_node->ingredient = ingredient;
		new_node->is_existing = true;
		new_node->left = NULL;
		new_node->right = NULL;
	}
	return new_node;

}


// ============================= CHECK INGRENDIENT FOR ORDER / WAIT LIST ================


RECIPE_TREE_NODE* Check_recipe_for_order(RECIPE_TREE_NODE* recipe_root, char* name) {
	bool find = false;

	if (recipe_root != NULL) {
		while (recipe_root != NULL && find != true) {

			if (strcmp(recipe_root->name, name) == 0) { // i've found the recipe
				find = true;
			}
			else {
				if (strcmp(recipe_root->name, name) < 0) { // tmp_name is bigger than the father, so i check on his right
					recipe_root = recipe_root->right;
				}
				else { // tmp_name is smaller than the father, so i check on his left
					recipe_root = recipe_root->left;
				}
			}
		}
	}
	return recipe_root;
}
PROD_TREE_NODE* Find_prod(PROD_TREE_NODE* prod_root, char* name, int quantity_tot, int program_clock) {
	bool find = false;

	if (prod_root != NULL) {
		while (prod_root != NULL && find != true) {

			if (strcmp(prod_root->name, name) == 0) { // i've found the prod	
				if (prod_root->item != NULL) {
					Check_if_some_item_is_decayed(prod_root, program_clock);
					if (prod_root->item != NULL) {
						if (prod_root->quantity_tot >= quantity_tot) // Check if there is enough item in storage to perform the order
							find = true;
						else
							break;
					}
					else
						break;
				}
				else
					break;
			}
			else {
				if (strcmp(prod_root->name, name) < 0) { // tmp_name is bigger than the father, so i check on his right
					prod_root = prod_root->right;
				}
				else { // tmp_name is smaller than the father, so i check on his left
					prod_root = prod_root->left;
				}
			}
		}
	}
	if (find == true)
		return prod_root;
	else
		return NULL;
}
void Delete_all_tmp_ing_pointer(PROD_POINTER* ing_pointer) {
	while (ing_pointer != NULL) {
		PROD_POINTER* tmp = ing_pointer;
		ing_pointer = ing_pointer->next;
		My_free(tmp);
	}

}
PROD_POINTER* Check_ing_for_order(int time, INGREDIENT* rec_ingredient, PROD_TREE_NODE* prod_root, int number, int program_clock) {
	PROD_POINTER* prod_pointer = NULL;
	PROD_POINTER* prod_pointer_tail = prod_pointer;

	while (rec_ingredient != NULL) {
		int quantity_tot = rec_ingredient->quantity * number;// is the total amount of item necessary
		PROD_TREE_NODE* prod_tmp = Find_prod(prod_root, rec_ingredient->name, quantity_tot, program_clock); // find the prod and if there is enough item in storage

		if (prod_tmp != NULL) {
			PROD_POINTER* new_prod_pointer = (PROD_POINTER*)malloc(sizeof(PROD_POINTER));
			new_prod_pointer->next = NULL;
			new_prod_pointer->prod_node = prod_tmp;
			if (prod_pointer == NULL) {
				prod_pointer = new_prod_pointer;
				prod_pointer_tail = prod_pointer;
			}
			else {
				prod_pointer_tail->next = new_prod_pointer;
				prod_pointer_tail = new_prod_pointer;
			}
		}
		else {
			Delete_all_tmp_ing_pointer(prod_pointer);// delete all the ing pointer that i found
			return NULL;
		}

		rec_ingredient = rec_ingredient->next;
	}

	return prod_pointer;
}

// =============================== FIND FOR REMOVING RECIPE ==============================

bool  Find_in_WaitList(WAIT_LIST* wait_list, char* name) {
	bool find = false;
	while (wait_list != NULL && find == false) {
		if (strcmp(wait_list->order_pointer->recipe->name, name) == 0)
			find = true;
		else
			wait_list = wait_list->next;
	}
	return find;
}
bool Find_in_order_ready(ORDER_READY* order_ready, char* name) {
	bool find = false;
	while (order_ready != NULL && find == false) {
		if (strcmp(order_ready->name, name) == 0)
			find = true;
		else
			order_ready = order_ready->next;
	}
	return find;
}

RECIPE_TREE_NODE* Get_successor(RECIPE_TREE_NODE* curr) {
	curr = curr->right;
	while (curr != NULL && curr->left != NULL)
		curr = curr->left;
	return curr;
}

void Delete_recipe(RECIPE_TREE_NODE* root, char* name) {

	if (root == NULL) {
		return ;
	}

	while(true){
		if (strcmp(name, root->name) == 0)
		{
			root->is_existing = false;

			while (root->ingredient != NULL) {
				INGREDIENT* tmp = root->ingredient;
				root->ingredient = root->ingredient->next;
				My_free(tmp);
			}
			printf("rimossa\n");
			return;
		}
		else if (strcmp(name, root->name) < 0)
		{
			if (root->left == NULL)
				return;
			else
				root = root->left;
		}
		else
			if (root->right == NULL)
				return;
			else
				root = root->right;
	}
	return;
}

// =============================== ORDER'S PREPARATION====================================
ORDER_READY* sort_the_new_order_ready(ORDER_READY* new_order, ORDER_READY* order_ready) {
	ORDER_READY* order_ready_tmp = order_ready;
	if (order_ready == NULL) {
		order_ready = new_order;
	}
	else if (order_ready->time > new_order->time) {
		new_order->next = order_ready;
		order_ready = new_order;
	}
	else {
		bool sorted = false;
		while (sorted != true) {

			if (order_ready_tmp->next == NULL) {
				order_ready_tmp->next = new_order;
				sorted = true;
			}
			else if (order_ready_tmp->next->time > new_order->time) {
				new_order->next = order_ready_tmp->next;
				order_ready_tmp->next = new_order;
				sorted = true;
			}
			else
				order_ready_tmp = order_ready_tmp->next;
		}
	}
	return order_ready;
}
void Take_item_from_prod(PROD_TREE_NODE* prod, int ing_weight) {

	while (ing_weight > 0) {
		if (ing_weight != 0) {
			if (ing_weight < prod->item->quantity) { // ing_weight is fully contained in the first slot of prod
				prod->item->quantity -= ing_weight;
				ing_weight = 0;
			}
			else {  // ing_weight needs more or equal than one slot of item so I need to do stuff

				ing_weight -= prod->item->quantity;
				ITEM* tmp = prod->item;
				prod->item = prod->item->next;
				My_free(tmp);
			}
		}
	}

}
ORDER_READY* Prepare_the_order(WAIT_LIST* order_from_waitlist, ORDER_READY* order_ready) {

	ORDER_READY* new_order = (ORDER_READY*)malloc(sizeof(ORDER_READY)); // create a new_order node
	new_order->time = order_from_waitlist->time;
	strcpy(new_order->name, order_from_waitlist->order_pointer->recipe->name);
	new_order->quantity = order_from_waitlist->quantity;
	new_order->weight = 0;
	new_order->next = NULL;

	INGREDIENT* rec_ing = order_from_waitlist->order_pointer->recipe->ingredient; //recipe ingredient's pointer
	PROD_POINTER* prod_item = order_from_waitlist->order_pointer->prod_pointer;//prod item's pointer

	while (rec_ing != NULL) {
		int ing_weight = new_order->quantity * rec_ing->quantity;
		prod_item->prod_node->quantity_tot = prod_item->prod_node->quantity_tot - ing_weight;
		Take_item_from_prod(prod_item->prod_node, ing_weight);

		new_order->weight += ing_weight;
		rec_ing = rec_ing->next;
		prod_item = prod_item->next;
	}

	order_ready = sort_the_new_order_ready(new_order, order_ready);

	return order_ready;
}

// =============================== DELIVERY TRUCK =========================================

DELIVERY_TRUCK* Create_new_delivery(ORDER_READY* order_ready) {
	DELIVERY_TRUCK* new_delivery = (DELIVERY_TRUCK*)malloc(sizeof(DELIVERY_TRUCK));
	if (order_ready != NULL) {
		strcpy(new_delivery->name, order_ready->name);
		new_delivery->quantity = order_ready->quantity;
		new_delivery->time = order_ready->time;
		new_delivery->weight = order_ready->weight;
		new_delivery->next = NULL;
	}
	else
		new_delivery = NULL;
	return new_delivery;
}
DELIVERY_TRUCK* Sort_delivery(DELIVERY_TRUCK* delivery, DELIVERY_TRUCK* new_delivery) {
	if (delivery != NULL) {
		DELIVERY_TRUCK* delivery_tmp = delivery;
		bool sorted = false;

		if (delivery->weight < new_delivery->weight) {
			new_delivery->next = delivery;
			delivery = new_delivery;
		}
		else {
			while (sorted != true) {

				if (delivery_tmp->next == NULL) {
					delivery_tmp->next = new_delivery;
					sorted = true;
				}
				else if (delivery_tmp->next->weight < new_delivery->weight) {
					new_delivery->next = delivery_tmp->next;
					delivery_tmp->next = new_delivery;
					sorted = true;
				}
				else
					delivery_tmp = delivery_tmp->next;
			}
		}
	}
	else
		delivery = new_delivery;
	return delivery;
}
void Print_and_empty_truck(DELIVERY_TRUCK* delivery) {
	while (delivery != NULL) {
		DELIVERY_TRUCK* tmp = delivery;
		printf("%d %s %d\n", delivery->time, delivery->name, delivery->quantity);
		delivery = delivery->next;
		My_free(tmp);
	}
}
ORDER_READY* Load_the_delivery_truck(ORDER_READY* order_ready, int delivery_dim) {
	int load_weight = 0;
	DELIVERY_TRUCK* delivery = NULL;
	while (order_ready != NULL && load_weight < delivery_dim) {
		if (order_ready != NULL) {
			load_weight += order_ready->weight;
			if (load_weight <= delivery_dim) {

				DELIVERY_TRUCK* new_delivery = Create_new_delivery(order_ready); // put the order on the truck
				delivery = Sort_delivery(delivery, new_delivery);
				ORDER_READY* tmp = order_ready; //delete the order from its queue
				order_ready = order_ready->next;
				My_free(tmp);


			}
		}
		else
			break;
	}
	Print_and_empty_truck(delivery);
	return order_ready;
}


void Print_recipes(RECIPE_TREE_NODE* father) {

	if (father == NULL)
		return;

	if (father->left != NULL)
		Print_recipes(father->left);

	printf("##### %s\n", father->name);

	if (father->right != NULL)
		Print_recipes(father->right);
}
void Print_order_ready(ORDER_READY* order) {
	while (order != NULL) {
		printf("%d %s %d\n", order->time, order->name, order->weight);
		order = order->next;
	}
}


void prod_root_trash_can(PROD_TREE_NODE* prod_root)
{
	if (prod_root == NULL)
		return;
	if (prod_root->left != NULL)
		prod_root_trash_can(prod_root->left);
	if (prod_root->right != NULL)
		prod_root_trash_can(prod_root->right);
	while (prod_root->item != NULL) {
		ITEM* tmp = prod_root->item;
		prod_root->item = prod_root->item->next;
		My_free(tmp);
	}
	My_free(prod_root);
}
void recipe_root_trash_can(RECIPE_TREE_NODE* recipe_root) {
	if (recipe_root == NULL)
		return;
	if (recipe_root->left != NULL)
		recipe_root_trash_can(recipe_root->left);
	if (recipe_root->right != NULL)
		recipe_root_trash_can(recipe_root->right);
	while (recipe_root->ingredient != NULL) {
		INGREDIENT* tmp = recipe_root->ingredient;
		recipe_root->ingredient = recipe_root->ingredient->next;
		My_free(tmp);
	}
	My_free(recipe_root);


}
void order_ready_trash_can(ORDER_READY* order_ready) {
	while (order_ready != NULL) {
		ORDER_READY* tmp = order_ready;
		order_ready = order_ready->next;
		My_free(tmp);
	}
}
void wait_list_trash_can(WAIT_LIST* wait_list) {

	while (wait_list != NULL)
	{

		while (wait_list->order_pointer->prod_pointer != NULL)
		{
			PROD_POINTER* prod_tmp = wait_list->order_pointer->prod_pointer;
			wait_list->order_pointer->prod_pointer = wait_list->order_pointer->prod_pointer->next;
			My_free(prod_tmp);

		}

		My_free(wait_list->order_pointer);
		WAIT_LIST* tmp = wait_list;
		wait_list = wait_list->next;
		My_free(tmp);
	}

}


int main() {

	int delete_recipe = 0;

	// recipes list
	RECIPE_TREE_NODE* recipe_root = NULL;

	//int rec_counter = 0;

	// PROD_TREE list
	PROD_TREE_NODE* prod_root = NULL;


	// order in wait list
	WAIT_LIST* wait_list = NULL;
	WAIT_LIST* wait_list_tail = NULL;

	// order ready for the DELIVERY_TRUCK
	ORDER_READY* order_ready = NULL;

	//def of the delivery truck

	int delivery_clock;
	int delivery_dim;

	char buffer[BLEN];
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);

	//int program_clock = 2; //id the debugger clock
	int program_clock = 0; // is the real clock

	if (fgets(buffer, BLEN, stdin) == NULL)
		return 0;
	if (sscanf(buffer, "%d %d", &delivery_clock, &delivery_dim) != 0)
	{

		


		char name[LEN];
		char key_menu[LEN];

		while (fgets(buffer, BLEN, stdin) != NULL)
		{
			//printf("%d ", program_clock);
			//chosing the option and considering the delivery
			if (program_clock != 0 && program_clock % delivery_clock == 0) {
				//delivery	

				if (order_ready != NULL)
					order_ready = Load_the_delivery_truck(order_ready, delivery_dim);
				else
					printf("camioncino vuoto\n");


			}



			// remove the command from the buffer
			char* my_ptr = buffer;
			if (sscanf(my_ptr, "%s", key_menu) < 1)
				break;
			my_ptr += strlen(key_menu) + 1;
			//----

			if (strcmp(key_menu, "aggiungi_ricetta") == 0)      // aggiungi ricetta
			{
				sscanf(my_ptr, "%s", name);
				my_ptr += strlen(name) + 1;

				INGREDIENT* ing = NULL;

				char ing_name[LEN];
				int quantity;
				char p[30];
				RECIPE_TREE_NODE* node_found = Find_Recipe(recipe_root, name);
				if (node_found != NULL && node_found->is_existing == true)
				{
					
						printf("ignorato\n");
						//continue;
					
					
				}
				else{

					while (sscanf(my_ptr, "%s %s", ing_name, p) > 0) {

						sscanf(p, "%d", &quantity);

						my_ptr += strlen(ing_name) + strlen(p) + 2;

						// creting a temporary recipe
						INGREDIENT* new_ing = Create_new_ingredient(ing_name, quantity);
						if (ing == NULL) {
							ing = new_ing;
						}
						else {
							new_ing->next = ing;
							ing = new_ing;
						}

					}

					RECIPE_TREE_NODE* recipe_tmp = Create_recipe_child_node(name, ing);


					if (recipe_root == NULL)
						recipe_root = recipe_tmp;
					else
						Add_new_recipe(recipe_root, recipe_tmp);

					printf("aggiunta\n");
				}
			}
			else if (strcmp(key_menu, "rimuovi_ricetta") == 0)	// rimuovi ricetta
			{


				sscanf(my_ptr, "%s", name);



				if (Find_in_WaitList(wait_list, name) == true)
					printf("ordini in sospeso\n");

				else if (Find_in_order_ready(order_ready, name) == true)
					printf("ordini in sospeso\n");

				else
				{
					RECIPE_TREE_NODE* recipe_found = Find_Recipe(recipe_root, name);
					if (recipe_found == NULL)
						printf("non presente\n");
					else if (recipe_found->is_existing == false)
						printf("non presente\n");
					else
						Delete_recipe(recipe_root, name);
				}

			}
			else if (strcmp(key_menu, "rifornimento") == 0)		// rifornimento
			{
				delete_recipe++;
				char tmp_name[LEN];

				//ingredient name


				//bebbo = Chop_word(buffer, tmp_name);
				while (sscanf(my_ptr, "%s", tmp_name) > 0)
				{
					my_ptr += strlen(tmp_name) + 1;

					ITEM* new_item = (ITEM*)malloc(sizeof(ITEM));
					char p[2][50];
					sscanf(my_ptr, "%d %d", &new_item->quantity, &new_item->decay);
					sscanf(my_ptr, "%s %s", p[0], p[1]);
					my_ptr += strlen(p[0]) + strlen(p[1]) + 2;


					new_item->next = NULL;


					if (prod_root == NULL) { // if the Prod_tree is empty i create the prod_root

						prod_root = Create_prod_child_node(tmp_name, new_item);
					}
					else {

						Sort_new_prod(prod_root, tmp_name, new_item, program_clock);

					}


				}
				printf("rifornito\n");


				// check if now some order in wait list can be processed

				if (wait_list != NULL) {
					WAIT_LIST* wl_iterator = wait_list;
					WAIT_LIST* still_in_wl = NULL;

					while (wl_iterator != NULL) {
						wl_iterator->order_pointer->prod_pointer = Check_ing_for_order(wl_iterator->time, wl_iterator->order_pointer->recipe->ingredient, prod_root, wl_iterator->quantity, program_clock);

						if (wl_iterator->order_pointer->prod_pointer != NULL) {
							// order is ready to be processed
							WAIT_LIST* tmp = wl_iterator;
							order_ready = Prepare_the_order(wl_iterator, order_ready);
							wl_iterator = wl_iterator->next;

							Delete_all_tmp_ing_pointer(tmp->order_pointer->prod_pointer);
							My_free(tmp->order_pointer);
							My_free(tmp);
						}
						else {
							//order need to stay in wait list
							if (still_in_wl == NULL) {
								still_in_wl = wl_iterator;
								wait_list = still_in_wl;

							}
							else {
								still_in_wl->next = wl_iterator;
								still_in_wl = wl_iterator;
							}
							wait_list_tail = still_in_wl;

							wl_iterator = wl_iterator->next;


						}

					}

					if (still_in_wl == NULL)
						wait_list = NULL;
					else if (wl_iterator == NULL) {
						still_in_wl->next = wl_iterator;
						wait_list_tail->next = NULL;
					}

				}

			}
			else if (strcmp(key_menu, "ordine") == 0)	// ordine
			{
				int number = 0;

				sscanf(my_ptr, "%s %d", name, &number);

				// struct for saving pointers for the order
				ORDER_POINTERS* order_pointers = (ORDER_POINTERS*)malloc(sizeof(ORDER_POINTERS));
				order_pointers->prod_pointer = NULL;
				order_pointers->recipe = NULL;


				order_pointers->recipe = Check_recipe_for_order(recipe_root, name);
				if (order_pointers->recipe != NULL && order_pointers->recipe->is_existing == true)
				{
					order_pointers->prod_pointer = Check_ing_for_order(program_clock, order_pointers->recipe->ingredient, prod_root, number, program_clock);

					WAIT_LIST* new_wait_list = (WAIT_LIST*)malloc(sizeof(WAIT_LIST)); // create wait_list

					new_wait_list->order_pointer = order_pointers;
					new_wait_list->time = program_clock;
					new_wait_list->quantity = number;
					new_wait_list->next = NULL;


					if (order_pointers->prod_pointer != NULL) {
						// delivery_truck
						//printf("%s nel carretto ", name);
						order_ready = Prepare_the_order(new_wait_list, order_ready);
						Delete_all_tmp_ing_pointer(new_wait_list->order_pointer->prod_pointer);
						My_free(new_wait_list->order_pointer);
						My_free(new_wait_list);//delete the temporary storing
					}
					else {
						// adding at the tail of waitlist
						if (wait_list == NULL) {
							new_wait_list->next = NULL;
							wait_list = new_wait_list;
							wait_list_tail = wait_list;
						}
						else {

							wait_list_tail->next = new_wait_list;
							wait_list_tail = new_wait_list;
						}
					}
					printf("accettato\n");
				}

				else
				{
					My_free(order_pointers);
					printf("rifiutato\n");

				}
				//Print_recipes(recipe_root);
			}

			program_clock++;

		}

		

	}

	if (program_clock != 0 && program_clock % delivery_clock == 0) {
		//delivery	

		if (order_ready != NULL)
			order_ready = Load_the_delivery_truck(order_ready, delivery_dim);
		else
			printf("camioncino vuoto\n");


	}



	//rilascio tutti i prodotti
	prod_root_trash_can(prod_root);
	recipe_root_trash_can(recipe_root);
	order_ready_trash_can(order_ready);
	wait_list_trash_can(wait_list);

	return 0;
}
