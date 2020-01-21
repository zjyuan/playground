#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

using namespace std;

int main()
{
	default_random_engine engine;
	uniform_int_distribution<int> dist(0, 10000);

	vector<int> data;
	for(int i=0; i<20; i++)
	{
		data.push_back(dist(engine));
	}

	copy(begin(data), end(data), ostream_iterator<int>(cout, " "));
	cout << endl;

	int k = 3;
	auto i = data.begin();
	while(1)
	{
		if(data.end() - i < k)
			break;

		auto j = i + k;
		reverse(i, j);
		i = j;
	}

	copy(begin(data), end(data), ostream_iterator<int>(cout, " "));
	cout << endl;
}

// g++ vector_k_reverse.cpp -o vector_k_reverse -std=c++11