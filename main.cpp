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
    Cvor<Tip> *root = nullptr;

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

    void RBTransplant(Cvor<Tip> *u, Cvor<Tip> *v) {
        if (u != nullptr){
            if (u->roditelj == nullptr)
                root = v;
            else if (u == u->roditelj->lijevi)
                u->roditelj->lijevi = v;
            else if (u->roditelj->desni = v)
                v->roditelj = u->roditelj;

            if (v != nullptr)
                v->roditelj = u->roditelj;
        }
    }

    Cvor<Tip>* tree_minimum (Cvor<Tip>* cvor) {
        while (cvor->lijevi != nullptr)
            cvor = cvor->lijevi;
        return cvor;
    }

    bool pronalazakCvora(Cvor<Tip> *cvor, Tip v) {
        if (cvor == nullptr)
            return false;
        else if (v < cvor->vrijednost)
            pronalazakCvora(cvor->lijevi, v);
        else if (v > cvor->vrijednost)
            pronalazakCvora(cvor->desni, v);
        else
            brisanjeCvora(cvor);
        return true;
    }

    void brisanjeCvora(Cvor<Tip> *z) {
        Cvor<Tip>* x;
        auto y = z;
        Boje yOriginalBoja = y->boja;
        if (z->lijevi == nullptr) {
            x = z->desni;
            RBTransplant(z,z->desni);
        }
        else if (z->desni == nullptr) {
            x = z->lijevi;
            RBTransplant(z,z->lijevi);
        }
        else {
            y = tree_minimum(z->desni);
            yOriginalBoja = y->boja;
            x = y->desni;
            if (y->roditelj == z && x != nullptr)
                x->roditelj = y;
            else {
                RBTransplant(y,y->desni);
                y->desni = z->desni;
                if (y->desni != nullptr)
                    y->desni->roditelj = y;
            }
            RBTransplant(z,y);
            y->lijevi = z->lijevi;
            y->lijevi->roditelj = y;
            y->boja = z->boja;
        }

        if (yOriginalBoja == CRNO)
            RBDeleteFixup(x);

    }

    void RBDeleteFixup(Cvor<Tip>* &x) {
        while (x != nullptr && x != root && x->boja == CRNO) {
            if (x->roditelj != nullptr && x == x->roditelj->lijevi) {
                auto w = x->roditelj->desni;
                if (w->boja == CRVENO) {
                    w->boja = CRNO;
                    x->roditelj->boja = CRVENO;
                    leftRotate(x->roditelj);
                    w = x->roditelj->desni;
                }
                if (w->lijevi->boja == CRNO && w->desni->boja == CRNO) {
                    w->boja = CRVENO;
                    x = x->roditelj;
                }
                else {
                    if (w->desni->boja == CRNO) {
                        w->lijevi->boja = CRNO;
                        w->boja = CRVENO;
                        rightRotate(w);
                        w = x->roditelj->desni;
                    }
                    w->boja = x->roditelj->boja;
                    x->roditelj->boja = CRNO;
                    w->desni->boja = CRNO;
                    leftRotate(x->roditelj);
                    x = root;
                }
            }
            else {
                auto w = x->roditelj->lijevi;
                if (w->boja == CRVENO) {
                    w->boja = CRNO;
                    x->roditelj->boja = CRVENO;
                    rightRotate(x->roditelj);
                    w = x->roditelj->lijevi;
                }
                if (w->desni->boja == CRNO && w->lijevi->boja == CRNO) {
                    w->boja = CRVENO;
                    x = x->roditelj;
                }
                else {
                    if (w->lijevi->boja == CRNO) {
                        w->desni->boja = CRNO;
                        w->boja = CRVENO;
                        leftRotate(w);
                        w = x->roditelj->lijevi;
                    }
                    w->boja = x->roditelj->boja;
                    x->roditelj->boja = CRNO;
                    w->lijevi->boja = CRNO;
                    rightRotate(x->roditelj);
                    x = root;
                }
            }
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

    bool RBDelete(Tip vrijednost) {
        return pronalazakCvora(root,vrijednost);
    }
};

int main() {
    vector<string> meni;
    meni.push_back("Ubacivanje elemenata");
    meni.push_back("InOrder ispis");
    meni.push_back("Brisanje elementa");
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
                cout << "Odabrali ste brisanje cvora." << endl;
                for (;;) {
                    cout << "Unesite vrijednost (-9999 za izlaz): ";
                    cin.clear();
                    cin.ignore();
                    cin >> vrijednost;
                    if (vrijednost == -9999)
                        break;
                    if (stablo.RBDelete(vrijednost))
                        cout << endl << "Uspjesno izbrisan element. " << endl;
                    else
                        cout << endl << "Element nije izbrisan. " << endl;
                }
                break;
            case 4:
                cout << "Dosli ste do kraja programa. Dovidjenja!" << endl;
                return 0;
            default:
                cout << "Pogresna komanda! Pokusajte opet." << endl;
        }
        cin.clear();
        cin.ignore();
    }
}
