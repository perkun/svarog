#ifndef MEMMORYLEDGER_H_
#define MEMMORYLEDGER_H_

#include <vector>

using namespace std;


// T has to be a pointer !!!
template <typename T>
class MemmoryLedger
{
public:
	void add(T p);
	void delete_all();

private:
	vector<T> pointer_list;
};





template <typename T>
void MemmoryLedger<T>::add(T p)
{
	pointer_list.push_back(p);
}


template <typename T>
void MemmoryLedger<T>::delete_all()
{
	for (T p: pointer_list)
		delete p;
}

// template <typename T>
// T MemmoryLedger<T>::ptr(T obj)
// {
// 	return obj;
// }
//
// template <typename T>
// T MemmoryLedger<T>::ptr(*T obj)
// {
// 	return obj;
// }


#endif
