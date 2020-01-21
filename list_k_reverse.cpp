#include <list>
#include <algorithm>
#include <iostream>
#include <random>
#include <iterator>

using namespace std;

int main()
{
	default_random_engine engine;
	uniform_int_distribution<int> dist(0, 1000);

	list<int> data;
	for(int i=0; i<20; i++)
	{
		data.push_back(dist(engine));
	}

	copy(data.begin(), data.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	int k = 3;
	auto i = data.begin();
	while(1)
	{
		auto count = distance(i, data.end());
		if(count < 3)
			break;

		auto j = i;
		advance(j, k);
		reverse(i, j);
		i = j;
	}

	copy(data.begin(), data.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}