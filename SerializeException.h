/*
 * SerializeException.h
 *
 *  Created on: 5 Jan 2016
 *      Author: martin
 */

#ifndef SERIALIZEEXCEPTION_H_
#define SERIALIZEEXCEPTION_H_

#include <string>

namespace cppserialize {
	using namespace std;

	enum class ErrorNumber
	{
		NO_ERROR 					= 0,
		UNKNOWN_ERROR 				= 1,
		XML_ERROR					= 2,
		DESERIALIZE_ITEM_NOTFOUND 	= 3,
		DESERIALIZE_ITEM_CONVERT	= 4,
	};

	class SerializeException
	{
	public:
		SerializeException(string error_text, ErrorNumber error_number)
		 : m_error_text(error_text), m_error_number(error_number)
		{

		}

		string toString() const
		{
			return m_error_text;
		}

		ErrorNumber errNum() const
		{
			return m_error_number;
		}

	protected:
		string m_error_text;
		ErrorNumber m_error_number;
	};
}


#endif /* SERIALIZEEXCEPTION_H_ */
