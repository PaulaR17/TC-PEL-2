#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/*
Un centro de formación profesional está intentando desarrollar una aplicación que
permita gestionar datos de los estudiantes. Para ello, necesitan un sistema en el que
los usuarios (estudiantes o profesores) puedan trabajar los siguientes recursos:*
*/

/*
Mochila de “material digital”.
Rol de “Estudiante”
El estudiante debe poder tener una mochila de “material digital”:
Almacenará “documentos” de actividades realizadas, proyectos entregados, etc.
Un documento, será un objeto “fichero” cargado en la mochila, que leerá del disco un fichero.txt con información del estudiante.
El alumno podrá cargar en la mochila varios “ficheros” de datos, y podrá ver su contenido en la aplicación.
*/

// Implementación manual de un array dinámico en vez de usar la ya creada "vector"


template<typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    DynamicArray(size_t initialCapacity = 2) : size(0), capacity(initialCapacity) {
        data = new T[capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void add(const T& element) {
        if (size == capacity) {
            resize();
        }
        data[size++] = element;
    }

    int find(const T& element) const {
        for (size_t i = 0; i < size; i++) {
            if (data[i] == element) {
                return i;
            }
        }
        return -1;
    }

    void removeAt(size_t index) {
        if (index < size) {
            for (size_t i = index; i < size - 1; i++) {
                data[i] = data[i + 1];
            }
            size--;
        }
    }

    size_t getSize() const {
        return size;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }
};


// Clase para la mochila digital que contiene los documentos del estudiante
class DigitalBackpack {
private:
    DynamicArray<string> documents;

public:
    void addDocument(const string& filename) {
        ifstream file(filename);
        if (file) {
            documents.add(filename);
            cout << "Documento '" << filename << "' agregado exitosamente.\n";
        } else {
            cout << "Error: No se pudo abrir el archivo " << filename << ".\n";
        }
        file.close();
    }

    void removeDocument(const string& filename) {
        int index = documents.find(filename);
        if (index != -1) {
            documents.removeAt(index);
            cout << "Documento '" << filename << "' eliminado correctamente.\n";
        } else {
            cout << "Error: El documento '" << filename << "' no se encuentra en la mochila.\n";
        }
    }

    void viewDocuments() const {
        if (documents.getSize() == 0) {
            cout << "La mochila está vacía.\n";
            return;
        }

        cout << "Documentos en la mochila:\n";
        for (size_t i = 0; i < documents.getSize(); i++) {
            cout << "Contenido del archivo '" << documents[i] << "':\n";
            ifstream file(documents[i]);
            if (!file) {
                cout << "No se pudo abrir el archivo " << documents[i] << endl;
                continue;
            }
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            cout << "--------------------------\n";
            file.close();
        }
    }
};

/*
Gestionar a los estudiantes de cada aula:
Si es rol profe → almacenar y visualizar datos de sus estudiantes.Para ello se utilizará un almacén personal asociado a cada profesor.
Un profesor solo puede tener una clase y un estudiante solo puede estar asociado a un profesor.
Los datos que se deben ver de sus alumnos son: Nombre, apellido y notas por asignatura. Los datos que se deben ver de la clase son: Número de inscritos, listado de estudiantes.
*/
//PUNTEROS A TODO
// Clase base User

class User {
protected:
    int id;
    string name;
    string surname;

public:
    User(int id, const string& name, const string& surname)
        : id(id), name(name), surname(surname) {}
    int getId() const { return id; }
    virtual void showInfo() const {
        cout << "ID: " << id << ", Nombre: " << name << ", Apellido: " << surname << endl;
    }

    virtual ~User() {}
};

class Student : public User {
private:
    DynamicArray<float> grades;
    DigitalBackpack backpack; // Agregar la mochila digital en Student

public:
    // Constructor de Student modificado para aceptar notas iniciales
    Student(int id, const string& name, const string& surname, const float* initialGrades = nullptr, size_t gradeCount = 0)
        : User(id, name, surname) {
        if (initialGrades != nullptr) {
            for (size_t i = 0; i < gradeCount; ++i) {
                grades.add(initialGrades[i]);
            }
        }
    }

    void addGrade(float grade) {
        grades.add(grade);
    }

    void showGrades() const {
        cout << "Grades for " << name << " " << surname << ": ";
        for (size_t i = 0; i < grades.getSize(); i++) {
            cout << grades[i] << " ";
        }
        cout << endl;
    }

    void showInfo() const override {
        User::showInfo();
        showGrades();
    }

    DigitalBackpack* getBackpack() { // Método para acceder a la mochila
        return &backpack;
    }
};
// Teacher class adjusted with fixed constructor
class Teacher : public User {
private:
    DynamicArray<Student*> students;
    int maxStudents;

public:
    Teacher(int id, const string& name, const string& surname, int maxStudents = 1)
        : User(id, name, surname), maxStudents(maxStudents) {}

    void addStudent(Student* student) {
        if (students.getSize() < maxStudents) {
            students.add(student);
            cout << "Estudiante añadido a la clase.\n";
        } else {
            cout << "El profesor ya tiene el número máximo de estudiantes asignados.\n";
        }
    }

    void listStudents() const {
        cout << "Estudiantes de " << name << " " << surname << ":\n";
        for (size_t i = 0; i < students.getSize(); i++) {
            students[i]->showInfo();
        }
    }
    size_t getStudentCount() const {  // New method to return student count
        return students.getSize();
    }
    void showInfo() const override {
        User::showInfo();
        cout << "Número de estudiantes: " << students.getSize() << endl;
    }
};

class Admin : public User {
public:
    Admin(int id, const string& name, const string& surname)
        : User(id, name, surname) {}

    void addUser(User* user, DynamicArray<User*>& users) {
        users.add(user);
        cout << "Usuario agregado con ID: " << user->getId() << endl;
    }

    void removeUser(int id, DynamicArray<User*>& users) {
        for (size_t i = 0; i < users.getSize(); i++) {
            if (users[i]->getId() == id) {
                delete users[i];
                users.removeAt(i);
                cout << "Usuario con ID: " << id << " eliminado.\n";
                return;
            }
        }
        cout << "Usuario con ID: " << id << " no encontrado.\n";
    }
};

/*
Calculadora avanzada:
Se debe incluir una funcionalidad de calculadora, que aparte de realizar operaciones básicas, también debe poder operar con matrices de n elementos, pudiendo ser las operaciones de suma, resta, multiplicación y división, calcular la matriz traspuesta a la dada, y, en caso de ser una matriz cuadrática, obtener su determinante.
*/
class AdvancedCalculator
{
public:
    AdvancedCalculator() {}

    int add(int a, int b)
    {
        return a + b;
    }

    int substract(int a, int b)
    {
        return a - b;
    }

    int multiply(int a, int b)
    {
        return a * b;
    }
    float divide(int a, int b)
    {
        if (b == 0)
        {
            cout << "No se puede dividir por 0\n"<<endl;
            return 0;
        }
        return (float)a/b;
    }

    void fillMatrix(int* mat, int rows, int cols)
    {
        cout << "Escriba los elementos de la matriz (" << rows << "x" << cols << "):" << endl;
        for (int i = 0; i < rows*cols; i++)
        {
            cin >> mat[i];
        }
    }

    void displayMatrix(int* mat, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << mat[i * cols + j] << " ";
            }
            cout << endl;
        }
    }

    bool addMatrices(int* mat1, int* mat2, int* result, int rows1, int cols1, int rows2, int cols2) {
        if (rows1 != rows2 || cols1 != cols2) {
            cout << "ERROR: Para sumarse las matrices deben tener las mismas dimensiones." << endl;
            return false;
        }
        for (int i = 0; i < rows1 * cols1; i++) {
            result[i] = mat1[i] + mat2[i];
        }
        return true;
    }
    bool subtractMatrices(int* mat1, int* mat2, int* result, int rows1, int cols1, int rows2, int cols2) {
        if (rows1 != rows2 || cols1 != cols2) {
            cout << "ERROR: Para restarse las matrices deben tener las mismas dimensiones." << endl;
            return false;
        }
        for (int i = 0; i < rows1 * cols1; i++) {
            result[i] = mat1[i] - mat2[i];
        }
        return true;
    }

    bool multiplyMatrices(int* mat1, int* mat2, int* result, int rows1, int cols1, int rows2, int cols2) {
        if (cols1 != rows2) {
            cout << "ERROR: Para que se  multipliquen el número de columnas de la primera matriz debe ser igual al número de filas de la segunda." << endl;
            return false;
        }
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                result[i * cols2 + j] = 0;
                for (int k = 0; k < cols1; k++) {
                    result[i * cols2 + j] += mat1[i * cols1 + k] * mat2[k * cols2 + j];
                }
            }
        }
        return true;
    }


    ~AdvancedCalculator() {}

};

void basicOperationsMenu(AdvancedCalculator* calculator) {
    int num1, num2, op;
    cout << "Ingrese dos números: ";
    cin >> num1 >> num2;
    cout << "Seleccione la operación: 1) Suma 2) Resta 3) Multiplicación 4) División" << endl;
    cin >> op;

    switch(op) {
    case 1: cout << "Resultado: " << calculator->add(num1, num2) << endl; break;
    case 2: cout << "Resultado: " << calculator->substract(num1, num2) << endl; break;
    case 3: cout << "Resultado: " << calculator->multiply(num1, num2) << endl; break;
    case 4: cout << "Resultado: " << calculator->divide(num1, num2) << endl; break;
    default: cout << "Operación no válida." << endl;
    }
}
void matrixOperationsMenu(AdvancedCalculator* calculator) {
    int rows, cols, rows2, cols2;
    cout << "Ingrese el número de filas y columnas de la primera matriz: ";
    cin >> rows >> cols;
    int* mat1 = new int[rows * cols];
    calculator->fillMatrix(mat1, rows, cols);

    cout << "Ingrese el número de filas y columnas de la segunda matriz: ";
    cin >> rows2 >> cols2;
    int* mat2 = new int[rows2 * cols2];
    calculator->fillMatrix(mat2, rows2, cols2);

    int matrixOp;
    cout << "Seleccione la operación de matrices: 1) Suma 2) Resta 3) Multiplicación" << endl;
    cin >> matrixOp;

    int* result = new int[rows * cols];  // Tamaño ajustado según la operación

    if (matrixOp == 1) {  // Suma
        if (rows == rows2 && cols == cols2) {
            if (calculator->addMatrices(mat1, mat2, result, rows, cols, rows2, cols2)) {
                cout << "Resultado:" << endl;
                calculator->displayMatrix(result, rows, cols);
            }
        } else {
            cout << "ERROR: Las matrices deben tener las mismas dimensiones." << endl;
        }
    }
    else if (matrixOp == 2) {  // Resta
        if (rows == rows2 && cols == cols2) {
            if (calculator->subtractMatrices(mat1, mat2, result, rows, cols, rows2, cols2)) {
                cout << "Resultado:" << endl;
                calculator->displayMatrix(result, rows, cols);
            }
        } else {
            cout << "ERROR: Las matrices deben tener las mismas dimensiones." << endl;
        }
    }
    else if (matrixOp == 3) {  // Multiplicación
        if (cols == rows2) {
            int* resultMult = new int[rows * cols2];
            if (calculator->multiplyMatrices(mat1, mat2, resultMult, rows, cols, rows2, cols2)) {
                cout << "Resultado:" << endl;
                calculator->displayMatrix(resultMult, rows, cols2);
            }
            delete[] resultMult;
        } else {
            cout << "ERROR: para multiplicarlas El número de columnas de la primera matriz debe ser igual al número de filas de la segunda." << endl;
        }
    }
    else {
        cout << "Operación de matrices no válida." << endl;
    }
    //liberamos memoria...
    delete[] mat1;
    delete[] mat2;
    delete[] result;
}

void calculatorMenu(AdvancedCalculator* calculator) {
    int operation;
    while (true) {
        cout << "\n--- MENÚ CALCULADORA ---\n";
        cout << "1. Operaciones básicas\n";
        cout << "2. Operaciones con matrices\n";
        cout << "0. Volver al Menú Principal\n";
        cout << "Elija una opción: ";
        cin >> operation;

        if (operation == 1) {
            basicOperationsMenu(calculator);
        } else if (operation == 2) {
            matrixOperationsMenu(calculator);
        } else if (operation == 0) {
            cout << "Volviendo al menú principal..." << endl;
            break;
        } else {
            cout << "Operación no válida, vuelva a intentarlo." << endl;
        }
    }
}
/*
Sistema de altas y bajas de usuarios.
“Administrador”.
Dicho sistema debe poder manejarse los datos de los usuarios, de forma que se les pueda dar de alta o baja a cada usuario.
Se les debe poder gestionar los datos del nombre, el apellido, y numero de usuario.
Cada usuario tendrá un id irrepetible.
Los usuarios deberán eliminarse de memoria cuando se den de baja.
*/

/*
Un poco de diversión
El sistema debe generar un número aleatorio entre 1 y 100
El usuario tiene que adivinarlo.
Cada vez que el usuario introduce un valor, el sistema debe indicar si el valor es mayor o menor que el introducido.
Cuando el usuario adivine el valor, debe imprimir por pantalla el número de veces que inentó adivinarlo.
*/

#include <iostream>
#include <random>    // Biblioteca para generar números aleatorios usando C++11
// Utilicé C++11 y su biblioteca <random> para la generación de números aleatorios
// porque al usar srand(time(0)) en C++ generaba secuencias de números predecibles.
// La consola advirtió que el generador con la hora actual como semilla tiene poca aleatoriedad.
// De la biblioteca de C++11, usé los generadores mt19937 (Mersenne Twister) y random_device,
// que proporcionan mejor aleatoriedad y son más seguros.



// Definición de la clase "GuessTheNumber"
class GuessTheNumber {
private:
    int* secret_number; // Puntero para almacenar el número secreto
    int* attempts;      // Puntero para contar el número de intentos del jugador

public:
    // Constructor de la clase
    GuessTheNumber() {
        // random_device: Genera aleatoriedad.
        // Se utiliza para obtener una semilla inicial verdaderamente aleatoria.
        // La diferencia entre srand(time(0)) y random_device es que random_device no se basa en el tiempo actual y utiliza valores del hardware para ser menos predecible.
        random_device random;

        // mt19937: Es un generador de números pseudoaleatorios basado en el algoritmo Mersenne Twister.
        // Es mucho más eficiente y proporciona una secuencia de números pseudoaleatorios de alta calidad
        // en comparación con el antiguo rand().
        // Se inicializa con la semilla obtenida de random_device, lo que mejora la seguridad y aleatoriedad.
        mt19937 gen(random());

        // uniform_int_distribution: Es una distribución uniforme que asegura que cada número entero
        // en el rango dado (en este caso de 1 a 100) tenga la misma probabilidad de ser seleccionado.
        // A diferencia del uso directo de % con rand(), esta distribución genera números de manera más
        // equitativa y precisa, especialmente en rangos amplios.
        uniform_int_distribution<> dis(1, 100);

        // Asigna memoria para almacenar el número secreto utilizando "new"
        // y guarda en el puntero secret_number.
        secret_number = new int;

        // Genera un número secreto utilizando el generador Mersenne Twister y la distribución uniforme
        // y almacena el valor resultante en la ubicación de memoria a la que apunta secret_number.
        *secret_number = dis(gen);

        // Asigna memoria para el contador de intentos, que registrará cuántos intentos ha hecho el jugador.
        attempts = new int;

        // Inicializa el contador de intentos a 0.
        *attempts = 0;
    }


    // Función "play" que permite jugar. El "const" garantiza que no modifica los miembros de la clase
    void play() const {
        int guess; // Variable para almacenar el número que el jugador adivina
        bool guessed = false; // Bandera para indicar si el jugador ha adivinado el número correcto, utiliza bool para no utilizar break en while cuando termina su trabajo

        // Bucle infinito hasta que el jugador adivina el número correcto
        while (!guessed) {
            cout << "Introduce tu adivinanza (1-100): "; // Solicita al jugador que ingrese su adivinanza
            try {
                if (!(cin >> guess)) { // Verifica que la entrada sea un número
                    throw invalid_argument("Entrada no válida. Por favor, introduce un número (1-100): ");
                }
                (*attempts)++; // Incrementa el contador de intentos utilizando el puntero

                // Calcula la diferencia entre la adivinanza y el número secreto
                int difference = abs(guess - *secret_number);

                // Si el número adivinado es menor que el número secreto
                if (guess < *secret_number) {
                    if (difference > 10) {
                        cout << "Demasiado bajo. ¡Intentalo de nuevo!" << endl; // Informa al jugador que su adivinanza es demasiado baja
                    } else {
                        cout << "Bajo. ¡Intentalo de nuevo!" << endl; // Informa al jugador que su adivinanza es baja
                    }
                }
                // Si el número adivinado es mayor que el número secreto
                else if (guess > *secret_number) {
                    if (difference > 10) {
                        cout << "Demasiado alto. ¡Intentalo de nuevo!" << endl; // Informa al jugador que su adivinanza es demasiado alta
                    } else {
                        cout << "Alto. ¡Intentalo de nuevo!" << endl; // Informa al jugador que su adivinanza es alta
                    }
                }
                // Si el jugador adivina correctamente el número secreto
                else {
                    cout << "¡Felicidades! Adivinaste el numero en " << *attempts << " intentos!" << endl; // Felicita al jugador
                    guessed = true; // Cambia la bandera a verdadero para salir del bucle
                }
            } catch (invalid_argument& e) {
                cin.clear(); // Limpia el estado de error de cin
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora la entrada no válida
                cout << e.what(); // Muestra el mensaje de error
            }
        }
    }

    // Destructor de la clase, se llama cuando el objeto es destruido
    ~GuessTheNumber() {
        // Libera la memoria asignada dinámicamente para el número secreto y los intentos
        delete secret_number;
        delete attempts;
    }
};


void playGuessTheNumber() {
    auto* game = new GuessTheNumber(); // Crea una instancia del juego
    game->play(); // Llama a la función "play" para iniciar el juego
    delete game; // Libera la memoria
}

void showMainMenu()
{
    cout<<"\n---MENU PRINCIPAL---\n";
    cout << "Seleccione una opción:\n ";
    cout<<"1. Gestion de Estudiantes y Professores\n";
    cout << "2. Mochila de Material Digital\n";
    cout << "3. Calculadora Avanzada\n";
    cout << "4. Sistema de Altas y Bajas de Usuarios\n";
    cout << "5. Juego de Adivinanza\n";
    cout << "6. Salir\n";


}

void manageStudentsAndTeachers() {
     // Crear un profesor con capacidad máxima de 5 estudiantes (puedes cambiar el valor si es necesario)
    Teacher teacher(1,"Profesor","Ejemplo", 5);
    int option;

    while (true) {
        cout << "\n--- GESTIÓN DE ESTUDIANTES Y PROFESORES ---\n";
        cout << "1. Añadir estudiante\n";
        cout << "2. Listar estudiantes\n";
        cout << "3. Ver número de inscritos\n";
        cout << "0. Volver al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> option;

        if (option == 1) {
            int id;
            string firstName, lastName;
            int gradeCount;

            cout << "Ingrese el ID del estudiante: ";
            cin >> id;
            cout << "Ingrese el nombre del estudiante: ";
            cin >> firstName;
            cout << "Ingrese el apellido del estudiante: ";
            cin >> lastName;
            cout << "Ingrese la cantidad de notas: ";
            cin >> gradeCount;

            float* grades = new float[gradeCount];
            cout << "Ingrese las notas:\n";
            for (int i = 0; i < gradeCount; i++) {
                cout << "Nota " << (i + 1) << ": ";
                cin >> grades[i];
            }

            // Crear el nuevo estudiante
            Student* student = new Student(id, firstName, lastName, grades, gradeCount);
            teacher.addStudent(student); // Agregar el estudiante al profesor

            delete[] grades; // Limpiar la memoria de las notas temporalmente usadas
            cout << "Estudiante añadido exitosamente.\n";

        } else if (option == 2) {
            teacher.listStudents();

        } else if (option == 3) {
            cout << "Número de inscritos: " << teacher.getStudentCount() << endl;

        } else if (option == 0) {
            cout << "Volviendo al menú principal...\n";
            break;

        } else {
            cout << "Opción no válida. Intente nuevamente.\n";
        }
    }
}

// Función para crear un nuevo archivo de documento
void createDocument() {
    string filename;
    cout << "Ingrese el nombre del nuevo archivo (con .txt): ";
    cin >> filename;

    ofstream file(filename);
    if (!file) {
        cout << "Error al crear el archivo " << filename << ".\n";
        return;
    }

    cout << "Ingrese el contenido del archivo (escriba 'FIN' en una línea nueva para terminar):\n";
    string line;
    cin.ignore(); // Ignorar el salto de línea previo
    while (true) {
        getline(cin, line);
        if (line == "FIN") break;
        file << line << endl;
    }
    file.close();
    cout << "Archivo '" << filename << "' creado exitosamente.\n";
}

// Menú para la mochila de un estudiante específico
void studentBackpackMenu(Student* student) {
    int option;
    string filename;
    DigitalBackpack* backpack = student->getBackpack();

    while (true) {
        cout << "\n--- MENÚ DE MOCHILA DIGITAL ---\n";
        cout << "1. Crear un nuevo documento\n";
        cout << "2. Añadir documento existente a la mochila\n";
        cout << "3. Eliminar documento de la mochila\n";
        cout << "4. Ver documentos en la mochila\n";
        cout << "0. Volver\n";
        cout << "Elija una opción: ";
        cin >> option;

        switch (option) {
            case 1: {
                cout << "Ingrese el nombre del nuevo archivo (con .txt): ";
                cin >> filename;

                ofstream file(filename);
                if (!file) {
                    cout << "Error al crear el archivo " << filename << ".\n";
                    return;
                }

                cout << "Ingrese el contenido del archivo (escriba 'FIN' para terminar):\n";
                string line;
                cin.ignore();
                while (true) {
                    getline(cin, line);
                    if (line == "FIN") break;
                    file << line << endl;
                }
                file.close();
                cout << "Archivo '" << filename << "' creado exitosamente.\n";
                break;
            }
            case 2:
                cout << "Ingrese el nombre del archivo a agregar: ";
                cin >> filename;
                backpack->addDocument(filename);
                break;
            case 3:
                cout << "Ingrese el nombre del archivo a eliminar: ";
                cin >> filename;
                backpack->removeDocument(filename);
                break;
            case 4:
                backpack->viewDocuments();
                break;
            case 0:
                cout << "Volviendo...\n";
                return;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    }
}
void userManagementSystem(DynamicArray<User*>& users, Admin& admin) {
    int option;

    while (true) {
        cout << "\n--- SISTEMA DE GESTIÓN DE USUARIOS ---\n";
        cout << "1. Añadir usuario\n2. Eliminar usuario\n3. Mostrar usuarios\n0. Volver\n";
        cin >> option;

        if (option == 0) break;
        if (option == 1) {
            int role, id = users.getSize() + 1;
            string name, surname;
            cout << "1. Estudiante\n2. Profesor\nSeleccione el rol: ";
            cin >> role >> name >> surname;

            if (role == 1) users.add(new Student(id, name, surname));
            else if (role == 2) users.add(new Teacher(id, name, surname));
            else cout << "Rol inválido.\n";

        } else if (option == 2) {
            int id;
            cout << "ID del usuario a eliminar: ";
            cin >> id;
            admin.removeUser(id, users);
        } else if (option == 3) {
            for (size_t i = 0; i < users.getSize(); i++) users[i]->showInfo();
        } else {
            cout << "Opción no válida. Intente nuevamente.\n";
        }
    }
}

// ---- Función Principal Unificada ----
int main() {
    AdvancedCalculator* calculator = new AdvancedCalculator();
    Admin admin(1, "Admin", "Principal");
    DynamicArray<User*> users;
    users.add(&admin);

    Student student(2, "Juan", "Perez");
    int choice;

    while (true) {
        showMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                manageStudentsAndTeachers();
            break;
            case 2:
                studentBackpackMenu(&student);
            break;
            case 3:
                calculatorMenu(calculator);
            break;
            case 4:
                userManagementSystem(users, admin);
            break;
            case 5:
                playGuessTheNumber();
            break;
            case 6:
                for (size_t i = 0; i < users.getSize(); i++) delete users[i];
            cout << "Saliendo del programa.\n";
            return 0;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    }
}
