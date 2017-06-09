#include <iostream>
#include<stdio.h>
using namespace std;

struct Reteta
{
	unsigned int nrReteta;
	unsigned int nrMedicamente;
	char** listaMedicamente;
	char* numeMedic;
	double valoareBruta;
	float procentCompensare;
};

struct CollisionList
{
	Reteta* data;
	CollisionList* next;
};

struct HashTable
{
	CollisionList** buckets;
	int htDim;
};

void initHashTable(HashTable& ht, int dim)
{
	ht.htDim = dim;
	ht.buckets = (CollisionList**)malloc(sizeof(CollisionList*)*dim);
	memset(ht.buckets, 0, sizeof(CollisionList*)*dim);
}

Reteta* createReteta(unsigned int nrRetata, unsigned int nrMedicamente, char** listaMedicamente, char* numeMedic, double valoareBruta, float procentCompensare)
{
	Reteta* r = (Reteta*)malloc(sizeof(Reteta));
	r->nrReteta = nrRetata;
	r->nrMedicamente = nrMedicamente;
	r->listaMedicamente = (char**)malloc(sizeof(char*)*nrMedicamente);
	for (int i = 0; i < nrMedicamente; i++)
	{
		r->listaMedicamente[i] = (char*)malloc(strlen(listaMedicamente[i]) + 1);
		strcpy(r->listaMedicamente[i], listaMedicamente[i]);
	}
	r->numeMedic = (char*)malloc(strlen(numeMedic) + 1);
	strcpy(r->numeMedic, numeMedic);
	r->valoareBruta = valoareBruta;
	r->procentCompensare = procentCompensare;
	return r;
}

CollisionList* creareNod(Reteta* nod)
{
	CollisionList* cl = (CollisionList*)malloc(sizeof(CollisionList));
	cl->data = nod;
	cl->next = nullptr;
	return cl;
}

int hashFunction(int nrReteta, int dim)
{
	int pozitie = nrReteta% dim;
	return pozitie;
}

CollisionList* getListHT(HashTable ht, int nrReteta)
{
	int index = hashFunction(nrReteta, ht.htDim);
	return ht.buckets[index];
}

void insertIntoHashTable(HashTable ht, Reteta* r)
{
	int index = hashFunction(r->nrReteta, ht.htDim);
	CollisionList* node = creareNod(r);
	if (ht.buckets[index] == NULL)
	{
		ht.buckets[index] = node;
	}
	else
	{
		CollisionList* temp = ht.buckets[index];
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = node;
	}
}

void parcurgere(HashTable ht)
{
	for (int i = 1; i <= ht.htDim; i++)
	{
		CollisionList* list = getListHT(ht, i);
		while (list)
		{
			printf("\nNume medic : %s , Numar Medicamente : %d ", list->data->numeMedic, list->data->nrMedicamente);
			for (int j = 0; j < list->data->nrMedicamente; j++)
			{
				printf("  Medicament : %s ", list->data->listaMedicamente[j]);
			}
			list = list->next;
		}
	}
}

void valideazaMedicament(HashTable ht,char* medicament, int nrReteta)
{
	CollisionList* list = getListHT(ht, nrReteta);
	while (list)
	{
		for (int i = 0; i < list->data->nrMedicamente; i++)
		{
			if (strcmp(list->data->listaMedicamente[i], medicament) == 0)
			{
				printf("\nMedicamentul %s este prezent in reteta cu numarul %d ", medicament, nrReteta);
			}
		}
		list = list->next;
	}
}

void calculeazaValoareNet(HashTable ht)
{
	for (int i = 1; i <= ht.htDim; i++)
	{
		CollisionList* list = getListHT(ht, i);
		while (list)
		{
			double valoareNet = list->data->valoareBruta*list->data->procentCompensare;
			printf("\nReteta : %d are valoarea %lf ", list->data->nrReteta, valoareNet);
			list = list->next;
		}	
	}
}


void main()
{
	HashTable ht;
	initHashTable(ht, 100);
	FILE* pFile = fopen("Text.txt", "r");
	if (pFile)
	{
		int nrReteta, nrMedicamente;
		double valoareBruta;
		float procentCompensare;
		char numeMedic[30];
		char* listaMedicamente[30];
		fscanf(pFile, "%d", &nrReteta);
		while (!feof(pFile))
		{
			fscanf(pFile, "%d", &nrMedicamente);
			for (int i = 0; i < nrMedicamente; i++)
			{
				listaMedicamente[i] = (char*)malloc(30);
				fscanf(pFile, "%s", listaMedicamente[i]);
			}
			fscanf(pFile, "%s", numeMedic);
			fscanf(pFile, "%f", &procentCompensare);
			fscanf(pFile, "%lf", &valoareBruta);
			Reteta * r = createReteta(nrReteta, nrMedicamente, listaMedicamente, numeMedic, valoareBruta, procentCompensare);
			insertIntoHashTable(ht, r);

			fscanf(pFile, "%d", &nrReteta);
		}
	}
	fclose(pFile);
	//parcurgere(ht);
	valideazaMedicament(ht, "paracetamol", 1);
	calculeazaValoareNet(ht);
}