#include <iostream>
#include <string>

using namespace std;

template <typename T>
class MatrizBase {

protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int filas, int columnas) {
        this->_filas = filas;
        this->_columnas = columnas;
    }

    virtual ~MatrizBase() {
    }

    virtual void cargarValores() = 0;
    virtual void imprimir() const = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual T obtenerValor(int fila, int col) const = 0;
    virtual void establecerValor(int fila, int col, T valor) = 0;

    virtual int obtenerFilas() const {
        return this->_filas;
    }

    virtual int obtenerColumnas() const {
        return this->_columnas;
    }

    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return this->sumar(otra);
    }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {

private:
    T **_datos;

    void allocarMemoria(int filas, int columnas) {
        cout << "    -> (Asignando memoria para Matriz Dinámica...)" << endl;
        
        this->_datos = new T*[filas];
        
        for (int i = 0; i < filas; i++) {
            this->_datos[i] = new T[columnas];
        }

        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                this->_datos[i][j] = T();
            }
        }
    }

    void liberarMemoria() {
        cout << "    -> (Liberando memoria de Matriz Dinámica... F:" << this->_filas << ")" << endl;
        
        if (this->_datos != nullptr) {
            for (int i = 0; i < this->_filas; i++) {
                delete[] this->_datos[i];
            }
            delete[] this->_datos;
            this->_datos = nullptr;
        }
    }

    void copiarDatos(const MatrizDinamica<T>& otra) {
        cout << "    -> (Realizando copia profunda...)" << endl;
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                this->_datos[i][j] = otra._datos[i][j];
            }
        }
    }


public:
    MatrizDinamica(int filas, int columnas)
        : MatrizBase<T>(filas, columnas) {
        
        cout << "Ejecutando Constructor de MatrizDinamica (" << filas << "x" << columnas << ")" << endl;
        this->allocarMemoria(filas, columnas);
    }

    virtual ~MatrizDinamica() {
        cout << "Ejecutando Destructor de MatrizDinamica..." << endl;
        this->liberarMemoria();
    }

    MatrizDinamica(const MatrizDinamica<T>& otra)
        : MatrizBase<T>(otra._filas, otra._columnas) {
        
        cout << "Ejecutando Constructor de Copia (Deep Copy) de MatrizDinamica..." << endl;
        this->allocarMemoria(this->_filas, this->_columnas);
        this->copiarDatos(otra);
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        cout << "Ejecutando Operador de Asignación (=) (Deep Copy) de MatrizDinamica..." << endl;
        
        if (this == &otra) {
            return *this;
        }

        this->liberarMemoria();
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        this->allocarMemoria(this->_filas, this->_columnas);
        this->copiarDatos(otra);
        return *this;
    }

    MatrizDinamica(MatrizDinamica<T>&& otra) noexcept
        : MatrizBase<T>(otra._filas, otra._columnas) {
        
        cout << "Ejecutando Constructor de Movimiento (Move) de MatrizDinamica..." << endl;
        this->_datos = otra._datos;
        otra._filas = 0;
        otra._columnas = 0;
        otra._datos = nullptr;
    }

    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& otra) noexcept {
        cout << "Ejecutando Operador de Asignación (Move) de MatrizDinamica..." << endl;

        if (this == &otra) {
            return *this;
        }

        this->liberarMemoria();
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        this->_datos = otra._datos;
        otra._filas = 0;
        otra._columnas = 0;
        otra._datos = nullptr;
        return *this;
    }

    void cargarValores() override {
        cout << "Cargando valores para Matriz Dinámica (" << this->_filas << "x" << this->_columnas << ")" << endl;
        
        for (int indice_fila = 0; indice_fila < this->_filas; indice_fila++) {
            for (int indice_columna = 0; indice_columna < this->_columnas; indice_columna++) {
                cout << "  Ingrese valor para [" << indice_fila << "][" << indice_columna << "]: ";
                cin >> this->_datos[indice_fila][indice_columna];
            }
        }
        cout << "Carga de Matriz Dinámica completa." << endl;
    }

    void imprimir() const override {
        cout << "Imprimiendo Matriz Dinámica (" << this->_filas << "x" << this->_columnas << "):" << endl;
        
        for (int i = 0; i < this->_filas; i++) {
            cout << "  |";
            for (int j = 0; j < this->_columnas; j++) {
                cout << " " << this->_datos[i][j] << " |";
            }
            cout << endl;
        }
    }

    T obtenerValor(int fila, int col) const override {
        return this->_datos[fila][col];
    }

    void establecerValor(int fila, int col, T valor) override {
        this->_datos[fila][col] = valor;
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        cout << "    -> (Suma ejecutada por MatrizDinamica)" << endl;

        if (this->obtenerFilas() != otra.obtenerFilas() || 
            this->obtenerColumnas() != otra.obtenerColumnas()) {
            
            cout << "ERROR: Las dimensiones de las matrices no coinciden. No se puede sumar." << endl;
            return nullptr;
        }

        MatrizDinamica<T>* matriz_resultado = 
            new MatrizDinamica<T>(this->_filas, this->_columnas);

        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                T valor_A = this->obtenerValor(i, j);
                T valor_B = otra.obtenerValor(i, j);
                T suma_total = valor_A + valor_B;
                matriz_resultado->establecerValor(i, j, suma_total);
            }
        }
        return matriz_resultado;
    }
};


template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {

private:
    T _datos[M][N];

public:
    MatrizEstatica()
        : MatrizBase<T>(M, N) {
        
        cout << "Ejecutando Constructor de MatrizEstatica (" << M << "x" << N << ")" << endl;
        
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                this->_datos[i][j] = T();
            }
        }
    }

    virtual ~MatrizEstatica() {
        cout << "Ejecutando Destructor de MatrizEstatica..." << endl;
    }

    void cargarValores() override {
        cout << "Cargando valores para Matriz Estática (" << this->_filas << "x" << this->_columnas << ")" << endl;
        
        for (int indice_fila = 0; indice_fila < M; indice_fila++) {
            for (int indice_columna = 0; indice_columna < N; indice_columna++) {
                cout << "  Ingrese valor para [" << indice_fila << "][" << indice_columna << "]: ";
                cin >> this->_datos[indice_fila][indice_columna];
            }
        }
        cout << "Carga de Matriz Estática completa." << endl;
    }

    void imprimir() const override {
        cout << "Imprimiendo Matriz Estática (" << this->_filas << "x" << this->_columnas << "):" << endl;
        
        for (int i = 0; i < M; i++) {
            cout << "  |";
            for (int j = 0; j < N; j++) {
                cout << " " << this->_datos[i][j] << " |";
            }
            cout << endl;
        }
    }

    T obtenerValor(int fila, int col) const override {
        return this->_datos[fila][col];
    }

    void establecerValor(int fila, int col, T valor) override {
        this->_datos[fila][col] = valor;
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        cout << "    -> (Suma ejecutada por MatrizEstatica)" << endl;
        
        if (this->obtenerFilas() != otra.obtenerFilas() || 
            this->obtenerColumnas() != otra.obtenerColumnas()) {
            
            cout << "ERROR: Las dimensiones de las matrices no coinciden. No se puede sumar." << endl;
            return nullptr; 
        }

        MatrizEstatica<T, M, N>* matriz_resultado = 
            new MatrizEstatica<T, M, N>();

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                T valor_A = this->obtenerValor(i, j);
                T valor_B = otra.obtenerValor(i, j);
                T suma_total = valor_A + valor_B;
                matriz_resultado->establecerValor(i, j, suma_total);
            }
        }
        return matriz_resultado;
    }
};


int main() {

    cout << "-----------------------------------------------" << endl;
    cout << "--- Sistema Genérico de Álgebra Lineal (MathSoft) ---" << endl;
    cout << "-----------------------------------------------" << endl << endl;

    MatrizBase<float>* ptrMatrizA = nullptr;
    MatrizBase<float>* ptrMatrizB = nullptr;
    MatrizBase<float>* ptrMatrizC_Resultado = nullptr;

    cout << ">> Demostración de Genericidad (Tipo FLOAT) <<" << endl << endl;

    cout << "// 1. Creación de Matriz Dinámica A (3x2)..." << endl;
    ptrMatrizA = new MatrizDinamica<float>(3, 2);
    ptrMatrizA->cargarValores();
    
    cout << endl << "Matriz A (Dinamica) =" << endl;
    ptrMatrizA->imprimir();
    cout << endl;

    cout << "// 2. Creación de Matriz Estática B (3x2)..." << endl;
    ptrMatrizB = new MatrizEstatica<float, 3, 2>();
    ptrMatrizB->cargarValores();
    
    cout << endl << "Matriz B (Estatica) =" << endl;
    ptrMatrizB->imprimir();
    cout << endl;

    cout << "// 3. Operación Polimórfica (Suma)" << endl;
    cout << "SUMANDO: Matriz C = A + B ..." << endl;
    
    ptrMatrizC_Resultado = (*ptrMatrizA) + (*ptrMatrizB);
    
    if (ptrMatrizC_Resultado != nullptr) {
        cout << endl << "Matriz Resultado C (Tipo FLOAT):" << endl;
        ptrMatrizC_Resultado->imprimir();
    }
    cout << endl;

    cout << "===============================================" << endl;
    cout << ">> Demostración de Genericidad (Tipo INT) <<" << endl << endl;

    MatrizBase<int>* ptrMatrizD = nullptr;
    MatrizBase<int>* ptrMatrizE = nullptr;
    MatrizBase<int>* ptrMatrizF_Resultado = nullptr;

    cout << "// 1. Creación de Matriz Estática D (2x2)..." << endl;
    ptrMatrizD = new MatrizEstatica<int, 2, 2>();
    ptrMatrizD->cargarValores();
    cout << endl << "Matriz D (Estatica) =" << endl;
    ptrMatrizD->imprimir();
    cout << endl;

    cout << "// 2. Creación de Matriz Dinámica E (2x2)..." << endl;
    ptrMatrizE = new MatrizDinamica<int>(2, 2);
    ptrMatrizE->cargarValores();
    cout << endl << "Matriz E (Dinamica) =" << endl;
    ptrMatrizE->imprimir();
    cout << endl;

    cout << "// 3. Operación PolimóFica (Suma)" << endl;
    cout << "SUMANDO: Matriz F = D + E ..." << endl;

    ptrMatrizF_Resultado = (*ptrMatrizD) + (*ptrMatrizE);

    if (ptrMatrizF_Resultado != nullptr) {
        cout << endl << "Matriz Resultado F (Tipo INT):" << endl;
        ptrMatrizF_Resultado->imprimir();
    }
    cout << endl;

    cout << "===============================================" << endl;
    cout << ">> Demostración de Limpieza de Memoria <<" << endl << endl;

    cout << "Llamando al destructor de C (Resultado FLOAT)..." << endl;
    delete ptrMatrizC_Resultado;
    cout << "Llamando al destructor de A (FLOAT)..." << endl;
    delete ptrMatrizA;
    cout << "Llamando al destructor de B (FLOAT)..." << endl;
    delete ptrMatrizB;

    cout << endl;

    cout << "Llamando al destructor de F (Resultado INT)..." << endl;
    delete ptrMatrizF_Resultado;
    cout << "Llamando al destructor de D (INT)..." << endl;
    delete ptrMatrizD;
    cout << "Llamando al destructor de E (INT)..." << endl;
    delete ptrMatrizE;

    cout << endl << "Sistema cerrado. Toda la memoria ha sido liberada." << endl;

    return 0;
}
