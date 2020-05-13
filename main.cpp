/*
(10 points) Consider a program that consists of two threads. The main thread should
print “Go ” to standard output, followed by the second thread printing “Hokies!”
followed by a newline. This sequence should run 10 times at which point the application
should exit successfully. The included CMakeLists.txt file is setup to correctly build
the application as an executable named P3 from the file main.cpp. You can only use
the C++11 standard library.
*/

#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <mutex>
#include <condition_variable>

class p3 {

public:
	p3() {};

	std::string out;
	mutable std::mutex the_mutex;
	std::condition_variable the_cond;
	

};

void vt_function(p3& p, std::string str) {

	for (int i = 0; i < 10; i++) {
		std::unique_lock<std::mutex> lock(p.the_mutex);	// lock it down
		std::cout << str;								// output the string

		p.the_cond.notify_one();						// notify the other that this thread is done

		if (i < 9)
			p.the_cond.wait(lock);						// .wait unlocks the mutex
														// do not unlock on final iteration <-- this fucked me up for an hour
		//lock.unlock();
	}
	
}

int main()
{
	p3 p;
	std::string go = "GO ";
	std::string ho = "Hokies!\n";

	std::thread th1(vt_function, std::ref(p), go);
	std::thread th2(vt_function, std::ref(p), ho);
	
	th1.join();
	th2.join();
	
	return EXIT_SUCCESS;
}
