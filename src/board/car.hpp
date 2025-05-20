#ifndef CAR_HPP
#define CAR_HPP

struct Position {
    int row;
    int col;
};

class Car {
public:
    Car();
    Car(char id, Position start, int length, bool isHorizontal, bool isPrimary);
    Position getPosition() const;
    int getLength() const;
    bool getIsHorizontal() const;
    bool getIsPrimary() const;
    char getId() const;

private:
    char id;
    Position start;
    int length;
    bool isHorizontal;
    bool isPrimary;
    
    friend class Board;
};

#endif