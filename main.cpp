//============================================================================
// Name        : cppserialize.cpp
// Author      : Martin K
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

#include "SerializeBase.h"
#include "tinyxml2.h"
using namespace tinyxml2;


class Point : public cppserialize::SerializeItem
{
public:
	Point()
	{
		x = 340;
		y = 120;

		serialize_set("x", &x);
		serialize_set("y", &y);
	}

	int x, y;
};

class Color : public cppserialize::SerializeItem
{
public:
	Color()
	{
		r = 100;
		g = 200;
		b = 255;

		serialize_set("r", &r);
		serialize_set("g", &g);
		serialize_set("b", &b);
	}

	unsigned char r, g, b;
};

class MyClass : public cppserialize::SerializeItem
{
public:
	MyClass()
	{
		m_some_float_val  = 1.23;
		m_some_double_val = 2.34;
		m_some_string_val = "serialization wooorks";
		m_int_vector.push_back(1);
		m_int_vector.push_back(2);
		m_int_vector.push_back(4);

		serialize_set("m_point", &m_point);
		serialize_set("m_color", &m_color);

		serialize_set("m_some_float_val", &m_some_float_val);
		serialize_set("m_some_double_val", &m_some_double_val);
		serialize_set("m_some_string_val", &m_some_string_val);
		serialize_set("m_int_vector", &m_int_vector);
	}
	Point m_point;
	Color m_color;
	float 	m_some_float_val;
	double 	m_some_double_val;
	string  m_some_string_val;
	vector<int> m_int_vector;
};


int main() {

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	MyClass test;

	test.serialize_get()->SaveFile("serialized_object.xml");


	return 0;
}




/*
	testcode for tinyxml2

	XMLDocument doc;

	XMLElement* elem_ptr = doc.NewElement("node0");
	doc.InsertEndChild(elem_ptr);
	elem_ptr->SetAttribute("isLecker", true);

	for(string i: {"hallo", "das", "ist", "ein", "test"}) {
	XMLElement* currElem = doc.NewElement(i.c_str());

	currElem->SetText("Hallo wie gehts denn so");

	elem_ptr->InsertEndChild( currElem );

	doc.SaveFile("blaablubb.xml", false);

	}
 */
