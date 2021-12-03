#include <iostream>

class Grid{
    struct Cell{
        bool state;
        int count;

        explicit
        Cell(bool state = false) : state(state), count(0) {};

        Cell(const Cell &c) = default;
        Cell& operator=(const Cell &c) = default;

        Cell(Cell &&c) {
            if (this != &c) {
                bool tmp_state = c.state;
                int tmp_count = c.count;
                c.~Cell();
                this->state = tmp_state;
                this->count = tmp_count;
            }
        };
        Cell& operator=(Cell &&c) {
            if (this == &c) {
                return *this;
            } else {
                bool tmp_state = c.state;
                int tmp_count = c.count;
                c.~Cell();
                state = tmp_state;
                count = tmp_count;
                return *this;
            }
        };

        ~Cell() = default;

        friend std::ostream& operator<<(std::ostream& out, const Cell& c) {
            out << c.state;
            return out;
        }
    };
private:
    Cell* field = new Cell[100];
public:
    explicit
    Grid(bool state = false) {
        for (int i = 0; i < 100; i++) {field[i] = Cell(state);}
    };

    Grid(const Grid &g) {
        for (int i = 0; i < 100; i++) {
            field[i] = g.field[i];
        }
    }
    Grid& operator=(const Grid &g) {
        if (this != &g) {
            for (int i = 0; i < 100; i++) {
                field[i] = g.field[i];
            }
        }
        return *this;
    }

    Grid(Grid &&g) {
        if (this != &g) {
            Cell* tmp = g.field;
            g.field = nullptr;
            field = tmp;
        }
    };
    Grid& operator=(Grid &&g) {
        if (this == &g) {
            return *this;
        } else {
            Cell* tmp = g.field;
            g.field = nullptr;
            field = tmp;
            return *this;
        }
    };

    ~Grid() {
        delete [] field;
    };

    friend std::ostream& operator<<(std::ostream& out, const Grid& g) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                out << g.field[i*10 + j].state << " ";
            }
            out << std::endl;
        }
        out << std::endl;

//        for (int i = 0; i < 10; i++) {
//            for (int j = 0; j < 10; j++) {
//                out << g.field[i*10 + j].count << " ";
//            }
//            out << std::endl;
//        }
//        out << std::endl;
        out << std::endl;
        return out;
    };

    void insert(int i, int j, bool state) {
        field[i*10 + j] = Cell(state);
    };

    bool operator[](int i) const {
        return &this->field[i];
    };

    int recalculate_cell_count(int i, int j) {
        int count = 0;
        for (int p = -1; p < 2; p++) {
            for (int q = -1; q < 2; q++) {
                if ((0 <= (i + p) && (i + p) < 10 && 0 <= (j + q) && (j + q) < 10 && p+p + q*q != 0) &&
                    (field[(i + p)*10 + (j + q)]).state) {
                    count++;
                }
            }
        }
        return count;
    };

    void recalculate_field_count() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                field[i*10 + j].count = recalculate_cell_count(i, j);
            }
        }
    }

    void recalculate_field_state() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                bool tmp_state = field[i*10 + j].state;
                int tmp_count = field[i*10 + j].count;
                if (tmp_state) {
                    if (tmp_count != 2 && tmp_count != 3) {field[i*10 + j].state = false;}
                } else {
                    if (tmp_count == 3) {field[i*10 + j].state = true;}
                }
            }
        }
    }

    void life_step() {
        this->recalculate_field_count();
        this->recalculate_field_state();
    }
};

int main() {
    Grid g(false);
    //std::cout << g;

    g.insert(3, 2, true);
    g.insert(3, 3, true);
    g.insert(3, 4, true);
    g.insert(2, 4, true);
    g.insert(1, 3, true);

    for (int i = 0; i < 5; i++) {
        std::cout << g;
        g.life_step();
    }
    return 0;
}
