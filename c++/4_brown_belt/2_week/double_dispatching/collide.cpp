#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"

#include <vector>
#include <memory>

using namespace std;

// Определите классы Unit, Building, Tower и Fence так, чтобы они наследовались от
// GameObject и реализовывали его интерфейс.

template<typename T>
struct Collider : GameObject {
    bool Collide(const GameObject& that) const override {
        return that.CollideWith(static_cast<const T&>(*this));
    }
};

class Unit : public Collider<Unit> {
public:
    explicit Unit(geo2d::Point position) : pos(position) {};

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

    const geo2d::Point& GetPosition() const {
        return pos;
    }

private:
    geo2d::Point pos;
};

class Building : public Collider<Building> {
public:
    explicit Building(geo2d::Rectangle geometry) : pos(geometry) {};

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

    const geo2d::Rectangle& GetPosition() const {
        return pos;
    }

private:
    geo2d::Rectangle pos;
};

class Tower : public Collider<Tower> {
public:
    explicit Tower(geo2d::Circle geometry) : pos(geometry) {};

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

    const geo2d::Circle& GetPosition() const {
        return pos;
    }

private:
    geo2d::Circle pos;
};

class Fence : public Collider<Fence> {
public:
    explicit Fence(geo2d::Segment geometry) : pos(geometry) {};

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

    const geo2d::Segment& GetPosition() const {
        return pos;
    }

private:
    geo2d::Segment pos;
};

#define GET_COLLISION(Class, OtherClass)                            \
bool Class::CollideWith(const OtherClass& that) const {\
    return geo2d::Collide(pos, that.GetPosition());                  \
}

GET_COLLISION(Unit, Unit)
GET_COLLISION(Unit, Building)
GET_COLLISION(Unit, Tower)
GET_COLLISION(Unit, Fence)

GET_COLLISION(Building, Unit)
GET_COLLISION(Building, Building)
GET_COLLISION(Building, Tower)
GET_COLLISION(Building, Fence)

GET_COLLISION(Tower, Unit)
GET_COLLISION(Tower, Building)
GET_COLLISION(Tower, Tower)
GET_COLLISION(Tower, Fence)

GET_COLLISION(Fence, Unit)
GET_COLLISION(Fence, Building)
GET_COLLISION(Fence, Tower)
GET_COLLISION(Fence, Fence)


// Реализуйте функцию Collide из файла GameObject.h

bool Collide(const GameObject& first, const GameObject& second) {
    return first.Collide(second);
}

void TestAddingNewObjectOnMap() {
    // Юнит-тест моделирует ситуацию, когда на игровой карте уже есть какие-то объекты,
    // и мы хотим добавить на неё новый, например, построить новое здание или башню.
    // Мы можем его добавить, только если он не пересекается ни с одним из существующих.
    using namespace geo2d;

    const vector<shared_ptr<GameObject>> game_map = {
            make_shared<Unit>(Point{3, 3}),
            make_shared<Unit>(Point{5, 5}),
            make_shared<Unit>(Point{3, 7}),
            make_shared<Fence>(Segment{{7, 3},
                                       {9, 8}}),
            make_shared<Tower>(Circle{Point{9, 4}, 1}),
            make_shared<Tower>(Circle{Point{10, 7}, 1}),
            make_shared<Building>(Rectangle{{11, 4},
                                            {14, 6}})
    };

    for (size_t i = 0; i < game_map.size(); ++i) {
        Assert(
                Collide(*game_map[i], *game_map[i]),
                "An object doesn't collide with itself: " + to_string(i)
        );

        for (size_t j = 0; j < i; ++j) {
            Assert(
                    !Collide(*game_map[i], *game_map[j]),
                    "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
            );
        }
    }

    auto new_warehouse = make_shared<Building>(Rectangle{{4, 3},
                                                         {9, 6}});
    ASSERT(!Collide(*new_warehouse, *game_map[0]));
    ASSERT(Collide(*new_warehouse, *game_map[1]));
    ASSERT(!Collide(*new_warehouse, *game_map[2]));
    ASSERT(Collide(*new_warehouse, *game_map[3]));
    ASSERT(Collide(*new_warehouse, *game_map[4]));
    ASSERT(!Collide(*new_warehouse, *game_map[5]));
    ASSERT(!Collide(*new_warehouse, *game_map[6]));

    auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
    ASSERT(!Collide(*new_defense_tower, *game_map[0]));
    ASSERT(!Collide(*new_defense_tower, *game_map[1]));
    ASSERT(!Collide(*new_defense_tower, *game_map[2]));
    ASSERT(Collide(*new_defense_tower, *game_map[3]));
    ASSERT(Collide(*new_defense_tower, *game_map[4]));
    ASSERT(!Collide(*new_defense_tower, *game_map[5]));
    ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestAddingNewObjectOnMap);
    return 0;
}