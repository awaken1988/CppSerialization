#include <iostream>
#include <sstream>
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
    template<>
    struct ItemTraits<int> {
        static int fromString(std::string aStr) {
            return std::stoi(aStr);
        }
        static std::string toString(int& aData) {
            std::stringstream ss;
            ss<<aData;
            return ss.str();
        }
    };

    template<>
    struct ContainerTraits<STest> {
        static std::map<std::string, Node::shptr> containerChilds(STest* aData) {
            std::map<std::string, Node::shptr> ret;
            ret["a"]     = Item<int>(&(aData->a)).toSharedPtr();
            ret["b"]     = Item<int>(&(aData->b)).toSharedPtr();
            ret["inner"] = Container<SInner>(&aData->inner).toSharedPtr();
            return ret;
        }
    };

    template<>
    struct ContainerTraits<SInner> {
        static std::map<std::string, Node::shptr> containerChilds(SInner* aData) {
            std::map<std::string, Node::shptr> ret;
            ret["i0"] = Item<int>(&(aData->i0)).toSharedPtr();
            ret["i1"] = Item<int>(&(aData->i1)).toSharedPtr();
            return ret;
        }
    };
}

 


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




    return 0;
}