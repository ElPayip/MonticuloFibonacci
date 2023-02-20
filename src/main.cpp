#include "montFib.cpp"
#include <fstream>
#include <ctime>
#include <unordered_map>
#include <locale.h>

using namespace std;

#define N_OPS 5
#define N_MED 3
#define MAX_MUESTRA 1000000
#define ESTADOS_FILE "estados.txt"

typedef void (*FuncionMedida) (ofstream&);

void medirTop(ofstream& out);
void medirInsert(ofstream& out);
void medirUnion(ofstream& out);
void medirBorraTop(ofstream& out);
void medirDecreceClave(ofstream& out);

const FuncionMedida medidas[N_OPS] = { &medirTop, &medirInsert, &medirUnion, &medirBorraTop, &medirDecreceClave };
const string files[N_OPS] = { "top.dat", "insert.dat", "union.dat", "borraTop.dat", "decreceClave.dat" };

int menuMf();
int menuOp();
int menuMed();

int main() {
	string estado;
	int mont, op, elem, nuevo, med;
	ofstream out, estadoOut;
	montFib<int> mf[2] = { montFib<int>() };
	unordered_map<int, montFib<int>::pNodo> nodos[2];

	setlocale(LC_ALL, "");
	estadoOut.open(ESTADOS_FILE, std::ofstream::out | std::ofstream::trunc);
	cout << "	MONTÍCULO DE FIBONACCI\n\n";
	do {
		mont = menuMf() - 1;
		if (mont >= 0)
			do {
				op = menuOp();
				switch (op) {
				case 1:
					if (mf[mont].top() == NULL)
						cout << "Montículo vacío.\n\n";
					else
						cout << "Cima de Montículo_" << mont + 1 << ": " << mf[mont].top() << ".\n\n";
					break;
				case 2:
					cout << "Elemento a insertar: ";
					cin >> elem;
					if (nodos[0].count(elem) || nodos[1].count(elem)) {
						cout << "Para asegurar la integridad de la aplicación, no se permiten elementos repetidos, "
							 << "ni siquiera entre distintos montículos (aunque internamente los montículos sí lo permitan).\n\n";
						break;
					}
					nodos[mont][elem] = mf[mont].insert(elem);
					cout << "Se ha insertado el número " << elem << "\n\n";
					break;
				case 3:
					mf[mont] = mf[mont].unionFib(mf[(mont + 1) % 2]);
					nodos[mont].insert(nodos[(mont + 1) % 2].begin(), nodos[(mont + 1) % 2].end());
					nodos[(mont + 1) % 2].clear();
					cout << "Se han transferido todos los elementos de Montículo_" << (mont + 1) % 2 + 1 << " a Montículo_" << mont + 1 << ".\n\n";
					break;
				case 4:
					nodos[mont].erase(mf[mont].top());
					cout << "Se ha eliminado el número " << mf[mont].borraTop() << ".\n\n";
					break;
				case 5:
					cout << "Elemento a decrecer: ";
					cin >> elem;
					if (!nodos[mont].count(elem)) {
						cout << "Elemento no existente en Montículo_ " << mont + 1 << ".\n\n";
						break;
					}
					cout << "Nuevo valor: ";
					cin >> nuevo;
					if (nuevo >= elem) {
						cout << "El nuevo valor debe ser menor que el anterior.\n\n";
						break;
					}
					if (nodos[0].count(nuevo) || nodos[1].count(nuevo)) {
						cout << "Para asegurar la integridad de la aplicación, no se permiten elementos repetidos, "
							 << "ni siquiera entre distintos montículos (aunque internamente los montículos sí lo permitan).\n\n";
						break;
					}
					mf[mont].decreceClave(nodos[mont][elem], nuevo);
					nodos[mont][nuevo] = nodos[mont][elem];
					nodos[mont].erase(elem);
					cout << "Se ha cambiado el número " << elem << " por " << nuevo << ".\n\n";
					break;
				case 6:
					cout << "Montículo_ " << mont + 1 << " en forma de lista de nodos con las listas de sus hijos:\n"
						 << mf[mont].show() << "\n\n";
					break;
				case 7:
					med = menuMed() -1;
					if (med < 0) break;
					cout << "Midiendo operación, puede tardar unos segundos...\n";
					out.open(files[med], std::ofstream::out | std::ofstream::trunc); 
					medidas[med](out); 
					out.close();
					cout << "Datos guardados en " << files[med] << ".\n\n";
					break;
				case 8:
					estado = mf[mont].show() + '\n';
					estadoOut << estado;
					cout << estado << "Estado guardado en " << ESTADOS_FILE << ".\n\n";
				default:
					break;
				}
			} while (op > 0);
	} while (mont >= 0);

	mf[0].clear();
	mf[1].clear();
	estadoOut.close();

	return 0;
}

int menuMf() {
	cout << "Elija un montículo a utilizar (ambos son montículos de mínimos para números enteros):\n"
		 << "	1.- Montículo_1\n"
		 << "	2.- Montículo_2\n"
		 << "	0.- Salir\n";
	int op;
	cin >> op;
	while (op < 0 || op > 2) {
		cout << "Opción no válida.\nElija un montículo:";
		cin >> op;
	}
	return op;
}

int menuOp() {
	cout << "Operaciones:\n"
		 << "	1.- Mostrar top\n"
		 << "	2.- Insertar\n"
		 << "	3.- Unión con el otro montículo\n"
		 << "	4.- Borrar top\n"
		 << "	5.- Decrecer clave\n"
		 << "	6.- Mostrar montículo\n"
		 << "	7.- Tomar medidas de tiempos\n"
		 << "	8.- Guardar estado\n"
		 << "	0.- Volver\n";
	int op;
	cout << "Elija una opción:";
	cin >> op;
	while (op < 0 || op > 8) {
		cout << "Opción no válida.\nElija una opción:";
		cin >> op;
	}
	return op;
}

int menuMed() {
	cout << "Operaciones:\n"
		 << "	1.- Top\n"
		 << "	2.- Inserción\n"
		 << "	3.- Unión\n"
		 << "	4.- BorraTop\n"
		 << "	5.- DecreceClave\n"
		 << "	0.- Volver\n";
	int op;
	cout << "Elija una operación a medir:";
	cin >> op;
	while (op < 0 || op > 5) {
		cout << "Opción no válida.\nElija una operación:";
		cin >> op;
	}
	return op;
}


void medirTop(ofstream& out) {
	montFib<int> mf = montFib<int>();
	long long int salto = MAX_MUESTRA / 1000, ncasos = 1000000;
	clock_t clk;
	double ms;

	for (int i = 0; i < MAX_MUESTRA / 2 + 1; ++i) {
		mf.insert(i);
		mf.insert(MAX_MUESTRA - i);
		if (i % salto == 0) {
			ms = 0;
			for (int j = 0; j < N_MED; ++j) {
				clk = clock();
				for (int k = 0; k < ncasos; ++k)
					mf.top();
				clk = clock() - clk;
				ms += (double) clk;
			}
			out << 2*i << ' ' << ms / N_MED << '\n';
		}
	}
	mf.clear();
}

void medirInsert(ofstream& out) {
	montFib<int> mf = montFib<int>();
	long long int salto = MAX_MUESTRA / 10;
	clock_t clk;

	for (int i = 0; i < MAX_MUESTRA * 20; i += salto) {
		clk = clock();
		for (int j = i; j < i + salto; j++)
			mf.insert(j);
		clk = clock() - clk;
		out << (i + salto) / 2 << ' ' << clk << '\n';
	}
	mf.clear();
}

void medirUnion(ofstream& out) {
	montFib<int> mf[N_MED] = { montFib<int>() }, mfU[N_MED][5000] = { { montFib<int>() } };
	long long int salto = 5;
	clock_t clk;
	double ms;

	for (int i = 0; i < 100 + 1; i += salto) {
		ms = 0;
		for (int k = 0; k < N_MED; k++) {
			for (int l = 0; l < 5000; ++l)
				for (int j = 0; j < i / 2; j++) {
					mfU[k][l].insert(i + j);
					mfU[k][l].insert(i + salto - j);
				}
			clk = clock();
			for (int l = 0; l < 5000; ++l)
				mf[k] = mf[k].unionFib(mfU[k][l]);
			clk = clock() - clk;
			ms += (double)clk;
		}
		out << i << ' ' << ms << '\n';
	}
	for (int i = 0; i < N_MED; ++i)
		mf[i].clear();
}

void medirBorraTop(ofstream& out) {
	montFib<int> mf = montFib<int>();
	int salto = 20, ncasos = 10000, n = 0;
	clock_t clk;
	double ms;

	for (int i1 = 0; i1 < 100; i1++) {
		mf.insert(++n);
		if (i1 % 2 == 0) {
			ms = 0;
			for (int k = 0; k < N_MED; k++) {
				clk = clock();
				for (int h = 0; h < ncasos; h++) {
					mf.insert(++n);
					mf.borraTop();
				}
				clk = clock() - clk;
				ms += clk;
			}
			out << i1 << ' ' << ms / N_MED << '\n';
		}
	}
	for (int i2 = 100; i2 < MAX_MUESTRA / 100 + 1; i2++) {
		mf.insert(++n);
		if (i2 % salto == 0) {
			ms = 0;
			for (int k = 0; k < N_MED; k++) {
				clk = clock();
				for (int j = 1; j <= ncasos; j++) {
					mf.insert(++n);
					mf.borraTop();
				}
				clk = clock() - clk;
				ms += clk;
			}
			out << i2 << ' ' << ms / N_MED << '\n';
		}
	}
	mf.clear();
}

void medirDecreceClave(ofstream& out) {
	montFib<int> mf = montFib<int>();
	vector<montFib<int>::pNodo> ptrs;
	int salto = 20000, ncasos = 10000, saltoClaves;
	clock_t clk;
	double ms;

	for (int i = 1; i < MAX_MUESTRA * 10 + 1; ++i) {
		ptrs.push_back(mf.insert(i));
		if (i % salto == 0) {
			saltoClaves = i / ncasos;
			mf.insert(INT_MIN);
			mf.borraTop();

			clk = clock();
			for (int j = 0; j < ncasos; j++) {
				//mf.decreceClave(ptrs[j * saltoClaves], -i-j); //Caso peor
				mf.decreceClave(ptrs[i-salto +j], -i - j);	// Caso amortizado
			}
			clk = clock() - clk;
			ms = (double)clk;
			out << i << ' ' << ms << '\n';
		}
	}
	mf.clear();
}