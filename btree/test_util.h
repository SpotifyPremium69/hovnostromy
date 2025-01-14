#ifndef IAL_BTREE_TEST_UTIL_H
#define IAL_BTREE_TEST_UTIL_H

#include "btree.h"
#include <stdio.h>

#define TEST(NAME, DESCRIPTION)                                                \
  void NAME() {                                                                \
    printf("[%s] %s\n", #NAME, DESCRIPTION);                                   \
    bst_node_t *test_tree;                                                     \
    bst_items_t *test_items = bst_init_items();

#define ENDTEST                                                                \
  printf("\n");                                                                \
  bst_reset_items(test_items);                                                 \
  free(test_items);                                                            \
  bst_dispose(&test_tree);                                                     \
  }

typedef enum direction { left, right, none } direction_t;

void bst_print_subtree(bst_node_t *tree, char *prefix, direction_t from);
void bst_print_tree(bst_node_t *tree);
void bst_print_search_result(bst_node_content_t* content);
bst_node_content_t create_integer_content(int value);
void bst_insert_many(bst_node_t **tree, const char keys[], const int values[],
                     int count);
bst_items_t* bst_init_items();
void bst_print_items(bst_items_t *items);
void bst_reset_items (bst_items_t *items);
#endif
