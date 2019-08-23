#include <vector>
#include <ctime>
#include <thread>
#include <mutex>
#include <iostream>
#include "transport.h"

using namespace std;

void main_loop(mutex& m_spawncar, mutex& m_movecar) {
        bool f_spawnCar = false;
        static int numberCar = 0;
        int numCar = 0;
        static vector<transport*> cars;
        transport* car = nullptr;
        while (1) {
                if (!f_spawnCar) {
                    m_spawncar.lock();
                    numCar = numberCar;
                    car = new transport(&numberCar);
                    int sleep = rand() % 1000;
                    this_thread::sleep_for(chrono::milliseconds(sleep));
                    if (car->spawn(cars)){
                        cars.push_back(car);
                        f_spawnCar = true;
                        numberCar++;
                    }

                    m_spawncar.unlock();
                }
                else {
                        m_movecar.lock();
                        car->move(cars);
                        bool screenBorder = car->getPosX() > dPar.SCREEN || car->getPosX() < 0 || car->getPosY() > dPar.SCREEN || car->getPosY() < 0;
                        if (screenBorder) {
                                car->spawn(cars);
                        }
                        m_movecar.unlock();
                        this_thread::sleep_for(chrono::milliseconds(200));
                }
        }
}

int main() {
        srand((unsigned int)time(nullptr));
        const int initialCarsCount = 10;
        thread car[initialCarsCount];
        mutex mSpawn, mMove;
        for (auto i = 0; i < initialCarsCount; i++) {
                car[i] = thread(main_loop, ref(mSpawn), ref(mMove));
        }
        for (auto i = 0; i < initialCarsCount; i++) {
                car[i].join();
        }
        return 0;
}
