# CppSerialization
With this lib you are able to serialize your own c++-objects to xml. Deserialization is also possible.

##Example
```c++
class MyClass : public cppserialize::SerializeItem
{
public:
	MyClass()
	{
		m_some_float_val  = 1.23;
		m_some_double_val = 2.34;
		m_some_string_val = "serialization wooorks";
		
		serialize_add("m_some_float_val", &m_some_float_val);
		serialize_add("m_some_double_val", &m_some_double_val);
		serialize_add("m_some_string_val", &m_some_string_val);
	}
	float 	m_some_float_val;
	double 	m_some_double_val;
	string  m_some_string_val;
	};
```
To make your class serializable you must to inherit from **SerializeItem**. 
Hereafter you can declare the members with the **serialize_add** method.
This works also rekursively (not shown in the example). E.g if you have a member which
is also a **SerializeItem** you can also add this via **serialize_add**.



Now we can *serialize* an object of the above class as following:
```c++
MyClass test;
string xml_representation = test.serialize_get();
```

For *deserialization* we do:
```c++
MyClass test;

string xml = read_file("serialized_object.xml");

test.serialize_set(xml);
```



##Custom objects
As you can see it in the example non **SerializeItem** objects included *indirectly*. 
E.g class members like int, double string, vector<T> ... .



Basically you can also serialize your own type without inheritin from **SerializeItem**.
For that use case you have to declare your own converter function as a template specialistation of
the following functions:

* Convert T to string: ```template<typename T> string external_converter_get(const T& value)```
* Convert string (from xml) to T: ```template<typename T> bool external_converter_set(const string& value, T& out )```

E.g This is useful if you have you own string class.

Note: this functionality is not yet fully implemented




