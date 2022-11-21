#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Boje { CRNO, CRVENO };

template <typename Tip>
struct Cvor {
    Tip vrijednost;
    Boje boja;
    Cvor *lijevi, *desni, *roditelj;
};

template <typename Tip>
class RBStablo {
    Cvor<Tip> *korijen, *T_NIL;

    void RBInsertFixUp(Cvor<Tip>* k) {
        Cvor<Tip>* u ;
        while (k->roditelj->boja == CRVENO) {
            if (k->roditelj == k->roditelj->roditelj->desni) {
                u = k->roditelj->roditelj->lijevi;
                if (u->boja == CRVENO) {
                    u->boja = CRNO;
                    k->roditelj->boja = CRNO;
                    k->roditelj->roditelj->boja = CRVENO;
                    k = k->roditelj->roditelj;
                }
                else {
                    if (k == k->roditelj->lijevi) {
                        k = k->roditelj;
                        rightRotate(k);
                    }
                    k->roditelj->boja = CRNO;
                    k->roditelj->roditelj->boja = CRVENO;
                    leftRotate(k->roditelj->roditelj);
                }
            }
            else {
                u = k->roditelj->roditelj->desni;

                if (u->boja) {
                    u->boja = CRNO;
                    k->roditelj->boja = CRNO;
                    k->roditelj->roditelj->boja = CRVENO;
                    k = k->roditelj->roditelj;
                }
                else {
                    if (k == k->roditelj->desni) {
                        k = k->roditelj;
                        leftRotate(k);
                    }
                    k->roditelj->boja = CRNO;
                    k->roditelj->roditelj->boja = CRVENO;
                    rightRotate(k->roditelj->roditelj);
                }
            }
            if (k == korijen)
                break;
        }
        korijen->boja = CRNO;
    }

    void inOrderPomocna(Cvor<Tip>* cvor) {
        if (cvor != T_NIL) {
            inOrderPomocna(cvor->lijevi);
            cout << cvor->vrijednost << " ";
            string pomocni = cvor->boja ? "CRVENO" : "CRNO";
            cout << "(" << pomocni << ") ";
            inOrderPomocna(cvor->desni);
        }
    }

    void RBTransplant(Cvor<Tip> *u, Cvor<Tip> *v) {
        if (u->roditelj == nullptr)
            korijen = v;
        else if (u == u->roditelj->lijevi)
            u->roditelj->lijevi = v;
        else
            u->roditelj->desni = v;
        v->roditelj = u->roditelj;
    }

    Cvor<Tip>* tree_minimum (Cvor<Tip>* cvor) {
        while (cvor->lijevi != T_NIL)
            cvor = cvor->lijevi;
        return cvor;
    }

    void RBDeleteFixup(Cvor<Tip>* x) {
        Cvor<Tip> *s;
        while (x != korijen && x->boja == CRNO) {
            if (x == x->roditelj->lijevi) {
                s = x->roditelj->desni;
                if (s->boja == CRVENO) {
                    s->boja = CRNO;
                    x->roditelj->boja = CRVENO;
                    leftRotate(x->roditelj);
                    s = x->roditelj->desni;
                }
                if (s->lijevi->boja == CRNO && s->desni->boja == CRNO) {
                    s->boja = CRVENO;
                    x = x->roditelj;
                }
                else {
                    if (s->desni->boja == CRNO) {
                        s->lijevi->boja = CRNO;
                        s->boja = CRVENO;
                        rightRotate(s);
                        s = x->roditelj->desni;
                    }
                    s->boja = x->roditelj->boja;
                    x->roditelj->boja = CRNO;
                    s->desni->boja = CRNO;
                    leftRotate(x->roditelj);
                    x = korijen;
                }
            }
            else {
                s = x->roditelj->lijevi;
                if (s->boja == CRVENO) {
                    s->boja = CRNO;
                    x->roditelj->boja = CRVENO;
                    rightRotate(x->roditelj);
                    s = x->roditelj->lijevi;
                }
                if (s->desni->boja == CRNO && s->lijevi->boja == CRNO) {
                    s->boja = CRVENO;
                    x = x->roditelj;
                }
                else {
                    if (s->lijevi->boja == CRNO) {
                        s->desni->boja = CRNO;
                        s->boja = CRVENO;
                        leftRotate(s);
                        s = x->roditelj->lijevi;
                    }
                    s->boja = x->roditelj->boja;
                    x->roditelj->boja = CRNO;
                    s->lijevi->boja = CRNO;
                    rightRotate(x->roditelj);
                    x = korijen;
                }
            }
        }
        x->boja = CRNO;
    }

    void printHelper(Cvor<Tip> *cvor, string indent, bool last) {
        if (cvor != T_NIL) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            }
            else {
                cout << "L----";
                indent += "   ";
            }
            string pomocni = cvor->boja ? "CRVENO" : "CRNO";
            cout << cvor->vrijednost << "(" << pomocni << ")" << endl;
            printHelper(cvor->lijevi, indent, false);
            printHelper(cvor->desni, indent, true);
        }
    }

    void leftRotate(Cvor<Tip> *x) {
        auto y = x->desni;
        x->desni = y->lijevi;
        if (y->lijevi != T_NIL)
            y->lijevi->roditelj = x;
        y->roditelj = x->roditelj;
        if (x->roditelj == nullptr)
            korijen = y;
        else if (x == x->roditelj->lijevi)
            x->roditelj->lijevi = y;
        else
            x->roditelj->desni = y;
        y->lijevi = x;
        x->roditelj = y;
    }

    void rightRotate(Cvor<Tip> *x) {
        auto y = x->lijevi;
        x->lijevi = y->desni;
        if (y->desni != T_NIL)
            y->desni->roditelj = x;
        y->roditelj = x->roditelj;
        if (x->roditelj == nullptr)
            korijen = y;
        else if (x == x->roditelj->desni)
            x->roditelj->desni = y;
        else
            x->roditelj->lijevi = y;
        y->desni = x;
        x->roditelj = y;
    }

    Cvor<Tip>* SearchTreeHelper(Cvor<Tip>* cvor, Tip kljuc) {
        if (cvor == T_NIL || kljuc == cvor->vrijednost)
            return cvor;
        if (kljuc < cvor->vrijednost)
            return SearchTreeHelper(cvor->lijevi, kljuc);
        return SearchTreeHelper(cvor->desni, kljuc);
    }
public:
    RBStablo() {
        T_NIL = new Cvor<Tip>;
        T_NIL->boja = CRNO;
        T_NIL->lijevi = T_NIL->desni = nullptr;
        korijen = T_NIL;
    }

    void RBInsert (Tip kljuc) {
        auto cvor = new Cvor<Tip>;
        cvor->roditelj = nullptr;
        cvor->vrijednost = kljuc;
        cvor->lijevi = T_NIL;
        cvor->desni = T_NIL;
        cvor->boja = CRVENO;

        Cvor<Tip>* y = nullptr;
        Cvor<Tip>* x = this->korijen;

        while (x != T_NIL) {
            y = x;
            if (cvor->vrijednost < x->vrijednost)
                x = x->lijevi;
            else
                x = x->desni;
        }
        cvor->roditelj = y;
        if (y == nullptr)
            korijen = cvor;
        else if (cvor->vrijednost < y->vrijednost)
            y->lijevi = cvor;
        else
            y->desni = cvor;

        if (cvor->roditelj == nullptr) {
            cvor->boja = CRNO;
            return;
        }

        if (cvor->roditelj->roditelj == nullptr)
            return;

        RBInsertFixUp(cvor);
    }

    void inOrder() {
        inOrderPomocna(korijen);
    }

    void RBDelete(Tip kljuc) {
        auto z = T_NIL;
        auto cvor = korijen;
        Cvor<Tip> *x, *y;
        while (cvor != T_NIL) {
            if (cvor->vrijednost == kljuc)
                z = cvor;

            if (cvor->vrijednost <= kljuc)
                cvor = cvor->desni;
            else
                cvor = cvor->lijevi;
        }
        if (z == T_NIL) {
            cout << "Kljuc nije pronadjen u stablu." << endl;
            return;
        }
        y = z;
        auto y_original_boja = y->boja;
        if (z->lijevi == T_NIL) {
            x = z->desni;
            RBTransplant(z, z->desni);
        }
        else if (z->desni == T_NIL) {
            x = z->lijevi;
            RBTransplant(z, z->lijevi);
        }
        else {
            y = tree_minimum(z->desni);
            y_original_boja = y->boja;
            x = y->desni;
            if (y->roditelj == z)
                x->roditelj = y;
            else {
                RBTransplant(y, y->desni);
                y->desni = z->desni;
                y->desni->roditelj = y;
            }
            RBTransplant(z,y);
            y->lijevi = z->lijevi;
            y->lijevi->roditelj = y;
            y->boja = z->boja;
        }
        delete z;
        if (!y_original_boja) {
            RBDeleteFixup(x);
        }
        cout << endl << "Element uspjesno izbrisan!" << endl;
    }

    Cvor<Tip>* root() {
        return korijen;
    }

    void printTree() {
        if (korijen) {
            printHelper(korijen, "", true);
        }
    }

    Cvor<Tip>* RBSearch(Tip kljuc) {
        return SearchTreeHelper(korijen,kljuc);
    }
};

int main() {
    vector<string> meni;
    meni.emplace_back("Ubacivanje elemenata");
    meni.emplace_back("InOrder ispis");
    meni.emplace_back("Brisanje elementa");
    meni.emplace_back("Ispis stabla");
    meni.emplace_back("Ubacivanje probnih elemenata iz postavke vjezbe");
    meni.emplace_back("Brisanje probnih elemenata iz postavke vjezbe");
    meni.emplace_back("EXIT");

    RBStablo<int> stablo;

    for (;;) {
        for (int i = 0; i < meni.size(); ++i)
            cout << i+1 << ". " << meni[i] << endl;

        cout << "Unesite jednu od opcija: " << endl;
        int odabir;
        cin >> odabir;
        switch(odabir) {
            case 1:
                int vrijednost;
                cout << "Odabrali ste kreiranje cvora." << endl;
                for (;;) {
                    cout << "Unesite vrijednost (bilo sta osim broja za izlaz): ";
                    cin.clear();
                    cin.ignore();
                    cin >> vrijednost;
                    if (cin.fail())
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
                    cout << "Unesite vrijednost (bilo sta osim broja za izlaz): ";
                    cin.clear();
                    cin.ignore();
                    cin >> vrijednost;
                    if (cin.fail())
                        break;
                    stablo.RBDelete(vrijednost);
                }
                break;
            case 4:
                cout << endl << "Odabrali ste ispis stabla." << endl;
                stablo.printTree();
                cout << endl;
                break;
            case 5:
                cout << "Odabrali ste unos elemenata iz postavke vjezbe." << endl;
                for (auto x: vector<int>{6,11,10,2,9,7,5,13,22,27,36,12,31})
                    stablo.RBInsert(x);
                break;
            case 6:
                cout << "Odabrali ste brisanje elemenata iz postavke vjezbe." << endl;
                for (auto x: vector<int>{5,27,36,12,11})
                    stablo.RBDelete(x);
                break;
            case 7:
                cout << "Dosli ste do kraja programa. Dovidjenja!" << endl;
                return 0;
            default:
                cout << "Pogresna komanda! Pokusajte opet." << endl;
        }
        cin.clear();
        cin.ignore();
    }
}