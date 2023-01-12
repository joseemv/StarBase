#include <iostream>
#include <vector>
#include <string>
#include "Util.h"
#include "Container.h"
#include "Ship.h"
#include "Base.h"

using namespace std;

const string askId = "Container id: ";
const string askName = "Ship name: ";

// Constructor
Base::Base(string name)
{
	this -> name = name;
}

// Destructor
Base::~Base() {}

string Base::getName() const
{
	return name;
}

unsigned int Base::getNumContainers() const
{
	unsigned int numContainers = 0;

	numContainers = containers.size();

	return numContainers;
}

unsigned int Base::getNumShips() const
{
	unsigned int numShips = 0;

	numShips = ships.size();

	return numShips;
}

Container Base::getContainer(unsigned int id) const
{
	unsigned int i = 0;
	int pos = -1;

	for (i = 0; ((i < getNumContainers()) && (pos == -1)); i++)
	{
		if (containers[i].getId() == id)
		{
			pos = i;
		}
	}

	if (pos == -1)
	{
		throw ERR_CONTAINER_ID;
	}

	else
	{
		return containers[pos];
	}
}

Ship* Base::getShip(string name) const
{
    Ship *pointer = NULL;
	unsigned int i = 0;
	int ship = -1;

	for (i = 0; ((i < getNumShips()) && (ship == -1)); i++)
	{
		if (ships[i]->getName() == name)
		{
			ship = i;
		}
	}

	if (ship == -1)
	{
		throw ERR_SHIP_NAME;
	}

	else
	{
		pointer = ships[ship];
	}	

	return pointer;
}

int Base::searchContainer(unsigned int id) const
{
	unsigned int i = 0;
	int con = -1;

	for (i = 0; ((i < getNumContainers()) && (con == -1)); i++)
	{
		if (containers[i].getId() == id)
		{
			con = i;
		}
	}
	
	return con;
}

int Base::searchShip(string name) const
{
	unsigned int i = 0;
	int ship = -1;

	for (i = 0; ((i < getNumShips()) && (ship == -1)); i++)
	{
		if (ships[i]->getName() == name)
		{
			ship = i;
		}
	}

	return ship;	
}

// -----------------Funciones auxiliares distribución-----------------

void Base::checkErrorsManual(int ship, int con)
{
	unsigned int finalWeight = 0;

	if (ships[ship]->getNumContainers() == ships[ship]->getMaxContainers()) // Límite de contenedores alcanzado
	{
		Util::error(ERR_SHIP_NO_MORE_CONTAINERS);
	}

	else
	{		
		finalWeight = containers[con].getWeight() + ships[ship]->getWeight();	

		if (finalWeight <= ships[ship]->getMaxWeight()) // Límite de peso alcanzado
		{		
			ships[ship]->addContainer(containers[con]);
			containers.erase(containers.begin()+con);
		}

		else			
		{		
			Util::error(ERR_SHIP_NO_MORE_WEIGHT); 
		}
	}	
}

int Base::chooseCon()
{
	unsigned int i = 0;
	unsigned int maxValueCon = -1;
	unsigned int maxValue = 0;
	unsigned int numContainers = 0;
	unsigned int value = 0;
	unsigned int weight = 0;
	unsigned int lastConWeight = 0;

	numContainers = getNumContainers();

	for (i = 0; i < numContainers; i++) // Recorre los contenedores
	{
		value = containers[i].getValue();

		if (value > maxValue) // Elige contenedor de más valor
		{
			maxValue = value;
			maxValueCon = i;         
		}

		else if (value == maxValue) // Si es igual a otro valor máximo
		{
			weight = containers[i].getWeight();
			lastConWeight = containers[maxValueCon].getWeight();

			if (weight < lastConWeight) // Elige el contenedor de menos peso
			{
				maxValue = value;
				maxValueCon = i;
			}
		}  
	}

	return maxValueCon; // Devuelve el contenedor elegido
}

bool Base::canDivide(int con)
{
	unsigned int value = 0;
	unsigned int weight = 0;
	unsigned int leftWeight = 0;
	unsigned int leftValue = 0;
	bool result = false;  

	weight = containers[con].getWeight();
	value = containers[con].getValue();
	leftWeight = (weight - (weight / 2));
	leftValue = (value - (value / 2));

	if (((weight / 2) >= Container::kMINWEIGHT) && (leftWeight >= Container::kMINWEIGHT)) // Comprueba que los nuevos contenedores alcancen el peso mínimo
	{
		if (((value / 2) >= Container::kMINVALUE) && (leftValue >= Container::kMINVALUE)) // Comprueba que los nuevos contenedores alcancen el valor mínimo
		{
			result = true;
		}
	}

	return result;
}

int Base::chooseShipOrDiv(int con)
{
	unsigned int i = 0;
	unsigned int minValue = 0;
	unsigned int minShip = -3;
	unsigned int spareWeight = 0;
	unsigned int finalWeight = 0;
	unsigned int numShips = 0;
	bool found = false; // Necesitamos encontrar al menos uno para establecer el valor mínimo
	bool needDiv = false; // Tendrán prioridad los que no necesiten dividirse

	numShips = getNumShips();
	for (i = 0; ((i < numShips) && (con >= 0)); i++) // Recorre las naves de la base
	{
		if (ships[i]->getNumContainers() < ships[i]->getMaxContainers()) // Comprueba que quepa contenedor
		{
			spareWeight = (ships[i]->getMaxWeight() - ships[i]->getWeight());

			if (spareWeight >= Container::kMINWEIGHT) // Comprueba que permita el peso mínimo de un contenedor
			{
				finalWeight = (containers[con].getWeight() + ships[i]->getWeight());

				if ((finalWeight > (ships[i]->getMaxWeight())) && (!found)) // Si hace falta dividir el contenedor
				{
					if (canDivide(con)) // Si se puede dividir
					{
						minValue = ships[i]->getValue();
						minShip = -1; // Ordena dividir
						found = true;
						needDiv = true;
					}
				}

				else if ((finalWeight <= (ships[i]->getMaxWeight())) && ((ships[i]->getValue() < minValue) || (needDiv) || (!found))) // Si se puede asignar el contenedor
				{
					minValue = ships[i]->getValue(); 
					minShip = i; // Indica el barco al que asignar
					found = true;
					needDiv = false;
				}
			}
		}

		else if ((ships[i]->getNumContainers() >= ships[i]->getMaxContainers()) && (!found)) // No cabe el contenedor
		{
			minShip = -2; // Error
		}    
	} 

	return minShip; // Devuelve el número de la nave elegida o indica si hace falta dividir el contenedor
}

bool Base::availableAction()
{
	bool available = false;
	int con = 0;
	int ship = -2;

	if ((getNumContainers() != 0) && (getNumShips() != 0)) // Si hay contenedores y naves en la base
	{
		con = chooseCon();
		ship = chooseShipOrDiv(con);

		if (ship > -2) // Si se pueden almacenar o dividir
		{
			available = true;
		}
	}  

	return available;
}

// -----------------------Funciones auxiliares-----------------------

Container Base::selectContainer(int ship, int id) const
{
	return ships[ship]->getContainer(id);
}

void Base::divideContainer(int con)
{
	unsigned int weight = 0;
	unsigned int value = 0;

	if (canDivide(con)) // Comprueba que se pueda dividir
	{
		// Crea primer contenedor  
		weight = (containers[con].getWeight() / 2);
		value = (containers[con].getValue() / 2);
		addContainer(weight, value);

		// Crea segundo contenedor  
		weight = (containers[con].getWeight() - (containers[con].getWeight() / 2));
		value = (containers[con].getValue() - (containers[con].getValue() / 2));
		addContainer(weight, value);

		// Borra contenedor original
		containers.erase(containers.begin()+con);
	}
}

// ---------------------------Funciones menú---------------------------

bool Base::addContainer(unsigned int weight, unsigned int value)
{
	bool added = false;

	// Se intenta añadir
	try
	{
		Container newContainer(weight, value);
		containers.push_back(newContainer);
		added = true;
	}

	// Si da error se captura la expeción
	catch (Error &e)
	{
		Util::error(e);
	}

	return added;
}

bool Base::removeContainer(unsigned int id)
{
	bool removed = false;
	int con = -1;

	if (id == 0) // Si no introduce id por parámetro
	{
		cout << askId;
		cin >> id;
	}

	con = Base::searchContainer(id); // Busca el contenedor
	
	if (con >= 0) // Si lo encuentra
	{
		containers.erase(containers.begin()+con); // Lo elimina
		removed = true;
	}
	
	else // Si no lo encuentra
	{
		Util::error(ERR_CONTAINER_ID);
	}

	return removed;
}

bool Base::addShip(Ship* s)
{
	bool exists = false;
	bool added = false;

	if (searchShip(s->getName()) >= 0)
	{
		exists = true;
	}

	if (exists)
	{
		Util::error(ERR_SHIP_NAME);
	}

	else
	{
    	ships.push_back(s);	
    	added = true;	
	}	

	return added;
}

bool Base::removeShip(string name)
{
	bool removed = false;
	unsigned int i = 1;
	unsigned int numContainers = 0;
	int ship = -1;
	vector<Container> tempContainers;

	if (name == "")
	{
		cout << askName;
		cin >> name;
	}

	ship = searchShip(name);

	if (ship >= 0) // Si la encuentra
	{
		numContainers = ships[ship]->getNumContainers();

		for (i = 0; i < numContainers; i++)
		{
			tempContainers = ships[ship]->releaseContainers();
			containers.insert(containers.end(), tempContainers.begin(), tempContainers.end());
		}

		ships.erase(ships.begin()+ship); // Elimina la nave
		removed = true;
	}

	else // Si no la encuentra
	{
		Util::error(ERR_SHIP_NAME);
	}

	return removed;
}

bool Base::manualDistribution(unsigned int id, string name)
{
	bool assigned = false;
	int con = 0;
	int ship = 0;

	if (id == 0) // Si no introduce id por parámetro
	{
		cout << askId;
		cin >> id;
   		cin.ignore();
	}
	con = searchContainer(id);
	if (con < 0) // No existe el contenedor
	{
		Util::error(ERR_CONTAINER_ID);
	}
	else
	{
		if (name == "") // Si no introduce nombre por parámetro
		{
			cout << askName;
			getline(cin, name);
		}
		ship = searchShip(name);		
		if (ship < 0) // Nave no encontrada
		{
			Util::error(ERR_SHIP_NAME);
		}
		else
		{
			try
			{
				checkErrorsManual(ship, con);
				assigned = true;
			}
			catch (Error &e)
			{
				Util::error(e);
			}
		}
	}
	return assigned;
}

bool Base::unassignContainer(unsigned int id, string name)
{
	bool unassigned = false;
	int ship = 0;

	if (name == "") // Si no introduce nombre por parámetro
	{
		cout << askName;
		getline(cin, name);
	}
	ship = searchShip(name);

	if (ship >= 0) // Si encuentra la nave
	{
		if (id == 0) // Si no introduce id por parámetro
		{
			cout << askId;
			cin >> id;
		}

		if (ships[ship]->searchContainer(id) >= 0) // Si encuentra el contenedor
		{
			containers.push_back(selectContainer(ship, id)); // Descarga contenedor
			ships[ship]->removeContainer(id); // Lo elimina de la nave
			unassigned = true;
		}

		else // Si no existe el contenedor
		{
			Util::error(ERR_CONTAINER_ID);
		}
	}

	else // Si no existe la nave
	{
		Util::error(ERR_SHIP_NAME);
	}	

	return unassigned;
}

void Base::automaticDistribution()
{
	int con = 0;
	int ship = 0;

	while (availableAction()) // Mientras se pueda hacer algo
	{
		con = chooseCon();
		ship = chooseShipOrDiv(con);

		if (chooseShipOrDiv(con) > -2) // Si la nave reune los requisitos mínimos
		{
			while (ship == -1) // Necesita dividir contenedor
			{
				divideContainer(con);
				con = chooseCon();
				ship = chooseShipOrDiv(con);
			}

			if (ship >= 0) // Se almacena contenedor correctamente
			{
				if(ships[ship]->admitsContainer(containers[con]))
				{
					ships[ship]->addContainer(getContainer(containers[con].getId()));
					containers.erase(containers.begin()+con);
				}
			}  
		}
	}
}

void Base::clearAssignations()
{
	unsigned int i = 0;
	unsigned int numShips = 0;
	vector<Container> tempContainers;

	numShips = getNumShips();

	for (i = 0; i < numShips; i++)
	{
		tempContainers = ships[i]->releaseContainers();
		containers.insert(containers.end(), tempContainers.begin(), tempContainers.end());;
	}
}

ostream& operator<<(ostream &os, const Base &b)
{
	unsigned int i = 0;
	unsigned int j = 0;

	os << "Name: " << b.name << endl;
	os << "Containers:" << endl;

	for (i = 0; (i < b.containers.size()); i++) // Lista contenedores pendientes
	{  
		os << b.containers[i] << endl;
	}

	os << "Ships:" << endl;

	for (j = 0; (j < b.ships.size()); j++) // Lista naves pendientes
	{  
		os << *(b.ships[j]);
	}

	return os;
}