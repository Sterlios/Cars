#include "transport.h"

transport::transport(int* numCar)
{
    number = *numCar;
    speed = dPar.SPEED;
    waitOtherCar = false;
}


bool transport::spawn(vector<transport*> cars)
{
    dir = (direction)(rand() % 4);
    switch (dir) {
    case NORTH:
        return spawnSouth(cars);
    case SOUTH:
        return spawnNorth(cars);
    case WEST:
        return spawnEast(cars);
    default:
        return spawnWest(cars);
    }
}

void transport::move(vector<transport*> cars)
{
    for (auto otherCar : cars) {
            if (number != otherCar->number) {
                    waitOtherCar = isWaiting(otherCar);
                    if (waitOtherCar) {
                            break;
                    }
            }
    }
    if (!waitOtherCar) {
            switch (dir) {
            case direction::NORTH: {
                makeMove(0, -speed);
                break;
            }
            case direction::SOUTH: {
                makeMove(0, speed);
                break;
            }
            case direction::WEST: {
                makeMove(speed, 0);
                break;
            }
            case direction::EAST: {
                makeMove(-speed, 0);
                break;
            }
            }
            //cout << number << " MOVE x = " << frontLeft.x << " y = " << frontLeft.y << endl;
    }
    
}

int transport::getPosX()
{
    return frontLeft.x;
}

int transport::getPosY()
{
    return frontLeft.y;
}

bool transport::spawnNorth(vector<transport*> cars)
{
    for (auto otherCar : cars) {
            if ((otherCar->dir == dir) && (otherCar->backLeft.y <= dPar.length + dPar.DELTA)) {
                    return false;
            }
    }        
    frontLeft.x = dPar.SCREEN / 2 - dPar.DELTA;
    frontLeft.y = dPar.length;
    frontRight.x = dPar.SCREEN / 2 - dPar.DELTA - dPar.width;
    frontRight.y = dPar.length;
    backLeft.x = dPar.SCREEN / 2 - dPar.DELTA;
    backLeft.y = 0;
    backRight.x = dPar.SCREEN / 2 - dPar.DELTA - dPar.width;
    backRight.y = 0;
    cout << number << " SPAWN north" << endl;
    return true;
}

bool transport::spawnSouth(vector<transport*> cars)
{    
    for (auto otherCar : cars) {
        if ((otherCar->dir == dir) && (otherCar->backLeft.y >= dPar.SCREEN - dPar.length - dPar.DELTA)) {
                return false;
        }
    }        
    frontLeft.x = dPar.SCREEN / 2 + dPar.DELTA;
    frontLeft.y = dPar.SCREEN - dPar.length;
    frontRight.x = dPar.SCREEN / 2 + dPar.DELTA + dPar.width;
    frontRight.y = dPar.SCREEN - dPar.length;
    backLeft.x = dPar.SCREEN / 2 + dPar.DELTA;
    backLeft.y = dPar.SCREEN;
    backRight.x = dPar.SCREEN / 2 + dPar.DELTA + dPar.width;
    backRight.y = dPar.SCREEN;
    cout << number << " SPAWN south" << endl;
    return true;   
}

bool transport::spawnWest(vector<transport*> cars)
{
    for (auto otherCar : cars) {
        if ((otherCar->dir == dir) && (otherCar->backLeft.x <= dPar.length + dPar.DELTA)) {
                return false;
        }
    }        
    frontLeft.x = dPar.SCREEN - dPar.length;
    frontLeft.y = dPar.SCREEN / 2 - dPar.DELTA;
    frontRight.x = dPar.SCREEN - dPar.length;
    frontRight.y = dPar.SCREEN / 2 - dPar.DELTA - dPar.width;
    backLeft.x = dPar.SCREEN;
    backLeft.y = dPar.SCREEN / 2 - dPar.DELTA;
    backRight.x = dPar.SCREEN;
    backRight.y = dPar.SCREEN / 2 - dPar.DELTA - dPar.width;
    cout << number << " SPAWN west" << endl;
    return true;  
}

bool transport::spawnEast(vector<transport*> cars)
{
    for (auto otherCar : cars) {
        if ((otherCar->dir == dir) && (otherCar->backLeft.x >= dPar.SCREEN - dPar.length - dPar.DELTA)) {
                return false;
        }
    }        
    frontLeft.x = dPar.length;
    frontLeft.y = dPar.SCREEN / 2 - dPar.DELTA;
    frontRight.x = dPar.length;
    frontRight.y = dPar.SCREEN / 2 - dPar.DELTA - dPar.width;
    backLeft.x = 0;
    backLeft.y = dPar.SCREEN / 2 - dPar.DELTA;
    backRight.x = 0;
    backRight.y = dPar.SCREEN / 2 - dPar.DELTA - dPar.width;
    cout << number << " SPAWN east" << endl;
    return true;  
}

void transport::makeMove(int dX, int dY)
{
    frontLeft.x += dX;
    frontLeft.y += dY;
    frontRight.x += dX;
    frontRight.y += dY;
    backLeft.x += dX;
    backLeft.y += dY;
    backRight.x += dX;
    backRight.y += dY;
}

bool transport::isWaiting(transport* otherCar)
{
    if (isOppositeDirection(otherCar)) {
            return false;
    }
    if (isSameDirection(otherCar)) {
            if (isNearCar(otherCar)) {
                    return true;
            }
            else {
                    return false;
            }
    }
    if (isOnTheLeft(otherCar)) {
            if (needPassLeftCar(otherCar)) {
                    return true;
            }
            else {
                    return false;
            }
    }
    if (isOnTheRight(otherCar)) {
            if (needPassRightCar(otherCar)) {
                    countOfStay++;
                    if ((countOfStay == 4) && (dir == direction::EAST)) {
                            countOfStay--;
                            return false;
                    }
                    return true;
            }
            else {
                if (waitOtherCar == true) {
                    countOfStay--;
                }
                return false;
            }
    }
    return false;
}

bool transport::isSameDirection(transport* otherCar)
{
    return (otherCar->dir == dir);
}

bool transport::isOppositeDirection(transport* otherCar)
{
    return (otherCar->dir == direction::NORTH && dir == direction::SOUTH) ||
            (otherCar->dir == direction::SOUTH && dir == direction::NORTH) ||
            (otherCar->dir == direction::WEST && dir == direction::EAST) ||
            (otherCar->dir == direction::EAST && dir == direction::WEST);
}

bool transport::isOnTheLeft(transport* otherCar)
{
    return (otherCar->dir == direction::SOUTH && dir == direction::WEST) ||
            (otherCar->dir == direction::NORTH && dir == direction::EAST) ||
            (otherCar->dir == direction::WEST && dir == direction::SOUTH) ||
            (otherCar->dir == direction::EAST && dir == direction::NORTH);
}

bool transport::isOnTheRight(transport* otherCar)
{
    return (otherCar->dir == direction::SOUTH && dir == direction::EAST) ||
            (otherCar->dir == direction::NORTH && dir == direction::WEST) ||
            (otherCar->dir == direction::WEST && dir == direction::NORTH) ||
            (otherCar->dir == direction::EAST && dir == direction::SOUTH);
    
}

bool transport::isNearCar(transport* otherCar)
{
    switch (dir) {
    case direction::SOUTH: {
            return ((otherCar->frontLeft.y - frontLeft.y - dPar.length) <= dPar.DELTA && (otherCar->frontLeft.y - frontLeft.y - dPar.length) > 0);
    }
    case direction::NORTH: {
            return ((frontLeft.y - otherCar->frontLeft.y - dPar.length) <= dPar.DELTA && (frontLeft.y - otherCar->frontLeft.y - dPar.length) > 0);
    }
    case direction::EAST: {
            return ((frontLeft.x - otherCar->frontLeft.x - dPar.width) <= dPar.DELTA && (frontLeft.x - otherCar->frontLeft.x - dPar.width) > 0);
    }
    case direction::WEST: {
            return ((otherCar->frontLeft.x - frontLeft.x - dPar.width) <= dPar.DELTA && (otherCar->frontLeft.x - frontLeft.x - dPar.width) > 0);
    }
    }
}

bool transport::needPassLeftCar(transport* otherCar)
{                
    auto &otherdir = otherCar->dir;
    int posIntersect_x0 = 0,
            posIntersect_x1 = 0,
            posIntersect_y0 = 0,
            posIntersect_y1 = 0;
    bool isStayThisCar = false;
    bool isMoveOtherCar = false;
    switch (dir) {
    case direction::NORTH:
        posIntersect_y0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 3;
        isStayThisCar = frontLeft.y <= posIntersect_y0 && frontLeft.y >= posIntersect_y0 - dPar.DELTA;
        if (isStayThisCar) {
            if (otherdir == direction::EAST) {
                posIntersect_x0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 2;
                posIntersect_x1 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 2;
                isMoveOtherCar = otherCar->frontLeft.x > posIntersect_x0 && otherCar->backLeft.x <= posIntersect_x1;
            }
        }
        break;
    case direction::SOUTH:
        posIntersect_y0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 3;
        isStayThisCar = frontLeft.y >= posIntersect_y0 && frontLeft.y <= posIntersect_y0 - dPar.DELTA;
        if (isStayThisCar) {
            if (otherdir == direction::WEST) {
                posIntersect_x0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 2;
                posIntersect_x1 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 2;
                isMoveOtherCar = otherCar->frontLeft.x < posIntersect_x0 && otherCar->backLeft.x >= posIntersect_x1;
            }
        }
        break;
    case direction::WEST:
        posIntersect_x0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 3;
        isStayThisCar = frontLeft.x >= posIntersect_x0 && frontLeft.x <= posIntersect_x0 - dPar.DELTA;
        if (isStayThisCar) {
            if (otherdir == direction::NORTH) {
                posIntersect_y0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 2;
                posIntersect_y1 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 2;
                isMoveOtherCar = otherCar->frontLeft.y > posIntersect_y0 && otherCar->backLeft.y <= posIntersect_y1;
            }
        }
        break;
    case direction::EAST:
        posIntersect_x0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 3;
        isStayThisCar = frontLeft.x <= posIntersect_x0 && frontLeft.x >= posIntersect_x0 - dPar.DELTA;
        if (isStayThisCar) {
            if (otherdir == direction::SOUTH) {
                posIntersect_y0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 2;
                posIntersect_y1 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 2;
                isMoveOtherCar = otherCar->frontLeft.y < posIntersect_y0 && otherCar->backLeft.y >= posIntersect_y1;
            }
            break;
        }
    }
    return isMoveOtherCar;    
}

bool transport::needPassRightCar(transport* otherCar)
{
    auto otherdir = otherCar->dir;
    int posIntersect_x0 = 0,
            posIntersect_x1 = 0,
            posIntersect_y0 = 0,
            posIntersect_y1 = 0;
    bool isStayThisCar = false;
    bool isMoveOtherCar = false;
    switch (dir) {
    case direction::NORTH:
            posIntersect_y0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 3;
            isStayThisCar = frontLeft.y <= posIntersect_y0 && frontLeft.y >= posIntersect_y0 - dPar.DELTA;
            if (isStayThisCar) {
                    if (otherdir == direction::WEST) {
                            posIntersect_x0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 3;
                            posIntersect_x1 = dPar.SCREEN / 2;
                            isMoveOtherCar = otherCar->frontLeft.x <= posIntersect_x0 && otherCar->backLeft.x >= posIntersect_x1;
                    }
            }
            break;
    case direction::SOUTH:
            posIntersect_y0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 3;
            isStayThisCar = frontLeft.y >= posIntersect_y0 && frontLeft.y <= posIntersect_y0 - dPar.DELTA;
            if (isStayThisCar) {
                    if (otherdir == direction::EAST) {
                            posIntersect_x0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 3;
                            posIntersect_x1 = dPar.SCREEN / 2;
                            isMoveOtherCar = otherCar->frontLeft.x >= posIntersect_x0 && otherCar->backLeft.x <= posIntersect_x1;
                    }
            }
            break;
    case direction::WEST:
            posIntersect_x0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 3;
            isStayThisCar = frontLeft.x >= posIntersect_x0 && frontLeft.x <= posIntersect_x0 - dPar.DELTA;
            if (isStayThisCar) {
                    if (otherdir == direction::NORTH) {
                            posIntersect_y0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 3;
                            posIntersect_y1 = dPar.SCREEN / 2;
                            isMoveOtherCar = otherCar->frontLeft.y <= posIntersect_y0 && otherCar->backLeft.y >= posIntersect_y1;
                    }
            }
            break;
    case direction::EAST:
            posIntersect_x0 = dPar.SCREEN / 2 + dPar.width + dPar.DELTA * 3;
            isStayThisCar = frontLeft.x <= posIntersect_x0 && frontLeft.x >= posIntersect_x0 - dPar.DELTA;
            if (isStayThisCar) {
                    if (otherdir == direction::SOUTH) {
                            posIntersect_y0 = dPar.SCREEN / 2 - dPar.width - dPar.DELTA * 3;
                            posIntersect_y1 = dPar.SCREEN / 2;
                            isMoveOtherCar = otherCar->frontLeft.y >= posIntersect_y0 && otherCar->backLeft.y <= posIntersect_y1;
                    }
            }
            break;
    }
    return isMoveOtherCar;
    
}
