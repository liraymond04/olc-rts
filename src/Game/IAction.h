#ifndef IACTION_H
#define IACTION_H

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

    virtual void Tick() = 0;
    virtual void Tock() = 0;
};

class Counter : public IAction {
  public:
    // using IAction::IAction;
    explicit Counter(float targetTime, int countTo)
        : IAction(targetTime, countTo) {}

    void Tick() {
        std::cout << "Hi"
                  << "\n";
    }

    void Tock() {
        std::cout << "Done"
                  << "\n";
    }
};

#endif // ! IACTION_H