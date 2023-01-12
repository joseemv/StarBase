#ifndef Container_H
#define Container_H

#include <iostream>

using namespace std;

class Container
{
		friend ostream& operator<<(ostream &os, const Container &c);
	protected:
		unsigned int id;
		unsigned int value;
		unsigned int weight;
		static unsigned int nextId;
	public:
		static const unsigned int kMINWEIGHT = 100;
		static const unsigned int kMINVALUE = 100;
		Container(unsigned int weight = 0, unsigned int value = 0);
		~Container();
		static void resetNextId();
		unsigned int getId() const;
		unsigned int getWeight() const;
		unsigned int getValue() const;
		void setWeight(unsigned int weight);
		void setValue(unsigned int value);
};

#endif