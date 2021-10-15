#include <iostream>


template <typename T>
class Grid {
private:
    size_t *size = nullptr;
    T **memory = nullptr;
    size_t x_size = 0, y_size = 0;

public:
    Grid<T>(size_t x_size, size_t y_size) : x_size(x_size), y_size(y_size) {
        memory = new T * [x_size*y_size];
        size = new size_t [x_size*y_size];
        for (int i = 0; i < x_size*y_size; i++) {
            memory[i] = new T;
            size[i] = 1;
        }
    }

    T operator()(size_t x_idx, size_t y_idx) const {
        float sum = 0;
        for (int i = 0; i < size[x_idx*(y_size) + y_idx]; i++) {
            sum += memory[x_idx*(y_size) + y_idx][i];
        }
        return sum/size[x_idx*(y_size) + y_idx];
    }

    T & operator()(size_t x_idx, size_t y_idx) {
        return *memory[x_idx*(y_size) + y_idx];
    };

    size_t get_xsize() const {
        return x_size;
    }

    size_t get_ysize() const {
        return y_size;
    }

    Grid& operator=(T x) {
        for (size_t i = 0; i < x_size*y_size; i++) {
            for (size_t j = 0; j < size[i]; j++) {
                memory[i][j] = x;
            }
        }
        return *this;
    }

    Grid& make_subgrid(size_t x_idx, size_t y_idx,
                       size_t x_sub_size, size_t y_sub_size) {
        delete [] memory[x_idx*(y_size) + y_idx];
        memory[x_idx*(y_size) + y_idx] = new T [x_sub_size*y_sub_size];
        size[x_idx*(y_size) + y_idx] = x_sub_size*y_sub_size;
        return *this;
    };

    Grid& collapse_subgrid(size_t x_idx, size_t y_idx,
                           size_t x_sub_size, size_t y_sub_size) {
        T sum = 0;
        for (size_t i = 0; i < x_sub_size*y_sub_size; i++) {
            sum += memory[x_idx*(y_size) + y_idx][i];
        }
        delete [] memory[x_idx*(y_size) + y_idx];
        memory[x_idx*(y_size) + y_idx] = new T(sum/(x_sub_size*y_sub_size));
        size[x_idx*(y_size) + y_idx] = 1;
        return *this;
    }

    Grid& get_subgrid(size_t x_idx, size_t y_idx) {
        return this[x_idx*(y_size) + y_idx];
    }

    bool is_subgrid(size_t x_idx, size_t y_idx) const {
        return size[x_idx*(y_size) + y_idx] - 1;
    }

    friend std::ostream& operator<<(std::ostream& out, Grid const& g) {
        for (size_t i = 0; i < g.x_size; i++) {
            for (size_t j = 0; j < g.y_size; j++) {
                out.width(5);
                out << g(i, j);
            }
            out << std::endl;
        }
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Grid & g) {
        for (size_t i = 0; i < g.x_size; i++) {
            for (size_t j = 0; j < g.y_size; j++) {
                in >> *g.memory[i*(g.y_size) + j];
            }
        }
        return in;
    }

    ~Grid() {
        delete [] memory;
    }
};

int main() {
    Grid<float> g = Grid<float>(2, 3);
    g = 5;
    std::cout << g(0, 0) << std::endl;
    std::cout << "x_size:" << g.get_xsize() << std::endl;
    std::cout << "y_size:" << g.get_ysize() << std::endl;
    std::cout << g;
    std::cin >> g;
    std::cout << g;
    g.make_subgrid(1, 1, 2, 2);
    std::cout << &g.get_subgrid(1, 1) << std::endl;
    g.collapse_subgrid(1, 1, 2, 2);
    std::cout << g;
    g.make_subgrid(1, 1, 20, 20);
    std::cout << g;
    std::cout << g.is_subgrid(1, 1) << std::endl;
    std::cout << g.is_subgrid(0, 0) << std::endl;
    return 0;
}