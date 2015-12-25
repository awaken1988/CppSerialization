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
#include <iostream>
#include <type_traits>

#include "SerializeConverters.h"

	namespace cppserialize {

	using namespace std;


	class SerializeBase {
	public:
		SerializeBase()
		{

		}

		virtual ~SerializeBase()
		{

		}

		virtual string serialize_get() = 0;

		static const bool is_serializeable = true;
	};



	struct ItemWrapperBase
	{
		virtual ~ItemWrapperBase(){};
		virtual string get() = 0;
	};

	template<class T>
	struct ItemWrapper : public ItemWrapperBase
	{
		T* obj;
		virtual string get()
		{
			return external_converter_get<T>(*obj);
		}
	};

	template<>
	struct ItemWrapper<SerializeBase> : public ItemWrapperBase
	{
		SerializeBase* obj;
		virtual string get()
		{
			return obj->serialize_get();
		}
	};


	class SerializeItem: public SerializeBase {
	public:
		SerializeItem(){}
		~SerializeItem() {}


		template<	typename E,
					typename = typename std::enable_if<   std::is_base_of<SerializeBase, E>::value   >::type>
		void serialize_set(string item_name, E* item)
		{
			ItemWrapper<SerializeBase>* item_wrapper = new ItemWrapper<SerializeBase>();
			item_wrapper->obj = item;
			m_content[item_name] = item_wrapper;
		}

		template<	typename E,
					typename = typename std::enable_if<   !std::is_base_of<SerializeBase, E>::value   >::type,
					typename = void>
		void serialize_set(string item_name, E* item)
		{
			ItemWrapper<E>* item_wrapper = new ItemWrapper<E>();
			item_wrapper->obj = item;
			m_content[item_name] = item_wrapper;
		}

		string serialize_get()
		{
			string ret="xyz={";


			for(auto i: m_content) {
				string child_val = i.second->get();

				cout << "get item " << i.first << "-" << child_val << endl;
				ret += i.first + "=" + child_val ;
			}

			return ret+"} ";
		}
	protected:
		map<string, ItemWrapperBase*> m_content;
	};





} /* namespace cppserialize */

#endif /* SERIALIZEBASE_H_ */


























	//	template<typename E> void SerializeItem::serialize_set(string item_name,
	//							std::enable_if<std::is_base_of<SerializeBase,E>::value,E>::type* item)
	//	{
	//		if( nullptr != dynamic_cast<SerializeBase*>((void*)item) )
	//		{
	//			ItemWrapper<SerializeBase>* item_wrapper = new ItemWrapper<SerializeBase>();
	//			item_wrapper->obj = item;
	//			m_content[item_name] = item_wrapper;
	//		}
	//		else
	//		{
	//			ItemWrapper<E>* item_wrapper = new ItemWrapper<E>();
	//			item_wrapper->obj = item;
	//			m_content[item_name] = item_wrapper;
	//		}
	//	}
