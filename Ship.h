#ifndef Ship_H
#define Ship_H

#include <iostream>
#include <vector>
#include "Container.h"

using namespace std;

class Ship
{
		friend ostream& operator<<(ostream &os, const Ship &s);
	protected:
		string name;
		unsigned int weight;
		unsigned int value;
		unsigned int maxWeight;
		unsigned int maxContainers;
		vector<Container> containers; // Composición
	public:
		static const unsigned int kMINCONTAINERS = 5;
		static const unsigned int kMINWEIGHT = 500;
		Ship();
		Ship(string name, unsigned int maxContainers, unsigned int maxWeight);
		~Ship();
		string getName() const;
		unsigned int getWeight() const;
		unsigned int getValue() const;
		unsigned int getMaxWeight() const;
		unsigned int getMaxContainers() const;
		unsigned int getNumContainers() const;
		int searchContainer(unsigned int id) const;
		bool admitsContainer(const Container &c) const;
		Container getContainer(unsigned int id) const;
		bool addContainer(const Container &c);
		bool removeContainer(unsigned int id);
		vector<Container> releaseContainers();
};	

#endif