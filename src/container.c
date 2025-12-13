#include <container.h>

void link_list_insert(link_list* node, link_list* head)
{
	node->next = head->next;
	if (head->next)
		head->next->prev = node;
	head->next = node;
}
