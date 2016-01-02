/*
 * SerializeBase.h
 *
 *  Created on: 18 Dec 2015
 *      Author: Martin K
 */

#ifndef SERIALIZEBASE_H_
#define SERIALIZEBASE_H_

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <type_traits>
#include "tinyxml2.h"

#include "SerializeConverters.h"

namespace cppserialize {

	using namespace std;
	using namespace tinyxml2;

	constexpr auto NODE_NAME_SERIALOBJ = "SerialObject";
	constexpr auto NODE_NAME_CUSTOMOBJ  = "CustomObject";
	constexpr auto NODE_NAME_ARRAY		= "CustomArray";
	constexpr auto ATTRIB_OBJNAME 			= "name";










	/*
	 * 	SerializeBase
	 *
	 * 		A common interface for Serializeable Objects
	 *
	 * 		Note:
	 * 			Please do not inherit directly from SerialBase
	 * 			use SerializeItem
	 */
	class SerializeBase {
	public:
		SerializeBase()
		{

		}

		virtual ~SerializeBase()
		{

		}

		virtual XMLElement* serialize_get(XMLElement* aElement) = 0;
		virtual XMLDocument* serialize_get() = 0;
		virtual bool serialize_set(XMLElement* aElement) = 0;
		virtual bool serialize_set(string xmlContent) = 0;

		static const bool is_serializeable = true;
	};










	/*
	 * 	ItemWrapperBase
	 *
	 * 		With this abstraction we can distinguish between the following cases
	 * 			- serialize of a class based on SerialBase
	 * 			- serialize of a scalar types
	 * 			- serialize of array types
	 */
	struct ItemWrapperBase
	{
		virtual ~ItemWrapperBase(){};
		virtual XMLElement* get(XMLElement* _outerElement) = 0;
		virtual bool set(XMLElement* _item) = 0;
	};

	template<class T>
	struct ItemWrapper : public ItemWrapperBase
	{
		T* obj;
		virtual XMLElement* get(XMLElement* _outerElement)
		{
			XMLDocument* doc =  _outerElement->GetDocument();
			XMLElement* currElem =  doc->NewElement(NODE_NAME_CUSTOMOBJ);
			_outerElement->InsertEndChild(currElem);

			string value = external_converter_get<T>(*obj);
			currElem->SetText(value.c_str());

			return currElem;
		}

		virtual bool set(XMLElement* _item)
		{
			string value(_item->GetText());

			return external_converter_set<T>(value, *this->obj );
		}
	};

	template<>
	struct ItemWrapper<SerializeBase> : public ItemWrapperBase
	{
		SerializeBase* obj;
		virtual XMLElement* get(XMLElement* _outerElement)
		{
			return obj->serialize_get(_outerElement);
		}

		virtual bool set(XMLElement* _item)
		{
			return obj->serialize_set(_item);
		}
	};

	template<typename T>
	struct ItemWrapper<vector<T>> : public ItemWrapperBase
	{
		vector<T>* obj;
		virtual XMLElement* get(XMLElement* _outerElement)
		{
			XMLDocument* doc =  _outerElement->GetDocument();
			XMLElement* currElem =  doc->NewElement(NODE_NAME_ARRAY);
			_outerElement->InsertEndChild(currElem);

			for(auto& i: *obj) {
				XMLElement* array_item = doc->NewElement(NODE_NAME_CUSTOMOBJ);
				currElem->InsertEndChild(array_item);

				string value = external_converter_get<T>(i);
				array_item->SetText(value.c_str());
			}

			return currElem;
		}

		virtual bool set(XMLElement* _item)
		{
			return false; /* not implemented */
		}
	};










	/*
	 *
	 *	SerializeItem
	 *
	 */
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

		//TODO: return shared_ptr
		XMLDocument* serialize_get()
		{
			XMLDocument* doc = new XMLDocument();

			XMLElement* root = doc->NewElement("CppSerialize");
			doc->InsertEndChild(root);

			XMLElement* childs = serialize_get(root);


			return doc;
		}

		XMLElement* serialize_get(XMLElement* _outerElement)
		{
			XMLDocument* doc =  _outerElement->GetDocument();
			XMLElement* currElem =  doc->NewElement(NODE_NAME_SERIALOBJ);
			_outerElement->InsertEndChild(currElem);

			//TODO: insert class name optionally

			for(auto i: m_content) {
				string object_name = i.first;

				XMLElement* childElement = i.second->get(currElem);

				childElement->SetAttribute(ATTRIB_OBJNAME, object_name.c_str());


//				cout << "get item " << i.first << "-" << child_val << endl;

			}

			return currElem;
		}

		virtual bool serialize_set(XMLElement* aElement)
		{
			for(XMLElement* iChild = aElement->FirstChildElement();
					iChild != nullptr; iChild = iChild->NextSiblingElement()) {

				const char* key = iChild->Attribute(ATTRIB_OBJNAME);

				auto item_iter = m_content.find( key );

				if( item_iter == m_content.end() )
					return false;


				if( !item_iter->second->set( iChild ) )
					return false;
			}

			return true;	/* all ok */
		}

		virtual bool serialize_set(string xmlContent)
		{
			XMLDocument doc;

			//FIXME: check for xml errors
			if( XML_NO_ERROR != doc.Parse(xmlContent.c_str()) )
				return false;

			return this->serialize_set(doc.RootElement()->FirstChildElement());
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
