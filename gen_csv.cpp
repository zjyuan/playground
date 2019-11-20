#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <sstream>

using namespace std;
using namespace boost::algorithm;

int main()
{
	ifstream ifs("log.txt");
	ofstream ofs("gpu_usage.csv");
	int line_no = 0;
	string line;
	stringstream ss;
	while(getline(ifs, line))
	{
		std::vector<string> items;
		split(items, line, is_space());
		for(auto it=begin(items); it!=end(items); it++)
		{
			if(*it == "GR3D_FREQ")
			{
				auto j = ++it;
				string usage = j->substr(0, j->find("%@"));
				ss << ++line_no << "," << usage << endl;
				break;
			}
		}
	}

	ofs << ss.str();
}

// g++ gen_csv.cpp -o gen_csv -std=c++11