/*
 * SerializeBase.h
 *
 *  Created on: 18 Dec 2015
 *      Author: martin
 */

#ifndef SERIALIZEBASE_H_
#define SERIALIZEBASE_H_

#include <string>
#include <map>

	namespace cppserialize {

	using namespace std;


	class SerializeBase {
	public:
		SerializeBase();
		virtual ~SerializeBase();

		virtual string serialize_get() = 0;
	};


	class SerializeItem: public SerializeBase {
	public:
		template<class E> void serialize_set(string item_name, E* item)
		{
			m_content[item_name] = item;
		}

		string serialize_get()
		{
			string ret;

			for(auto i: m_content) {
				ret += i.first + "=" + i.second->serialize_get();
			}

			return ret;
		}
	protected:
		map<string, SerializeBase*> m_content;
	};

} /* namespace cppserialize */

#endif /* SERIALIZEBASE_H_ */
