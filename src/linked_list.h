#ifndef LINKED_LIST_H
#define LINKED_LIST_H


struct art_collection* createArtCollection(char* name, int size, int price);
struct warehouse* createWarehouse(int id, int size);
struct warehouse* addToWarehouse(struct warehouse* temp_warehouse, struct art_collection* temp_art_collection);
struct warehouse_list* createWarehouseList (struct warehouse* warehouse, char* type, int size);
struct warehouse_sf_list* createWarehouseSfList(int class_size, struct warehouse_list* warehouse_list_head);

#endif /* LINKED_LIST_H */
