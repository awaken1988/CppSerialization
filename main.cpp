//============================================================================
// Name        : cppserialize.cpp
// Author      : Martin K
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "SerializeBase.h"



class Class1 : public cppserialize::SerializeItem
{
public:
	int x1;
};

class Class2 : public cppserialize::SerializeItem
{
public:
	int x2;
};

class MyClass : public cppserialize::SerializeItem
{
public:
	MyClass()
	{
		serialize_set("m_class1", &m_class1);
		serialize_set("m_class2", &m_class2);
	}
	Class1 m_class1;
	Class2 m_class2;
	int xmy;
};


int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	MyClass test;
	cout<<test.serialize_get()<<endl;


	return 0;
}
