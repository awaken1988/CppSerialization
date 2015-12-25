/*
 * SerializeConverters.h
 *
 *  Created on: 23 Dec 2015
 *      Author: martin
 */

#ifndef SERIALIZECONVERTERS_H_
#define SERIALIZECONVERTERS_H_

#include <string>
using namespace std;

template<typename T> string external_converter_get(const T& value)
{
	return "!!!error: no converter available!!!";
}

template<> string external_converter_get<int>(const int& value)
{
	return "blablubb";
}




#endif /* SERIALIZECONVERTERS_H_ */
