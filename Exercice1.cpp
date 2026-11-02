#include <iostream>
#include <string>
using namespace std;

class Contact {
private:
    string nom;
    string telephone;
    string email;

public:
    Contact(string n, string t, string e) {
        nom = n;
        telephone = t;
        email = e;
    }

    // Getter du nom : utile pour chercher un contact
    string getNom() { return nom; }

    void afficher() {
        cout << "Nom : " << nom
             << " | Tel : " << telephone
             << " | Email : " << email << endl;
    }
};

// Ajoute un contact dans le carnet.
// On passe carnet par référence (Contact**&) car on va le remplacer par un
// nouveau tableau plus grand. nb est aussi par référence car il change.
void ajouterContact(Contact**& carnet, int& nb, string nom, string tel, string email) {
    // On crée un nouveau tableau de pointeurs, plus grand d'une case
    Contact** nouveau = new Contact*[nb + 1];

    // On recopie les anciens pointeurs (pas les objets, juste les adresses)
    for (int i = 0; i < nb; i++) {
        nouveau[i] = carnet[i];
    }

    // On crée le nouveau contact avec new et on le met à la fin
    nouveau[nb] = new Contact(nom, tel, email);

    // On libère l'ancien tableau de pointeurs (pas les contacts !)
    delete[] carnet;

    carnet = nouveau;
    nb++;
}

// Affiche tous les contacts du carnet
void afficherContacts(Contact** carnet, int nb) {
    if (nb == 0) {
        cout << "Le carnet est vide." << endl;
        return;
    }
    for (int i = 0; i < nb; i++) {
        cout << i + 1 << ". ";
        carnet[i]->afficher();
    }
}

// Supprime un contact par son nom.
// Signature demandée : supprimerContact(Contact**, int&, string)
// Comme on ne change pas l'adresse du tableau, Contact** suffit.
void supprimerContact(Contact** carnet, int& nb, string nom) {
    int position = -1;

    // On cherche le contact dans le tableau
    for (int i = 0; i < nb; i++) {
        if (carnet[i]->getNom() == nom) {
            position = i;
            break;
        }
    }

    if (position == -1) {
        cout << "Contact \"" << nom << "\" introuvable." << endl;
        return;
    }

    // On libère l'objet Contact lui-même (sinon fuite mémoire !)
    delete carnet[position];

    // On décale les pointeurs suivants pour boucher le trou
    for (int i = position; i < nb - 1; i++) {
        carnet[i] = carnet[i + 1];
    }

    nb--; // un contact en moins
    cout << "Contact \"" << nom << "\" supprime." << endl;
}

// Libère toute la mémoire avant de quitter
void libererCarnet(Contact** carnet, int nb) {
    // D'abord chaque contact
    for (int i = 0; i < nb; i++) {
        delete carnet[i];
    }
    // Ensuite le tableau de pointeurs
    delete[] carnet;
}

int main() {
    Contact** carnet = new Contact*[0]; // carnet vide au départ
    int nb = 0;                         // nombre de contacts actuels

    int choix;
    do {
        cout << "\n===== CARNET D'ADRESSES =====" << endl;
        cout << "1. Ajouter un contact" << endl;
        cout << "2. Afficher les contacts" << endl;
        cout << "3. Supprimer un contact" << endl;
        cout << "0. Quitter" << endl;
        cout << "Votre choix : ";
        cin >> choix;
        cin.ignore(); // vider le buffer avant les getline

        if (choix == 1) {
            string nom, tel, email;
            cout << "Nom : ";
            getline(cin, nom);
            cout << "Telephone : ";
            getline(cin, tel);
            cout << "Email : ";
            getline(cin, email);
            ajouterContact(carnet, nb, nom, tel, email);
        }
        else if (choix == 2) {
            afficherContacts(carnet, nb);
        }
        else if (choix == 3) {
            string nom;
            cout << "Nom du contact a supprimer : ";
            getline(cin, nom);
            supprimerContact(carnet, nb, nom);
        }
        else if (choix != 0) {
            cout << "Choix invalide !" << endl;
        }

    } while (choix != 0);

    // On libère tout avant de quitter pour éviter les fuites mémoire
    libererCarnet(carnet, nb);
    carnet = nullptr;

    cout << "Au revoir !" << endl;
    return 0;
}