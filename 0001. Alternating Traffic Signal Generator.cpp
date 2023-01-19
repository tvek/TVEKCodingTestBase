/***********************************************************************************************************
* Program to print the out of a traffic signal.
* Currently there are two signals in the system which can generate output alternatively in every 5 seconds.
* For example. when signal#1 is Red, signal#2 should be green and alternatively.
* Program should execute indefinitely until there is forced stop.
************************************************************************************************************/
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

class ITrafficSignalHandler {
    public:
        virtual void configure(std::vector<std::string> signalList) = 0;
        virtual void start() = 0;
};

class TrafficSignalHandler : public ITrafficSignalHandler {
    private:
        std::vector<std::string> mSignal;
        bool mIsRunning;
    public:
        TrafficSignalHandler() {
            mIsRunning = true;
        }
        
        void configure(std::vector<std::string> signalList){
            mSignal.clear();
            mSignal = signalList;
        }
        
        void start() {
            while (mIsRunning) {
                if (0 == mSignal.size()){
                    std::cout << "There is no item in signal. Waiting for an item to get added." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
                for (std::vector<std::string>::iterator currentSignal = mSignal.begin();
                    currentSignal != mSignal.end(); currentSignal++) {
                    std::cout << std::endl << *currentSignal << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
            }
        }
        
        
};

class TrafficPoleFactory {
    public:
        typedef enum {
            TRAFFIC_SIGNAL_TYPE_MIN,
            TRAFFIC_SIGNAL_TYPE_RG,
            TRAFFIC_SIGNAL_TYPE_GR,
            TRAFFIC_SIGNAL_TYPE_MAX,
        } TRAFFIC_SIGNAL_TYPE;
    
        static ITrafficSignalHandler* create(TRAFFIC_SIGNAL_TYPE type) {
            ITrafficSignalHandler* pole = nullptr;
            switch (type) {
                case TRAFFIC_SIGNAL_TYPE_RG: {
                    std::vector<std::string> mPoleDataList1;
                    mPoleDataList1.push_back("R");
                    mPoleDataList1.push_back("G");
                    pole = new TrafficSignalHandler();
                    if (nullptr != pole) {
                        pole->configure(mPoleDataList1);
                    }
                    break;
                }
                case TRAFFIC_SIGNAL_TYPE_GR: {
                    std::vector<std::string> mPoleDataList2;
                    mPoleDataList2.push_back("G");
                    mPoleDataList2.push_back("R");
                    pole = new TrafficSignalHandler();
                    if (nullptr != pole) {
                        pole->configure(mPoleDataList2);
                    }
                    break;
                }
                default: {
                    std::cout << "Unhandled TrafficSignalHandler type";
                    break;
                }
            }
            return pole;
        }
};

static void pole1ThreadMethod() {
    ITrafficSignalHandler* pole = TrafficPoleFactory::create(TrafficPoleFactory::TRAFFIC_SIGNAL_TYPE_RG);
    pole->start();
}

void pole2ThreadMethod() {
    ITrafficSignalHandler* pole = TrafficPoleFactory::create(TrafficPoleFactory::TRAFFIC_SIGNAL_TYPE_GR);
    pole->start();
}

int main() {
    std::thread pole1Thread(pole1ThreadMethod);
    std::thread pole2Thread(pole2ThreadMethod);
    pole1Thread.join();
    pole2Thread.join();
    return 0;
}
