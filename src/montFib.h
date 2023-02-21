#pragma once
#include <iostream>
#include <math.h>

using namespace std;

template <typename T, class Comparator = less<T>>
class montFib {
public:
	/*Unidad m�nima que conforma el mont�culo de fibonacci. Contiene el elemento (clave) que lo representa, 
	as� como el valor de su marca (si ha perdido un hijo o no), su grado (n�m de hijos) y punteros a los 
	nodos padre, hijo, izquierdo y derecho.*/
	struct Nodo {
	private:
		friend class montFib;
		T key;
		bool marca;
		int grado;
		Nodo* izq, * der, * hijo, * padre;
		// Inicializa el elemento del nodo al valor dado, la marca a false, el grado a 0 y todos los punteros a nullptr.
		Nodo(T const& k) :
			key(k), izq(nullptr), der(nullptr), padre(nullptr), hijo(nullptr), marca(false), grado(0) {}
	};
	/*Puntero a una estructura de tipo Nodo.*/
	using pNodo = Nodo*;

private:
	// ATRIBUTOS:

	// Puntero el elemento en la cima del mont�culo (equivalente a min en un mont�culo de m�nimos).
	pNodo _top;
	// N�mero de elementos del mont�culo.
	size_t _n;
	// Criterio de comparaci�n del mont�culo (por defecto es less<T>, para un mont�culo de m�nimos).
	Comparator _cmp;

	// FUNCIONES AUXILIARES:

	/*Devuelve el m�ximo grado posible de un �rbol en el mont�culo, utilizado para tama�o que deber� 
	tener el vector de grados en consolidar().*/
	int maxGrado() const { return size_t(log2(_n)) + 1; }

	/*Concatena el nodo 'nuevo' junto al nodo 'viejo' en la lista de ra�ces del mont�culo, coloc�ndolo entre este �ltimo y 
	el nodo que hab�a a su izquierda.*/
	pNodo concatenar(pNodo viejo, pNodo nuevo);

	/*Concatena el nodo 'nuevo' junto al nodo en la cima, coloc�ndolo entre este �ltimo y el nodo que hab�a a su izquierda.
	Si el nuevo nodo tiene mayor prioridad, se convierte en la nueva cima.*/
	pNodo concatenar(pNodo nuevo);

	/*Combina los �rboles (nodos) en la lista de ra�ces del mont�culo de dos en dos, de forma que 
	no haya dos �rboles con el mismo grado en la lista final.*/
	void consolidar();

	/*A�ade el nodo 'hijo' a la lista de hijos del nodo 'padre', y devuelve la ra�z del �rbol resultante.*/
	pNodo hacerHijo(pNodo hijo, pNodo padre);

	/*Combina los nodos recibidos como par�metros en un �nico �rbol, utilizando como padre aquel con mayor
	prioridad. El �rbol resultante estar� ubicado en la posici�n del nodo 'n1', independientemente de cu�l
	sea el padre, y se devolver� un puntero a su ra�z.*/
	pNodo linkNodos(pNodo n1, pNodo n2);

	/*Intercambia las posiciones de dos nodos en la lista de ra�ces del mont�culo. De este modo, los punteros
	dados como par�metros apuntar�n a los mismos nodos, en posiciones distintas.*/
	void swapNodos(pNodo& n1, pNodo& n2);

	/*Extrae un nodo de la lista en la que se encontraba (ya sea la de ra�ces o la de hijos de otro nodo),
	junto con sus hijos, y devuelve un puntero a �l.*/
	pNodo corte(pNodo nodo);

	/*Marca y decrece el grado del nodo 'padre', cuyo hijo ha sido previamente cortado. En caso de estar ya
	marcado, se corta, desmarca y se propaga el corte al que era su padre.*/
	void propagaCorte(pNodo padre);

	/*Forma recursivamente la representaci�n de nodos y listas de nodos utilizada en la operaci�n show().*/
	string showRecursion(pNodo ptr);

	/*Libera la memoria din�mica de cada nodo, y se llama recursivamente para sus hijos*/
	void clearRecursion(pNodo nodo);

public:
	// OPERACIONES PRINCIPALES:

	/*Inicializa la cima del mont�culo a nullptr y el n�mero de elementos a 0.*/
	montFib() : _top(nullptr), _n(0), _cmp() {}

	/*Comprueba si el montículo está vacío*/
	bool empty() const;

	/*Devuelve el elemento en la cima del mont�culo*/
	T top() const;

	/*Concatena las listas de ra�ces de ambos mont�culos, dejando el mf pasado como par�metro vac�o.*/
	montFib unionFib(montFib& mf);

	/*A�ade un nuevo nodo con el elemento dado en la lista de ra�ces del mont�culo, devolviendo un puntero a dicho nodo.*/
	pNodo insert(T const& elem);

	/*Elimina el nodo en la cima del mont�culo, organizando los nodos restantes mediante la funci�n auxiliar consolidar().*/
	T borraTop();

	/*Cambia el valor de la clave a uno con mayor prioridad dado (si no lo es lanza una excepci�n), llevando su nodo a la
	lista de ra�ces en caso de volverse m�s prioritario que su nodo padre. En el proceso, marca y corta los nodos superiores
	seg�n la funci�n auxiliar propagaCorte().*/
	pNodo decreceClave(pNodo nodo, T const& key);

	/*Proporciona una representaci�n visual recursiva del mont�culo en forma de lista de nodos con las listas de sus hijos.*/
	string show();

	/*Libera toda la memoria din�mica del mont�culo llamando a clearRecursion() desde _top*/
	void clear();
};