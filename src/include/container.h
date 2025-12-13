#ifndef CONTAINER_H
#define CONTAINER_H

typedef struct link_list {
	struct link_list* next;
	struct link_list* prev;
} link_list;

void link_list_insert(link_list* node, link_list* head);

// clang-format off
#define link_list_offset(type, member)   \
	((size_t)&(((type*)0)->member))

#define link_list_element(ptr, type, member) \
	((type*)((char*)ptr - (char*)link_list_offset(type, member)))
// clang-format on
#endif
