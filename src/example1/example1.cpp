#include <iostream>
using namespace std;

#include "CppSerialization.h"
using namespace CppSerialization;



struct STest {
    int a=0;
    int b=1;
};

namespace CppSerialization {
    template<>
    struct BuilderTrait<STest> {
        NodeShptr buildSerdeNode(STest& aData) {
            auto node = std::make_shared<Container>();

            node->add("a", std::make_shared<Item<int>>(aData.a) );

            node->addItem("a", aData.a);


            return nullptr;
        }
    };
}

 


int main()
{
    cout<<"test"<<endl;

    VirtBase<Car, Skoda, Vw> cars_info;

    cout << "Vw: " << cars_info.create("Vw")->drive() << endl;
    cout << "Skoda: " << cars_info.create("Skoda")->drive() << endl;

    //should crash
    cout << "Skodaaaa: " << cars_info.create("Skodaaaa")->drive() << endl;







    return 0;
}