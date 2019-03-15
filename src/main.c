#include <stdio.h>
#include "warehouse.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

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
                	warehouse_file = fopen(warehouse_filename, "rt");

                	int id;
                	int size;
                	char type[30];
                	struct* warehouse temp_warehouse;

                	while (!feof(warehouse_file)){
				fscanf(warehouse_file, "%d %d %s", id, size, filename);

				temp_warehouse = createWarehouse(id, size);
			}
                }
            

            else if (strcmp(operation, "art") == 0){
                //do stuff, check filename
                printf("Load Operation: %s\n", operation);
                printf("Warehouse Filename: %s\n", warehouse_filename);
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
