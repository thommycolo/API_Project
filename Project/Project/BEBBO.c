#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Globar variable

#define LEN 256

//structure declaring
typedef struct {
	char key[LEN];
	int value;
}MENU;

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

// Function declaring

int New_line(char word[LEN]) {
	int tmp = 1;
	if (strcmp(word, "aggiungi_ricetta") == 0 || strcmp(word, "rimuovi_ricetta") == 0 || strcmp(word, "rifornimento") == 0 || strcmp(word, "ordine") == 0)
		tmp=0;
	else
		tmp=1;
	printf("DIO\n");
	return tmp;
}

INGREDIENT Add_ingredient() {
	INGREDIENT* head = NULL;
	INGREDIENT* new_ingredient = NULL;
	char new_ing[LEN] ="";
	
	do{
		scanf("%s", new_ing);
		new_ingredient = malloc(sizeof(INGREDIENT));
		scanf("%d",&new_ingredient->quantity);
		strcpy(new_ingredient->name, new_ing);
		new_ingredient->next = head;
		head = new_ingredient;
		//printf("%d\n",New_line(new_ing));
	} while (New_line(new_ing) > 0);
	
		return *head;
}




int main()
{
	//def of the delivery truck
	int delivery_clock=0;
	int delivery_dim=0;
	//scanf(" %d %d ", &corr_clock, &corr_dim);

	// def of the switch
	char key_menu[LEN];
	int choice=0;

	MENU menu[4];
	for (int i=0; i>4;i++)
		menu[i].value = i;
	int clock = 0;

	//def of the recipes list
	INGREDIENT* ing_head = NULL;
	INGREDIENT* new_ing = NULL;

	RECIPE* rec_head = NULL;
	RECIPE* recipes = NULL;

	int cclock = 0;
	
	while (scanf("%s ", &key_menu) != EOF)
	{
		//printf("%s", key_menu);

		//chosing the option and considering the delivery
		if (cclock != delivery_clock) {
			//delivery
			clock = 0;
		}
		else {
			for (int i = 0; i > 4;i++)
			{
				if (strcmp(key_menu, menu[i].key) == 0)
					choice = menu[i].value;
			}
		}

		char new_recipe[256];

		switch (choice)
		{
		case(0):// aggiungi ricetta
			scanf("%s", &new_recipe);
			char ing_tmp[LEN];
			int quant_tmp = 0;
			if (rec_head == NULL) {
				recipes = malloc(sizeof(RECIPE));
				strcpy(recipes->name, new_recipe);
				recipes->ingredient = Add_ingredient();

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
		cclock++;
	}
		return 0;
>>>>>>> b6a0d8d0889f5447972896d9b8131edc2d0300ca
}