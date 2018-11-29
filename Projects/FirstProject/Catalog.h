#pragma once
#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include <map>

namespace engine {

	template <class T>
	class Catalog {
		std::map<std::string, T> cat;
		Catalog<T>() = default;
		~Catalog<T>() = default;
		static Catalog<T> *s_instance;
	public:
		void insert(const std::string& id, const T& t) {
			cat.insert(std::pair<std::string, T>(id, t));
		};

		void remove(const std::string& id) {
			cat.erase(id);
		};

		T& get(const std::string &id) {
			return cat.at(id);
		};

		static Catalog<T> *instance() {
			static Catalog<T> *s_instance = new Catalog<T>();
			return s_instance;
		}
	};
}

#endif // !CATALOG_H

#pragma once
