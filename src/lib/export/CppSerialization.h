#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <functional>
#include <tuple>


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




namespace CppSerialization 
{
    struct SerializerInterface {
        virtual std::string to_string() = 0;
    };

    struct DeserializerInterface {
        virtual void set(std::string) = 0;
    };

   
    template<typename T, typename ENIF=void> 
    struct Serialize : public SerializerInterface {

    };

   template<typename T, typename ENIF=void> 
    struct Deserialize : public DeserializerInterface {

    };

    //int
    template<>
    struct Serialize<int> : public SerializerInterface {
        Serialize(int& aData) : data{aData}{}

        virtual std::string to_string() override {
            std::stringstream ss; ss << data; return ss.str();
        }

        int& data;
    };

    template<> 
    struct Deserialize<int> : public DeserializerInterface {
        Deserialize(int& aData) : data{aData}{}

        virtual void set(std::string aData) override {
           data = std::stoi(aData);
        }

        int& data;
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