#include <iostream>
#include <vector>
#include <string>
#include "Util.h"
#include "Container.h"
#include "Ship.h"

using namespace std;

const string askName = "Ship name: ";
const string askMaxCon = "Ship max. containers: ";
const string askMaxWeight = "Ship max. weight: ";

// Constructor
Ship::Ship()
{
	string name = "";
	unsigned int maxContainers = 0;
	unsigned int maxWeight = 0;

	cout << askName;
	getline(cin, name);
	cout << askMaxCon;
	cin >> maxContainers;

	if (maxContainers < kMINCONTAINERS) // Si los contenedores máximos son muy pocos
	{
		throw ERR_SHIP_MAXCONTAINERS;
	}

	else
	{
		cout << askMaxWeight;
		cin >> maxWeight;

		if (maxWeight < kMINWEIGHT) // Si el peso máximo es muy bajo
		{
			throw ERR_SHIP_MAXWEIGHT;
		}

		else
		{
            this -> name = name;  
            this -> maxContainers = maxContainers;
            this -> maxWeight = maxWeight;	
		}
	}
}

// Constructor con parámetros
Ship::Ship(string name, unsigned int maxContainers, unsigned int maxWeight)
{

	if (maxContainers < kMINCONTAINERS) // Si los contenedores máximos son muy pocos
	{
		throw ERR_SHIP_MAXCONTAINERS;
	}

	else
	{
		if (maxWeight < kMINWEIGHT) // Si el peso máximo es muy bajo
		{
			throw ERR_SHIP_MAXWEIGHT;
		}

		else
		{
            this -> name = name;  
            this -> maxContainers = maxContainers;
            this -> maxWeight = maxWeight;	
		}
	}
}

// Destructor
Ship::~Ship() {}

string Ship::getName() const
{
	return name;
}

unsigned int Ship::getWeight() const
{
	unsigned int i = 0;
	unsigned int weight = 0;

	for (i = 0; i < containers.size(); i++)
	{
		weight = weight + containers[i].getWeight();
	}

	return weight;
}

unsigned int Ship::getValue() const
{
	unsigned int i = 0;
	unsigned int value = 0;

	for (i = 0; i < containers.size(); i++)
	{
		value = value + containers[i].getValue();
	}

	return value;
}

unsigned int Ship::getMaxWeight() const
{
	return maxWeight;
}

unsigned int Ship::getMaxContainers() const
{
	return maxContainers;
}

unsigned int Ship::getNumContainers() const
{
	return containers.size();
}

int Ship::searchContainer(unsigned int id) const
{
	int con = -1;
	unsigned int i = 0;

    for (i = 0; ((i < getNumContainers()) && (con == -1)); i++)
    {
		if (containers[i].getId() == id)
		{
			con = i;
		}
    }

    return con;
}

bool Ship::admitsContainer(const Container &c) const
{
	bool admits = false;
	unsigned int i = 0;
	unsigned int weight = 0;
	unsigned int numContainers = 0;

	numContainers = getNumContainers();

	if (numContainers < getMaxContainers())
	{
		for (i = 0; i < numContainers; i++)
		{
			weight = weight + containers[i].getWeight();
		}

		if (weight <= getMaxWeight())
		{
			admits = true;
		}
	}

	return admits;
}

Container Ship::getContainer(unsigned int id) const
{
	int con = -1;

	con = searchContainer(id);

	if (con == -1)
	{
		throw ERR_CONTAINER_ID;
	}

	else
	{
		return(containers[con]);
	}
}

bool Ship::addContainer(const Container &c)
{
	bool canAdd = false;
	unsigned int i = 0;
	unsigned int weight = 0;
	unsigned int numContainers = 0;

	numContainers = getNumContainers();

	if (numContainers < getMaxContainers())
	{
		for (i = 0; i < numContainers; i++)
		{
			weight = weight + containers[i].getWeight();
		}

		if (weight <= getMaxWeight())
		{
			canAdd = true;
    		containers.push_back(c);
		}	

		else
		{
			Util::error(ERR_SHIP_NO_MORE_WEIGHT);
		}	
	}

	else
	{
		Util::error(ERR_SHIP_NO_MORE_CONTAINERS);
	}

	return canAdd;
}

bool Ship::removeContainer(unsigned int id)
{
	bool exists = false;
	int con = 0;

	con = searchContainer(id);

	if (con >= 0)
	{
		exists = true;
		containers.erase(containers.begin()+con);
	}

	if (!exists)
	{
		Util::error(ERR_CONTAINER_ID);
	}

	return exists;
}

vector<Container> Ship::releaseContainers()
{
	vector<Container> returnContainers;
	returnContainers = containers;

	containers.clear();
	return returnContainers;
}

ostream& operator<<(ostream &os, const Ship &s)
{
	unsigned int i = 0;

	os << "{" << s.getName() << ": " << s.getWeight() << " (";
	os << s.getMaxWeight() << "), " << s.getNumContainers() << " (";
	os << s.getMaxContainers() << "), " << s.getValue() << endl; 

	for (i = 0; i < s.getNumContainers(); i++)
	{  
   		os << s.containers[i];
	}

	os << endl << "}" << endl;

	return os;
}