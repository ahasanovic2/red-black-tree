#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Boje { CRVENO, CRNO };

template <typename Tip>
struct Cvor {
    Tip vrijednost;
    Boje boja;
    Cvor *lijevi, *desni, *roditelj;

    // Konstruktor
    Cvor(Tip v) {
        vrijednost = v;
        boja = CRVENO;
        lijevi = desni = roditelj = nullptr;
    }

    //Destruktor
    ~Cvor() {
        delete lijevi;
        lijevi = nullptr;
        delete desni;
        desni = nullptr;
    }
};

template <typename Tip>
class RBStablo {
    Cvor<Tip> *root;

    void RBInsertFixUp(Cvor<Tip>* &korijen, Cvor<Tip>* &novi) {
        while (novi != korijen && novi->boja != CRNO && novi->roditelj->boja == CRVENO) {
            if (novi->roditelj == novi->roditelj->roditelj->lijevi) {
                auto y = novi->roditelj->roditelj->desni;
                if (y != nullptr && y->boja == CRVENO) {
                    novi->roditelj->boja = CRNO;
                    y->boja = CRNO;
                    novi->roditelj->roditelj->boja = CRVENO;
                    novi = novi->roditelj->roditelj;
                }
                else {
                    if (novi == novi->roditelj->desni) {
                        novi = novi->roditelj;
                        leftRotate(novi);
                    }
                    novi->roditelj->boja = CRNO;
                    novi->roditelj->roditelj->boja = CRVENO;
                    rightRotate(novi->roditelj->roditelj);
                }
            }
            else {
                auto y = novi->roditelj->roditelj->lijevi;
                if (y != nullptr && y->boja == CRVENO) {
                    novi->roditelj->boja = CRNO;
                    y->boja = CRNO;
                    novi->roditelj->roditelj->boja = CRVENO;
                    novi = novi->roditelj->roditelj;
                }
                else {
                    if (novi == novi->roditelj->lijevi) {
                        novi = novi->roditelj;
                        rightRotate(novi);
                    }
                    novi->roditelj->boja = CRNO;
                    novi->roditelj->roditelj->boja = CRVENO;
                    leftRotate(novi->roditelj->roditelj);
                }
            }
        }
        korijen->boja = CRNO;
    }

    void inOrderPomocna(Cvor<Tip>* cvor) {
        if (cvor != nullptr) {
            inOrderPomocna(cvor->lijevi);
            cout << cvor->vrijednost << " ";
            inOrderPomocna(cvor->desni);
        }
    }
public:
    void leftRotate(Cvor<Tip> *x) {
        auto y = x->desni;
        x->desni = y->lijevi;
        if (y->lijevi != nullptr)
            y->lijevi->roditelj = x;
        y->roditelj = x->roditelj;
        if (x->roditelj == nullptr)
            root = y;
        else if (x == x->roditelj->lijevi)
            x->roditelj->lijevi = y;
        else
            x->roditelj->desni = y;
        y->lijevi = x;
        x->roditelj = y;
    }

    void rightRotate(Cvor<Tip> *y) {
        auto x = y->lijevi;
        y->lijevi = x->desni;
        if (y->lijevi != nullptr)
            y->lijevi->roditelj = y;
        x->roditelj = y->roditelj;
        if (y->roditelj == nullptr)
            root = x;
        else if (y == y->roditelj->lijevi)
            y->roditelj->lijevi = x;
        else
            y->roditelj->desni = x;
        x->desni = y;
        y->roditelj = x;
    }

    void RBInsert (Tip kljuc) {
        auto *noviCvor = new Cvor<Tip>(kljuc);
        Cvor<Tip> *y = nullptr;
        Cvor<Tip> *x = root;

        while (x != nullptr) {
            y = x;
            if (noviCvor->vrijednost < x->vrijednost)
                x = x->lijevi;
            else
                x = x->desni;
        }

        noviCvor->roditelj = y;

        if (y == nullptr)
            root = noviCvor;
        else if (noviCvor->vrijednost < y->vrijednost)
            y->lijevi = noviCvor;
        else
            y->desni = noviCvor;

        noviCvor->lijevi = noviCvor->desni = nullptr;
        noviCvor->boja = CRVENO;

        RBInsertFixUp(root,noviCvor);
    }

    void inOrder() {
        inOrderPomocna(this->root);
    }
};

int main() {
    vector<string> meni;
    meni.push_back("Ubacivanje elemenata");
    meni.push_back("InOrder ispis");
    meni.push_back("EXIT");

    RBStablo<int> stablo;

    for (;;) {
        for (int i = 0; i < meni.size(); ++i) {
            cout << i+1 << ". " << meni[i] << endl;
        }
        cout << "Unesite jednu od opcija: " << endl;
        int odabir;
        cin >> odabir;
        switch(odabir) {
            case 1:
                int vrijednost;
                cout << "Odabrali ste kreiranje cvora." << endl;
                for (;;) {
                    cout << "Unesite vrijednost (-9999 za izlaz): ";
                    cin.clear();
                    cin.ignore();
                    cin >> vrijednost;
                    if (vrijednost == -9999)
                        break;
                    stablo.RBInsert(vrijednost);
                    cout << "Uspjesno uneseno " << vrijednost << " u stablo." << endl;
                }
                break;
            case 2:
                cout << endl << "Odabrali ste InOrder ispis stabla. "<< endl;
                cout << "Clanovi su: ";
                stablo.inOrder();
                cout << endl << endl;
                break;
            case 3:
                cout << "Dosli ste do kraja programa. Dovidjenja!" << endl;
                return 0;
            default:
                cout << "Pogresna komanda! Pokusajte opet." << endl;
        }
        cin.clear();
        cin.ignore();
    }
}
