#include <iomanip>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <list>

using namespace std;

class TPunto
{
public :
	int id;
	string nombre;
	float x;
	float y;
	float distancia;
	string estado;
};

// Clase contenedor
class CMapa
{
	string nombreArchivo;
	string ciudad;
	vector<TPunto> listaPuntos; // Agregar puntos relacion 1 a muchos	
public:

	// Declaracion de constructores
	CMapa()
	{
		ciudad = "Lima";
		nombreArchivo = "coordenadas.csv";
	}

	CMapa(string ciudad, string nombreArchivo)
	{
		this->ciudad = ciudad;
		this->nombreArchivo = nombreArchivo;
	}

	void obtenerData(string csv){

		ifstream file (csv); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/		

		file.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );

		string identificador, posX, posY, Estado;

		if (!file.fail()){
			while(!file.eof()){			
				getline(file, identificador,';');			
				getline(file, posX,';');
				getline(file, posY,';');
				getline(file, Estado,file.widen('\n'));

				if (identificador != ""){
					TPunto punto = {atoi(identificador.c_str()), identificador.c_str(), stof(posX), stof(posY), 0, Estado};
					listaPuntos.push_back(punto);			
				}
			}
		}

	}

	// Metodos de Procesamiento
	void agregarPunto(string nombre,float x, float y, string estado)
	{	
		int idx = listaPuntos.size() + 1;
		TPunto punto = {idx, nombre, x, y, 0, estado};

		listaPuntos.push_back(punto);
		grabarArchivo();
	}

	void eliminarPuntoPorIdentificador(string nombre) //Moverse de atras para adelante en el vector
	{
		for (int i = listaPuntos.size() -1; i >=0; i--)
		{
			if (listaPuntos[i].nombre == nombre)
				listaPuntos.erase(listaPuntos.begin()+i);
		}
		grabarArchivo();
	}

	void armarTabla(vector<TPunto> listaPuntos, int cantidad){
		if (listaPuntos.size() == 0){
			cout << "No hay registros para mostrar.\n\n";
			return;
		}

		cout << setfill('-') << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << endl;
		cout << setfill(' ') << setw(1) << "|" << setw(15) << left << "Nombre" << setw(1) << "|" << setw(10) << left << "Pos X" << setw(1) << "|"  << setw(10) << left << "Pos Y" << setw(1) << "|" << setw(10) << left << "Estado" << setw(1) << "|" << endl;

		for (int i = 0; i <= listaPuntos.size()-1; i++)
		{
			if (i > cantidad) break;
			cout << setfill('-') << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(5) << "-" << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(1)  << endl;
			cout << setfill(' ') << setw(1) << "|" << setw(15) << left << listaPuntos[i].nombre << setw(1) << "|" << setw(10) << left << listaPuntos[i].x << setw(1) << "|"  << setw(10) << left << listaPuntos[i].y << setw(1) << "|" << setw(10) << left << listaPuntos[i].estado << setw(1) << "|" << endl;
		}

		cout << setfill('-') << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << endl;
		cout << "\n\n";

	}

	void listarCienPuntos(int cantidad)
	{
		armarTabla(listaPuntos, 100);
	}

	void listarPuntosAlrededor(float x, float y)
	{
		if (listaPuntos.size() == 0){
			cout << "No hay registros para mostrar.\n\n";
			return;
		}

		string puntoAlrededor = "";
		for (int i = 0; i<=listaPuntos.size() - 1; i++)
		{
			double x_diff = x - listaPuntos[i].x;
			double y_diff = y - listaPuntos[i].y;

			double distancia = sqrt(x_diff * x_diff + y_diff * y_diff); 

			listaPuntos[i].distancia = distancia;

			if (distancia < 15){
				puntoAlrededor = puntoAlrededor + listaPuntos[i].nombre;
			}
		}

		if (puntoAlrededor == ""){
			cout << "No hay puntos cerccanos " << endl;			
			return;
		}

		cout << "Los puntos mas cercanos son " << puntoAlrededor << endl;			
	}

	void hallarDistancia(){
		if (listaPuntos.size() == 0){
			cout << "No hay registros para mostrar.\n\n";
			return;
		}

		string puntoAlrededor = "";
		double totalDistancia = 0;
		for (int i = 0; i<=listaPuntos.size() - 1; i++)
		{
			double x = 0;
			double y = 0;

			if (listaPuntos.size() <= i){
				x = listaPuntos[i + 1].x;
				y = listaPuntos[i + 1].y;
			}

			double x_diff = listaPuntos[i].x - x;
			double y_diff = listaPuntos[i].y - y;

			double distancia = sqrt(x_diff * x_diff + y_diff * y_diff); 

			listaPuntos[i].distancia = distancia;
			totalDistancia += distancia;
		}

		cout << "La distancia promedio es de " << totalDistancia / (listaPuntos.size() - 1)  << endl;			
	}

	void distanciaTodos(){
		if (listaPuntos.size() == 0){
			cout << "No hay registros para mostrar.\n\n";
			return;
		}

		string puntoAlrededor = "";
		double totalDistancia = 0;
		int totalPuntos = listaPuntos.size() - 1;
		for (int i = 0; i<=totalPuntos; i++)
		{
			double xIni = listaPuntos[i].x;
			double yIni = listaPuntos[i].y;

			double x = 0;
			double y = 0;
			if (totalPuntos > i){
				x = listaPuntos[i + 1].x;
				y = listaPuntos[i + 1].y;
			}

			double x_diff = xIni - x;
			double y_diff = yIni - y;

			double distancia = sqrt(x_diff * x_diff + y_diff * y_diff); 
			totalDistancia =+ distancia;

			listaPuntos[i].distancia = distancia;

			if (totalPuntos > i){
				cout << "La distancia del " << listaPuntos[i].nombre << " es " << distancia << endl;			
			}
		}

		cout << "La distancia del " << listaPuntos[totalPuntos].nombre << " es " << totalDistancia << endl;
	}

	void listarPuntosAbiertos()
	{
		vector<TPunto> listaPuntosA;
		for (int i = 0; i<=listaPuntos.size() - 1; i++)
		{
			double x = 0;
			double y = 0;

			if (listaPuntos.size() <= i){
				x = listaPuntos[i + 1].x;
				y = listaPuntos[i + 1].y;
			}

			double x_diff = listaPuntos[i].x - x;
			double y_diff = listaPuntos[i].y - y;

			double distancia = sqrt(x_diff * x_diff + y_diff * y_diff); 

			if (listaPuntos[i].estado == "A" && distancia < 100)
			{
				listaPuntosA.push_back(listaPuntos[i]);
			}
			if (listaPuntos[i].estado == "Abierto" && distancia < 100)
			{
				listaPuntosA.push_back(listaPuntos[i]);
			}
			if (listaPuntos[i].estado == "ABIERTO" && distancia < 100)
			{
				listaPuntosA.push_back(listaPuntos[i]);
			}
			if (listaPuntos[i].estado == "abierto" && distancia < 100)
			{
				listaPuntosA.push_back(listaPuntos[i]);
			}
		}

		armarTabla(listaPuntosA, 100);
	}

	void listarPuntosFiltro(std::string nombre)
	{
		vector<TPunto> listaPuntosFiltro;
		for (int i = 0; i<=listaPuntos.size() - 1; i++)
		{
			if (listaPuntos[i].nombre == nombre){
				listaPuntosFiltro.push_back(listaPuntos[i]);
			}
		}

		armarTabla(listaPuntosFiltro, 100);
	}

	void grabarArchivo()
	{
		// Creando el archivo
		ofstream archivo(nombreArchivo);
		archivo << "Identificador;posX;posY;Estado" << endl;
		for (auto punto: listaPuntos)
			archivo << punto.nombre << ";" << punto.x << ";" << punto.y << ";" << punto.estado << endl;
		archivo.close();
	}
};

// Metodo Menu
int menu()
{
	int opcion = 0;

	do
	{	
			system("clear");
		cout << "MENU" << endl;
		cout << string(4, '~') << endl << endl;

		cout << "1. Agregar una nueva posicion." << endl;
		cout << "2. Eliminar una posicion por nombre." << endl;
		cout << "3. Listar 100 primeros puntos." << endl;
		cout << "4. Listar los puntos ubicados alrededor." << endl;
		cout << "5. Hallar la distancia de cada punto y obtener el promedio." << endl;
		cout << "6. Hallar la mayor distancia entre los puntos. " << endl;
		cout << "7. Hallar la menor distancia entre los puntos." << endl;
	    cout << "8. Listar los puntos con estado abierto a menos de 100 metros." << endl;
		cout << "9. Filtrar ubicacion." << endl;
		cout << "0. Para salir." << endl;
		cout << "Opcion: " << endl;
		cin >> opcion;
		return opcion;
	}
	while (opcion < 0 || opcion > 3);
};

int main()
{
	string ciudad;
	cout << "CIUDAD: ";
	cin >> ciudad;
	cout << "\n\n";
	string nombre;
	string estado;

	string nombreCSV = ciudad + ".CSV";
	CMapa mapa(ciudad, nombreCSV);       // Mapa de Lima

	mapa.obtenerData(nombreCSV);

	int opcion = 0;
	do 
	{
		opcion = menu();    // Opcion del menu
		float x,y;
		int id;
		string estado;
		// Creamos un switch de opciones
		switch (opcion)
		{
		case 1:	
		system("clear");
			cout << "Ingrese el nombre del punto: ";
			cin >> nombre; 
			cout << "Ingrese posicion X: ";
			cin >> x;	

			while(1)
			{				
				if(cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Valor ingresado incorrecto" << endl; 					
				}				
				else
					break;
				cout << "Ingrese posicion X: ";
				cin >> x;
			}

			cout << "Ingrese posicion Y: ";			
			cin >> y;		

			while(1)
			{				
				if(cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Valor ingresado incorrecto" << endl; 					
				}				
				else
					break;
				cout << "Ingrese posicion Y: ";
				cin >> y;
			}
			
			cout << "Ingrese el estado del local: ";
			cin >> estado;

			cout << "\n\n";

			mapa.agregarPunto(nombre,x, y,estado);
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
			
		case 2: 
		system("clear");
			cout << "Ingrese nombre: "; 
			cin >> nombre;		

			while(1)
			{				
				if(cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Valor ingresado incorrecto\n\n"; 					
				}				
				else
					break;
				cout << "Ingrese nombre: "; 
				cin >> nombre;		
			}
			mapa.eliminarPuntoPorIdentificador(nombre);
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		case 3: 
		system("clear");
			mapa.listarCienPuntos(100);
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		case 4:	
		system("clear");
			cout << "Ingrese posicion X: ";
			cin >> x;	

			while(1)
			{				
				if(cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Valor ingresado incorrecto\n\n"; 					
				}				
				else
					break;
				cout << "Ingrese posicion X: ";
				cin >> x;
			}

			cout << "Ingrese posicion Y: ";			
			cin >> y;		

			while(1)
			{				
				if(cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Valor ingresado incorrecto\n\n"; 					
				}				
				else
					break;
				cout << "Ingrese posicion Y: ";
				cin >> y;
			}

			cout << "\n\n";

			mapa.listarPuntosAlrededor(x, y);
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		case 5: 
		system("clear");
			mapa.hallarDistancia();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		case 6: 
		system("clear");
			mapa.distanciaTodos();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		case 7: 
		system("clear");
			mapa.distanciaTodos();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		case 8: 
		system("clear");
			mapa.listarPuntosAbiertos();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		case 9: 
		system("clear");
			string nombre;

			cout << "Ingrese nombre: "<< endl;			
			cin >> nombre;		

			mapa.listarPuntosFiltro(nombre);
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cin.get();
			break;
		}

	}
	while (opcion !=0);

	return 0;
};