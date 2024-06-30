#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>

class Ant {
public:
    virtual ~Ant() {}
    virtual void move() = 0;
    virtual void printPosition() const = 0;
};

class WorkerAnt : public Ant {
    double x, y;
    double init_x, init_y;
    double speed;
    bool toOrigin;
public:
    WorkerAnt(double x, double y, double speed) : x(x), y(y), init_x(x), init_y(y), speed(speed), toOrigin(true) {}
    void move() override {
        if (toOrigin) {
            if (x > 0) x -= speed;
            if (y > 0) y -= speed;
            if (x <= 0 && y <= 0) toOrigin = false;
        } else {
            if (x < init_x) x += speed;
            if (y < init_y) y += speed;
            if (x >= init_x && y >= init_y) toOrigin = true;
        }
    }
    void printPosition() const override {
        std::cout << "WorkerAnt at (" << x << ", " << y << ")\n";
    }
};

class WarriorAnt : public Ant {
    double x, y, angle;
    double radius;
    double speed;
public:
    WarriorAnt(double x, double y, double radius, double speed) : x(x), y(y), radius(radius), speed(speed), angle(0) {}
    void move() override {
        angle += speed / radius;
        x = radius * std::cos(angle);
        y = radius * std::sin(angle);
    }
    void printPosition() const override {
        std::cout << "WarriorAnt at (" << x << ", " << y << ")\n";
    }
};

void antThreadFunction(Ant* ant) {
    for (int step = 0; step < 50; ++step) {
        ant->move();
        ant->printPosition();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::vector<std::thread> threads;

    std::vector<Ant*> ants;
    ants.push_back(new WorkerAnt(10, 10, 1));
    ants.push_back(new WorkerAnt(5, 5, 0.5));
    ants.push_back(new WarriorAnt(0, 0, 10, 0.1));
    ants.push_back(new WarriorAnt(0, 0, 5, 0.2));

    for (auto ant : ants) {
        threads.push_back(std::thread(antThreadFunction, ant));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (auto ant : ants) {
        delete ant;
    }

    return 0;
}
