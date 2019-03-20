#include <stdio.h>
#include "warehouse.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include "linked_list.h"

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
    struct warehouse_sf_list* pointer = sf_head;

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
    
    if (q_flag == 1){
       if (access(w_filename, F_OK) != -1) {
            warehouse_file = fopen(w_filename, "rt");

            int id;
            int size;
            char type[30];
            struct warehouse *temp_warehouse;
            struct warehouse_list *temp_warehouse_list;
            struct warehouse_sf_list *pointer2;
            struct warehouse_sf_list *temp_warehouse_sf;
            char singleLine[255];

            while (!feof(warehouse_file)) {
                fgets(singleLine, 255, warehouse_file);
                sscanf(singleLine, "%d %d %s", &id, &size, type);
                pointer2 = pointer;
                if (size < 4) {
                    printf("Size cannot be less than 4\n");
                    exit(0);
                }

                if (size % 2 != 0) {
                    printf("Size must be divisible by 2\n");
                    exit(0);
                }

                temp_warehouse = createWarehouse(id, size);

                temp_warehouse_list = createWarehouseList(temp_warehouse, type, size);
                temp_warehouse_list->next_warehouse = NULL;

                if (pointer == NULL) {
                    temp_warehouse_sf = createWarehouseSfList(size, temp_warehouse_list);
                    pointer = temp_warehouse_sf;
                    pointer2 = temp_warehouse_sf;
                    //temp_warehouse_sf -> warehouse_list_head = temp_warehouse_list;
                } else if (pointer2 != NULL) {
                    while (pointer2->sf_next_warehouse != NULL) {
                        if (pointer2->class_size == size) {
                            struct warehouse_list *warehouse_list = pointer2->warehouse_list_head;

                            while (warehouse_list->next_warehouse != NULL) {
                                warehouse_list = warehouse_list->next_warehouse;
                            }
                            warehouse_list->next_warehouse = temp_warehouse_list;
                            break;
                        } else {
                            pointer2 = pointer2->sf_next_warehouse;
                        }
                    }


                    if (pointer2->sf_next_warehouse == NULL) {
                        if (pointer2->class_size == size) {
                            struct warehouse_list *warehouse_list = pointer2->warehouse_list_head;

                            while (warehouse_list->next_warehouse != NULL) {
                                warehouse_list = warehouse_list->next_warehouse;
                            }
                            warehouse_list->next_warehouse = temp_warehouse_list;
                        } else {
                            temp_warehouse_sf = createWarehouseSfList(size, temp_warehouse_list);
                            pointer2->sf_next_warehouse = temp_warehouse_sf;
                        }

                    }
                }
            }
        } else {
            printf("File cannot be opened\n");
        }

        if (access(a_filename, F_OK) != -1) {
            art_file = fopen(a_filename, "rt");

            char name[50];
            int size;
            int price;
            char line[255];
            int rep;
            struct art_collection *temp_art_collection;
            struct warehouse_sf_list *pointer3;
            struct warehouse_list *temp_ware_list;
            struct warehouse *temp_ware;
            uint64_t occupied;
            int art_coll_occupied;

            while (!feof(art_file)) {
                fgets(line, 255, art_file);
                rep = sscanf(line, "%s %d %d", name, &size, &price);
                if (rep != 3) {
                    sscanf(line, "\"%[^\"]\" %d %d", name, &size, &price);
                }

                art_coll_occupied = 0;
                temp_art_collection = createArtCollection(name, size, price);
                pointer3 = pointer;

                if (pointer3 == NULL) {
                    printf("Currently have no warehouses(lists\n");
                } else {
                    while (pointer3 != NULL) {
                        if (pointer3->class_size == size) {
                            temp_ware_list = pointer3->warehouse_list_head;

                            while (temp_ware_list != NULL) {
                                occupied = ((temp_ware_list->meta_info) & 2);
                                if (occupied == 0) {
                                    temp_ware = temp_ware_list->warehouse;
                                    addToWarehouse(temp_ware, temp_art_collection);
                                    //temp_ware_list->warehouse = temp_ware;
                                    (temp_ware_list->meta_info) |= 2;
                                    art_coll_occupied = 1;
                                    break;
                                } else {
                                    temp_ware_list = temp_ware_list->next_warehouse;
                                }
                            }

                            break;
                        } else {
                            pointer3 = pointer3->sf_next_warehouse;
                        }

                    }

                    if (art_coll_occupied == 0) {
                        pointer3 = pointer;
                        while (pointer3 != NULL) {
                            if (pointer3->class_size > size) {
                                temp_ware_list = pointer3->warehouse_list_head;

                                while (temp_ware_list != NULL) {
                                    occupied = ((temp_ware_list->meta_info) & 2);
                                    if (occupied == 0) {
                                        temp_ware = temp_ware_list->warehouse;
                                        addToWarehouse(temp_ware, temp_art_collection);
                                        temp_ware_list->warehouse = temp_ware;
                                        (temp_ware_list->meta_info) |= 2;
                                        art_coll_occupied = 1;
                                        break;
                                    } else {
                                        temp_ware_list = temp_ware_list->next_warehouse;
                                    }

                                }

                                break;
                            } else {
                                pointer3 = pointer3->sf_next_warehouse;
                            }

                        }

                    }

                    if (art_coll_occupied == 0) {
                        printf("Sorry, no warehouse found to place art collection\n");
                        exit(0);

                    }

                }
            }

        } else {
            printf("File cannot be opened\n");
        }

        while(pointer!= NULL){
            struct warehouse_list* cursor = pointer->warehouse_list_head;
            while(cursor!=NULL){
		if (cursor->warehouse->art_collection == NULL){}
		else{
                	printf("%s %d %d", cursor->warehouse->art_collection->name, cursor->warehouse->art_collection->size, cursor->warehouse->art_collection->price);
		}
                cursor = cursor->next_warehouse;
            }
            pointer = pointer->sf_next_warehouse;
        }
        exit(0);
	
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
				temp_warehouse_list->next_warehouse = NULL;

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
							temp_warehouse_sf = createWarehouseSfList(size, temp_warehouse_list);
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
			struct warehouse_sf_list* pointer3;
			struct warehouse_list* temp_ware_list;
			struct warehouse* temp_ware;
			uint64_t occupied;
			int art_coll_occupied;

			while (!feof(art_file)){
				fgets (line, 255, art_file);
				rep = sscanf(line, "%s %d %d", name, &size, &price);
				if (rep != 3){
					sscanf(line, "\"%[^\"]\" %d %d", name, &size, &price);
				}
				
				art_coll_occupied = 0;
				temp_art_collection = createArtCollection (name, size, price);
				pointer3 = pointer;
				
				if (pointer3 == NULL){
					printf("Currently have no warehouses(lists\n");
				}
				
				 else {
               				while (pointer3 != NULL) {
                   				if (pointer3->class_size == size) {
                        				temp_ware_list = pointer3->warehouse_list_head;

                       					while(temp_ware_list != NULL){
                            					occupied = ((temp_ware_list->meta_info) & 2);
                            					if (occupied == 0) {
                                					temp_ware = temp_ware_list->warehouse;
                                					addToWarehouse(temp_ware, temp_art_collection);
                                					//temp_ware_list->warehouse = temp_ware;
                                					(temp_ware_list->meta_info) |= 2;
                                					art_coll_occupied = 1;
                                					break;
                            					}
                           					else {
                                					temp_ware_list = temp_ware_list->next_warehouse;
                            					}
                        				}
			
                        				break;
                    				}
                    				else {
                       					 pointer3 = pointer3->sf_next_warehouse;
                    				}

                			}				

                		if (art_coll_occupied == 0){
                    			pointer3 = pointer;
                    			while (pointer3 != NULL) {
                        			if (pointer3->class_size > size) {
                            				temp_ware_list = pointer3->warehouse_list_head;

                            				while(temp_ware_list != NULL){
                                				occupied = ((temp_ware_list->meta_info) & 2);
                                				if (occupied == 0) {
                                    					temp_ware = temp_ware_list->warehouse;
                                    					addToWarehouse(temp_ware, temp_art_collection);
                                    					temp_ware_list->warehouse = temp_ware;
                                    					(temp_ware_list->meta_info) |= 2;
                                    					art_coll_occupied = 1;
                                   					break;
                                				}
                                				else {
                                   					 temp_ware_list = temp_ware_list->next_warehouse;
                                				}
                            			
							}	
							
							break;		
                        			}			
                        			else {
                            				pointer3 = pointer3->sf_next_warehouse;
                        			}

                    			}	

                		}
			
				 if(art_coll_occupied == 0){
                   			printf("Sorry, no warehouse found to place art collection\n");
                   			exit (0);

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

                        char name[50];
       			int size;
        		int price;
        		char line[255];
        		int rep;
        		struct art_collection *temp_art_collection;
        		struct warehouse_list *temp_ware_list;
        		struct warehouse *temp_ware;
        		struct warehouse_sf_list* pointer3;
       	 		uint64_t occupied;
        		int art_collection_occ = 0;

			art_collection_occ = 0;
            		temp_art_collection = createArtCollection(name, size, price);
            		pointer3 = pointer;

			if (pointer3 == NULL) {
                		printf("Currently have no warehouses(lists\n");
            		}
			else {
               			 while (pointer3 != NULL) {
                   			 if (pointer3->class_size == size) {
                        			temp_ware_list = pointer3->warehouse_list_head;

                        		while(temp_ware_list != NULL){
                            			occupied = ((temp_ware_list->meta_info) & 2);
                           			
						if (occupied == 0) {
                                			temp_ware = temp_ware_list->warehouse;
                                			addToWarehouse(temp_ware, temp_art_collection);
                                			//temp_ware_list->warehouse = temp_ware;
                                			(temp_ware_list->meta_info) |= 2;
                                			art_collection_occ = 1;
                                			break;
                            			}		
                           	 else {
                                	temp_ware_list = temp_ware_list->next_warehouse;
                            	}	
                        }
                        break;
                    }
                    else {
                        pointer3 = pointer3->sf_next_warehouse;
                    }

                }

                if (art_collection_occ == 0){
                    pointer3 = pointer;
                    while (pointer3 != NULL) {
                        if (pointer3->class_size > size) {
                            temp_ware_list = pointer3->warehouse_list_head;

                            while(temp_ware_list != NULL){
                                occupied = ((temp_ware_list->meta_info) & 2);
                                if (occupied == 0) {
                                    temp_ware = temp_ware_list->warehouse;
                                    addToWarehouse(temp_ware, temp_art_collection);
                                    temp_ware_list->warehouse = temp_ware;
                                    (temp_ware_list->meta_info) |= 2;
                                    art_collection_occ = 1;
                                    break;
                                }
                                else {
                                    temp_ware_list = temp_ware_list->next_warehouse;
                                }
                            }
			    break;
                        }
                        else {
                            pointer3 = pointer3->sf_next_warehouse;
                        }

                    }

                }
		
	      if (art_collection_occ == 0){
			printf("No warehouse available\n");
			exit (0);
	      }

            }	
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
                struct warehouse_sf_list* pointer3;
		
		if(pointer3== NULL){
            		printf("Have no warehouses so cannot delete\n");
        	}
		
		while(pointer3->sf_next_warehouse!=NULL){
            		while(pointer3->warehouse_list_head!=NULL){
                		struct warehouse_list* temp_warehouse_list = pointer3->warehouse_list_head;
                		if (temp_warehouse_list->warehouse->art_collection== NULL){}
                		else if (strcmp((temp_warehouse_list->warehouse->art_collection->name),(name))==0){
					free(temp_warehouse_list->warehouse->art_collection->name);
					free(temp_warehouse_list->warehouse->art_collection);
                    			temp_warehouse_list->warehouse->art_collection = NULL;
                    			temp_warehouse_list->meta_info &= ~(1 << 1);
                		}
                		pointer3->warehouse_list_head = pointer3->warehouse_list_head->next_warehouse;
            		}
            		pointer3 = pointer3->sf_next_warehouse;
        	}
		
		if(pointer3->sf_next_warehouse == NULL){
            		while(pointer3->warehouse_list_head!=NULL){
                		struct warehouse_list* temp_warehouse_list = pointer3->warehouse_list_head;
                		if (temp_warehouse_list->warehouse->art_collection== NULL){}
                		else if (strcmp((temp_warehouse_list->warehouse->art_collection->name),(name))==0){
                    			free(temp_warehouse_list->warehouse->art_collection->name);
                    			free(temp_warehouse_list->warehouse->art_collection);
                    			temp_warehouse_list->warehouse->art_collection = NULL;
                    			temp_warehouse_list->meta_info &= ~(1 << 1);
                		}

                	pointer3->warehouse_list_head = pointer3->warehouse_list_head->next_warehouse;
            		}
        	}			
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
