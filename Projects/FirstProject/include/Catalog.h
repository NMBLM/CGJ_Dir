#pragma once
#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include <vector>
#include <map>

namespace engine{

    template <class T>
    class Catalog{
        std::map<std::string, T> cat;
        Catalog<T>() = default;
        ~Catalog<T>() = default;
        static Catalog<T> *s_instance;
        public:
        void insert( const std::string& id, const T& t ){
            cat.insert( std::pair<std::string, T>( id, t ) );
        };

        void remove( const std::string& id ){
            cat.erase( id );
        };

        T& get( const std::string &id ){
            return cat.at( id );
        };

        std::vector<std::string>* getKeys(){
            std::vector<std::string>* str = new std::vector<std::string>();
            for( auto& pair : cat ){
                str->push_back( pair->first() );
            }
            return str;
        }

        std::vector<T>* getValues(){
            std::vector<T>* ts = new std::vector<T>();
            for( auto& pair : cat ){
                ts->push_back( pair->second() );
            }
            return ts;
        }

        static Catalog<T> *instance(){
            static Catalog<T> *s_instance = new Catalog<T>();
            return s_instance;
        }
    };
}

#endif // !CATALOG_H

#pragma once
