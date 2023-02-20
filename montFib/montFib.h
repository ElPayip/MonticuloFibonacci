#pragma once
#include <iostream>
#include <math.h>

using namespace std;

template <typename T, class Comparator = less<T>>
class montFib {
public:
	/*Unidad mínima que conforma el montículo de fibonacci. Contiene el elemento (clave) que lo representa, 
	así como el valor de su marca (si ha perdido un hijo o no), su grado (núm de hijos) y punteros a los 
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

	// Puntero el elemento en la cima del montículo (equivalente a min en un montículo de mínimos).
	pNodo _top;
	// Número de elementos del montículo.
	size_t _n;
	// Criterio de comparación del montículo (por defecto es less<T>, para un montículo de mínimos).
	Comparator _cmp;

	// FUNCIONES AUXILIARES:

	/*Devuelve el máximo grado posible de un árbol en el montículo, utilizado para tamaño que deberá 
	tener el vector de grados en consolidar().*/
	int maxGrado() const { return size_t(log2(_n)) + 1; }

	/*Concatena el nodo 'nuevo' junto al nodo 'viejo' en la lista de raíces del montículo, colocándolo entre este último y 
	el nodo que había a su izquierda.*/
	pNodo concatenar(pNodo viejo, pNodo nuevo);

	/*Concatena el nodo 'nuevo' junto al nodo en la cima, colocándolo entre este último y el nodo que había a su izquierda.
	Si el nuevo nodo tiene mayor prioridad, se convierte en la nueva cima.*/
	pNodo concatenar(pNodo nuevo);

	/*Combina los árboles (nodos) en la lista de raíces del montículo de dos en dos, de forma que 
	no haya dos árboles con el mismo grado en la lista final.*/
	void consolidar();

	/*Añade el nodo 'hijo' a la lista de hijos del nodo 'padre', y devuelve la raíz del árbol resultante.*/
	pNodo hacerHijo(pNodo hijo, pNodo padre);

	/*Combina los nodos recibidos como parámetros en un único árbol, utilizando como padre aquel con mayor
	prioridad. El árbol resultante estará ubicado en la posición del nodo 'n1', independientemente de cuál
	sea el padre, y se devolverá un puntero a su raíz.*/
	pNodo linkNodos(pNodo n1, pNodo n2);

	/*Intercambia las posiciones de dos nodos en la lista de raíces del montículo. De este modo, los punteros
	dados como parámetros apuntarán a los mismos nodos, en posiciones distintas.*/
	void swapNodos(pNodo& n1, pNodo& n2);

	/*Extrae un nodo de la lista en la que se encontraba (ya sea la de raíces o la de hijos de otro nodo),
	junto con sus hijos, y devuelve un puntero a él.*/
	pNodo corte(pNodo nodo);

	/*Marca y decrece el grado del nodo 'padre', cuyo hijo ha sido previamente cortado. En caso de estar ya
	marcado, se corta, desmarca y se propaga el corte al que era su padre.*/
	void propagaCorte(pNodo padre);

	/*Forma recursivamente la representación de nodos y listas de nodos utilizada en la operación show().*/
	string showRecursion(pNodo ptr);

	/*Libera la memoria dinámica de cada nodo, y se llama recursivamente para sus hijos*/
	void clearRecursion(pNodo nodo);

public:
	// OPERACIONES PRINCIPALES:

	/*Inicializa la cima del montículo a nullptr y el número de elementos a 0.*/
	montFib() : _top(nullptr), _n(0), _cmp() {}

	/*Devuelve el elemento en la cima del montículo*/
	T top() const;

	/*Concatena las listas de raíces de ambos montículos, dejando el mf pasado como parámetro vacío.*/
	montFib unionFib(montFib& mf);

	/*Añade un nuevo nodo con el elemento dado en la lista de raíces del montículo, devolviendo un puntero a dicho nodo.*/
	pNodo insert(T const& elem);

	/*Elimina el nodo en la cima del montículo, organizando los nodos restantes mediante la función auxiliar consolidar().*/
	T borraTop();

	/*Cambia el valor de la clave a uno con mayor prioridad dado (si no lo es lanza una excepción), llevando su nodo a la
	lista de raíces en caso de volverse más prioritario que su nodo padre. En el proceso, marca y corta los nodos superiores
	según la función auxiliar propagaCorte().*/
	pNodo decreceClave(pNodo nodo, T const& key);

	/*Proporciona una representación visual recursiva del montículo en forma de lista de nodos con las listas de sus hijos.*/
	string show();

	/*Libera toda la memoria dinámica del montículo llamando a clearRecursion() desde _top*/
	void clear();
};