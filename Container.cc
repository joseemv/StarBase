#include <iostream>
#include "Util.h"
#include "Container.h"

using namespace std;

const string askWeight = "Container weight: ";
const string askValue = "Container value: ";

unsigned int Container::nextId = 1;

// Constructor
Container::Container(unsigned int weight, unsigned int value)
{
    if (weight == 0) // Si no introduce peso por parámetro
    {
        cout << askWeight;
        cin >> weight;
    }

    if (weight < kMINWEIGHT)
    {
        throw ERR_CONTAINER_WEIGHT;
    }

    else
    {  
        if (value == 0) // Si no introduce valor por parámetro
        {
            cout << askValue;
            cin >> value;
        }

        if (value < kMINVALUE)
        {
            throw ERR_CONTAINER_VALUE;
        }

        else
        {
            this -> weight = weight;  
            this -> value = value;
            this -> id = nextId;
            nextId++;
        }    
    }
}

// Destructor
Container::~Container() {}

void Container::resetNextId()
{    
    nextId = 1;
}

unsigned int Container::getId() const
{
	return id;
}

unsigned int Container::getWeight() const
{
	return weight;
}

unsigned int Container::getValue() const
{
	return value;
}

void Container::setWeight(unsigned int weight)
{
    if (weight < kMINWEIGHT)
    {
        throw ERR_CONTAINER_WEIGHT;
    }

    else
    {
        this -> weight = weight;
    }	
}

void Container::setValue(unsigned int value)
{
    if (value < kMINVALUE)
    {
        throw ERR_CONTAINER_VALUE;
    }

    else
    {
        this -> value = value;
    }	
}

ostream& operator<<(ostream &os, const Container &c)
{
	os << "[" << c.getId() << " " << c.getWeight() << ":";
	os << c.getValue() << "]";

	return os;
}