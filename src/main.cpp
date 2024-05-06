#include <iostream>
#include <memory>
#include <vector>

template <typename T>
class Id {
    inline static int _id = -1;

public:
    static int get() {
        return _id;
    }

protected:
    friend struct Objects;
    static void set(int id) {
        _id = id;
    }
};

struct Position {
    Id<Position> id;
    float x = 0;
    float y = 0;
    float vx = 0;
    float vy = 0;

    void update(double t) {
        x += vx * t;
        y += vy * t;
        std::cout << "new position(" << x << ", " << y << ")\n";
    }
};

struct MonsterSound {
    Id<MonsterSound> id;

    std::string sound = "grr!";

    void update(double t) {
        std::cout << "groff: " << sound << "\n";
    }
};

struct Drawable {
    std::string apperance;

    void draw() {
        std::cout << "draw: " << apperance << "\n";
    }

    void update(double t) {}
};

struct SystemBase {
    virtual ~SystemBase() = default;

    virtual void update(double t) = 0;
};

template <typename T>
struct System : SystemBase {
    std::vector<T> elements;

    void update(double t) override {
        for (auto &e : elements) {
            e.update(t);
        }
    }
};

struct Objects {
    std::vector<std::shared_ptr<SystemBase>> systems;

    template <typename T>
    int getId() {
        if (Id<T>::get() == -1) {
            systems.push_back(std::make_shared<System<T>>());
            Id<T>::set(systems.size() - 1);
        }

        return Id<T>::get();
    }

    template <typename T>
    System<T> &getSystem() {
        return static_cast<System<T> &>(*systems.at(getId<T>()));
    }

    template <typename T>
    T &create() {
        auto &system = getSystem<T>();
        return system.elements.emplace_back();
    }

    void update(double t) {
        for (auto &system : systems) {
            system->update(t);
        }
    }
};

int main(int argc, char *argv[]) {
    auto objects = Objects{};

    // TODO: references will be invalid if resize happends on vector
    // Choose for example safe_ptr
    auto &position = objects.create<Position>();
    position.x = 10;
    auto &sound = objects.create<MonsterSound>();
    auto &drawable = objects.create<Drawable>();
    drawable.apperance = "ugly monster";

    // Wind
    float windConstant = .01;
    for (auto &p : objects.getSystem<Position>().elements) {
        p.vx += windConstant;
    }

    double t = 0.1;
    for (int i = 0; i < 10; ++i) {
        objects.update(t);
        for (auto &d : objects.getSystem<Drawable>().elements) {
            d.draw();
        }
    }

    return 0;
}
