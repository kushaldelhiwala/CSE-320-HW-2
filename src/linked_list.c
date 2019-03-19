#include <stdlib.h>
#include <stdio.h>
#include "warehouse.h"
#include <string.h>

struct art_collection* createArtCollection(char* name, int size, int price){
	struct art_collection* newArtCollection = (struct art_collection*)malloc(sizeof(struct art_collection));
        newArtCollection -> name = (char*)malloc(50*sizeof(char));
	strcpy((newArtCollection -> name), name);
	newArtCollection -> size = size;
	newArtCollection -> price = price;

	return newArtCollection;
	 
}

struct warehouse* createWarehouse(int id, int size){
	struct warehouse* newWarehouse = (struct warehouse*)malloc(sizeof(struct warehouse));
	newWarehouse -> id = id;
	newWarehouse -> size = size;
	newWarehouse -> art_collection = NULL;
	// Set the art_collection to the passed in art_collection 

	return newWarehouse;
}

struct warehouse* addToWarehouse(struct warehouse* temp_warehouse, struct art_collection* temp_art_collection){
	temp_warehouse -> art_collection = temp_art_collection;
	return temp_warehouse;
}

struct warehouse_list* createWarehouseList (struct warehouse* warehouse, char* type, int size){
	struct warehouse_list* newWarehouseList = (struct warehouse_list*)malloc(sizeof(struct warehouse));
	newWarehouseList -> warehouse = warehouse;
	newWarehouseList -> next_warehouse = NULL;

	uint64_t temp = 0;
	temp = size;
	temp << 2;

	if (strcmp(type, "public") == 0){
		uint8_t temp_int = 0;
		temp |= temp_int;
	}

	else if (strcmp(type, "private") == 0){
		uint8_t temp_int = 1;
		temp |= temp_int;
	}


	newWarehouseList -> meta_info = temp;

	return newWarehouseList;

}

struct warehouse_sf_list* createWarehouseSfList(int class_size, struct warehouse_list* warehouse_list_head){
	struct warehouse_sf_list* newWarehouseSfList = (struct warehouse_sf_list*)malloc(sizeof(struct warehouse_sf_list));
	newWarehouseSfList -> class_size = class_size; 
	newWarehouseSfList -> warehouse_list_head = warehouse_list_head;
	newWarehouseSfList -> sf_next_warehouse = NULL;

	return newWarehouseSfList;
}

