#ifndef ORVIBO_LIST_H
#define ORVIBO_LIST_H

#include <stdbool.h>
#include <stddef.h>

struct node {
    void *data;
    struct node *next;
};

typedef bool (*MATCH) (const void *, const void *);
typedef void (*DESTROY) (void *);

struct node *
node_create(void *data);

void
node_destroy(struct node *node, DESTROY destroy);

void
node_destroy_all(struct node *node, DESTROY destroy);

void
node_prepend(struct node *node, struct node *before);

void
node_remove(struct node *node, const void *data, MATCH match);

struct node *
node_find(struct node *node, const void *data, MATCH match);

#endif
