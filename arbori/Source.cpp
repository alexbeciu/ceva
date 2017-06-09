#include<stdio.h>
#include<iostream>
using namespace std;

struct Bicicleta
{
	int nrIdentificare;
	int durataUtilizare;
	int idStatieParcare;
	int nrUtilizari;
	char* numeUtilizator;
};

struct NodArbore
{
	Bicicleta* info;
	NodArbore* left;
	NodArbore* right;
	int balancingFactor;
};

Bicicleta* creareBicicleta(int _nrIdentif, int _durata, int _idStatie, int _nrUtilizari, char* _numeUtilizator)
{
	Bicicleta* b = (Bicicleta*)malloc(sizeof(Bicicleta));
	b->nrIdentificare = _nrIdentif;
	b->durataUtilizare = _durata;
	b->idStatieParcare = _idStatie;
	b->nrUtilizari = _nrUtilizari;
	b->numeUtilizator = (char*)malloc(strlen(_numeUtilizator) + 1);
	strcpy(b->numeUtilizator, _numeUtilizator);
	return b;
}

NodArbore* creareNodArbore(Bicicleta* _info)
{
	NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
	nod->info = _info;
	nod->right = nod->left = NULL;
	nod->balancingFactor = 0;
	return nod;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int getHeight(NodArbore* root)
{
	if (root != NULL)
		return 1 + max(getHeight(root->left), getHeight(root->right));
	else
		return 0;
}

int getBalancingFactor(NodArbore* root)
{
	int bFactor = getHeight(root->left) - getHeight(root->right);
	return bFactor;
}

void SRL(NodArbore*& pivot)
{
	NodArbore* desc = pivot->right;
	pivot->right = desc->left;
	desc->left = pivot;
	pivot->balancingFactor = getBalancingFactor(pivot);
	desc->balancingFactor = getBalancingFactor(desc);
	pivot = desc;
}

void SRR(NodArbore*& pivot)
{
	NodArbore* desc = pivot->left;
	pivot->left = desc->right;
	desc->right = pivot;
	pivot->balancingFactor = getBalancingFactor(pivot);
	desc->balancingFactor = getBalancingFactor(desc);
	pivot = desc;
}

void rebalansareArbore(NodArbore*& pivot)
{
	NodArbore* temp = NULL;
	if (pivot->balancingFactor == 2)
	{
		temp = pivot->right;
		if (temp->balancingFactor == 1)
		{
			SRL(pivot);
		}
		else
		{
			SRR(pivot->right);
			SRL(pivot);
		}
	}
	else if (pivot->balancingFactor == -2)
	{
		temp = pivot->left; 
		if (temp->balancingFactor == -1)
		{
			SRR(pivot);
		}
		else
		{
			SRL(pivot->left);
			SRR(pivot);
		}
	}
}

void inserareNod(NodArbore*& root, NodArbore* node)
{
	if (root == NULL)
	{
		root = node;
	}
	else
	{
		if (root->info->durataUtilizare > node->info->durataUtilizare)
		{
			inserareNod(root->left, node);
		}
		else if (root->info->durataUtilizare < node->info->durataUtilizare)
		{
			inserareNod(root->right, node);
		}
		else
		{
			printf("\nKey already exists ! ");
		}
		root->balancingFactor = getBalancingFactor(root);
		rebalansareArbore(root);
	}
}

void printArbore(NodArbore* root)
{
	if (root)
	{
		printArbore(root->left);
		printf("\nNr Identificare : %d , DurataUtilizare: %d , IdStatieParcare : %d , NumarUtilizari : %d , NumeUtilizitor : %s",
				root->info->nrIdentificare, root->info->durataUtilizare, root->info->idStatieParcare, root->info->nrUtilizari, root->info->numeUtilizator);
		printArbore(root->right);
	}
}

void calculeazaNrMinute(NodArbore* root,int& nrMinute)
{
	if (root)
	{
		calculeazaNrMinute(root->left,nrMinute);
		nrMinute += root->info->durataUtilizare;
		calculeazaNrMinute(root->right,nrMinute);
	}
}

void calculeazaValoareaTotala(NodArbore* root,int& durata)
{
	if (root)
	{
		calculeazaValoareaTotala(root->left,durata);
		if (root->info->durataUtilizare > 10)
		{
			durata = root->info->durataUtilizare - 10;
		}
		calculeazaValoareaTotala(root->right,durata);
	}
}

void nrDisctinctStatii(NodArbore* root, int& nr)
{
	if (root)
	{
		nrDisctinctStatii(root->left, nr);

		nrDisctinctStatii(root->right, nr);
	}
}

void main()
{
	NodArbore* root = NULL;
	FILE* pFile = fopen("Text.txt", "r");
	if (pFile)
	{
		int nrIdentificare;
		int durataUtilizare;
		int idStatieParcare;
		int nrUtilizari;
		char numeUtilizator[30];
		fscanf(pFile, "%d", &nrIdentificare);
		while (!feof(pFile))
		{
			fscanf(pFile, "%d", &durataUtilizare);
			fscanf(pFile, "%d", &idStatieParcare);
			fscanf(pFile, "%d", &nrUtilizari);
			fscanf(pFile, "%s", numeUtilizator);

			Bicicleta* b = creareBicicleta(nrIdentificare, durataUtilizare, idStatieParcare, nrUtilizari, numeUtilizator);
			NodArbore* nod = creareNodArbore(b);
			inserareNod(root, nod);
			fscanf(pFile, "%d", &nrIdentificare);
		}
	}
	fclose(pFile);
	printArbore(root); int r = 0;
	calculeazaNrMinute(root,r);
	printf("\nNr minute : %d \n", r);
	int durata = 0;
	calculeazaValoareaTotala(root, durata);
	printf("\nValoarea totala : %d lire \n", durata * 2);
}