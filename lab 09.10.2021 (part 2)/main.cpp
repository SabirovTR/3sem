#include <iostream>

template <typename T>
class Grid {
private:
    int x_size;
    int y_size;
    Grid* subGrids;
    T value;
public:
    explicit
    Grid(T value = 0) : value(value) {
        x_size = 1;
        y_size = 1;
        subGrids = nullptr;
    }

    Grid(Grid& g) : x_size(g.x_size), y_size(g.y_size), value(g.value) {
        subGrids = new Grid[x_size*y_size];
        if (g.subGrids != nullptr) {
            for (int i = 0; i < x_size * y_size; i++) {
                if (g.subGrids[i].subGrids != nullptr) {
                    Grid tmp(g.subGrids[i]);
                    subGrids[i] = tmp;
                } else {
                    this->insert(i, g.subGrids[i].value);
                }
            }
        } else {this->insert(0, g.subGrids[0].value);}
    }

    T operator()(int x_idx, int y_idx) const {
        return subGrids[x_idx*x_size + y_idx].value;
    }

    int get_xsize() const {return x_size;}
    int get_ysize() const {return y_size;}

    void insert(int x_idx, int y_idx, T value_in) {
        subGrids[x_idx*y_size + y_idx].x_size = 1;
        subGrids[x_idx*y_size + y_idx].y_size = 1;
        subGrids[x_idx*y_size + y_idx].value = value_in;
        subGrids[x_idx*y_size + y_idx].subGrids = nullptr;

        T mid = 0;
        for (int j = 0; j < x_size*y_size; j++) {
            mid += subGrids[j].value/(x_size*y_size);
        }
        value = mid;
    }

    void insert(int i, T value_in) {
        subGrids[i].x_size = 1;
        subGrids[i].y_size = 1;
        subGrids[i].value = value_in;
        subGrids[i].subGrids = nullptr;

        T mid = 0;
        for (int j = 0; j < x_size*y_size; j++) {
            mid += subGrids[j].value/(x_size*y_size);
        }
        value = mid;
    }

    void insert(int x_idx, int y_idx, const Grid& g) {
        subGrids[x_idx*y_size + y_idx].x_size = g.x_size;
        subGrids[x_idx*y_size + y_idx].y_size = g.y_size;
        subGrids[x_idx*y_size + y_idx].value = g.value;
        subGrids[x_idx*y_size + y_idx].subGrids = g.subGrids;

        T mid = 0;
        for (int j = 0; j < x_size*y_size; j++) {
            mid += subGrids[j].value/(x_size*y_size);
        }
        value = mid;
    }

    void insert(int i, const Grid& g) {
        subGrids[i].x_size = g.x_size;
        subGrids[i].y_size = g.y_size;
        subGrids[i].value = g.value;
        subGrids[i].subGrids = g.subGrids;

        T mid = 0;
        for (int j = 0; j < x_size*y_size; j++) {
            mid += subGrids[j].value/(x_size*y_size);
        }
        value = mid;
    }

    Grid(int x_size, int y_size, T value) : x_size(x_size), y_size(y_size), value(value) {
        subGrids = new Grid[x_size*y_size];
        for (int i = 0; i < x_size*y_size; i++) {
            this->insert(i, value);
        }
    }

    Grid& operator=(T value_in) {
        value = value_in;
        for (int i = 0; i < x_size*y_size; i++) {
            subGrids[i].value = value_in;
        }
        return *this;
    }

    Grid& collapse_subgrid(int x_idx, int y_idx) {
        subGrids[x_idx*y_size + y_idx] = Grid(subGrids[x_idx*y_size + y_idx].value);
        return *this;
    }

    Grid& get_subgrid(size_t x_idx, size_t y_idx) {
        if (this->is_subrid(x_idx, y_idx)) {
            return subGrids[x_idx*y_size + y_idx];
        } else {
            std::cout << "Error" << std::endl;
        }
    }

    bool is_subrid(size_t x_idx, size_t y_idx) const {
        if (subGrids[x_idx*y_size + y_idx].subGrids != nullptr) {
            return true;
        } else {return false;}
    }

    friend std::ostream& operator<<(std::ostream& out, const Grid& g) {
        if (g.x_size*g.y_size != 1) {
            for (int i = 0; i < g.x_size; i++) {
                for (int j = 0; j < g.y_size; j++) {
                    out << g.subGrids[i*g.y_size + j].value << " "
                        << g.subGrids[i*g.y_size + j].x_size*g.subGrids[i*g.y_size + j].y_size << " ";
                        //Выводим размер подсетки, чтобы видеть наличие подсеток
                }
                out << std::endl;
            }
        } else {out << g.value << " ";}
        out << std::endl;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Grid& g) {
        for (size_t i = 0; i < g.x_size; i++) {
            for (size_t j = 0; j < g.y_size; j++) {
                in >> g.subGrids[i*(g.y_size) + j].value;
            }
        }
        return in;
    }

    ~Grid() {
        if (subGrids != nullptr) {
            for (int i = 0; i < x_size*y_size; i++) {
                this->subGrids[i].~Grid();
            }
        } else {delete [] subGrids;}
    }
};


int main() {
    Grid<double> g(4, 2, 11);
    std::cout << g;
    Grid<double> gg = g;
    std::cout << gg;
    g.insert(4, gg);
    g.insert(6, gg);
    std::cout << g;
    Grid<double> ggg(g);
    std::cout << ggg;
    g.insert(3, ggg);
    std::cout << g;

    std::cout << "testing is_subgrid" << std::endl;
    gg.~Grid();
    gg = 5;
    std::cout << gg;
    g.insert(1, 1, gg);
    std::cout << gg.is_subrid(1, 1) << std::endl;
    std::cout << g.is_subrid(1, 1) << std::endl;
    return 0;
}