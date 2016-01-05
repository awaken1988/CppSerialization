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

//not needed for this example
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

const char* saved_object =
"		<CppSerialize>"
"		    <SerialObject>"
"		        <SerialObject name=\"m_color\">"
"		            <CustomObject name=\"b\">100</CustomObject>"
"		            <CustomObject name=\"g\">150</CustomObject>"
"		            <CustomObject name=\"r\">155</CustomObject>"
"		        </SerialObject>"
"		        <CustomArray name=\"m_int_vector\">"
"		            <CustomObject>3</CustomObject>"
"		            <CustomObject>6</CustomObject>"
"		            <CustomObject>9</CustomObject>"
"		        </CustomArray>"
"		        <SerialObject name=\"m_point\">"
"		            <CustomObject name=\"x\">1</CustomObject>"
"		            <CustomObject name=\"y\">-1</CustomObject>"
"		        </SerialObject>"
"		        <CustomObject name=\"m_some_double_val\">9.87</CustomObject>"
"		        <CustomObject name=\"m_some_float_val\">8.76</CustomObject>"
"		        <CustomObject name=\"m_some_string_val\">load from xml succesfully</CustomObject>"
"		    </SerialObject>"
"		</CppSerialize>";


int main() {

	MyClass test;

	try {
		cout<<"----------------------------------------------"<<endl;
		cout<<"Serialize the object with their default values"<<endl;
		cout<<"----------------------------------------------"<<endl;

		string serialized_str = test.serialize_get();
		cout<<serialized_str<<endl;




		cout<<"----------------------------------------------"<<endl;
		cout<<"Load values from xml and Serialize to show results as xml"<<endl;
		cout<<"----------------------------------------------"<<endl;
		test.serialize_set(saved_object);
		cout<<test.serialize_get()<<endl;
	}
	catch(cppserialize::SerializeException e) {
		cout<<e.toString()<<endl;
		//cout<<"\terrno="<<e.errNum()<<endl;
	}

	return 0;
}
