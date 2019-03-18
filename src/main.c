#include <stdio.h>
#include "warehouse.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

int main(int argc, char** argv){

    char user_command [255];
    char operation [255];
    char operation2 [255];
    char warehouse_filename[255];
    char print_operation[255];
    char name [255];
    int size;
    int price;
    int exit_flag = 0;
    int ret = -1;
    int q_flag = 0;
    int w_flag = 0;
    int a_flag = 0;
    int s_flag = 0;
    char* w_filename = NULL;
    char* a_filename = NULL;
    char* s_indicator = NULL;
    int c;
    FILE* warehouse_file;
    int load_warehouse_flag = 0;
    FILE* art_file;

    while((c = getopt(argc, argv, "qw:a:s:")) != -1){
        switch(c){
            case 'q':
                q_flag = 1;
                break;

            case 'w':
                w_flag = 1;
                w_filename = optarg;
                break;

            case 'a':
                a_flag = 1;
                a_filename = optarg;
                break;

            case 's':
                s_flag = 1;
                s_indicator = optarg;
                break;

            default:
                printf("OPTARG ERROR\n");
                break;
        }
    }

    if ((q_flag == 1 && w_flag == 0 && a_flag == 0) || (q_flag == 1 && w_flag == 1 && a_flag == 0) || (q_flag == 1 && w_flag == 0 && a_flag == 1)){
        printf("FLAG ERROR\n");
	exit(0);
    }

    if (s_indicator != NULL){
    	if ((strcmp(s_indicator, "s") != 0) && (strcmp(s_indicator, "p") != 0)){
    		printf("FLAG ERROR\n");
		exit(0);
    	}
    }

    do{
        printf("Insert Command: ");
        fgets(user_command, 255, stdin);

        char k = user_command[0];

        if (k == 'l'){
            ret = sscanf(user_command, "%*s %s %s", operation, warehouse_filename);

            if (strcmp(operation, "warehouse") == 0){
                //do stuff, check filename
                if (access(warehouse_filename, F_OK)!= -1){
			load_warehouse_flag = 1;
                	warehouse_file = fopen(warehouse_filename, "rt");

                	int id;
                	int size;
                	char type[30];
                	struct warehouse* temp_warehouse;
			struct warehouse_list* temp_warehouse_list;
			struct warehouse_sf_list* pointer = sf_head;
			struct warehouse_sf_list* pointer2;
			struct warehouse_sf_list* temp_warehouse_sf;
			char singleLine [255];

                	while (!feof(warehouse_file)){
				fgets (singleLine, 255, warehouse_file);
				sscanf(singleLine, "%d %d %s", &id, &size, type);
				pointer2 = pointer;
				if (size < 4) {
					printf("Size cannot be less than 4\n");
					exit (0);
				}
				
				if (size % 2 != 0){
					printf("Size must be divisible by 2\n");
					exit (0);
				}

				temp_warehouse = createWarehouse(id, size);
				
				temp_warehouse_list = createWarehouseList(temp_warehouse, type, size);

				if (pointer == NULL){
					temp_warehouse_sf = createWarehouseSfList(size, temp_warehouse_list);
					pointer = temp_warehouse_sf;
					pointer2 = temp_warehouse_sf;
					//temp_warehouse_sf -> warehouse_list_head = temp_warehouse_list;
				}
				
				
				else if (pointer2 != NULL){
               				 while (pointer2 -> sf_next_warehouse != NULL){
                   	 			if (pointer2->class_size == size){
                       					struct warehouse_list* warehouse_list = pointer2->warehouse_list_head;

                        				while (warehouse_list->next_warehouse != NULL){
                           					 warehouse_list = warehouse_list -> next_warehouse;
                        				}
                       					 warehouse_list->next_warehouse = temp_warehouse_list;
							 break;
                   					}	

                   				 else{
                        				pointer2 = pointer2 ->sf_next_warehouse;
                    				 }
                			}

                			
					if(pointer2 -> sf_next_warehouse == NULL){
                				if (pointer2 -> class_size == size){
                					struct warehouse_list* warehouse_list = pointer2->warehouse_list_head;

                        				while (warehouse_list->next_warehouse != NULL){
                            					warehouse_list = warehouse_list -> next_warehouse;
                        				}		
                        				warehouse_list->next_warehouse = temp_warehouse_list;
                					}
                							

               					 else{
							temp_warehouse_sf = createWarehouseList(size, temp_warehouse_list);
							pointer2 -> sf_next_warehouse = temp_warehouse_sf;
                				}			
            
            				}
         			}
			}
		}
		else{
    			printf("File cannot be opened\n");
		}
	   }
            

            else if (strcmp(operation, "art") == 0){
                //do stuff, check filename
                if (load_warehouse_flag != 1){
			printf("You have not loaded warehouses yet\n");
		}
		
		if (access(warehouse_filename, F_OK)!= -1){
			art_file = fopen(warehouse_filename, "rt");
			
			char name[50];
			int size;
			int price;
			char line[255];
			int rep;
			struct art_collection* temp_art_collection;
			struct warehouse_sf_list* head = sf_head;
			struct warehouse_list* temp_ware_list;
			struct warehouse* temp_ware;
			uint64_t occupied;

			while (!feof(warehouse_file)){
				fgets (line, 255, warehouse_file);
				rep = sscanf(line, "%s %d %d", name, &size, &price);
				if (rep != 3){
					sscanf(line, "\"%[^\"]\" %d %d", name, &size, &price);
				}
			
				temp_art_collection = createArtCollection (name, size, price);
				
				if (head == NULL){
					printf("Currently have no warehouses(lists\n");
				}
				
				else{
					while(head!=NULL){
						if (head -> class_size = size){
							temp_ware_list = head->warehouse_list_head;
							occupied =(((temp_ware_list->meta_info) & (1 << 1)) >> 1);
							if (occupied == 0){
								//temp_ware = warehouse_list->warehouse;
								addToWarehouse(temp_ware, temp_art_collection);
								(temp_ware_list->meta_info) |= 1;
							}		
							else{
								temp_ware_list = temp_ware_list->next_warehouse;
							}
						}
						else{
							head = head->sf_next_warehouse;
						}
							
					}	
				
				}
			
			}
		}
		
		else{
			printf("File cannot be opened\n");

		}

		
            }

            else{
                printf("Such a command is not available. The list of available commands are in help\n");
            }
        }


        else if (k == 'p'){
            ret = sscanf(user_command, "%s", print_operation);

            if(strcmp(print_operation, "print") != 0 && strcmp(print_operation, "printall")!= 0){
                printf("Such a command is not available. This list of available are in help\n");
            }

            if (strcmp(print_operation, "print") == 0){
                ret = sscanf(user_command, "%*s %s", operation);
            }

            if (strcmp(print_operation, "printall") == 0){
                printf("Printall\n");
                //do printall stuff
            }

            else if (strcmp(operation, "public") == 0){
                printf("Print Public\n");
                //do print public stuff
            }

            else if (strcmp(operation, "private") == 0){
                printf("Print Private\n");
                //do print private stuff
            }
            else{
                printf("Such a command is not available. This list of available are in help\n");
            }

        }

        else if (k == 'a'){
            ret = sscanf(user_command, "%s %s %s %d %d",operation, operation2, name, &size, &price);

            if (ret != 5){
                ret = sscanf(user_command, "%s %s \"%[^\"]\" %d %d",operation, operation2, name, &size, &price);
            }

            if (strcmp(operation, "add") != 0){
                printf("Such a command is not available. The list of available commands are in help\n");
            }

            else if (strcmp(operation2, "art") != 0){
                printf("Such a command is not available. The list of available commands are in help\n");
            }

            else {

                printf("Add Art\n");
                printf("%s\n", name);
                printf("%d\n", size);
                printf("%d\n", price);
            }
        }


        else if(k == 'd'){
            ret = sscanf(user_command, "%s %s \"%[^\"]\"", operation, operation2, name);

            if (ret != 3){
                ret = sscanf(user_command, "%s %s %s", operation, operation2, name);
            }

            if (strcmp(operation, "delete") != 0){
                printf("Such a command is not available. The list of available commands are in help\n");
            }

            else if (strcmp(operation2, "art") != 0){
                printf("Such a command is not available. The list of available commands are in help\n");
            }

            else {
                printf("Delete Art\n");
                printf("%s\n", name);
            }
        }

        else if(k == 'u' || k == 'h' || k == 'e'){
            sscanf(user_command, "%s", operation);

            if (strcmp(operation, "help") == 0){
                printf("Here are the functions of the various commands\n");
                printf("help: This command gives a description of all the other commands\n");
                printf("load warehouse: This commands populates your linked list of warehouses\n");
                printf("load art: This commands populates your warehouse with art collections provided in the file\n");
                printf("printall: This commands prints information about all the art collections in database\n");
                printf("print public: This command prints information about art collections that are in public warehouses\n");
                printf("print private: This command prints information about art collections that are in private warehouses\n");
                printf("add art: This commands adds a new art collection to database with given attributes\n");
                printf("delete: This command deletes art collections with matching name\n");
                printf("utilization: This command prints number of occupied warehouses vs total, and total size of art collections vs total warehouse capacity\n");
                printf("exit: will exit program\n");
            }

            else if(strcmp(operation, "exit") == 0){
                exit_flag = 1;
            }

            else if(strcmp(operation, "utilization") == 0){
                printf("Utilization\n");
            }

	    else{
           	 printf("Such a command is not available. The list of available commands are in help\n");
       	    }
        }

        else{
            printf("Such a command is not available. The list of available commands are in help\n");
        }

    } while(exit_flag == 0);

    return 0;
}
