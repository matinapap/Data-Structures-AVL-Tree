#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream> // Για χρήση της ostringstream
using namespace std;

// Δομή ενός κόμβου στο AVL δέντρο
struct Node {
    int key;        // Το κλειδί του κόμβου
    Node* left;     // Δείκτης προς το αριστερό παιδί
    Node* right;    // Δείκτης προς το δεξί παιδί
    int height;     // Ύψος του κόμβου
    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {} // Αρχικοποίηση κόμβου
};

// Κλάση AVLTree που περιέχει τις βασικές λειτουργίες του AVL δέντρου
class AVLTree {
public:
    AVLTree() : root(nullptr) {}

    // Εισαγωγή νέου κλειδιού στο AVL δέντρο
    void insert(int key) {
        root = insert(root, key);
    }

    // Αφαίρεση κλειδιού από το AVL δέντρο
    void remove(int key) {
        root = remove(root, key);
    }

    // Αναζήτηση κλειδιού στο AVL δέντρο
    bool search(int key) {
        return search(root, key) != nullptr;
    }

    // Συνένωση δύο AVL δέντρων με ένα ενδιάμεσο κλειδί
    void join(AVLTree& t1, int k, AVLTree& t2) {
        root = join(t1.root, k, t2.root);
    }

    // Διάσπαση του AVL δέντρου σε δύο δέντρα βάσει ενός κλειδιού
    void split(int k, AVLTree& t1, AVLTree& t2) {
        split(root, k, t1.root, t2.root);
    }

    // Εύρεση κλειδιών σε συγκεκριμένο εύρος [k1, k2]
    vector<int> find(int k1, int k2) {
        vector<int> result;
        find(root, k1, k2, result);
        return result;
    }

    // Εύρεση γειτονικού κλειδιού (προκατόχου ή διάδοχου) βάσει κατεύθυνσης
    int findNeighbor(int k, string direction) {
        Node* neighbor = nullptr;
        if (direction == "predecessor") {
            findPredecessor(root, k, neighbor);
        }
        else if (direction == "successor") {
            findSuccessor(root, k, neighbor);
        }
        return neighbor ? neighbor->key : -1; // Επιστροφή -1 αν δεν βρεθεί γείτονας
    }

private:
    Node* root; // Ρίζα του AVL δέντρου

    // Επιστροφή του ύψους ενός κόμβου
    int height(Node* n) {
        return n ? n->height : 0;
    }

    // Υπολογισμός του συντελεστή ισορροπίας ενός κόμβου
    int balanceFactor(Node* n) {
        return n ? height(n->left) - height(n->right) : 0;
    }

    // Δεξιά περιστροφή ενός υποδέντρου με ρίζα τον y
    Node* rotateRight(Node* y) {
        Node* x = y->left; // Αποθηκεύει το αριστερό παιδί του y στον x
        Node* T2 = x->right; // Αποθηκεύει το δεξί παιδί του x στον T2

        x->right = y; // Ορίζει το δεξί παιδί του x να είναι ο y
        y->left = T2; // Ορίζει το αριστερό παιδί του y να είναι ο T2

        // Ενημέρωση των υψών
        y->height = max(height(y->left), height(y->right)) + 1; // Ενημέρωση ύψους του y
        x->height = max(height(x->left), height(x->right)) + 1; // Ενημέρωση ύψους του x

        return x; // Επιστροφή της νέας ρίζας του υποδέντρου
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right; // Αποθηκεύει το δεξί παιδί του x στον y
        Node* T2 = y->left; // Αποθηκεύει το αριστερό παιδί του y στον T2

        y->left = x; // Ορίζει το αριστερό παιδί του y να είναι ο x
        x->right = T2; // Ορίζει το δεξί παιδί του x να είναι ο T2

        // Ενημέρωση των υψών
        x->height = max(height(x->left), height(x->right)) + 1; // Ενημέρωση ύψους του x
        y->height = max(height(y->left), height(y->right)) + 1; // Ενημέρωση ύψους του y

        return y; // Επιστροφή της νέας ρίζας του υποδέντρου
    }


    // Εισαγωγή νέου κλειδιού στο δέντρο με διατήρηση της ισορροπίας
    Node* insert(Node* node, int key) {
        // Αν ο κόμβος είναι κενός, δημιουργία νέου κόμβου
        if (!node) return new Node(key);

        // Καθορισμός της θέσης εισαγωγής
        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        else {
            return node; // Διπλά κλειδιά δεν επιτρέπονται
        }

        // Ενημέρωση του ύψους του κόμβου
        node->height = 1 + max(height(node->left), height(node->right));

        // Έλεγχος και διόρθωση του συντελεστή ισορροπίας
        int balance = balanceFactor(node);

        // LL περίπτωση
        if (balance > 1 && key < node->left->key) {
            return rotateRight(node);
        }

        // RR περίπτωση
        if (balance < -1 && key > node->right->key) {
            return rotateLeft(node);
        }

        // LR περίπτωση
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RL περίπτωση
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }


    // Εύρεση του κόμβου με την ελάχιστη τιμή κλειδιού σε ένα δέντρο
    Node* minValueNode(Node* node) {
        Node* current = node;
        // Διασχίζουμε προς τα αριστερά για να βρούμε το μικρότερο κλειδί
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Αφαίρεση ενός κλειδιού από το δέντρο με διατήρηση της ισορροπίας
    Node* remove(Node* root, int key) {
        if (!root) return root;

        // Εύρεση του κόμβου που θα αφαιρεθεί
        if (key < root->key) {
            root->left = remove(root->left, key);
        }
        else if (key > root->key) {
            root->right = remove(root->right, key);
        }
        else {
            // Ο κόμβος βρέθηκε
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    // Ο κόμβος δεν έχει παιδιά
                    temp = root;
                    root = nullptr;
                }
                else {
                    // Ένας από τους δύο κόμβους είναι null
                    *root = *temp;
                }
                delete temp;
            }
            else {
                // Ο κόμβος έχει δύο παιδιά
                Node* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = remove(root->right, temp->key);
            }
        }

        if (!root) return root;

        // Ενημέρωση του ύψους του κόμβου
        root->height = 1 + max(height(root->left), height(root->right));

        // Έλεγχος και διόρθωση του συντελεστή ισορροπίας
        int balance = balanceFactor(root);

        // LL περίπτωση
        if (balance > 1 && balanceFactor(root->left) >= 0) {
            return rotateRight(root);
        }

        // LR περίπτωση
        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // RR περίπτωση
        if (balance < -1 && balanceFactor(root->right) <= 0) {
            return rotateLeft(root);
        }

        // RL περίπτωση
        if (balance < -1 && balanceFactor(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    // Αναζήτηση ενός κλειδιού στο δέντρο
    Node* search(Node* root, int key) {
        // Αν ο κόμβος είναι κενός (δέντρο ή υποδέντρο είναι κενό) ή το κλειδί του κόμβου είναι το ζητούμενο κλειδί, επιστρέφουμε τον κόμβο
        if (!root || root->key == key) return root;

        // Αν το ζητούμενο κλειδί είναι μικρότερο από το κλειδί του κόμβου, ψάχνουμε στο αριστερό υποδέντρο
        if (key < root->key) return search(root->left, key);

        // Αν το ζητούμενο κλειδί είναι μεγαλύτερο από το κλειδί του κόμβου, ψάχνουμε στο δεξί υποδέντρο
        return search(root->right, key);
    }


    // Συνένωση δύο δέντρων με ένα ενδιάμεσο κλειδί
    Node* join(Node* T1, int k, Node* T2) {
        // Αν το πρώτο δέντρο είναι κενό, εισάγουμε το κλειδί στο δεύτερο δέντρο και το επιστρέφουμε
        if (!T1) return insert(T2, k);

        // Αν το δεύτερο δέντρο είναι κενό, εισάγουμε το κλειδί στο πρώτο δέντρο και το επιστρέφουμε
        if (!T2) return insert(T1, k);

        // Δημιουργία νέου κόμβου που θα είναι η ρίζα του νέου δέντρου
        Node* newRoot = new Node(k);

        // Το αριστερό υποδέντρο του νέου κόμβου είναι το T1
        newRoot->left = T1;

        // Το δεξί υποδέντρο του νέου κόμβου είναι το T2
        newRoot->right = T2;

        // Ενημέρωση του ύψους του νέου κόμβου
        newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));

        // Εξισορρόπηση του νέου δέντρου και επιστροφή της νέας ρίζας
        return balance(newRoot);
    }

    // Διάσπαση του δέντρου σε δύο υποδέντρα βάσει ενός κλειδιού
    void split(Node* T, int k, Node*& T1, Node*& T2) {
        // Αν το δέντρο είναι κενό, και τα δύο υποδέντρα θα είναι κενά
        if (!T) {
            T1 = T2 = nullptr;
            return;
        }

        // Αν το κλειδί k είναι μικρότερο από το κλειδί του τρέχοντος κόμβου
        if (k < T->key) {
            // Το T2 θα περιέχει το τρέχον δέντρο
            T2 = T;
            // Διάσπαση του αριστερού υποδέντρου
            split(T->left, k, T1, T2->left);
            // Ενημέρωση του ύψους του T2
            T2->height = 1 + max(height(T2->left), height(T2->right));
        }
        // Αν το κλειδί k είναι μεγαλύτερο ή ίσο από το κλειδί του τρέχοντος κόμβου
        else {
            // Το T1 θα περιέχει το τρέχον δέντρο
            T1 = T;
            // Διάσπαση του δεξιού υποδέντρου
            split(T->right, k, T1->right, T2);
            // Ενημέρωση του ύψους του T1
            T1->height = 1 + max(height(T1->left), height(T1->right));
        }

        // Εξισορρόπηση των υποδέντρων T1 και T2
        T1 = balance(T1);
        T2 = balance(T2);
    }


    // Εύρεση όλων των κλειδιών σε ένα εύρος [k1, k2]
    void find(Node* T, int k1, int k2, vector<int>& result) {
        // Αν το δέντρο είναι κενό, επιστροφή
        if (!T) return;

        // Αν το κλειδί του τρέχοντος κόμβου είναι μεγαλύτερο από k1,
        // τότε τα κλειδιά στο εύρος μπορεί να βρίσκονται και στο αριστερό υποδέντρο
        if (k1 < T->key) {
            find(T->left, k1, k2, result);
        }

        // Αν το κλειδί του τρέχοντος κόμβου βρίσκεται μέσα στο εύρος [k1, k2],
        // προσθέτουμε το κλειδί στη λίστα αποτελεσμάτων
        if (k1 <= T->key && k2 >= T->key) {
            result.push_back(T->key);
        }

        // Αν το κλειδί του τρέχοντος κόμβου είναι μικρότερο από k2,
        // τότε τα κλειδιά στο εύρος μπορεί να βρίσκονται και στο δεξί υποδέντρο
        if (k2 > T->key) {
            find(T->right, k1, k2, result);
        }
    }


    // Εύρεση προκατόχου ενός κλειδιού
    void findPredecessor(Node* T, int k, Node*& predecessor) {
        // Αν το δέντρο είναι κενό, επιστροφή
        if (!T) return;

        // Αν το κλειδί του τρέχοντος κόμβου είναι μικρότερο από το κλειδί k,
        // ο τρέχων κόμβος μπορεί να είναι ο πρόγονος του κλειδιού k
        if (T->key < k) {
            // Αποθηκεύουμε τον τρέχοντα κόμβο ως πιθανό πρόγονο
            predecessor = T;
            // Συνεχίζουμε την αναζήτηση στο δεξί υποδέντρο
            findPredecessor(T->right, k, predecessor);
        }
        else {
            // Αν το κλειδί του τρέχοντος κόμβου δεν είναι μικρότερο από το κλειδί k,
            // συνεχίζουμε την αναζήτηση στο αριστερό υποδέντρο
            findPredecessor(T->left, k, predecessor);
        }
    }


    // Εύρεση διαδόχου ενός κλειδιού σε ένα δυαδικό αναζητούμενο δέντρο (BST)
    void findSuccessor(Node* T, int k, Node*& successor) {
        if (!T) return; // Αν το δέντρο είναι κενό, επιστρέφουμε χωρίς να κάνουμε τίποτα

        if (T->key > k) {
            successor = T; // Αποθηκεύουμε τον τρέχοντα κόμβο ως πιθανό διάδοχο
            findSuccessor(T->left, k, successor); // Αναζητούμε τον διάδοχο στο αριστερό υποδέντρο
        }
        else {
            findSuccessor(T->right, k, successor); // Αναζητούμε τον διάδοχο στο δεξί υποδέντρο
        }
    }


    // Ισορροπία ενός υποδέντρου βάσει του συντελεστή ισορροπίας
    Node* balance(Node* node) {
        if (!node) return node; // Αν ο κόμβος είναι κενός, επιστρέφουμε τον κόμβο

        // Ενημέρωση του ύψους του κόμβου
        node->height = 1 + max(height(node->left), height(node->right));

        // Υπολογισμός του συντελεστή ισορροπίας για τον κόμβο
        int balance = balanceFactor(node);

        // LL περίπτωση (ανισορροπία αριστερά-αριστερά)
        if (balance > 1 && balanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }

        // LR περίπτωση (ανισορροπία αριστερά-δεξιά)
        if (balance > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RR περίπτωση (ανισορροπία δεξιά-δεξιά)
        if (balance < -1 && balanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }

        // RL περίπτωση (ανισορροπία δεξιά-αριστερά)
        if (balance < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        // Επιστροφή του κόμβου αν δεν απαιτείται περιστροφή
        return node;
    }

};


int main() {
    AVLTree tree;

    // Εισαγωγή στοιχείων στο AVL δέντρο
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    // Αναζήτηση στοιχείων στο AVL δέντρο
    cout << "Search 100: " << (tree.search(100) ? "Found" : "Not Found") << endl;
    cout << "Search 50: " << (tree.search(50) ? "Found" : "Not Found") << endl;

    // Αφαίρεση στοιχείου από το AVL δέντρο
    tree.remove(20);
    cout << "Search 20 after removal: " << (tree.search(20) ? "Found" : "Not Found") << endl;

    // Δημιουργία δύο AVL δέντρων για τη λειτουργία join
    AVLTree tree1;
    tree1.insert(1);
    tree1.insert(2);
    tree1.insert(3);

    AVLTree tree2;
    tree2.insert(5);
    tree2.insert(6);
    tree2.insert(7);

    // Λειτουργία join
    AVLTree joinedTree;
    joinedTree.join(tree1, 4, tree2);
    cout << "Joined Tree contains 4: " << (joinedTree.search(4) ? "Found" : "Not Found") << endl;

    // Λειτουργία split
    AVLTree splitTree1, splitTree2;
    joinedTree.split(4, splitTree1, splitTree2);
    cout << "Split Tree1 contains 3: " << (splitTree1.search(3) ? "Found" : "Not Found") << endl;
    cout << "Split Tree2 contains 5: " << (splitTree2.search(5) ? "Found" : "Not Found") << endl;

    // Λειτουργία find
    vector<int> foundElements = joinedTree.find(3, 6);
    cout << "Elements in range [3, 6]: ";
    for (int elem : foundElements) {
        cout << elem << " ";
    }
    cout << endl;

    // Λειτουργία find_neighbor
    int predecessor = joinedTree.findNeighbor(4, "predecessor");
    int successor = joinedTree.findNeighbor(4, "successor");

    // Χρήση ostringstream για μετατροπή αριθμών σε συμβολοσειρές
    ostringstream oss;
    oss << predecessor;
    string predecessorStr = predecessor != -1 ? oss.str() : "None";
    oss.str(""); // Καθαρισμός του buffer
    oss.clear(); // Επαναφορά της κατάστασης του stream
    oss << successor;
    string successorStr = successor != -1 ? oss.str() : "None";

    cout << "Predecessor of 4: " << predecessorStr << endl;
    cout << "Successor of 4: " << successorStr << endl;

    return 0;
}
