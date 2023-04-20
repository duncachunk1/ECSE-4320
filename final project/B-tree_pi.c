// Searching a key on a B-tree in C

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#define MAX 32
#define MIN 2

struct BTreeNode {
  int val[MAX + 1], count;
  struct BTreeNode *link[MAX + 1];
};

struct BTreeNode *root;

// Create a node
struct BTreeNode *createNode(int val, struct BTreeNode *child) {
  struct BTreeNode *newNode;
  newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
  newNode->val[1] = val;
  newNode->count = 1;
  newNode->link[0] = root;
  newNode->link[1] = child;
  return newNode;
}

// Insert node
void insertNode(int val, int pos, struct BTreeNode *node,
        struct BTreeNode *child) {
  int j = node->count;
  while (j > pos) {
    node->val[j + 1] = node->val[j];
    node->link[j + 1] = node->link[j];
    j--;
  }
  node->val[j + 1] = val;
  node->link[j + 1] = child;
  node->count++;
}

// Split node
void splitNode(int val, int *pval, int pos, struct BTreeNode *node,
         struct BTreeNode *child, struct BTreeNode **newNode) {
  int median, j;

  if (pos > MIN)
    median = MIN + 1;
  else
    median = MIN;

  *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
  j = median + 1;
  while (j <= MAX) {
    (*newNode)->val[j - median] = node->val[j];
    (*newNode)->link[j - median] = node->link[j];
    j++;
  }
  node->count = median;
  (*newNode)->count = MAX - median;

  if (pos <= MIN) {
    insertNode(val, pos, node, child);
  } else {
    insertNode(val, pos - median, *newNode, child);
  }
  *pval = node->val[node->count];
  (*newNode)->link[0] = node->link[node->count];
  node->count--;
}

// Set the value
int setValue(int val, int *pval,
           struct BTreeNode *node, struct BTreeNode **child) {
  int pos;
  if (!node) {
    *pval = val;
    *child = NULL;
    return 1;
  }

  if (val < node->val[1]) {
    pos = 0;
  } else {
    for (pos = node->count;
       (val < node->val[pos] && pos > 1); pos--)
      ;
    if (val == node->val[pos]) {
      //printf("Duplicates are not permitted\n");
      return 0;
    }
  }
  if (setValue(val, pval, node->link[pos], child)) {
    if (node->count < MAX) {
      insertNode(*pval, pos, node, *child);
    } else {
      splitNode(*pval, pval, pos, node, *child, child);
      return 1;
    }
  }
  return 0;
}

// Insert the value
void insert(int val) {
  int flag, i;
  struct BTreeNode *child;

  flag = setValue(val, &i, root, &child);
  if (flag)
    root = createNode(i, child);
}

// Search node
void search(int val, int *pos, struct BTreeNode *myNode) {
  if (!myNode) {
    return;
  }

  if (val < myNode->val[1]) {
    *pos = 0;
  } else {
    for (*pos = myNode->count;
       (val < myNode->val[*pos] && *pos > 1); (*pos)--)
      ;
    if (val == myNode->val[*pos]) {
      printf("%d is found", val);
      return;
    }
  }
  search(val, pos, myNode->link[*pos]);

  return;
}

// Traverse then nodes
void traversal(struct BTreeNode *myNode) {
  int i;
  if (myNode) {
    for (i = 0; i < myNode->count; i++) {
      traversal(myNode->link[i]);
      printf("%d ", myNode->val[i + 1]);
    }
    traversal(myNode->link[i]);
  }
}

int main(int argc, char **argv) {
  int val, ch;
  struct stat file_stats;
  char *filename = argv[1];
  int searchItem = atoi(argv[2]);
  clock_t start_time, end_time;
  double elapsed_time;

//*

  //Verifies command line arguments
  if (argc != 3) {
      fprintf(stderr, "Usage: %s input_file \n", argv[0]);
      fprintf(stderr, "ARGC: %d\n", argc);
      fprintf(stderr, "%s, %s\n", argv[0], argv[1]);
      return 1;
  }

  //checks if the input file has items in it
  FILE *infile = fopen(argv[1], "r");
  if (infile == NULL) {
      perror("Error opening input file");
      return 1;
  }
  if (getc(infile) == EOF) {
      fprintf(stderr, "Error: Input file is empty\n");
      return 1;
  }

  //File stats
  if (stat(filename, &file_stats) == 0) {
      // Print the file size in bytes
      printf("File size: %ld bytes\n", file_stats.st_size);
  }

  //looping insertions
  int number;
  start_time = clock();
  while (fscanf(infile, "%d", &number) != EOF) {
      insert(number);
  }
  end_time = clock();
  elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("Elapsed time: %f seconds\n", elapsed_time);

  printf("Throughput: %f bps", file_stats.st_size / elapsed_time);

  fclose(infile);

  

  //traversal(root);

  printf("\n\n");

  start_time = clock();
  search(searchItem, &ch, root);
  end_time = clock();
  elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("\nSearch time: %f seconds\n", elapsed_time);

  return 0;

}