#include "IteratorMDO.h"
#include "MDO.h"




//O(h) (h-inaltimea arborelui)
// initializează iteratorul pentru a putea itera
IteratorMDO::IteratorMDO(const MDO& d) : dict(d) {
	prim();
}



//O(h) (h-inaltimea arborelui)
//Aceasta setează iteratorul la prima cheie și valoare
void IteratorMDO::prim() {
	curent = dict.minim(dict.rad);
	poz = 0;
}



//O(h) (h-inaltimea arborelui)
//Aceasta avansează iteratorul la următoarea cheie și valoare
void IteratorMDO::urmator() {
	if (valid()) {
		poz++;
		if (poz >= curent->e.second.size()) { curent = dict.succesor(curent); poz = 0; }
	}
}



////O(1) 
//Aceasta verifică dacă iteratorul este valid
bool IteratorMDO::valid() const {
	if (curent == nullptr)
		return false;
	return true;
}



////O(1) 
//Aceasta întoarce elementul curent la care se află iteratorul
TElem IteratorMDO::element() const {
	if (valid())
		return TElem(curent->e.first, curent->e.second[poz]);
	else throw std::exception();

}