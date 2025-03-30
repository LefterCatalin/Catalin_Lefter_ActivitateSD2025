#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct NodListaDubla {
	Masina info;
	struct NodListaDubla* prev;
	struct NodListaDubla* next;
};
typedef struct NodListaDubla Nod;

struct ListaDubla {
	Nod* inceput;
	Nod* final;
};
typedef struct ListaDubla Ldi;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaInceput(Ldi lista) {
	Nod* p = lista.inceput;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareListaMasiniDeLaFinal(Ldi lista) {
	Nod* p = lista.final;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(Ldi* lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = lista->final;
	if (lista->final != NULL) {
		lista->final->next = nodNou;
	}
	else {
		lista->inceput = nodNou;
	}
	lista->final = nodNou;
}

void adaugaLaInceputInLista(Ldi* lista, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = lista->inceput;
	nodNou->prev = NULL;
	if (lista->inceput) {
		lista->inceput->prev = nodNou;
	}
	else {
		lista->final = nodNou;
	}
	lista->inceput = nodNou;
}

Ldi citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Ldi lista;
	lista.final = NULL;
	lista.inceput = NULL;
	while (!feof(f)) {
		Masina m;
		m = citireMasinaDinFisier(f);
		adaugaLaInceputInLista(&lista, m);

	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(Ldi* lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	while (lista->inceput) {
		if (lista->inceput->info.numeSofer) {
			free(lista->inceput->info.numeSofer);
		}
		if (lista->inceput->info.model) {
			free(lista->inceput->info.model);
		}
		Nod* p = lista->inceput;
		lista->inceput = p->next;
		free(p);
	}
	lista->final = NULL;
}

float calculeazaPretMediu(Ldi lista) {
	float suma = 0;
	int contor = 0;
	Nod* temp = lista.inceput;
	while (temp) {
		suma += temp->info.pret;
		contor++;
		temp = temp->next;
	}
	return (contor>0)?suma/contor:0;
}

void stergeMasinaDupaID(Ldi* lista, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	Nod* p = lista->inceput;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p) {
		if (p->prev) {
			p->prev->next = p->next;
			if (p->next) {
				p->next->prev = p->prev;
			}
			else {
				lista->final = p->prev;
			}
		}
		else {
			lista->inceput = p->next;
			if (p->next) {
				p->next->prev = NULL;
			}
			else {
				lista->final = NULL;
			}
		}
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

char* getNumeSoferMasinaScumpa(Ldi lista) {
	Nod* p = lista.inceput;
	Nod* maxPretMasina = p;
	while (p) {
		if (p->info.pret > maxPretMasina->info.pret) {
			maxPretMasina = p;
		}
		p = p->next;
	}
	char* nume = malloc(strlen(maxPretMasina->info.numeSofer) + 1, maxPretMasina->info.numeSofer);

	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
	Ldi lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	afisareListaMasiniDeLaFinal(lista);

	float medie = calculeazaPretMediu(lista);
	printf("Pretul mediu este %.2f\n", medie);
	stergeMasinaDupaID(&lista, 1);
	char* nume = getNumeSoferMasinaScumpa(lista);
	printf("%s \n", nume);

	dezalocareLDMasini(&lista);
	return 0;
}