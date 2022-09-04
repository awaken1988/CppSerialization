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
    //helper
    std::string indent(size_t aDept) {
        std::string ret;
        for(size_t i=0; i<aDept; i++) {
            ret += "    ";
        }
        return ret;
    }



    enum class eNodeType {
        ITEM, CONTAINER, DYNAMIC_CONTAINER,
    };

    struct Node {
        virtual ~Node(){}

        virtual std::string debugString() {
            return "<no_debug_info>";
        }

        //virtual std::string type_name() const = 0 ;

        using shptr = std::shared_ptr<Node>;
    };

    template<typename T, typename E=void>
    struct NodeTrait {};

    using NodeShptr = std::shared_ptr<Node>;

    template<typename T>
    struct ItemTraits;

    //-----------------------------------------------
    // Item
    //-----------------------------------------------
    struct ItemBase : public Node {
        virtual std::string to_string()      = 0;
        virtual void        set(std::string) = 0;

        using shptr = std::shared_ptr<ItemBase>;
    };

    template<typename T>    //, typename ENIF=void
    struct Item : public ItemBase {
        T* data=nullptr;

        Item(T* aData) : data{aData} {

            std::cout << "*" << data << std::endl;
        }

        virtual std::string to_string() {
            return NodeTrait<T>::toString(*data); 
        }

        virtual void set(std::string aStr) {
            *data = NodeTrait<T>::fromString(aStr);
        }

        std::shared_ptr<Item<T>> toSharedPtr() {
            return std::make_shared<Item<T>>(*this);
        }

        virtual std::string debugString() override {
            std::stringstream ss; ss<<data; return ss.str();
        }
    };

    //-----------------------------------------------
    // Container
    //-----------------------------------------------
    using container_map = std::map<std::string, Node::shptr>;

    struct ContainerBase : public Node {
        container_map name_value;  

        void add(std::string aName, Node::shptr aNode) {
            name_value[aName] = aNode;
        }  
    };

    //-----------------------------------------------
    // DynamicContainer
    //  like vector, list...
    //-----------------------------------------------
    struct DynamicContainerBase {
        container_map name_value;


    };


    //-----------------------------------------------
    // Traits
    //-----------------------------------------------
    template<typename T>
    struct Container : public ContainerBase {
        Container(T* aData) {
            auto childs = NodeTrait<T>::containerChilds(aData);
            for(auto iChild: childs) {
                //std::cout << iChild.first << " - " << iChild.second << std::endl;
                add(iChild.first, iChild.second);
            }
        }

        std::shared_ptr<Container<T>> toSharedPtr() {
            return std::make_shared<Container<T>>(*this);
        }
    };

 

  
    template<typename T>
    Node::shptr create(T& aData) {
        return std::shared_ptr<Container<T>>(new Container<T>{&aData});
    }

    void traverse(std::string aName, Node::shptr aNode, size_t aDept=0) {
        std::cout << indent(aDept) << " Name=" << aName << "; Debug=" << aNode->debugString() << std::endl ;

        if(auto container = std::dynamic_pointer_cast<ContainerBase>(aNode); container) {
            for(auto i_child: container->name_value) {
                traverse(i_child.first, i_child.second, aDept+1);
            }
        }
        else if(auto item = std::dynamic_pointer_cast<ItemBase>(aNode); item) {
            
        }
    }

    //-----------------------------------------------
    // Tells if its an Item, Container...
    //-----------------------------------------------
    struct ContainerBuilder {

        template<typename T>
            requires (NodeTrait<T>::node_type == eNodeType::CONTAINER)
        ContainerBuilder& add(const std::string& aName, T* aContainer) {
            data[aName] = std::make_shared<Container<T>>(aContainer);
            return *this;
        }

        template<typename T>
            requires (NodeTrait<T>::node_type == eNodeType::ITEM)
        ContainerBuilder& add(const std::string& aName, T* aItem) {
            data[aName] = std::make_shared<Item<T>>(aItem);
            return *this;
        }

        container_map build() {
            return data;
        }

        container_map data;
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