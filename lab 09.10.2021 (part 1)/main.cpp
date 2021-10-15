#include <iostream>

template <typename T>
class Grid {
private:
    T *memory = nullptr;
    size_t x_size = 0, y_size = 0;

public:
    Grid(size_t x_size, size_t y_size) : x_size(x_size), y_size(y_size) {
        memory = new T[x_size*y_size];
    }

    T operator()(size_t x_idx, size_t y_idx) const {
        return memory[x_idx*(y_size) + y_idx];
    }

    T& operator()(size_t x_idx, size_t y_idx) {
        return memory[x_idx*(y_size) + y_idx];
    };

    size_t get_xsize() const {
        return x_size;
    }

    size_t get_ysize() const {
        return y_size;
    }

    Grid& operator=(T x) {
        for (size_t i = 0; i < x_size; i++) {
            for (size_t j = 0; j < y_size; j++) {
                memory[i*(y_size) + j] = x;
            }
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, Grid const& g) {
        for (size_t i = 0; i < g.x_size; i++) {
            for (size_t j = 0; j < g.y_size; j++) {
                out << g(i, j) << " ";
            }
            out << std::endl;
        }
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Grid & g) {
        for (size_t i = 0; i < g.x_size; i++) {
            for (size_t j = 0; j < g.y_size; j++) {
                in >> g.memory[i*(g.y_size) + j];
            }
        }
        return in;
    }

    ~Grid() {
        delete [] memory;
    }
};

int main() {
    Grid<char> g = Grid<char>(2, 3);
    g = 'a';
    std::cout << g;
    std::cin >> g;
    std::cout << g;
    std::cout << "x_size:" << g.get_xsize() << std::endl;
    std::cout << "y_size:" << g.get_ysize() << std::endl;
    std::cout << g(1, 1) << std::endl;
    return 0;
}
