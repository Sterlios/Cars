#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <vector>
#include <ctime>
#include <iostream>

using namespace std;

class transport;
static int countOfStay = 0;

static struct defaultParam {
        int width = 100;
        int length = 100;
        const int SCREEN = 800;
        const int SPEED = 5;
        const int DELTA = 2 * SPEED;
} dPar;

struct coord{
    int x, y;
};

enum direction {
        NORTH,
        SOUTH,
        WEST,
        EAST
};



class transport
{
public:
    transport(int* numCar);
    bool spawn(vector<transport*> cars);
    void move(vector<transport*> cars);
    int getPosX();
    int getPosY();
private:
    direction dir;
    int speed;
    int number;
    bool waitOtherCar = false;
    coord frontLeft,
          frontRight,
          backLeft,
          backRight;
    
    bool spawnNorth(vector<transport*> cars);
    bool spawnSouth(vector<transport*> cars);
    bool spawnWest(vector<transport*> cars);
    bool spawnEast(vector<transport*> cars);
    
    void makeMove(int dX, int dY);
    
    bool isWaiting(transport* otherCar);
    bool isSameDirection(transport* otherCar);
    bool isOppositeDirection(transport* otherCar);
    bool isOnTheLeft(transport* otherCar);
    bool isOnTheRight(transport* otherCar);
    bool isNearCar(transport* otherCar);
    bool needPassLeftCar(transport* otherCar);
    bool needPassRightCar(transport* otherCar);
};

#endif // TRANSPORT_H
