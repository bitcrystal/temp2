#ifndef VECTOR_C_H
#define VECTOR_C_H

typedef ULONG64__ unsigned long long int
typedef ULONG64__ VECTOR_C_CAP_TYPE

#define VECTOR_INIT_C_CAPACITY 4

#define VECTOR_INIT_C(vec) vector_c vec; vector_c_init(&vec)
#define VECTOR_INIT_C_EX(vec) vector_c_init(vec)
#define VECTOR_ADD_C(vec, item) vector_add_c(&vec, (void *) item)
#define VECTOR_ADD_C_EX(vec, item) vector_add_c(vec, (void *) item)
#define VECTOR_ADD_C_STRUCT(vec, ab, rx) struct_saver rx; rx.item=((void*)(&(ab))); rx.size=((VECTOR_C_CAP_TYPE)(sizeof((ab)))); vector_add_c_struct(((vector_c*)&(vec)), ((struct_saver *) &rx))
#define VECTOR_ADD_C_STRUCT_EX(vec, ab, rx) struct_saver rx; rx.item=((void*)((ab))); rx.size=((VECTOR_C_CAP_TYPE)(sizeof(*(ab)))); vector_add_c_struct(((vector_c*)(vec)), ((struct_saver *) &rx))
#define VECTOR_SET_C(vec, id, item) vector_set_c(&vec, id, (void *) item)
#define VECTOR_SET_C_EX(vec, id, item) vector_set_c(vec, id, (void *) item)
#define VECTOR_SET_C_STRUCT(vec, id, ab, rx) struct_saver rx; rx.item=((void*)(&(ab))); rx.size=((VECTOR_C_CAP_TYPE)(sizeof((ab)))); vector_set_c_struct(((vector_c*)&(vec)), (((VECTOR_C_CAP_TYPE))(id)), ((struct_saver *) &rx))
#define VECTOR_SET_C_STRUCT_EX(vec, id, ab, rx) struct_saver rx; rx.item=((void*)((ab))); rx.size=((VECTOR_C_CAP_TYPE)(sizeof(*(ab)))); vector_set_c_struct(((vector_c*)(vec)), (((VECTOR_C_CAP_TYPE))(id)), ((struct_saver *) &rx))
#define VECTOR_GET_C(vec, type, id) (type) vector_get_c(&vec, id)
#define VECTOR_GET_C_EX(vec, type, id) (type) vector_get_c(vec, id)
#define VECTOR_GET_C_STRUCT(vec, id) (struct_saver*)(vector_get_c_struct(((vector_c*)&(vec)), ((VECTOR_C_CAP_TYPE)(id))))
#define VECTOR_GET_C_STRUCT_EX(vec, id) (struct_saver*)(vector_get_c_struct(((vector_c*)(vec)), ((VECTOR_C_CAP_TYPE)(id))))
#define VECTOR_DELETE_C(vec, id) vector_delete_c(&vec, id)
#define VECTOR_DELETE_C_EX(vec, id) vector_delete_c(vec, id)
#define VECTOR_DELETE_C_STRUCT(vec, id) vector_delete_c_struct(((vector_c*)&(vec)), (((VECTOR_C_CAP_TYPE))(id)))
#define VECTOR_DELETE_C_STRUCT_EX(vec, id) vector_delete_c_struct(((vector_c*)(vec)), (((VECTOR_C_CAP_TYPE))(id)))
#define VECTOR_TOTAL_C(vec) vector_total_c(&vec)
#define VECTOR_TOTAL_C_EX(vec) vector_total_c(vec)
#define VECTOR_FREE_C(vec) vector_free_c(&vec)
#define VECTOR_FREE_C_EX(vec) vector_free_c(vec)
#define VECTOR_FREE_C_STRUCT(vec,id) vector_free_c_struct(((vector_c*)&(vec)), (((VECTOR_C_CAP_TYPE))(id)))
#define VECTOR_FREE_C_STRUCT_EX(vec,id) vector_free_c_struct(((vector_c*)(vec)), (((VECTOR_C_CAP_TYPE))(id)))
#define VECTOR_MALLOC(vec,size) vector_memory_alloc(((vector_c*)&(vec)),((VECTOR_C_CAP_TYPE)size),1)
#define VECTOR_MALLOC_EX(vec,size) vector_memory_alloc(((vector_c*)(vec)),((VECTOR_C_CAP_TYPE)size),1)
#define VECTOR_IS_GOOD_POINTER_C(vec) vector_is_good_pointer_c(((vector_c*)&(vec)))
#define VECTOR_IS_GOOD_POINTER_C_EX(vec) vector_is_good_pointer_c(((vector_c*)(vec)))
#define VECTOR_RESERVE_CFS(vec,capacity) vector_reserve_c_for_structs(((vector_c *)&(vec)),((VECTOR_C_CAP_TYPE)capacity))
#define VECTOR_RESERVE_CFS_EX(vec,capacity) vector_reserve_c_for_structs(((vector_c *)(vec)),((VECTOR_C_CAP_TYPE)capacity))
typedef struct _vector_c {
    void **items;
    VECTOR_C_CAP_TYPE capacity;
    VECTOR_C_CAP_TYPE total;
	void * reserved;
	VECTOR_C_CAP_TYPE reserved_size;
	VECTOR_C_CAP_TYPE reserved_size_total;
	void * reserved_pointer;
	VECTOR_C_CAP_TYPE reserved_pointer_offset;
} vector_c;

typedef struct struct_saver_
{
	void * item;
	VECTOR_C_CAP_TYPE size;
	int allgood;
} struct_saver;

void vector_init_c(vector_c *);
int vector_total_c(vector_c *);
static void vector_resize_c(vector_c *, VECTOR_C_CAP_TYPE);
void vector_add_c(vector_c *, void *);
void vector_add_c_struct(vector_c *v, struct_saver * saver);
void vector_set_c(vector_c *, VECTOR_C_CAP_TYPE, void *);
void vector_set_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index, struct_saver *item);
void *vector_get_c(vector_c *, VECTOR_C_CAP_TYPE);
struct_saver *vector_get_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index);
void vector_delete_c(vector_c *, VECTOR_C_CAP_TYPE);
void vector_delete_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index);
void vector_free_c(vector_c *);
void vector_free_c_struct(vector_c *v, VECTOR_C_CAP_TYPE index);
int vector_is_good_pointer_c(void * v);
void * vector_memory_alloc(vector * v, VECTOR_C_CAP_TYPE size, int allgood);
void vector_reserve_c_for_structs(vector_c * v,VECTOR_C_CAP_TYPE capacity);
#endif