#include <iostream>
using namespace std;

#include "CppSerialization.h"
using namespace CppSerialization;

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