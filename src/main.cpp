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
};

struct MonsterSound {
    Id<MonsterSound> id;

    std::string sound = "grr!";
};

struct SystemBase {
    virtual ~SystemBase() = default;
};

template <typename T>
struct System : SystemBase {
    std::vector<T> elements;
};

struct Objects {
    std::vector<std::shared_ptr<SystemBase>> systems;

    template <typename T>
    int getId() {
        if (Id<T>::get() == -1) {
            systems.emplace_back();
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
};

int main(int argc, char *argv[]) {
    auto objects = Objects{};

    // TODO: references will be invalid if resize happends on vector
    // Choose for example safe_ptr
    auto &position = objects.create<Position>();
    auto &sound = objects.create<MonsterSound>();

    return 0;
}
