#include <boost/asio/steady_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	using namespace std;
	using namespace boost::asio;
	io_service srv1, srv2;

	steady_timer t1{srv1, std::chrono::seconds{3}};
	t1.async_wait([](const boost::system::error_code &ec){
		cout << "t1 timeout" << endl;
	});

	steady_timer t2{srv2, std::chrono::seconds{5}};
	t2.async_wait([](const boost::system::error_code &ec){
		cout << "t2 timeout" << endl;
	});

	thread thrd1{[&](){srv1.run();}};
	thread thrd2{[&](){srv2.run();}};

	thrd1.join();
	thrd2.join();
}

//g++ boost_thread_timer.cpp -o boost_thread_timer -std=c++11 -lpthread