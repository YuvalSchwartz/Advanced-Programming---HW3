#include "Pokemon.h"
#include "BattleByCategory.h"


static status TypeArrayDestroy(Type* t_array_to_destroy, int n_of_types) {
	if (t_array_to_destroy == NULL)
		return null_pointer_failure;
	int i;
	for (i = 0; i < n_of_types; i++) {
		if (TypeDestroy(t_array_to_destroy + i) == null_pointer_failure)
			return null_pointer_failure;
	}
	return success;
}

static status Destroy(Type* type_array_to_destroy, int number_of_types, Battle battle_to_destroy) {
	if (type_array_to_destroy == NULL || battle_to_destroy == NULL)
		return null_pointer_failure;
	if (TypeArrayDestroy(type_array_to_destroy, number_of_types) == null_pointer_failure || destroyBattleByCategory(battle_to_destroy) == null_pointer_failure)
		return null_pointer_failure;
	return success;
}

static Type* ArrayTypeSearchByName(Type* types_array_pointer, int num_of_types, char* type_name) {
	if (types_array_pointer == NULL || type_name == NULL) {
		return NULL;
	}
	int i;
	for (i = 0; i < num_of_types; i++){
		if (strcmp((types_array_pointer + i)->type_name, type_name) == 0) {
			return ((types_array_pointer + i));
		}
	}
	return NULL;
}


int pokemon_equal_function(element p1, element p2) {
	if (p1 == NULL || p2 == NULL) return null_pointer_failure;
	if (((Pokemon*)(p1))->pokemon_biological_information->attack > ((Pokemon*)(p2))->pokemon_biological_information->attack) return 1;
	else if (((Pokemon*)(p1))->pokemon_biological_information->attack < ((Pokemon*)(p2))->pokemon_biological_information->attack) return -1;
	return 0;
}

element pokemon_copy_function(element p) {
	if (p == NULL) return NULL;
	BiologicalInformation* bio_copy = (BiologicalInformation*) malloc (sizeof(BiologicalInformation));
	if (bio_copy == NULL) return NULL;
	bio_copy->attack = ((Pokemon*)(p))->pokemon_biological_information->attack;
	bio_copy->height = ((Pokemon*)(p))->pokemon_biological_information->height;
	bio_copy->weight = ((Pokemon*)(p))->pokemon_biological_information->weight;
	Pokemon* copy = (Pokemon*) malloc (sizeof(Pokemon));
	if (copy == NULL) {
		BilogicalInformationDestroy(bio_copy);
		return NULL;
	}
	copy->pokemon_biological_information = bio_copy;
	copy->pokemon_name = (char *) malloc ((strlen(((Pokemon*)(p))->pokemon_name) + 1) * sizeof(char));
	if (copy->pokemon_name == NULL) PokemonDestroy(copy);
	strcpy(copy->pokemon_name, ((Pokemon*)(p))->pokemon_name);
	copy->pokemon_species = (char *) malloc ((strlen(((Pokemon*)(p))->pokemon_species) + 1) * sizeof(char));
	if (copy->pokemon_species == NULL) PokemonDestroy(copy);
	strcpy(copy->pokemon_species, ((Pokemon*)(p))->pokemon_species);
	copy->pokemon_type = ((Pokemon*)(p))->pokemon_type;
	return copy;
}

status pokemon_free_function(element p) {
	if (p == NULL) return null_pointer_failure;
	return PokemonDestroy(((Pokemon*)(p)));
}

char* pokemon_get_category_function(element p) {
	if (p == NULL) return NULL;
	return ((Pokemon*)(p))->pokemon_type->type_name;
}

int pokemon_get_attack_function(element p1, element p2, int* p1_attack, int* p2_attack) {
	if (p1 == NULL || p2 == NULL || p1_attack == NULL || p2_attack == NULL) return INT_MIN;
	bool flag = false;
	*p1_attack = ((Pokemon*)(p1))->pokemon_biological_information->attack;
	*p2_attack = ((Pokemon*)(p2))->pokemon_biological_information->attack;
	int i;
	for (i = 0; i < ((Pokemon*)(p1))->pokemon_type->amount_of_against_me; i++) {
		if (strcmp(((Pokemon*)(p1))->pokemon_type->effective_against_me[i]->type_name, ((Pokemon*)(p2))->pokemon_type->type_name) == 0) flag = true;
	}
	for (i = 0; i < ((Pokemon*)(p2))->pokemon_type->amount_of_against_me; i++) {
		if (strcmp(((Pokemon*)(p1))->pokemon_type->type_name, ((Pokemon*)(p2))->pokemon_type->effective_against_me[i]->type_name) == 0) flag = true;
	}
	if (flag == true)
		*p1_attack -= 10;
	return *p1_attack-*p2_attack;
}

status pokemon_print_function(element p) {
	if (p == NULL) return null_pointer_failure;
	return PokemonPrint(((Pokemon*)(p)));
}



int main(int argc, char* argv[])
{
	int numberOfTypes = strtol(argv[1], NULL, 10);
	int capacityInType = strtol(argv[2], NULL, 10);
	FILE *file_pointer;
	status condition;
	char buffer[MAX_INPUT_SIZE];
	char types_title[] = "Types"; //string that first line should be if there are any types
	char pokemons_title[] = "Pokemons"; //string that indicates end of types messing and start of pokemons messing
	char effective_against_me_title[] = "effective-against-me";
	char *token;
	int i, j;

	Battle b;
	Pokemon* temp_pokemon_pointer;
	BiologicalInformation* temp_biological_information_pointer;
	Type types_array[numberOfTypes];
	//temp-pokemon attributes declaration for pokemon's initialization:
	int pokemon_line_indexes[6]; //indexes of string details in pokemons line reading (name,species,height,weight,attack,type)
	pokemon_line_indexes[0] = 0; //index of name is always at the beggining of pokemons line


	file_pointer = fopen(argv[3], "r"); //opens the file

	fgets(buffer, MAX_INPUT_SIZE, file_pointer); //reads the first word of the file
	buffer[strcspn(buffer, "\n")] = 0;
	buffer[strcspn(buffer, "\r")] = 0;
	if (strcmp(types_title, buffer) == 0) { //checks if word is "Types"
		fgets(buffer, MAX_INPUT_SIZE, file_pointer); //reads next line
		buffer[strcspn(buffer, "\n")] = 0;
		buffer[strcspn(buffer, "\r")] = 0;
		b = createBattleByCategory(capacityInType,
								   numberOfTypes,
								   buffer,
								   pokemon_equal_function,
								   pokemon_copy_function,
								   pokemon_free_function,
								   pokemon_get_category_function,
								   pokemon_get_attack_function,
								   pokemon_print_function);
		if (strcmp(pokemons_title, buffer) != 0) { //checks if line is not 'Pokemons'
			token = strtok(buffer, ",");
			for (i = 0; i < numberOfTypes; i++) {
				condition = TypeInit(types_array + i, token); //initialize each type
				if (condition == memory_failure) {
					printf("No memory available.\n");
					Destroy(types_array, numberOfTypes, b);
					fclose(file_pointer);
					return 1;
				}
				else if (condition == null_pointer_failure) {
					printf("Null Pointer Problem\n");
					Destroy(types_array, numberOfTypes, b);
					fclose(file_pointer);
					return 1;
				}
				token = strtok(NULL, ",");
			}
			fgets(buffer, MAX_INPUT_SIZE, file_pointer); //reads next line
			buffer[strcspn(buffer, "\n")] = 0;
			buffer[strcspn(buffer, "\r")] = 0;
			buffer[strcspn(buffer, "\t")] = 0;
			while (buffer[0] != 'P' && buffer[1] != '\0') { //checks if line is not "Pokemons"
				which_to_enter against;
				token = strtok(buffer + 1, " "); //now token is name of a type
				Type* enter_to = ArrayTypeSearchByName(types_array, numberOfTypes, token);
				token = strtok(NULL, ":"); //now token is "effective-against-me" or "effective-against-others"
				if (strcmp(effective_against_me_title, token) == 0) {
					against = me;
				}
				else {
					against = others;
				}
				token = strtok(NULL, ","); //continues to names of types
				while (token != NULL) {
					Type* entered = ArrayTypeSearchByName(types_array, numberOfTypes, token);
					if (against == me) {
						condition = AddTypeToEffectiveAgainstMe(enter_to, entered);
					}
					else {
						condition = AddTypeToEffectiveAgainstOthers(enter_to, entered);
					}
					if (condition == memory_failure) {
						printf("No memory available.\n");
						Destroy(types_array, numberOfTypes, b);
						fclose(file_pointer);
						return 1;
					}
					else if (condition == null_pointer_failure) {
						printf("Null Pointer Problem\n");
						Destroy(types_array, numberOfTypes, b);
						fclose(file_pointer);
						return 1;
					}
					token = strtok(NULL, ",");
				}
				fgets(buffer, MAX_INPUT_SIZE, file_pointer); //reads another line of "effective-against" or "Pokemons" line
				buffer[strcspn(buffer, "\n")] = 0;
				buffer[strcspn(buffer, "\r")] = 0;
				buffer[strcspn(buffer, "\t")] = 0;
			}
		}

	}
	if (strcmp(pokemons_title, buffer) == 0) { //checks if line is "Pokemons"
		i = 0;
		while (fgets(buffer, MAX_INPUT_SIZE, file_pointer) != NULL) {
			buffer[strcspn(buffer, "\n")] = 0;
			buffer[strcspn(buffer, "\r")] = 0;
			token = strtok(buffer, ",");
			for (j = 1; j < 6; j++) {
				pokemon_line_indexes[j] = pokemon_line_indexes[j - 1] + strlen(token) + 1;
				token = strtok(NULL, ",");
			}
			temp_biological_information_pointer = (BiologicalInformation*) malloc (sizeof(BiologicalInformation));
			if (temp_biological_information_pointer == NULL) {
				printf("No memory available.\n");
				Destroy(types_array, numberOfTypes, b);
				fclose(file_pointer);
				return 1;
			}
			condition = BiologicalInformationInit(temp_biological_information_pointer,
												  strtof(buffer + pokemon_line_indexes[2], NULL),
												  strtof(buffer + pokemon_line_indexes[3], NULL),
												  strtol(buffer + pokemon_line_indexes[4], NULL, 10));
			if (condition == null_pointer_failure) {
				printf("Null Pointer Problem\n");
				Destroy(types_array, numberOfTypes, b);
				fclose(file_pointer);
				return 1;
			}
			temp_pokemon_pointer = (Pokemon*) malloc (sizeof(Pokemon));
			if (temp_pokemon_pointer == NULL) {
				printf("No memory available.\n");
				BilogicalInformationDestroy(temp_biological_information_pointer);
				Destroy(types_array, numberOfTypes, b);
				fclose(file_pointer);
				return 1;
			}
			condition = PokemonInit(temp_pokemon_pointer,
									buffer,
									buffer + pokemon_line_indexes[1],
									temp_biological_information_pointer,
									ArrayTypeSearchByName(types_array, numberOfTypes, buffer + pokemon_line_indexes[5]));
			if (condition == null_pointer_failure) {
				printf("Null Pointer Problem\n");
				BilogicalInformationDestroy(temp_biological_information_pointer);
				Destroy(types_array, numberOfTypes, b);
				fclose(file_pointer);
				return 1;
			}
			condition = insertObject(b, temp_pokemon_pointer);
			if (condition == memory_failure) {
				printf("No memory available.\n");
				PokemonDestroy(temp_pokemon_pointer);
				Destroy(types_array, numberOfTypes, b);
				fclose(file_pointer);
				return 1;
			}
			else if (condition == null_pointer_failure) {
				printf("Null Pointer Problem\n");
				PokemonDestroy(temp_pokemon_pointer);
				Destroy(types_array, numberOfTypes, b);
				fclose(file_pointer);
				return 1;
			}
			condition = PokemonDestroy(temp_pokemon_pointer);
			if (condition == null_pointer_failure) {
				printf("Null Pointer Problem\n");
				Destroy(types_array, numberOfTypes, b);
				fclose(file_pointer);
				return 1;
			}
			i++;
		}
	}

	fclose(file_pointer);

	Type* users_type;
	float users_height, users_weight;
	int users_attack;
	char users_choice[2] = "";
	while(strcmp(users_choice, "6") != 0) {
		printf("Please choose one of the following numbers:\n");
		printf("1 : Print all Pokemons by types\n");
		printf("2 : Print all Pokemons types\n");
		printf("3 : Insert Pokemon to battles training camp\n");
		printf("4 : Remove strongest Pokemon by type\n");
		printf("5 : Fight\n");
		printf("6 : Exit\n");
		scanf("%2s", users_choice);

	    if (strcmp(users_choice, "1") == 0) {
	    	displayObjectsByCategories(b);
	    	continue;
	    }

        else if (strcmp(users_choice, "2") == 0) {
    		for (i = 0; i < numberOfTypes; i++) {
    			condition = TypePrint(&(types_array[i]));
    			if (condition == print_failure) {
    				printf("Print Failure\n");
    		    }
    			else if (condition == null_pointer_failure) {
    				printf("Null Pointer Problem\n");
    				Destroy(types_array, numberOfTypes, b);
    				return 1;
    			}
    		}
    		continue;
        }

	    else if (strcmp(users_choice, "3") == 0) {
	    	printf("Please enter Pokemon type name:\n");
	    	scanf("%s", buffer);
	    	users_type = ArrayTypeSearchByName(types_array, numberOfTypes, buffer);
	    	if (users_type != NULL) {
	    		if (users_type->amount_of == capacityInType) {
	    			printf("Type at full capacity.\n");
	    			continue;
	    		}
	    		else {
			    	printf("Please enter Pokemon name:\n");
			    	scanf("%s", buffer);
			    	char temp_pokemon_name[strlen(buffer) + 1];
			    	strcpy(temp_pokemon_name, buffer);
			    	printf("Please enter Pokemon species:\n");
			    	scanf("%s", buffer);
			    	char temp_pokemon_species[strlen(buffer) + 1];
			    	strcpy(temp_pokemon_species, buffer);
			    	printf("Please enter Pokemon height:\n");
			    	scanf("%f", &users_height);
			    	printf("Please enter Pokemon weight:\n");
			    	scanf("%f", &users_weight);
			    	printf("Please enter Pokemon attack:\n");
			    	scanf("%d", &users_attack);

					temp_biological_information_pointer = (BiologicalInformation*) malloc (sizeof(BiologicalInformation));
					if (temp_biological_information_pointer == NULL) {
						printf("No memory available.\n");
						Destroy(types_array, numberOfTypes, b);
						return 1;
					}

					condition = BiologicalInformationInit(temp_biological_information_pointer, users_height, users_weight, users_attack);
					if (condition == null_pointer_failure) {
						printf("Null Pointer Problem\n");
						Destroy(types_array, numberOfTypes, b);
						BilogicalInformationDestroy(temp_biological_information_pointer);
						return 1;
					}

					temp_pokemon_pointer = (Pokemon*) malloc (sizeof(Pokemon));
					if (temp_pokemon_pointer == NULL) {
						printf("No memory available.\n");
						Destroy(types_array, numberOfTypes, b);
						BilogicalInformationDestroy(temp_biological_information_pointer);
						return 1;
					}

					condition = PokemonInit(temp_pokemon_pointer, temp_pokemon_name, temp_pokemon_species, temp_biological_information_pointer, users_type);
					if (condition == null_pointer_failure) {
						printf("Null Pointer Problem\n");
						BilogicalInformationDestroy(temp_biological_information_pointer);
						Destroy(types_array, numberOfTypes, b);
						free(temp_pokemon_pointer);
						return 1;
					}
					else if (condition == memory_failure) {
						printf("No memory available.\n");
						BilogicalInformationDestroy(temp_biological_information_pointer);
						Destroy(types_array, numberOfTypes, b);
						free(temp_pokemon_pointer);
						return 1;
					}

					condition = insertObject(b, temp_pokemon_pointer);
					if (condition == success) {
						printf("The Pokemon was successfully added.\n");
						if (PokemonPrint(temp_pokemon_pointer) == null_pointer_failure) {
							printf("Null Pointer Problem\n");
							PokemonDestroy(temp_pokemon_pointer);
							Destroy(types_array, numberOfTypes, b);
							return 1;
						}
						condition = PokemonDestroy(temp_pokemon_pointer);
						if (condition == null_pointer_failure) {
							printf("Null Pointer Problem\n");
							BilogicalInformationDestroy(temp_biological_information_pointer);
							Destroy(types_array, numberOfTypes, b);
							return 1;
						}
					}
					else if (condition == memory_failure){
						printf("No memory available.\n");
						PokemonDestroy(temp_pokemon_pointer);
						Destroy(types_array, numberOfTypes, b);
						return 1;
					}
					else if (condition == null_pointer_failure) {
						printf("Null Pointer Problem\n");
						PokemonDestroy(temp_pokemon_pointer);
						Destroy(types_array, numberOfTypes, b);
						return 1;
					}
	    		}
	    	}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
	    	continue;
	    }

	    else if (strcmp(users_choice, "4") == 0) {
	    	printf("Please enter type name:\n");
	    	scanf("%s", buffer);
	    	users_type = ArrayTypeSearchByName(types_array, numberOfTypes, buffer);
	    	if (users_type != NULL) {
	    		temp_pokemon_pointer = removeMaxByCategory(b, users_type->type_name);
	    		if (temp_pokemon_pointer != NULL) {
					printf("The strongest Pokemon was removed:\n");
					if (PokemonPrint(temp_pokemon_pointer) == null_pointer_failure) {
						printf("Null Pointer Problem\n");
						PokemonDestroy(temp_pokemon_pointer);
						Destroy(types_array, numberOfTypes, b);
						return 1;
					}
					condition = PokemonDestroy(temp_pokemon_pointer);
					if (condition == null_pointer_failure) {
						printf("Null Pointer Problem\n");
						Destroy(types_array, numberOfTypes, b);
						return 1;
					}
					users_type->amount_of--;
	    		}
	    		else printf("There is no Pokemon to remove.\n");
	    	}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
	    	continue;
	    }

	    else if (strcmp(users_choice, "5") == 0) {
	    	printf("Please enter Pokemon type name:\n");
	    	scanf("%s", buffer);
	    	users_type = ArrayTypeSearchByName(types_array, numberOfTypes, buffer);
	    	if (users_type != NULL) {
		    	printf("Please enter Pokemon name:\n");
		    	scanf("%s", buffer);
		    	char temp_pokemon_name[strlen(buffer) + 1];
		    	strcpy(temp_pokemon_name, buffer);
		    	printf("Please enter Pokemon species:\n");
		    	scanf("%s", buffer);
		    	char temp_pokemon_species[strlen(buffer) + 1];
		    	strcpy(temp_pokemon_species, buffer);
		    	printf("Please enter Pokemon height:\n");
		    	scanf("%f", &users_height);
		    	printf("Please enter Pokemon weight:\n");
		    	scanf("%f", &users_weight);
		    	printf("Please enter Pokemon attack:\n");
		    	scanf("%d", &users_attack);
				temp_biological_information_pointer = (BiologicalInformation*) malloc (sizeof(BiologicalInformation));
				if (temp_biological_information_pointer == NULL) {
					printf("No memory available.\n");
					Destroy(types_array, numberOfTypes, b);
					return 1;
				}
				condition = BiologicalInformationInit(temp_biological_information_pointer, users_height, users_weight, users_attack);
				if (condition == null_pointer_failure) {
					printf("Null Pointer Problem\n");
					Destroy(types_array, numberOfTypes, b);
					return 1;
				}
				temp_pokemon_pointer = (Pokemon*) malloc (sizeof(Pokemon));
				if (temp_pokemon_pointer == NULL) {
					printf("No memory available.\n");
					BilogicalInformationDestroy(temp_biological_information_pointer);
					Destroy(types_array, numberOfTypes, b);
					return 1;
				}
				condition = PokemonInit(temp_pokemon_pointer, temp_pokemon_name, temp_pokemon_species, temp_biological_information_pointer, users_type);
				if (condition == null_pointer_failure) {
					printf("Null Pointer Problem\n");
					BilogicalInformationDestroy(temp_biological_information_pointer);
					Destroy(types_array, numberOfTypes, b);
					return 1;
				}
				printf("You choose to fight with:\n");
				if (PokemonPrint(temp_pokemon_pointer) == null_pointer_failure) {
					printf("Null Pointer Problem\n");
					PokemonDestroy(temp_pokemon_pointer);
					Destroy(types_array, numberOfTypes, b);
					return 1;
				}
				Pokemon* winner = fight(b, temp_pokemon_pointer);
				if (winner != temp_pokemon_pointer)
					PokemonDestroy(winner);
				condition = PokemonDestroy(temp_pokemon_pointer);
				if (condition == null_pointer_failure) {
					printf("Null Pointer Problem\n");
					Destroy(types_array, numberOfTypes, b);
					return 1;
				}
				users_type->amount_of--;
	    	}
    		else {
    			printf("Type name doesn't exist.\n");
    		}
	    	continue;
	    }

	    else if (strcmp(users_choice, "6") == 0) {
	    	Destroy(types_array, numberOfTypes, b);
    		printf("All the memory cleaned and the program is safely closed.\n");
    		break;
	    }

        else {
        	printf("Please choose a valid number.\n");
        	continue;
        }

	}
	return 0;
}
