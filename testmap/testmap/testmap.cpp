// testmap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <unordered_map>
#include <iostream>

int main()
{
	typedef std::unordered_multimap<std::string, int> occmap;

	occmap mymap;

	mymap.insert({"ciao",1});
	mymap.insert({ "wow",2 });
	mymap.insert({ "wow",3 });

	for (std::pair<std::string,int> x : mymap)
		std::cout << x.first.c_str() << " " << x.second << std::endl;

	auto its = mymap.equal_range("wow");

	for (auto it = its.first; it != its.second; ++it) {
		std::cout << it->first.c_str() << " " << it->second << std::endl;
	}
	
	system("PAUSE");
    return 0;
}

