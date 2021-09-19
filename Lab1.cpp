#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <cmath>

// Electromagnetic interaction
// You have to create world and be able to add and delete particles from it
// When you add or delete particle you must recalculate all interactions

struct vector3 {
    double x = 0;
    double y = 0;
    double z = 0;

    vector3 ()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    vector3 (double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vector3(const vector3& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
    vector3& operator+=(const vector3 v)
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
    }
    vector3 operator+(const vector3 v)
    {
        vector3 sum = *this;
        sum.x += v.x;
        sum.y += v.y;
        sum.z += v.z;
        return sum;
    }
    vector3 operator-(const vector3 v)
    {
        vector3 sum = *this;
        sum.x -= v.x;
        sum.y -= v.y;
        sum.z -= v.z;
        return sum;
    }
    vector3 operator*(const double a)
    {
        vector3 mult = *this;
        mult.x *= a;
        mult.y *= a;
        mult.z *= a;
        return mult;
    }

    double abs()
    {
        vector3 v = *this;
        return std::pow((v.x*v.x + v.y*v.y + v.z*v.z), 0.5);
    }
};

class Particle {
protected:
    double mass = 0;
    double charge = 0;
    vector3 coordinate = {0, 0, 0};
    vector3 velocity = {0, 0, 0};
    vector3 force = {0, 0, 0};
    std::string name = "NONE";


public:
    Particle() = default;
    Particle(double m, double q, vector3 r, vector3 v, vector3 f, std::string name) : mass(m), charge(q), coordinate(r), velocity(v), force(f), name(name) {};
    std::string getName() const
    {
        return name;
    }
    double getMass() const
    {
        return mass;
    }
    double getCharge() const
    {
        return charge;
    }
    vector3 getCoordinate() const
    {
        return coordinate;
    }
    vector3 getVelocity() const
    {
        return velocity;
    }
    vector3 getForce() const
    {
        return force;
    }
    double getEnergy()
    {
        return 0.5*mass*std::pow(velocity.abs(), 2);
    }
    void forceRecalculation(const vector3 new_force)
    {
        force = new_force;
    }
};

class Electron : public Particle
{
    double energy_rest = 0;
    double spin_number = 0;
public:
    Electron(double m, double q, vector3 r, vector3 v, vector3 f, std::string& name, double energy_rest, double spin_number)
    {
        this->mass = m;
        this->charge = q;
        this->coordinate = r;
        this->velocity = v;
        this->force = f;
        this->name = name;
        this->energy_rest = energy_rest;
        this->spin_number = spin_number;
    }
    double getEnergy_rest() const
    {
        return energy_rest;
    }
    double getSpin_nuber() const
    {
        return spin_number;
    };
};

class Neitrino : public Particle
{
    double frequency = 1;
public:
    Neitrino() = default;
    Neitrino(double frequency) : frequency(frequency) {};
    //Neitrino(double m, double q, vector3 r, vector3 v, vector3 f, std::string& name, double frequency) : mass(m), charge(q), coordinate(r), velocity(v), force(f), name(name), frequency(frequency) {};
    double getFrequency() const
    {
        return frequency;
    }
};



class Logger {
    std::vector<std::string> Data;
public:
    void creatingLog(Particle p)
    {
        Data.push_back("Created: " + p.getName());
    }
    void deletingLog(Particle p)
    {
        Data.push_back("Deleted: " + p.getName());
    }
    void show() const
    {
        std::cout << "Logger:" << '\n';
        for (int i = 0; i < Data.size(); i++)
        {
            std::cout << Data[i] << '\n';
        }
        std::cout << '\n';
    }

};

std::ostream& operator<<(std::ostream& os, const vector3& v)
{
    std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}
// в этом классе опишите мир, состоящий из некоторого числа частиц
class World {
    std::vector<Particle> v;
    Logger l;
    int counter = 0;
public:
    void addParticle(const Particle& p)
    {
        v.push_back(p);
        l.creatingLog(p);
        counter++;
        recalculation();
    };
    void deleteParticle(const Particle& p)
    {
        int tmp = 0;
        for (int i = 0; i < v.size(); i++)
        {
            if (v[i].getName() == p.getName())
            {
                tmp = i;
                break;
            }
        }
        v.erase(v.begin() + tmp);
        l.deletingLog(p);
        counter--;
        recalculation();
    };
    int length() const
    {
        return counter;
    }
    Particle getParticle(int i) const
    {
        return v[i];
    }
    void showLog() const
    {
        l.show();
    }

    void recalculation()
    {
        for (int i = 0; i < v.size(); i++)
        {
            vector3 tmp_force(0, 0, 0);
            for (int j = 0; j < v.size(); j++)
            {
                if (i != j)
                {
                    tmp_force += (v[i].getCoordinate() - v[j].getCoordinate())*((v[i].getCharge()*v[j].getCharge())/std::pow((v[i].getCoordinate() - v[j].getCoordinate()).abs(), 3));
                }
            }
            v[i].forceRecalculation(tmp_force);
        }
    }
};

// перегрузите оператор вывода, чтобы можно было вывести состав мира
// (может потребоваться перегрузка оператора вывода для частиц)

std::ostream& operator<<(std::ostream& os, const Particle& p)
{
    std::cout << "Name:       " << p.getName() << '\n';
    std::cout << "Mass:       " << p.getMass() << '\n';
    std::cout << "Charge:     " << p.getCharge() << '\n';
    std::cout << "Coordinate: " << p.getCoordinate() << '\n';
    std::cout << "Velocity:   " << p.getVelocity() << '\n';
    std::cout << "Force:      " << p.getForce() << '\n';
}
std::ostream& operator<<(std::ostream& os, const World& w)
{
    w.showLog();
    std::cout << "World: " << '\n';
    for (int i = 0; w.length() > i; i++)
    {
        std::cout << w.getParticle(i);
        std::cout << '\n';
    }
}

// здесь создайте мир/миры, куда добавьте некоторое число частиц, взаимодействующих друг с другом
int main() {
    World w;
    Particle a(4.0, -6, {0, 8.3, 0}, {3, 4, 120}, {0, 0, 0}, "Major Tom");
    Particle b(4.0, +6, {-3, 7.2, 7}, {3, 4, 12}, {0, 0, 0}, "Thief Jerry");
    Neitrino n(4.5);
    w.addParticle(a);
    w.addParticle(b);
    w.deleteParticle(a);
    Particle c(1, 1, {2, 3, 5}, {8, 13, 21}, {0, 0, 0}, "Mona Lisa");
    w.addParticle(c);
    w.addParticle(a);
    w.addParticle(n);
    std::cout << w;
    std::cout << '\n' << "Neitrino's frequency: " << n.getFrequency() << '\n';
    std::cout << '\n' << n.getCharge() << " - Neitrino has zero charge\n";
}
