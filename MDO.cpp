#include "IteratorMDO.h"
#include "MDO.h"
#include <iostream>
#include <vector>
#include <queue>
#include <exception>
using namespace std;



// O(1)
MDO::MDO(Relatie r) {
	rel = r;
	rad = nullptr;
}



//O(h)
void MDO::adauga(TCheie c, TValoare v) { 
	rad = adauga_rec(rad, c, v, nullptr);
}



//O(h)
PNod MDO::cauta_rec(PNod r, TCheie c) const { 
	//înseamnă că am ajuns la o frunză a arborelui 
	//și cheia c nu a fost găsită
	if (r == nullptr) 
		return nullptr;
	//înseamnă că am găsit nodul căutat și se 
	//returnează adresa acestuia
	else if (r->e.first == c) 
		return r;
	//înseamnă că trebuie să continuăm căutarea 
	//în subarborele stâng
	else if (!rel(r->e.first, c)) 
		return cauta_rec(r->st, c);
	//înseamnă că trebuie să continuăm căutarea 
	//în subarborele drept
	else if (rel(r->e.first, c)) 
		return cauta_rec(r->dr, c);
}



//teta(h)
PNod MDO::minim(PNod p)  const { 
	//înseamnă că suntem la o frunză și nu există noduri
	//mai mici decât acesta
	if (p == nullptr) 
		return p;
	//Deci, pentru a găsi cheia minimă, mergem întotdeauna 
	//în stânga până când nu mai există subarbore stâng
	while (p->st != nullptr)
		p = p->st;
	return p;

}



//o(h)
PNod MDO::succesor(PNod p) const 
{
	//Dacă nodul p are un subarbore drept atunci
	//succesorul său este cheia minimă din subarborele drept
	if (p->dr != nullptr)
		return minim(p->dr);
	else {
		//Dacă aceste condiții sunt îndeplinite,
		//înseamnă că nodul p este cel mai din dreapta nod
		auto prec = p->tata;
		while (prec != nullptr && p == prec->dr) {
			p = prec;
			prec = p->tata;
		}
		return prec;
	}
}



PNod MDO::sterge_rec(PNod p, TCheie c, TValoare v)
{
	return nullptr;

}



vector<TValoare> MDO::cauta(TCheie c) const {
	PNod r = cauta_rec(rad, c);
	if (r == nullptr) 
		return {};
	return r->e.second;
}





// O(h)
PNod MDO::adauga_rec(PNod p, TCheie c, TValoare v, PNod p_ant)
{
	//înseamnă că am ajuns la o poziție liberă în arbore 
	//și trebuie creat un nou nod
	if (p == nullptr)
	{
		p = new Nod(TElem(c, v), nullptr, nullptr, p_ant);
	}
	//înseamnă că avem aceeași cheie în arbore și 
	//trebuie adăugată valoarea v
	else if (p->e.first == c) {
		p->e.second.push_back(v);

	}
	//înseamnă că trebuie să adăugăm noul nod în subarborele stâng
	else if (!rel(p->e.first, c)) {
		p->st = adauga_rec(p->st, c, v, p);
	}
	//înseamnă că trebuie să adăugăm noul nod în subarborele drept
	else if (rel(p->e.first, c)) {
		p->dr = adauga_rec(p->dr, c, v, p);
	}
	return p;

}



//o(h)
bool MDO::sterge(TCheie c, TValoare v) { 

	//Verificam daca exista nodul
	PNod r = cauta_rec(rad, c);
	if (r == nullptr) 
		return false;
	//Se verifică dacă valoarea v se găsește în lista de valori a nodului găsit
	auto it = std::find(r->e.second.begin(), r->e.second.end(), v);
	if (it == r->e.second.end())
		return false;
	//valoarea este eliminată din lista de valori asociate cheii respective
	r->e.second.erase(it);
	//Se verifică dacă lista de valori a nodului găsit este goală. 
	//Dacă este goală, înseamnă că nodul trebuie șters complet
	if (r->e.second.size() != 0) 
		return true;

	//Nodul este un nod frunză
	if (r->dr == nullptr && r->st == nullptr)
	{
		if (r != rad) {
			if (r->tata->st == r)
				r->tata->st == nullptr;
			else r->tata->dr = nullptr;
		}
		//Dacă este rădăcină, atunci se actualizează rad 
		//cu nullptr pentru a indica că MDO-ul devine vid.
		else {
			rad = nullptr;
		}
		//se elibereaza memoria
		delete r;
	}
	// Nodul are doar fiu drept
	else if (r->dr != nullptr) {
		//Se găsește nodul succesor al nodului
		PNod temp = minim(r->dr);

		//Se actualizeaza legaturile cu parintele
		if (temp == r->dr) {
			if (temp->dr != nullptr)
				temp->dr->tata = r;
			r->dr = temp->dr;
		}
		else {
			if (temp->dr != nullptr)
				temp->dr->tata = temp->tata;
			temp->tata->st = temp->dr;
		}
		//Se găsește nodul succesor al nodului
		r->e.first = temp->e.first;
		r->e.second = temp->e.second;

		//Se elibereaza memoria
		delete temp;
	}
	//Nodul are doar fiu stâng
	else {
		//Dacă este rădăcină, atunci se actualizează rad cu fiul stâng al nodului r 
		if (r == rad) {
			rad = r->st;
			delete r;
		}
		//Se actualizeaza legaturile cu parintele
		else {
			r->st->tata = r->tata;
			if (r->tata->dr == r)
				r->tata->dr = r->st;
			else
				r->tata->st = r->st;
			delete r;
		}
	}
	return true;

}



// teta(n), n - nr chei
int MDO::dim() const { 
	int dimensiune = 0;

	//Verificam daca radacina este vida
	if (rad == nullptr)
		dimensiune = 0;
	else {
		//Parcurgem BFS, se adaiga rad in coada
		queue<PNod>q;
		q.push(rad);
		//Pana cand coada devine goala
		while (!q.empty()) {
			PNod p = q.front();
			q.pop();
			//Daca acel rad are fiu stang se adauga in coada
			if (p->st != nullptr)
				q.push(p->st);
			//Daca acel rad are fiu drept se adauga in coada
			if (p->dr != nullptr)
				q.push(p->dr);
			//numărul total de perechi cheie-valoare
			dimensiune += p->e.second.size();
		}
	}

	return dimensiune;

}



// teta(1)
bool MDO::vid() const { 
	return rad == nullptr;
}



IteratorMDO MDO::iterator() const {
	return IteratorMDO(*this);
}



// teta(n)
MDO::~MDO() { 
	if (rad != nullptr) {
		queue<PNod>q;
		q.push(rad);
		while (!q.empty()) {
			PNod p = q.front();
			q.pop();
			if (p->st != nullptr)
				q.push(p->st);
			if (p->dr != nullptr)
				q.push(p->dr);
			delete p;
		}
	}


}



// teta(1)
Nod::Nod(TElem e, PNod st, PNod dr, PNod tata) 
{
	this->e.first = e.first;
	this->e.second.push_back(e.second);
	this->st = st;
	this->dr = dr;
	this->tata = tata;
}




PNod Nod::stanga()
{
	return this->st;
}




PNod Nod::dreapta()
{
	return this->dr;
}




pair < TCheie, vector<TValoare> >& Nod::element()
{
	return e;
}

/*
Pseudocod:
AdaugareInexistente(Multidictionar md):
	bool = false
	it<- iterator
	cat timp iterator valid executa
		Daca c exista in mulidictionar atunci
			adauga(chei, elem)
			nr_perechi++
		Altfel
			valori = cauta(cheie)  //verificam daca la acea cheie avem o valoare asociata
			for val in valori executa
				daca val = elem.second atunci
					bool = true
					break
			Daca bool = true atunci
				adauga(element.first, element.second)
				nr_perechi++
			it.urmator

	AdaugareInexistente<-nr_perechi

*/
//O(C) , unde C este numarul de chei
int MDO::adaugaInexistente(MDO& md) {
	int numarPerechiAdaugate = 0;

	IteratorMDO it = md.iterator();

	while (it.valid()) {
		TElem element = it.element();

		// Verificăm dacă cheia există în multidicționarul curent
		vector<TValoare> v = md.cauta(element.first);
		if (size(v) !=0) {
			// Cheia nu există, deci adăugăm perechea în multidicționarul curent
			adauga(element.first, element.second);
			numarPerechiAdaugate++;
		}
		else {
			// Cheia există, verificăm dacă valoarea există deja pentru această cheie
			vector<TValoare> valori = cauta(element.first);
			bool valoareExistenta = false;
			for (const auto& valoare : valori) {
				if (valoare == element.second) {
					valoareExistenta = true;
					break;
				}
			}
			if (!valoareExistenta) {
				// Valoarea nu există, deci o adăugăm în lista de valori asociată cheii
				adauga(element.first, element.second);
				numarPerechiAdaugate++;
			}
		}
		it.urmator();
	}
	return numarPerechiAdaugate;
}
