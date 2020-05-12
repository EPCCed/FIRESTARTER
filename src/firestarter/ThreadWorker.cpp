#include <firestarter/Firestarter.hpp>
#include <firestarter/Logging/Log.hpp>

#include <cstdlib>

using namespace firestarter;

void Firestarter::run(void) {

	log::debug() << "Spawn " << this->requestedNumThreads << " threads.";

	this->threads = static_cast<pthread_t *>(std::aligned_alloc(64, this->requestedNumThreads * sizeof(pthread_t)));

	bool ack;

	for (int i=0; i<this->requestedNumThreads; i++) {
		auto td = new ThreadData(i);
		
		this->threadData.push_back(std::ref(td));

		auto t = pthread_create(&this->threads[i], NULL, threadWorker, std::ref(td));

		// TODO: set thread data high address
		td->mutex.lock();
		td->comm = THREAD_WORK;
		td->mutex.unlock();

		do {
			td->mutex.lock();
			ack = td->ack;
			td->mutex.unlock();
		} while (!ack);

		td->mutex.lock();
		td->ack = false;
		td->mutex.unlock();
	}

	log::debug() << "Started all work functions.";

	for (ThreadData *td : this->threadData) {
		td->mutex.lock();
		td->comm = THREAD_STOP;
		td->mutex.unlock();

		do {
			td->mutex.lock();
			ack = td->ack;
			td->mutex.unlock();
		} while (!ack);

		td->mutex.lock();
		td->ack = false;
		td->mutex.unlock();
	}

	log::debug() << "Stopped all threads.";

}

void *Firestarter::threadWorker(void *threadData) {

	int old = THREAD_WAIT;
	
	auto td = (ThreadData *)threadData;
	
	std::cout << "in thread " << td->getId() << std::endl;

	for (;;) {

		td->mutex.lock();
		int comm = td->comm;
		td->mutex.unlock();

		if (comm != old) {
			old = comm;

			td->mutex.lock();
			td->ack = true;
			td->mutex.unlock();
		} else {
			continue;
		}

		switch (comm) {
		// allocate and initialize memory
		case THREAD_INIT:
			break;
		// perform stress test
		case THREAD_WORK:
			log::debug() << "Thread " << td->getId() << " working.";
			break;
		case THREAD_WAIT:
			break;
		case THREAD_STOP:
		default:
			pthread_exit(0);
		}
	}
}