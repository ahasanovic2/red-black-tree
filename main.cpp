#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Colors { BLACK, RED };

template <typename Tip>
struct Node {
    Tip value;
    Colors color;
    Node *left, *right, *parent;
};

template <typename Type>
class RBStablo {
    Node<Type> *root, *T_NIL;

    void RBInsertFixUp(Node<Type>* k) {
        Node<Type>* u ;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            }
            else {
                u = k->parent->parent->right;

                if (u->color) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root)
                break;
        }
        root->color = BLACK;
    }

    void inOrderHelper(Node<Type>* node) {
        if (node != T_NIL) {
            inOrderHelper(node->left);
            cout << node->value << " ";
            string helper = node->color ? "RED" : "BLACK";
            cout << "(" << helper << ") ";
            inOrderHelper(node->right);
        }
    }

    void RBTransplant(Node<Type> *u, Node<Type> *v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    Node<Type>* tree_minimum (Node<Type>* node) {
        while (node->left != T_NIL)
            node = node->left;
        return node;
    }

    void RBDeleteFixup(Node<Type>* x) {
        Node<Type> *s;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                }
                else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else {
                s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }
                if (s->right->color == BLACK && s->left->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                }
                else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void printHelper(Node<Type> *node, string indent, bool last) {
        if (node != T_NIL) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            }
            else {
                cout << "L----";
                indent += "   ";
            }
            string helper = node->color ? "RED" : "BLACK";
            cout << node->value << "(" << helper << ")" << endl;
            printHelper(node->left, indent, false);
            printHelper(node->right, indent, true);
        }
    }

    void leftRotate(Node<Type> *x) {
        auto y = x->right;
        x->right = y->left;
        if (y->left != T_NIL)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node<Type> *x) {
        auto y = x->left;
        x->left = y->right;
        if (y->right != T_NIL)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }
public:
    RBStablo() {
        T_NIL = new Node<Type>;
        T_NIL->color = BLACK;
        T_NIL->left = T_NIL->right = nullptr;
        root = T_NIL;
    }

    void RBInsert (Type key) {
        auto node = new Node<Type>;
        node->parent = nullptr;
        node->value = key;
        node->left = T_NIL;
        node->right = T_NIL;
        node->color = RED;

        Node<Type>* y = nullptr;
        Node<Type>* x = this->root;

        while (x != T_NIL) {
            y = x;
            if (node->value < x->value)
                x = x->left;
            else
                x = x->right;
        }
        node->parent = y;
        if (y == nullptr)
            root = node;
        else if (node->value < y->value)
            y->left = node;
        else
            y->right = node;

        if (node->parent == nullptr) {
            node->color = BLACK;
            return;
        }

        if (node->parent->parent == nullptr)
            return;

        RBInsertFixUp(node);
    }

    void inOrder() {
        inOrderHelper(root);
    }

    void RBDelete(Type key) {
        auto z = T_NIL;
        auto node = root;
        Node<Type> *x, *y;
        while (node != T_NIL) {
            if (node->value == key)
                z = node;

            if (node->value <= key)
                node = node->right;
            else
                node = node->left;
        }
        if (z == T_NIL) {
            cout << "Key not found in tree." << endl;
            return;
        }
        y = z;
        auto yOriginalColor = y->color;
        if (z->left == T_NIL) {
            x = z->right;
            RBTransplant(z, z->right);
        }
        else if (z->right == T_NIL) {
            x = z->left;
            RBTransplant(z, z->left);
        }
        else {
            y = tree_minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                RBTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            RBTransplant(z,y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (!yOriginalColor) {
            RBDeleteFixup(x);
        }
        cout << endl << "Node successfully deleted!" << endl;
    }

    void printTree() {
        if (root) {
            printHelper(root, "", true);
        }
    }

};

int main() {
    vector<string> meni;
    meni.emplace_back("Insertion of elements");
    meni.emplace_back("InOrder print");
    meni.emplace_back("Deletion of elements");
    meni.emplace_back("Tree print");
    meni.emplace_back("Default insertion");
    meni.emplace_back("Default deletion");
    meni.emplace_back("EXIT");

    RBStablo<int> stablo;

    for (;;) {
        for (int i = 0; i < meni.size(); ++i)
            cout << i+1 << ". " << meni[i] << endl;

        cout << "Choose an option: " << endl;
        int odabir;
        cin >> odabir;
        switch(odabir) {
            case 1:
                int vrijednost;
                cout << "You have chosen node insertion" << endl;
                for (;;) {
                    cout << "Insert value. (Insert anything except a number for insertion to end)";
                    cin.clear();
                    cin.ignore();
                    cin >> vrijednost;
                    if (cin.fail())
                        break;
                    stablo.RBInsert(vrijednost);
                    cout << "Value " << vrijednost << " is successfuly inserted in tree." << endl;
                }
                break;
            case 2:
                cout << endl << "You have chosen InOrder printout. "<< endl;
                cout << "Members are: ";
                stablo.inOrder();
                cout << endl << endl;
                break;
            case 3:
                cout << "You have chosen node deletion." << endl;
                for (;;) {
                    cout << "Insert value. (Insert anything except a number for insertion to end)";
                    cin.clear();
                    cin.ignore();
                    cin >> vrijednost;
                    if (cin.fail())
                        break;
                    stablo.RBDelete(vrijednost);
                }
                break;
            case 4:
                cout << endl << "You have chosen tree printout." << endl;
                stablo.printTree();
                cout << endl;
                break;
            case 5:
                cout << "You have chosen default insertion" << endl;
                for (auto x: vector<int>{6,11,10,2,9,7,5,13,22,27,36,12,31})
                    stablo.RBInsert(x);
                break;
            case 6:
                cout << "You have chosen default deletion." << endl;
                for (auto x: vector<int>{5,27,36,12,11})
                    stablo.RBDelete(x);
                break;
            case 7:
                cout << "Thank you for your time. Goodbye!" << endl;
                return 0;
            default:
                cout << "Pogresna komanda! Pokusajte opet." << endl;
        }
        cin.clear();
        cin.ignore();
    }
}