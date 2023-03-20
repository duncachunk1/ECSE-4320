#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_KEYS = 5;

struct Node {
    int count;
    int keys[MAX_KEYS];
    Node* child[MAX_KEYS + 1];
    Node() : count(0), keys{0}, child{nullptr} {}
};

void insert(Node* root, int value) {
    Node* curr = root;
    //while current root isn't a leaf 
    while (curr->child[0] != nullptr) {
        int i = 0;
        //count to correct child node
        while (i < curr->count && curr->keys[i] < value) {
            i++;
        }
        //set current node to correct notde
        curr = curr->child[i];
    }
    //as long as we havent reached the limit for keys in a node add a new key
    if (curr->count < MAX_KEYS) {
        int i = curr->count - 1;
        while (i >= 0 && curr->keys[i] > value) {
            //shift current key in current node down one value
            curr->keys[i + 1] = curr->keys[i];
            //step back to past value
            i--;
        }
        //add current value to keys
        curr->keys[i + 1] = value;
        //add one to the number of keys currently in the node
        curr->count++;
    } else {
        //there is a problem here where instead of getting a parent and two child nodes we have 
        //if we are at the new limit for keys add a new node and split old node
        Node* new_node = new Node();
        //print original node keys
        cout << "Original node: " << endl;
        for (int i = 0; i < MAX_KEYS; i++){
            cout << curr->keys[i] << " ";
        }
        cout << endl;
        //find which keys should go into which new node
        int split_point = MAX_KEYS / 2;
        int new_value = curr->keys[split_point];
        //for each key above the splitting point add current keys and child pointers to new node
        // changed form split_point to split_point + 1
        for (int i = split_point; i < MAX_KEYS; i++) {
            new_node->keys[new_node->count] = curr->keys[i];
            new_node->child[new_node->count] = curr->child[i];
            new_node->count++;
        }
        //add the last 
        new_node->child[new_node->count] = curr->child[MAX_KEYS];
        curr->count = split_point;
        //set all keys and child pointers that were split to 0 or null
        for (int i = curr->count; i < MAX_KEYS; i++) {
            curr->keys[i] = 0;
            curr->child[i + 1] = nullptr;
        }
        //This line and below is where the problem is 
        insert(curr == root ? new Node() : curr->child[MAX_KEYS], new_value);
        //add new child from insertion
        new_node->child[0] = curr->child[split_point];
        //set current child node to new node
        curr->child[split_point] = new_node;
        //print curr node keys
        cout << "Left: " << endl;
        for (int i = 0; i < MAX_KEYS; i++){
            cout << curr->keys[i] << " ";
        }
        cout << endl;
        //print new node keys
                cout << "Right: " << endl;
        for (int i = 0; i < MAX_KEYS; i++){
            cout << new_node->keys[i] << " ";
        }
        cout << endl;
    }
}

Node* build_btree(string filename) {
    //create root and check if file is open
    Node* root = new Node();
    ifstream file(filename);
    if (file.is_open()) {
        int value;
        int c = 0;
        //while not at end of file keep inserting until we have reached the end of file
        while (file >> value) {
            cout << "Value being inserted: " << value << endl;
            insert(root, value);
            c++;
        }
        file.close();
        cout << "Number of values inserted: " << c << endl;
    } else {
        cout << "Unable to open file";
    }
    return root;
}
/*
void print_btree(Node* root) {
    if (root != nullptr) {
        for (int i = 0; i <= root->count; i++) {  // modify loop condition
            print_btree(root->child[i]);
            if (i < root->count) {
                cout << root->keys[i] << " ";
            }
        }
    }

}
*/
//this print function goes through and just prints every node then if that node has any child nodes it prints 
void print_btree(Node *root) {
    if (root != nullptr) {
        for (int i = 0; i < root->count; i++) {
            cout << root->keys[i] << " ";
        }
        cout << endl;

        if (root->child) {
            for (int i = 0; i <= root->count; i++) {
                print_btree(root->child[i]);
            }
        }
    }
}


void encode_btree(Node* root, ofstream& outfile) {
    if (root != nullptr) {
        // Write the number of keys in the current node
        outfile.write(reinterpret_cast<char*>(&root->count), sizeof(int));
        
        // Write the keys in the current node
        for (int i = 0; i < root->count; i++) {
            outfile.write(reinterpret_cast<char*>(&root->keys[i]), sizeof(int));
        }
        
        // Recursively encode the child nodes
        for (int i = 0; i <= root->count; i++) {
            encode_btree(root->child[i], outfile);
        }
    }
}



int main() {
    Node* root = build_btree("input.txt");
    
    // Open the output file for writing
    ofstream outfile("output.bin", ios::out | ios::binary);
    if (!outfile.is_open()) {
        cout << "Unable to open file for writing";
        return 1;
    }
    
    // Encode the B-tree and write it to the output file
    encode_btree(root, outfile);
    
    // Close the output file
    outfile.close();
    
    // Print the B-tree
    print_btree(root);
    
    return 0;
}
