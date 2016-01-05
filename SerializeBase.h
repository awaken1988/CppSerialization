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
#include "SerializeException.h"

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
		virtual string serialize_get() = 0;
		virtual void serialize_set(XMLElement* aElement) = 0;
		virtual void serialize_set(string xmlContent) = 0;

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
		virtual void set(XMLElement* _item) = 0;
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

		virtual void set(XMLElement* _item)
		{
			string value(_item->GetText());
			external_converter_set<T>(value, *this->obj );
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

		virtual void set(XMLElement* _item)
		{
			obj->serialize_set(_item);
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

		virtual void set(XMLElement* _item)
		{
			for(XMLElement* iChild=_item->FirstChildElement();
					iChild != nullptr; iChild = iChild->NextSiblingElement()) {

				string value(iChild->GetText());
				T deserialized;
				if( !external_converter_set<T>(value, deserialized ) )
					throw SerializeException(	"set: cannot convert \"+string(iChild->GetText())+\"",
												ErrorNumber::DESERIALIZE_ITEM_NOTFOUND);

				obj->push_back(deserialized);
			}
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
		void serialize_add(string item_name, E* item)
		{
			ItemWrapper<SerializeBase>* item_wrapper = new ItemWrapper<SerializeBase>();
			item_wrapper->obj = item;
			m_content[item_name] = item_wrapper;
		}

		template<	typename E,
					typename = typename std::enable_if<   !std::is_base_of<SerializeBase, E>::value   >::type,
					typename = void>
		void serialize_add(string item_name, E* item)
		{
			ItemWrapper<E>* item_wrapper = new ItemWrapper<E>();
			item_wrapper->obj = item;
			m_content[item_name] = item_wrapper;
		}


		string serialize_get()
		{
			XMLDocument* doc = new XMLDocument();

			XMLElement* root = doc->NewElement("CppSerialize");
			doc->InsertEndChild(root);

			XMLElement* childs = serialize_get(root);

			//extract to string
			XMLPrinter printer;
			doc->Print(&printer);

			return string(printer.CStr());
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
			}

			return currElem;
		}

		virtual void serialize_set(XMLElement* aElement)
		{
			for(XMLElement* iChild = aElement->FirstChildElement();
					iChild != nullptr; iChild = iChild->NextSiblingElement()) {

				const char* key = iChild->Attribute(ATTRIB_OBJNAME);

				auto item_iter = m_content.find( key );

				if( item_iter == m_content.end() )
					throw SerializeException(	"serialize_set: item \""+string(key)+"\" not found",
												ErrorNumber::DESERIALIZE_ITEM_NOTFOUND);

				item_iter->second->set( iChild );
			}
		}

		virtual void serialize_set(string xmlContent)
		{
			XMLDocument doc;

			//TODO: Exception needs more details about the xml error
			if( XML_NO_ERROR != doc.Parse(xmlContent.c_str()) )
				throw SerializeException(	"serialize_set: tinyxml2 report errors",
											ErrorNumber::XML_ERROR);

			this->serialize_set(doc.RootElement()->FirstChildElement());
		}





	protected:
		map<string, ItemWrapperBase*> m_content;
	};





} /* namespace cppserialize */

#endif /* SERIALIZEBASE_H_ */
