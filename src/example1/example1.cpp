//References
//  https://stackoverflow.com/questions/16337610/how-to-know-if-a-type-is-a-specialization-of-stdvector
//


#include <iostream>
#include <sstream>
#include <vector>
#include <list>
using namespace std;

#include "CppSerialization.h"
using namespace CppSerialization;


struct SInner {
    int i0=10;
    int i1=20;
};

struct STest {
    int a=0;
    int b=1;

    SInner inner;
};

namespace CppSerialization {
    template<> struct NodeTrait<int> {
        static constexpr auto node_type = eNodeType::ITEM;

        static int fromString(std::string aStr) {
            return std::stoi(aStr);
        }
        static std::string toString(int& aData) {
            std::stringstream ss;
            ss<<aData;
            return ss.str();
        }
    };

    template<> struct NodeTrait<SInner> {
        static constexpr auto node_type = eNodeType::CONTAINER;
        static std::map<std::string, Node::shptr> containerChilds(SInner* aData) {
            return ContainerBuilder{}
                .add("i0", &(aData->i0))
                .add("i1", &(aData->i1))
                .build();
        }
    };

    template<> struct NodeTrait<STest> {
        static constexpr auto node_type = eNodeType::CONTAINER;
        static container_map containerChilds(STest* aData) {
            return ContainerBuilder{}
                .add("a",     &aData->a)
                .add("b",     &aData->b)
                .add("inner", &aData->inner)
                .build();
        }
    };


}

template<typename T, template<typename...> typename S>
struct is_dynamic_cotainer : public std::false_type {  };

template<template<typename...> typename T, typename... S>
struct is_dynamic_cotainer<T<S...>, T> : public std::true_type {  };



int main()
{
 //   cout<<"test"<<endl;
 //   VirtBase<Car, Skoda, Vw> cars_info;
 //   cout << "Vw: " << cars_info.create("Vw")->drive() << endl;
 //   cout << "Skoda: " << cars_info.create("Skoda")->drive() << endl;
 //   //should crash
 //   cout << "Skodaaaa: " << cars_info.create("Skodaaaa")->drive() << endl;

    STest test;
    
    std::cout << &test.a << std::endl;

    auto root = CppSerialization::create(test);
    CppSerialization::traverse("<root>", root);


   

    using ivec  = std::vector<int>;
    using ilist = std::list<int> ;

    std::cout << "is_std_vector<a>" << is_dynamic_cotainer<ivec,std::vector>::value << std::endl;
    std::cout << "is_std_vector<a>" << is_dynamic_cotainer<ilist,std::vector>::value << std::endl;
    std::cout << "is_std_vector<a>" << is_dynamic_cotainer<ivec,std::list>::value << std::endl;
    std::cout << "is_std_vector<a>" << is_dynamic_cotainer<ilist,std::list>::value << std::endl;

    



    return 0;
}