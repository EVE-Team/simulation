#pragma once

class Cell;

class Rabbit
{
public:
    Rabbit();

    int getId() const;

    void setParent(Cell *newParent);
    Cell *getParent() const;

    // move this rabbit one tick forward in time
    void advance(int tickNumber);

    // remove itself from parent cell
    void die();

    // remove itself to another cell
    void moveTo(Cell *newCell);

protected:
    Cell *parent;
    int id;
    int lastTick;
};
