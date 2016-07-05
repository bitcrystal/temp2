#include <stdio.h>
#include <stdlib.h>

#include "vector_c.h"

void * vector_memory_alloc(vector *v, VECTOR_C_CAP_TYPE size, int alloc)
{
	VECTOR_C_CAP_TYPE reserved_size_total = ((VECTOR_C_CAP_TYPE)(size));
	if(alloc!=0)
	{
		reserved_size_total+=v->reserved_pointer_offset;
	} else {
		reserved_size_total+=v->reserved_size_total;
	}
	if(v->reserved_size<=reserved_size_total)
	{
		v->reserved_size_total=reserved_size_total;
		while(v->reserved_size<=v->reserved_size_total)
		{
			v->reserved_size+=((VECTOR_C_CAP_TYPE)(size));
		}
		vector_resize_reserved_c(v, v->reserved_size);
	}
	if(alloc!=0)
	{
		v->reserved_pointer=(void*)(((VECTOR_C_CAP_TYPE)v->reserved)+((VECTOR_C_CAP_TYPE)v->reserved_pointer_offset));
		v->reserved_pointer_offset+=((VECTOR_C_CAP_TYPE)(size));
	} else {
		return (void*)0;
	}
	return v->reserved_pointer;
}

void vector_init_c(vector_c *v)
{
    v->capacity = VECTOR_INIT_CAPACITY_C;
    v->total = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
	v->reserved = NULL;
	v->reserved_size=0;
	v->reserved_size_total=0;
	v->reserved_pointer = NULL;
	v->reserved_pointer_offset=0;
}

void vector_reserve_c_for_structs(vector_c * v,VECTOR_C_CAP_TYPE capacity)
{
	vector_resize_c(v,(capacity * sizeof(void*)));
	vector_memory_alloc(v, capacity, 0);
}

int vector_total_c(vector_c *v)
{
    return v->total;
}

static void vector_resize_c(vector_c *v, VECTOR_C_CAP_TYPE capacity)
{
    #ifdef DEBUG_ON
    printf("vector_resize: %d to %d\n", v->capacity, capacity);
    #endif

    void **items = realloc(v->items, sizeof(void *) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}

static void vector_resize_reserved_c(vector_c *v, VECTOR_C_CAP_TYPE reserved_size)
{
    #ifdef DEBUG_ON
    printf("vector_resize_reserved_c: %d to %d\n", v->reserved_size, reserved_size);
    #endif

    void * reserved = realloc(v->reserved, reserved_size);
    if (reserved) {
        v->reserved = reserved;
        v->reserved_size = reserved_size;
    }
}

void vector_add_c(vector_c *v, void *item)
{
    if (v->capacity == v->total)
        vector_resize_c(v, v->capacity * 2);
    v->items[v->total++] = item;
}

void vector_add_c_struct(vector_c *v, struct_saver * saver)
{	
	void * xx = (void*)VECTOR_MALLOC_EX(saver->size);
	if(xx==NULL) {
		saver->allgood=0;
		return;
	}
	
	memcpy(xx,saver->item,saver->size);
	void * yy = (void*)VECTOR_MALLOC_EX(v,sizeof(struct_saver));
	if(yy==NULL)
	{
			saver->allgood=0;
			return;
	}
	saver->item=xx;
	saver->allgood=1;
	memcpy(yy,saver,sizeof(struct_saver));
    vector_add_c(v,yy);
}

void vector_set_c(vector_c *v, VECTOR_C_CAP_TYPE index, void *item)
{
    if (index >= 0 && index < v->total)
        v->items[index] = item;
}

void vector_set_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index, struct_saver *item)
{
    if (index >= 0 && index < v->total)
    {
		struct_saver * olditem = (struct_saver*)v->items[index];
		void * xx = (void*)VECTOR_MALLOC_EX(v,item->size);
		if(xx==NULL)
		{
			item->allgood=0;
			return;
		}
		memcpy(xx,item->item,item->size);
		olditem->allgood=1;
		olditem->item=xx;
		olditem->size=item->size;
		return;
	}
}

void *vector_get_c(vector_c *v, VECTOR_C_CAP_TYPE index)
{
    if (index >= 0 && index < v->total)
        return v->items[index];
    return NULL;
}

struct_saver *vector_get_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index)
{
    return (struct_saver*)vector_get_c(v,index);
}

void vector_delete_c(vector_c *v, VECTOR_C_CAP_TYPE index)
{
    if (index < 0 || index >= v->total)
        return;

    v->items[index] = NULL;

    for (int i = 0; i < v->total - 1; i++) {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }

    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
        vector_resize_c(v, v->capacity / 2);
}

void vector_delete_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index)
{
	vector_free_c_struct(v,index);
	vector_delete_c(v,index);
}

void vector_free_c(vector_c *v)
{
    free(v->items);
	free(v->reserved);
	v->items=NULL;
	v->reserved=NULL;
	v->reserved_pointer=NULL;
	v->reserved_size=0;
	v->reserved_size_total=0;
	v->capacity=0;
	v->total=0;
}

void vector_free_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index)
{
	struct_saver * get = (struct_saver*)vector_get_c_struct(v,index);
	if(get==NULL)
	{
		return;
	}
	if(get->item!=NULL)
	{
		get->item=NULL;
		get->size=0;
	}
}

int vector_is_good_pointer(void * v)
{
	if(v==NULL)
	{
		return 0;
	} else {
		return 1;
	}
	return 0;
}