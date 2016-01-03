//============================================================================
// Name        : cppserialize.cpp
// Author      : Martin K
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

#include "SerializeBase.h"
#include "tinyxml2.h"
using namespace tinyxml2;


string read_file(string filename)
{
	string ret;
	ifstream file(filename);
	stringstream buffer;

	if( !file.good() ) {
		cout<<"error: cannot open "<<filename<<endl;
		return ret;
	}

	buffer<<file.rdbuf();

	return buffer.str();

}

class Point : public cppserialize::SerializeItem
{
public:
	Point()
	{
		x = 340;
		y = 120;

		serialize_add("x", &x);
		serialize_add("y", &y);
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

		serialize_add("r", &r);
		serialize_add("g", &g);
		serialize_add("b", &b);
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

		serialize_add("m_point", &m_point);
		serialize_add("m_color", &m_color);

		serialize_add("m_some_float_val", &m_some_float_val);
		serialize_add("m_some_double_val", &m_some_double_val);
		serialize_add("m_some_string_val", &m_some_string_val);
		serialize_add("m_int_vector", &m_int_vector);
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

	if(false)
	{	//serialization example
		test.serialize_get()->SaveFile("serialized_object.xml");
	}
	else
	{	//deserilization example
		string xml = read_file("serialized_object_copy.xml");
		test.serialize_set(xml);
	}

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
