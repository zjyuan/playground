#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

void Print1()
{
	for(auto i=0; i<500; i++)
	{
		std::cout << "Print1:" << i << std::endl;
		boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	}
}

void Print2()
{
	for(auto i=0; i<500; i++) 
	{
		std::cout << "Print2:" << i << std::endl;
		boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
	}
}

int main()
{
	boost::thread_group threads;
	threads.create_thread(Print1);
	threads.create_thread(Print2);
	threads.join_all();
}

//g++ -std=c++11  boost_thread.cpp -o boost_thread -lboost_thread -lpthread -lboost_chrono
