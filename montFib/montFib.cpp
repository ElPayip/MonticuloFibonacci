#include <iostream>
#include <vector>
#include <sstream>
#include "montFib.h"

using namespace std;

// - - - - - - - - - - - - - - FUNCIONES AUXILIARES - - - - - - - - - - - - - - //

template<typename T, class Comparator>
typename montFib<T, Comparator>::pNodo montFib<T, Comparator>::concatenar(pNodo viejo, pNodo nuevo)
{
	nuevo->padre = nullptr;		// En la lista de ra�ces los nodos no tienen padre.
	nuevo->der = viejo;
	nuevo->izq = viejo->izq;
	viejo->izq->der = nuevo;	// Se coloca el nodo 'nuevo' entre 'viejo' y el nodo a la izquierda de 'viejo'.
	viejo->izq = nuevo;
	return nuevo;
}

template<typename T, class Comparator>
typename montFib<T, Comparator>::pNodo montFib<T, Comparator>::concatenar(pNodo nuevo)
{
	if (_top == nullptr) {				// Si el mont�culo estaba vac�o se coloca directamente en la cima.
		nuevo->der = nuevo;
		nuevo->izq = nuevo;
		_top = nuevo;
	}
	else {								
		nuevo = concatenar(_top, nuevo);// e.o.c. se concatena junto a _top y se comprueba si debe ser la nueva cima.
		if (_cmp(nuevo->key, _top->key))
			_top = nuevo;
	}
	return nuevo;
}

template<typename T, class Comparator>
void montFib<T, Comparator>::consolidar()
{
	vector<pNodo> grados = vector<pNodo>(maxGrado(), nullptr); // Vector de �rboles de distintos grados.
	pNodo ptr = _top, repe;
	while (ptr != grados[ptr->grado]) {		// El bucle termina al llegar a un �rbol que ya hab�amos tratado.
		repe = grados[ptr->grado];
		if (repe != nullptr) {				// Si existe una �rbol distinto con el mismo grado, se combinan.
			grados[ptr->grado] = nullptr;
			ptr = linkNodos(ptr, repe);		// Es importante que el nuevo �rbol se encuentre en la posici�n actual, de modo que 
		}									//	no se retroceda y se cumpla la condici�n de salida del bucle antes de tiempo.
		else {
			grados[ptr->grado] = ptr;		// Para cada �rbol v�lido nuevo, se comprueba si debe ser la nueva ra�z.
			if (ptr->key == top() || _cmp(ptr->key, top()))
				_top = ptr;
			ptr = ptr->der;
		}
	}
}

template<typename T, class Comparator>
typename montFib<T, Comparator>::pNodo montFib<T, Comparator>::hacerHijo(pNodo hijo, pNodo padre)
{
	hijo = corte(hijo);				// Se extrae al hijo de su posici�n anterior.

	if (padre->hijo == nullptr)		// Si 'padre' no ten�a hijos ser� el primero de la lista.
		padre->hijo = hijo;

	hijo->padre = padre;			// Se concatena a la lista de hijos del padre.
	hijo->izq = padre->hijo;		
	hijo->der = padre->hijo->der;
	hijo->marca = false;
	
	padre->hijo->der->izq = hijo;	
	padre->hijo->der = hijo;
	padre->grado++;
	return padre;
}

template<typename T, class Comparator>
typename montFib<T, Comparator>::pNodo montFib<T, Comparator>::linkNodos(pNodo n1, pNodo n2)
{
	if (_cmp(n2->key, n1->key)) {	// Si 'n2' es m�s prioritario que 'n1', se intercambian sus posiciones.
		swapNodos(n1, n2);
		return hacerHijo(n1, n2);	// El �rbol resultante siempre se ubicar� en la posici�n inicial de 'n1'.
	}
	return hacerHijo(n2, n1);
}

template<typename T, class Comparator>
typename montFib<T, Comparator>::pNodo montFib<T, Comparator>::corte(pNodo nodo)
{
	if (nodo->padre != nullptr && nodo->padre->hijo == nodo) {  // Si el nodo tiene padre,
		if (nodo->izq == nodo)									// se asegura la integridad de su lista de hijos.
			nodo->padre->hijo = nullptr;
		else
			nodo->padre->hijo = nodo->izq;
	}
	nodo->der->izq = nodo->izq;		// Los nodos a ambos lados pasan a apuntarse entre s�.
	nodo->izq->der = nodo->der;
	nodo->izq = nodo;				// El nodo pasa a formar parte de una lista con �l como �nico elemento.
	nodo->der = nodo;
	nodo->marca = false;
	return nodo;
}

template<typename T, class Comparator>
void montFib<T, Comparator>::swapNodos(pNodo& n1, pNodo& n2)
{
	if (n1->izq == n2)	// Si ambos nodos son adyacentes de la forma (n2)<=>(n1), basta con mover 'n1' a la izquierda de 'n2'.
		n1 = concatenar(n2, corte(n1));
	else {				// e.o.c. intercambian posiciones.
		pNodo der = n2->der;
		n2 = concatenar(n1, corte(n2));
		n1 = concatenar(der, corte(n1));
	}
}

template<typename T, class Comparator>
void montFib<T, Comparator>::propagaCorte(pNodo nodo)
{
	nodo->grado--;					// Decrementa el grado
	if (nodo->marca) {
		pNodo padre = nodo->padre;
		concatenar(corte(nodo));	// Si estaba marcado, se corta y propaga
		if (padre != nullptr)
			propagaCorte(padre);
	}
	else if (nodo->padre != nullptr)
		nodo->marca = true;			// e.o.c., si no est� en la lista de ra�ces se marca
}

template<typename T, class Comparator>
string montFib<T, Comparator>::showRecursion(pNodo ptr)
{
	pNodo ini = ptr;
	stringstream ss;
	do {
		ss << " (" << ptr->key;
		if (ptr->marca) ss << "*";
		if (ptr->hijo != nullptr)
			ss << " [" << showRecursion(ptr->hijo) << "]";
		ss << ")";
		ptr = ptr->izq;
	} while (ptr != ini);
	return ss.str();
}

template<typename T, class Comparator>
void montFib<T, Comparator>::clearRecursion(pNodo nodo)
{
	pNodo ptr = nodo, sig;
	ptr->izq->der = nullptr;
	while (ptr != nullptr) {
		if (ptr->hijo != nullptr)
			clearRecursion(ptr->hijo);
		sig = ptr->der;
		delete ptr;
		ptr = sig;
	}
}

// - - - - - - - - - - - - - - OPERACIONES PRINCIPALES - - - - - - - - - - - - - - //

template<typename T, class Comparator>
T montFib<T, Comparator>::top() const
{
	return _top != nullptr ? _top->key : NULL;
}

template<typename T, class Comparator>
montFib<T, Comparator> montFib<T, Comparator>::unionFib(montFib<T, Comparator>& mf)
{
	if (mf._top == nullptr)
		return *this;

	if (_top == nullptr)		// Si el mont�culo es vac�o, pasa a ser 'mf'.
		_top = mf._top;
	else {
		pNodo topU = mf._top;	// Se enlazan ambas listas de ra�ces.
		topU->izq->der = _top;
		_top->izq->der = topU;
		pNodo izqU = topU->izq;
		topU->izq = _top->izq;
		_top->izq = izqU;
								// La nueva cima ser� el _top m�s prioritario de ambos mont�culos.
		if (_cmp(topU->key, _top->key))
			_top->key = topU->key;
	}
	mf._top = nullptr;			// 'mf' pasa a ser vac�o.
	_n += mf._n;
	mf._n = 0;
	return *this;
}

template<typename T, class Comparator>
typename montFib<T, Comparator>::pNodo montFib<T, Comparator>::insert(T const& elem)
{
	pNodo nuevo = new Nodo(elem);	// Se crea un nuevo nodo con el elemento 'elem'
	_n++;
	return concatenar(nuevo);
}

template<typename T, class Comparator>
T montFib<T, Comparator>::borraTop()
{
	pNodo oldMin = _top;
	T minVal = top();
	if (oldMin != nullptr) {
		pNodo ptr = oldMin->hijo, sig;
		if (ptr != nullptr)	// Anulando un puntero es m�s f�cil recorrer la lista de hijos, para marcar el final.
			ptr->izq->der = nullptr;
		while (ptr != nullptr) {		// Se concatenan todos los hijos de _top a la lista de ra�ces.
			sig = ptr->der;
			concatenar(ptr);
			ptr = sig;
		}
		if (_top == _top->der)			// Si s�lo hab�a un elemento, la lista queda vac�a.
			_top = nullptr;
		else {
			_top = oldMin->der;			// e.o.c. se elimina el nodo _top,
			delete corte(oldMin);
			_n--;
			consolidar();				// y se consolida el mont�culo.
		}
	}
	return minVal;
}

template<typename T, class Comparator>
typename montFib<T, Comparator>::pNodo montFib<T, Comparator>::decreceClave(pNodo nodo, T const& key)
{
	if (_cmp(nodo->key, key))		// La nueva clave debe tener una prioridad mayor que la interior
		throw domain_error("La nueva clave es mayor que la anterior\n"); // (Menor valor en el caso del mont�culo de m�nimos).

	nodo->key = key;				// Si el nodo pasa a ser m�s prioritario que el padre, se corta y pasa a la lista de ra�ces.
	if (nodo->padre != nullptr && _cmp(nodo->key, nodo->padre->key)) {
		pNodo padre = nodo->padre;
		concatenar(corte(nodo));
		propagaCorte(padre);
	}
	if (_cmp(key, top()))			// Siempre se comprueba si el nodo ha pasado a ser el m�s prioritario.
		_top = nodo;
	return nodo;
}

template<typename T, class Comparator>
string montFib<T, Comparator>::show()
{
	if (_top == nullptr)
		return "{}";
	string str = "{->";
	str += showRecursion(_top);
	str += "}";
	return str;
}

template<typename T, class Comparator>
void montFib<T, Comparator>::clear()
{
	if (_top != nullptr) {
		clearRecursion(_top);
		_n = 0;
		_top = nullptr;
	}
}