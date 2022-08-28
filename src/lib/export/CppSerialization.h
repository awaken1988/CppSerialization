#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <functional>
#include <tuple>
#include <vector>
#include <memory>


struct Car {
    Car() {}

    virtual ~Car(){}

    virtual std::string drive() {
        return "drive like a car";
    }

    int car=1;
};

struct Skoda : public Car {
    Skoda() {}
    virtual ~Skoda(){}

    virtual std::string drive() {
        return "drive like a Skoda";
    }

    int skoda=1;
};

struct Vw : public Car {
    Vw() {}
    virtual ~Vw(){}

    virtual std::string drive() {
        return "drive like a Vw";
    }

    int vw=1;
};


struct TestRootStruct {
    int a=1;
    int b=2;
    int c=3;
};


namespace CppSerialization 
{
    enum class eNodeType {
        Item,           //aka Leaf node with pure data
        Container,      //node which have child nodes
    };

    struct Node {
        virtual ~Node(){}

        //virtual std::string type_name() const = 0 ;
    };

    using NodeShptr = std::shared_ptr<Node>;

    //-----------------------------------------------
    // Item
    //-----------------------------------------------
    struct ItemBase : public Node {
        virtual std::string to_string()      = 0;
        virtual void        set(std::string) = 0;
    };

    template<typename T>    //, typename ENIF=void
    struct Item : public ItemBase {
        virtual std::string to_string()      = 0;
        virtual void        set(std::string) = 0;
    };

    //some base types for test
    template<>
    struct Item<int> : public ItemBase {
        Item() = default;
        Item(int& aData) 
            : data{&aData} {}

        virtual std::string to_string() override {
            return "adsf"; //TODO: do convert
        }

        virtual void set(std::string aStr) override {
            *data = std::stoi(aStr);
        }

        int*        data = nullptr;
    };

  

    //-----------------------------------------------
    // Container
    //-----------------------------------------------
    struct Container : public Node {

        void add(std::string aName, NodeShptr aNode) {
            name_value[aName] = aNode;
        }

        template<typename T>
        void addItem(std::string aName, T& aData) {
            auto child_node = std::make_shared<Item<T>>();
            add(aName, child_node);
        }

        std::map<std::string, NodeShptr> name_value;   
    };

    //-----------------------------------------------
    // Builder
    //-----------------------------------------------
    template<typename T>
    struct BuilderTrait {
        NodeShptr buildSerdeNode(T& aData) {
            return nullptr;
        }
    };



    //virtual base classes

    template<typename T>
    struct Trait {};
   
    template<>
    struct Trait<Car> {
        static constexpr std::string_view name = "Car";
    };

    template<>
    struct Trait<Skoda> {
        static constexpr std::string_view name = "Skoda";
    };

    template<>
    struct Trait<Vw> {
        static constexpr std::string_view name = "Vw";
    };


    namespace PrivVirtBase {

        template<typename T_PARENT>
        using map_type = std::map<std::string,std::function<T_PARENT*()>>;

        template<size_t IDX, typename T_PARENT, typename... T_CHILD>
        void traverse_child(map_type<T_PARENT>& aOut) {
            using curr_tuple = std::tuple<T_CHILD...>;
            constexpr auto MAX_SIZE = std::tuple_size_v<curr_tuple>;
            if constexpr(IDX<MAX_SIZE) {              
                using curr_type  = std::tuple_element<IDX,curr_tuple>::type;

                //std::cout << "added" << IDX << std::endl;

                const auto curr_name = std::string{Trait<curr_type>::name};
                aOut[curr_name] = []() -> T_PARENT* {
                    //std::cout << "create type" << std::endl;
                    return new curr_type;
                };
                traverse_child<IDX+1, T_PARENT, T_CHILD...>(aOut);
            }
        }

       
    }

    template<typename T_PARENT, typename... T_CHILD>
    struct VirtBase {
        using map_type = PrivVirtBase::map_type<T_PARENT>;

        VirtBase() {
            PrivVirtBase::traverse_child<0,T_PARENT,T_CHILD...>(mapping);
        }

        T_PARENT* create(std::string aName) {
            return mapping[aName]();
        }

        map_type mapping;
    };






}