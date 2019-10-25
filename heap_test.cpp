#include <iostream>
#include <random>
#include <algorithm>
#include <boost/timer.hpp>

using namespace std;

int main()
{
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0, 1000000);

	int nrolls = 100000000;
	vector<int> data(nrolls);
	for(auto i=0; i<nrolls; i++)
	{
		data.push_back(distribution(generator));
	}

	boost::timer t;
	make_heap(begin(data), end(data));
	for(auto i=0; i<1000; i++)
	{
		pop_heap(begin(data), end(data));
		cout << "maximum is " << data.back() << endl;
		data.pop_back();
	}
	cout << "total time:" << t.elapsed() << endl;
}

//g++ heap_test.cpp -o heap_test -std=c++11