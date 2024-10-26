#include <iostream>
using namespace std;

/*
Un centro de formación profesional está intentando desarrollar una aplicación que
permita gestionar datos de los estudiantes. Para ello, necesitan un sistema en el que
los usuarios (estudiantes o profesores) puedan trabajar los siguientes recursos:*
*/


/*
Gestionar a los estudiantes de cada aula:
Si es rol profe → almacenar y visualizar datos de sus estudiantes.Para ello se utilizará un almacén personal asociado a cada profesor.
Un profesor solo puede tener una clase y un estudiante solo puede estar asociado a un profesor.
Los datos que se deben ver de sus alumnos son: Nombre, apellido y notas por asignatura. Los datos que se deben ver de la clase son: Número de inscritos, listado de estudiantes.
*/
//PUNTEROS A TODO

class Student {
private:
    string firstName;
    string lastName;
    float* grades;
    int gradeCount;

public:
    // Constructor
    Student(string firstName, string lastName, float* Grades, int gradeCount) {
        this->firstName = firstName;
        this->lastName = lastName;
        this->gradeCount = gradeCount;
        this->grades = new float[gradeCount]; // Inicializa el array de notas
        for (int i = 0; i < gradeCount; i++) {
            this->grades[i] = Grades[i]; // Copia las notas proporcionadas
        }
    }

    // Destructor
    ~Student() {
        delete[] grades; // Libera el array de notas
    }

    // Getters
    string getFirstName() const {
        return firstName;
    }

    string getLastName() const {
        return lastName;
    }

    int getGradeCount() const {
        return gradeCount;
    }

    // Metodo para asignar y acceder a las notas
    void setGrade(int index, float grade) {
        if (index >= 0 && index < gradeCount) {
            grades[index] = grade;
        }
    }

    // Metodo para mostrar las notas del estudiante
    void displayGrades() const {
        cout << "Notas de " << firstName << " " << lastName << ": ";
        for (int i = 0; i < gradeCount; i++) {
            cout << grades[i] << " ";
        }
        cout << endl;
    }
};

class Teacher {
private:
    string name;
    Student** students;
    int capacity;
    int studentCount;

public:
    // Constructor
    Teacher(string name, int capacity) {
        this->name = name;
        this->students = new Student*[capacity]; // Almacén dinámico de estudiantes
        this->capacity = capacity;
        this->studentCount = 0;
    }

    string getName() const {
        return name;
    }
    int getStudentCount() const {
        return studentCount;
    }



    // Destructor
    ~Teacher() {
        for (int i = 0; i < studentCount; i++) {
            delete students[i]; // Libera la memoria de cada estudiante
        }
        delete[] students; // Libera el array de estudiantes
    }

    // Metodo para agregar estudiantes
    void addStudent(Student* student) {
        if (studentCount < capacity) {
            students[studentCount++] = student;
        } else {
            cout << "Capacidad máxima alcanzada\n";
        }
    }

    // Metodo para listar estudiantes y sus notas
    void listStudents() const {
        cout << "Estudiantes de " << name << ":\n";
        for (int i = 0; i < studentCount; i++) {
            cout << students[i]->getFirstName() << " " << students[i]->getLastName() << endl;
            students[i]->displayGrades(); // Muestra las notas de cada estudiante
        }
    }
};


/*
Mochila de “material digital”.
Rol de “Estudiante”
El estudiante debe poder tener una mochila de “material digital”:
Almacenará “documentos” de actividades realizadas, proyectos entregados, etc.
Un documento, será un objeto “fichero” cargado en la mochila, que leerá del disco un fichero.txt con información del estudiante.
El alumno podrá cargar en la mochila varios “ficheros” de datos, y podrá ver su contenido en la aplicación.
*/

/*
Calculadora avanzada:
Se debe incluir una funcionalidad de calculadora, que aparte de realizar operaciones básicas, también debe poder operar con matrices de n elementos, pudiendo ser las operaciones de suma, resta, multiplicación y división, calcular la matriz traspuesta a la dada, y, en caso de ser una matriz cuadrática, obtener su determinante.
*/

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
        bool guessed = false; // Bandera para indicar si el jugador ha adivinado el número correcto utilize bool para no utilizar break en while cuando termina su trabajo

        // Bucle infinito hasta que el jugador adivina el número correcto
        while (!guessed) {
            cout << "Enter your guess (1-100): "; // Solicita al jugador que ingrese su adivinanza
            cin >> guess; // Captura el número ingresado por el jugador
            (*attempts)++; // Incrementa el contador de intentos utilizando el puntero

            // Si el número adivinado es menor que el número secreto
            if (guess < *secret_number) {
                cout << "Too low. Try again!" << endl; // Informa al jugador que su adivinanza es demasiado baja
            }
            // Si el número adivinado es mayor que el número secreto
            else if (guess > *secret_number) {
                cout << "Too high. Try again!" << endl; // Informa al jugador que su adivinanza es demasiado alta
            }
            // Si el jugador adivina correctamente el número secreto
            else {
                cout << "Congratulations! You guessed the number in " << *attempts << " attempts!" << endl; // Felicita al jugador
                guessed = true; // Cambia la bandera a verdadero para salir del bucle
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

// Función principal
int main() {
    auto* game = new GuessTheNumber(); // Crea una instancia del juego dinámicamente
    game->play(); // Llama a la función "play" para iniciar el juego
    delete game; // Elimina la instancia del juego, llamando al destructor
    return 0;
}

