#include "list.h"

#include <stdlib.h>

static bool
is_reference_equal(const void *x, const void *y);

struct node *
node_create(void *const data) {
	struct node *const node = malloc(sizeof(*node));
	if (node != NULL) {
		node->data = data;
		node->next = NULL;
	}
	return node;
}

void
node_destroy(struct node *node, const NODE_DATA_DESTROY destroy) {
	if (node != NULL && destroy != NULL) {
		destroy(node->data);
	}
	free(node);
}

void
node_destroy_all(struct node *node, const NODE_DATA_DESTROY destroy) {
	struct node *temp = NULL;
	while (node != NULL) {
		temp = node->next;
		node_destroy(node, destroy);
		node = temp;
	}
}

void
node_prepend(struct node *const node, struct node *const before) {
	if (before != NULL) {
		before->next = node;
	}
}

void
node_remove(struct node *node, const void *const data, NODE_DATA_MATCH match) {
	if (match == NULL) {
		match = is_reference_equal;
	}
	while (node != NULL && node->next != NULL) {
		if (match(node->next->data, data)) {
			node->next = node->next->next;
		}
		node = node->next;
	}
	node_destroy(node, NULL);
}

struct node *
node_find(struct node *node, const void *data, NODE_DATA_MATCH match) {
	if (match == NULL) {
		match = is_reference_equal;
	}
	while (node != NULL) {
		if (match(node->data, data)) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

static bool
is_reference_equal(const void *const x, const void *const y) {
	return x == y;
}
