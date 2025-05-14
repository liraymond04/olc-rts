#ifndef IACTION_H
#define IACTION_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

class IAction {
  public:
    IAction(float targetTime, int amount = -1) {
        this->targetTime = targetTime * 60.0f;
        countTo = amount;
    }

  private:
    float targetTime;
    float accumulatedTime = 0.0f;

    int countTo;
    int count = 0;

  public:
    bool done = false;
    bool started = false;
    void Update(float gameSpeed) {
        bool tick = true;
        accumulatedTime += gameSpeed;
        if (accumulatedTime > targetTime) {
            accumulatedTime -= targetTime;
        } else {
            tick = false;
        }
        if (tick) {
            Tick();
            if (countTo != -1) {
                count++;
                if (count >= countTo) {
                    done = true;
                }
            }
        }
        if (done) {
            Tock();
        }
    }

    virtual void Start() = 0;

    virtual void Tick() = 0;
    virtual void Tock() = 0;
};

class Counter : public IAction {
  public:
    // using IAction::IAction;
    explicit Counter(float targetTime, int countTo)
        : IAction(targetTime, countTo) {}

    void Start() {}

    void Tick() {
        std::cout << "Hi"
                  << "\n";
    }

    void Tock() {
        std::cout << "Done"
                  << "\n";
    }
};

class Hex;
class HexGrid;
class Unit;

class MoveAction : public IAction {
  private:
    HexGrid *hexGrid;

  public:
    Unit *unit;

    MoveAction(float targetTime, HexGrid *hexGrid, Unit *unit, Hex startPos,
               Hex newPos);

    void Start();
    void Tick();
    void Tock();
};

#endif // ! IACTION_H
