#ifndef ORVIBO_LIST_H
#define ORVIBO_LIST_H

#include <stdbool.h>
#include <stddef.h>

struct node {
	void *data;
	struct node *next;
};

typedef bool (*NODE_DATA_MATCH) (const void *, const void *);
typedef void (*NODE_DATA_DESTROY) (void *);

struct node *
node_create(void *data);

void
node_destroy(struct node *node, NODE_DATA_DESTROY destroy);

void
node_destroy_all(struct node *node, NODE_DATA_DESTROY destroy);

void
node_prepend(struct node *node, struct node *before);

void
node_remove(struct node *node, const void *data, NODE_DATA_MATCH match);

struct node *
node_find(struct node *node, const void *data, NODE_DATA_MATCH match);

#endif
