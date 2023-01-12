#ifndef Base_H
#define Base_H

#include <iostream>
#include <vector>
#include "Container.h"
#include "Ship.h"

using namespace std;

class Base
{
		friend ostream& operator<<(ostream &os, const Base &b);
	protected:
		string name;
		vector<Container> containers; // Composición		
		vector<Ship*> ships; // Agregación
	private:
		void checkErrorsManual(int ship, int con);
		void divideContainer(int con);
		int chooseCon();
		int chooseShipOrDiv(int con);
		bool canDivide(int con);
		bool availableAction();
		Container selectContainer(int ship, int id) const;
	public:
		Base(string name);
		~Base();
		string getName() const;
		unsigned int getNumContainers() const;
		unsigned int getNumShips() const;
		Container getContainer(unsigned int id) const;
		Ship* getShip(string name) const;
		int searchContainer(unsigned int id) const;
		int searchShip(string name) const;
		bool addContainer(unsigned int id = 0, unsigned int value = 0);
		bool removeContainer(unsigned int id = 0);
		bool addShip(Ship* s);
		bool removeShip(string name = "");
		bool manualDistribution(unsigned int id = 0, string name = "");
		bool unassignContainer(unsigned int id = 0, string name = "");
		void automaticDistribution();
		void clearAssignations();
};

#endif