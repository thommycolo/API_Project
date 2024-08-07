#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Globar variable

#define LEN 256


//structure declaring


typedef struct ing_node {
	char name[LEN];
	int quantity;
	struct ing_node*next;
}INGREDIENT;

typedef struct rec_node{
	char name[LEN];
	INGREDIENT ingredient;
	struct rec_node*next;
}RECIPE;

//def of the recipes list
INGREDIENT* ing_head = NULL;
INGREDIENT* new_ing = NULL;

RECIPE* rec_head = NULL;
RECIPE* recipes = NULL;


// Function declaring

/*int New_line(char word[LEN]) {
	int tmp = 1;
	if (strcmp(word, "aggiungi_ricetta") == 0 || strcmp(word, "rimuovi_ricetta") == 0 || strcmp(word, "rifornimento") == 0 || strcmp(word, "ordine") == 0)
		tmp=0;
	else
		tmp=1;
	printf("DIO\n");
	return tmp;
}*/


INGREDIENT Add_ingredient() {
	INGREDIENT* head = NULL;
	INGREDIENT* new_ingredient = NULL;
	char new_ing[LEN] ="";
	new_ingredient = malloc(sizeof(INGREDIENT));

	scanf("%s", &new_ing);
	//printf("%s", new_ing);
	/*while(strcmp(new_ing,"aggiungi_ricetta")!=0 || strcmp(new_ing, "rimuovi_ricetta") != 0 || strcmp(new_ing, "ordine") != 0 || strcmp(new_ing, "rifornimento") != 0){
	//while(scanf("%s",&new_ing)!='\n'){
		//scanf("%s", new_ing);
		new_ingredient = malloc(sizeof(INGREDIENT));
		strcpy(new_ingredient->name, new_ing);

		scanf("%d",&new_ingredient->quantity);
		new_ingredient->next = head;
		head = new_ingredient;
		printf("%s %d\n",new_ingredient->name,new_ingredient->quantity);
		//printf("%d\n",New_line(new_ing));
	}
	*/
	int quantity;
	//strcpy(new_ingredient->name, new_ing);
	
	return *head;
}


void Add_recipe(char *new_recipe[LEN]){
	
		recipes = malloc(sizeof(RECIPE));
		strcpy(recipes->name, new_recipe);
		//printf("%s\n", new_recipe);
		char new_ing[LEN];
		int quantity=0;
		char tmp[LEN];
		printf("BEBBO");
		scanf("%s %d", new_ing, &quantity);
		scanf("%s %d", new_ing, &quantity);
		scanf("%s %d", new_ing, &quantity);
		scanf("[^\n]", tmp);
		printf("%s", tmp);
		printf("BEBBO");
		//printf("%s %d\n", new_ing, quantity);


		//recipes->ingredient = Add_ingredient();
		recipes->next = rec_head;
		rec_head = recipes;
	
	}

bool Find_recipe(char *new_recipe[LEN]) {

	bool find = false;

	while (recipes->next != NULL) {

		if (strcmp(new_recipe, recipes->name) == 0) {
			printf("ricetta gia' nota\n");
			find = true;
			break;
		}
		else
			find = false;

	}
	return find;
}

	




int main()
{
	//def of the delivery truck
	int delivery_clock=0;
	int delivery_dim=0;
	
	scanf(" %d %d ", &delivery_clock, &delivery_dim);
	printf(" %d %d \n", delivery_clock, delivery_dim);
	// def of the switch
	char key_menu[LEN];



	int cclock = 0;
	char new_recipe[LEN];

	while (scanf("%s ", &key_menu) != EOF)
	{
		
		//chosing the option and considering the delivery
		if (cclock == delivery_clock) {
			//delivery
			cclock = 0;
		}


		else if (strcmp(key_menu, "aggiungi_ricetta") == 0)      // aggiungi ricetta
		{
			

			scanf("%s", &new_recipe);
			
			char ing_tmp[LEN];
			int quant_tmp = 0;
			if (rec_head == NULL) {
				Add_recipe(new_recipe);
				printf(" aggiunta\n");
				//printf("%s %s %d", recipes-> new_recipe, recipes->ingredient->name,recipes->ingredient->quantity);
			}
			else {

				if (Find_recipe(new_recipe) == false) {
					Add_recipe(new_recipe);
					printf(" aggiunta\n");
				}
				else
					printf("ignorato\n");

			}
		}
		else if (strcmp(key_menu, "rimuovi_ricetta") == 0) 
		{
			// rimuovi ricetta

			bool find = false;
			char str[LEN];

			scanf("%s", &new_recipe);

			if (strcmp(rec_head->name, str) == 0) 
				rec_head = rec_head->next;
			else {
				while (recipes->next != NULL) {

					if (strcmp(new_recipe, recipes->next->name) == 0) {
						printf("ricetta eliminata\n");
						recipes->next = recipes->next->next;
							find = true;
						break;
					}
					if (find == false)
						printf("ricetta non presente\n");
				}
			}
			
		}
		else if (strcmp(key_menu, "rifornimento") == 0) 
		{
			// rifornimento

		}
		else if (strcmp(key_menu, "ordine") == 0) 
		{
			// ordine

		}

		//increasing the clock
			cclock++;
		
	}
	
	printf("out\n");
	return 0;
		
}