#include "phonebook.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Contact *root = NULL;

static int insert_count = 0;

static int count_nodes(Contact *node) {
  if (node == NULL) {
    return 0;
  }
  return 1 + count_nodes(node->left) + count_nodes(node->right);
}

static void fill_array(Contact *node, Contact **arr, int *idx) {
  if (node == NULL) {
    return;
  }
  fill_array(node->left, arr, idx);
  arr[*idx] = node;
  (*idx)++;
  fill_array(node->right, arr, idx);
}

static Contact *build_balanced(Contact **arr, int lo, int hi) {
  if (lo > hi) {
    return NULL;
  }
  int mid = (lo + hi) / 2;
  Contact *node = arr[mid];
  node->left = build_balanced(arr, lo, mid - 1);
  node->right = build_balanced(arr, mid + 1, hi);
  return node;
}

void rebalance_tree() {
  int n = count_nodes(root);
  if (n == 0) {
    return;
  }
  Contact **arr = malloc(n * sizeof(Contact *));
  if (arr == NULL) {
    return;
  }
  int idx = 0;
  fill_array(root, arr, &idx);
  root = build_balanced(arr, 0, n - 1);
  free(arr);
}

void add_contact() {
  Contact *node = malloc(sizeof(Contact));
  if (node == NULL) {
    printf("Memory allocation failed.");
    return;
  }
  printf("Name: ");
  read_input(node->name, sizeof(node->name));
  printf("Phone: ");
  read_input(node->phone, sizeof(node->phone));
  node->left = NULL;
  node->right = NULL;

  if (root == NULL) {
    root = node;
  } else {
    Contact *cur = root;
    while (1) {
      int cmp = strcmp(node->name, cur->name);
      if (cmp < 0) {
        if (cur->left == NULL) {
          cur->left = node;
          break;
        }
        cur = cur->left;
      } else {
        if (cur->right == NULL) {
          cur->right = node;
          break;
        }
        cur = cur->right;
      }
    }
  }

  insert_count++;
  if (insert_count % 10 == 0) {
    rebalance_tree();
  }
}

static void in_order(Contact *node) {
  if (node == NULL) {
    return;
  }
  in_order(node->left);
  printf("%s — %s\n", node->name, node->phone);
  in_order(node->right);
}

void show_contacts() {
  if (root == NULL) {
    printf("Phonebook is empty");
    return;
  }
  in_order(root);
}

static void free_tree(Contact *node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}

static void delete_node(Contact **link, Contact *target) {
  if (*link == NULL)
    return;
  int cmp = strcmp(target->name, (*link)->name);
  if (cmp < 0) {
    delete_node(&(*link)->left, target);
  } else if (cmp > 0 || *link != target) {
    delete_node(&(*link)->right, target);
  } else {
    Contact *node = *link;
    if (node->left == NULL) {
      *link = node->right;
      free(node);
    } else if (node->right == NULL) {
      *link = node->left;
      free(node);
    } else {
      Contact *succ = node->right;
      while (succ->left != NULL)
        succ = succ->left;
      strcpy(node->name, succ->name);
      strcpy(node->phone, succ->phone);
      delete_node(&node->right, succ);
    }
  }
}

void edit_contact() {
  int n = count_nodes(root);
  if (n == 0) {
    printf("Phonebook is empty.\n");
    return;
  }
  Contact **arr = malloc(n * sizeof(Contact *));
  int idx = 0;
  fill_array(root, arr, &idx);
  for (int i = 0; i < n; i++) {
    printf("[%d] %s — %s\n", i + 1, arr[i]->name, arr[i]->phone);
  }
  printf("Enter number to edit: ");
  char buf[8];
  fgets(buf, sizeof(buf), stdin);
  int sel = atoi(buf);
  if (sel < 1 || sel > n) {
    printf("Invalid number.\n");
    free(arr);
    return;
  }
  Contact *target = arr[sel - 1];
  free(arr);

  char new_name[64];
  printf("New name (old: %s): ", target->name);
  read_input(new_name, sizeof(new_name));

  char new_phone[32];
  printf("New phone (old: %s): ", target->phone);
  read_input(new_phone, sizeof(new_phone));

  if (strcmp(target->name, new_name) == 0) {
    strcpy(target->phone, new_phone);
    printf("Updated.\n");
    return;
  }

  delete_node(&root, target);

  Contact *node = malloc(sizeof(Contact));
  strcpy(node->name, new_name);
  strcpy(node->phone, new_phone);
  node->left = NULL;
  node->right = NULL;
  if (root == NULL) {
    root = node;
  } else {
    Contact *cur = root;
    while (1) {
      int cmp = strcmp(node->name, cur->name);
      if (cmp < 0) {
        if (cur->left == NULL) {
          cur->left = node;
          break;
        }
        cur = cur->left;
      } else {
        if (cur->right == NULL) {
          cur->right = node;
          break;
        }
        cur = cur->right;
      }
    }
  }
  printf("Updated.\n");
}

void delete_contact() {
  int n = count_nodes(root);
  if (n == 0) {
    printf("Phonebook is empty.\n");
    return;
  }
  Contact **arr = malloc(n * sizeof(Contact *));
  int idx = 0;
  fill_array(root, arr, &idx);
  for (int i = 0; i < n; i++) {
    printf("[%d] %s — %s\n", i + 1, arr[i]->name, arr[i]->phone);
  }
  printf("Enter number to delete: ");
  char buf[8];
  fgets(buf, sizeof(buf), stdin);
  int sel = atoi(buf);
  if (sel < 1 || sel > n) {
    printf("Invalid number.\n");
    free(arr);
    return;
  }
  delete_node(&root, arr[sel - 1]);
  free(arr);
  printf("Deleted.\n");
}

void clear_phonebook() {
  free_tree(root);
  root = NULL;
}
