#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_KEYS = 4;

struct Node {
    int count;
    int keys[MAX_KEYS];
    Node* child[MAX_KEYS + 1];
    Node() : count(0), keys{0}, child{nullptr} {}
};

void insert(Node* root, int value) {
    Node* curr = root;
    while (curr->child[0] != nullptr) {
        int i = 0;
        while (i < curr->count && curr->keys[i] < value) {
            i++;
        }
        curr = curr->child[i];
    }
    if (curr->count < MAX_KEYS) {
        int i = curr->count - 1;
        while (i >= 0 && curr->keys[i] > value) {
            curr->keys[i + 1] = curr->keys[i];
            i--;
        }
        curr->keys[i + 1] = value;
        curr->count++;
    } else {
        Node* new_node = new Node();
        int split_point = MAX_KEYS / 2;
        int new_value = curr->keys[split_point];
        for (int i = split_point + 1; i < MAX_KEYS; i++) {
            new_node->keys[new_node->count] = curr->keys[i];
            new_node->child[new_node->count] = curr->child[i];
            new_node->count++;
        }
        new_node->child[new_node->count] = curr->child[MAX_KEYS];
        curr->count = split_point;
        for (int i = curr->count; i < MAX_KEYS; i++) {
            curr->keys[i] = 0;
            curr->child[i + 1] = nullptr;
        }
        insert(curr == root ? new Node() : curr->child[MAX_KEYS], new_value);
        new_node->child[0] = curr->child[split_point];
        curr->child[split_point] = new_node;
    }
}

Node* build_btree(string filename) {
    Node* root = new Node();
    ifstream file(filename);
    if (file.is_open()) {
        int value;
        while (file >> value) {
            insert(root, value);
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
    return root;
}

void print_btree(Node* root) {
    if (root != nullptr) {
        for (int i = 0; i < root->count; i++) {
            print_btree(root->child[i]);
            cout << root->keys[i] << " ";
        }
        print_btree(root->child[root->count]);
    }
}

int main() {
    Node* root = build_btree("input.txt");
    print_btree(root);
    return 0;
}
