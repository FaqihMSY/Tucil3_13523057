#include "car.hpp"

Car::Car() 
    : id('.'), start({0, 0}), length(0), isHorizontal(true), isPrimary(false) {}

Car::Car(char id, Position start, int length, bool isHorizontal, bool isPrimary)
    : id(id), start(start), length(length), isHorizontal(isHorizontal), isPrimary(isPrimary) {}

Position Car::getPosition() const {
    return start;
}

int Car::getLength() const {
    return length;
}

bool Car::getIsHorizontal() const {
    return isHorizontal;
}

bool Car::getIsPrimary() const {
    return isPrimary;
}

char Car::getId() const {
    return id;
}