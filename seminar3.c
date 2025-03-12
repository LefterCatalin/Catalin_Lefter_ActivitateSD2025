#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

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

void afisareMasina(Masina masina) {
	printf("ID: %d \n", masina.id);
	printf("Nr usi: %d \n", masina.nrUsi);
	printf("Pret: %.2f \n", masina.pret);
	printf("Model: %s \n", masina.model);
	printf("Nume Sofer: %s \n", masina.numeSofer);
	printf("Seria: %c \n\n", masina.serie);

}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	int i;
	for (i = 0;i < nrMasini;i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(sizeof(Masina)*(*nrMasini)+1);
	for (int i = 0;i < (*nrMasini) ;i++) {
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini] = masinaNoua;
	free((*masini));
	(*masini) = temp;
	(*nrMasini) = (*nrMasini) + 1;
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	Masina masina;
	char delim[3] = ",;\n";
	char buffer[100];
	fgets(buffer, 100, file);
	char* aux;
	aux = strtok(buffer, delim);
	masina.id = atoi(aux);
	masina.nrUsi = atoi(strtok(NULL, delim));
	masina.pret = atof(strtok(NULL, delim));
	aux = strtok(NULL, delim);
	masina.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(masina.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, delim);
	masina.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(masina.numeSofer, strlen(aux) + 1, aux);
	aux = strtok(NULL, delim);
	masina.serie = aux[0];
	

	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	for (int i = 0; i < (*nrMasini);i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

int main() {

	Masina* masini = NULL;
	int nrMasini = 0;

	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);

	return 0;
}
