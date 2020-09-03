#pragma once
#include "Item.h"
#include <vector>

namespace rls{

template<typename T>
class IDataBase{
public:
	virtual T get(int id) = 0;
	virtual std::vector<T> getAll() = 0;
	virtual std::vector<int> getIDs() = 0;
	virtual void add(T&) = 0;
	virtual void update(T&) = 0;
	virtual void del(int id) = 0;
	virtual ~IDataBase(){};
};

}