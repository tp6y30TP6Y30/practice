#include <iostream>
#include <ctime>
#include <thread>
#include <chrono> // time library
#include <mutex>
#include <vector>
using namespace std;
using namespace chrono;

#ifdef THREAD_DEMO
int getRandomNum(int maxLimit){
	srand(time(nullptr));
	return rand() % (maxLimit + 1);
}

void executeThread(int id){
	auto currentTime = system_clock::now();
	time_t sleepTime = chrono::system_clock::to_time_t(currentTime);
	tm localTime = *localtime(&sleepTime);
	cout << "Thread: " << id << " sleepTime: " << ctime(&sleepTime) << endl;
	
	/*
	// noteOfTime
	cout << "Month: " << localTime.tm_mon + 1 << endl;
	cout << "Day: " << localTime.tm_mday << endl;
	cout << "Year: " << localTime.tm_year + 1900 << endl;
	cout << "Hours: " << localTime.tm_hour << endl;
	cout << "Minutes: " << localTime.tm_min << endl;
	cout << "Seconds: " << localTime.tm_sec << endl;
	*/

	this_thread::sleep_for(chrono::seconds(getRandomNum(3)));
	currentTime = chrono::system_clock::now();
	sleepTime = chrono::system_clock::to_time_t(currentTime);
	cout << "Thread: " << id << " awakeTime: " << ctime(&sleepTime) << endl;
}
#elif THREAD_POOL
double accountBalance = 100;
mutex accountLock;

string getTime(){
	auto currentTime = chrono::system_clock::now();
	time_t sleepTime = chrono::system_clock::to_time_t(currentTime);
	return ctime(&sleepTime);
}

void getMoney(int id, double withdrawal){
	// wait for 3 seconds
	this_thread::sleep_for(chrono::seconds(3));
	// if no mutex lock and unlock, threads will face some critical section problems
	// but if we lock too many, multi-threads may have no advantages from parallel execution
	accountLock.lock();
	cout << id << " tries to withdrawal $" << withdrawal << " on " << getTime() << endl;
	if(accountBalance - withdrawal >= 0){
		accountBalance -= withdrawal;
		cout << "New Account Balance is $" << accountBalance << endl;
	}
	else{
		cout << "Not Enough Money in Account" << endl;
		cout << "Current Balance is $" << accountBalance << endl;
	}	
	accountLock.unlock();
}
#else
// mutex tool or data that is not distributed well for multi-threads needs to be capable
// for each thread to access
mutex vectLock;
vector <unsigned int> primeVector;

void findPrimes(unsigned int start, unsigned int end){
	for(unsigned int num = start; num <= end; num += 2){
		for(unsigned int factor = 2; factor < num; ++factor){
			if(num % factor == 0){
				break;
			}
			else if(factor + 1 == num){
				vectLock.lock();
				primeVector.push_back(num);
				vectLock.unlock();
			}
		}
	}
}

// the code ran by threads must be written parallel executable
// like distribute the range: [start, end] for each thread
// but don't forget the mutex part to avoid critical section problems
void findPrimesWithThreads(unsigned int start, unsigned int end, unsigned int numOfThreads){
	// vector <thread> threadVector; /* a */
	vector <thread> threadVector(numOfThreads); /* b */
	unsigned int threadSpread = (end - start + 1) / numOfThreads;
	unsigned int newEnd = start + threadSpread - 1;
	for(unsigned int id = 0; id < numOfThreads; ++id){
		// threadVector.emplace_back(findPrimes, start, newEnd); /* a */
		threadVector[id] = thread(findPrimes, start, newEnd); /* b */
		start += threadSpread;
		newEnd += threadSpread;
	}
	for(auto& t: threadVector){
		t.join();
	}
}
#endif

int main(){
#ifdef THREAD_DEMO
	// create a thread and call function: executeThread() with its arguement
	thread th (executeThread, 1);
	// real thread ID looks like: 0x700005508000
	// cout << "Real thread_ID: " << th.get_id() << endl;
	// thread.join() meaning main thread waits for this thread 
	// to stop executing before continuing execution of code in main
	th.join();
	thread th_(executeThread, 2);
	th_.join();
#elif THREAD_POOL
	thread threads[10];
	// thread size: 8 bytes
	// cout << "thread size: " << sizeof(thread) << endl;
	for(int i = 0; i < sizeof(threads) / sizeof(thread); ++i){
		threads[i] = thread(getMoney, i, 15);
	}
	for(int i = 0; i < sizeof(threads) / sizeof(thread); ++i){
		threads[i].join();
	}
#else
	int startTime = clock();
	findPrimesWithThreads(1, 100000, 3);
	int endTime = clock();
	for(auto p: primeVector){
		cout << p << endl;
	}
	cout << "Execution time: " << (endTime - startTime) / double(CLOCKS_PER_SEC) << " seconds" << endl;
#endif
}
