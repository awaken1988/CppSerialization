/*
 * SerializeConverters.h
 *
 *  Created on: 23 Dec 2015
 *      Author: Martin K
 */

#ifndef SERIALIZECONVERTERS_H_
#define SERIALIZECONVERTERS_H_

#include <string>
#include <sstream>
#include "tinyxml2.h"
using namespace std;

//!Default converter using stringstream
template<typename T> string external_converter_get(const T& value)
{
	T ret;

	std::stringstream ss;
	ss<<value;

	return ss.str();
}

//! special function for char because stringstream char not as numerical value
template<> string external_converter_get(const unsigned char& value)
{
	char ret;

	int int_value = value;

	std::stringstream ss;
	ss<<int_value;

	return ss.str();
}


/*	here we can specialize converters
template<> string external_converter_get<int>(const int& value)
{
	return "blablubb";
}
*/




#endif /* SERIALIZECONVERTERS_H_ */
