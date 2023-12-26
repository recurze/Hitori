#include <functional>
#include <iostream>
#include <vector>


enum Color { WHITE, GRAY, BLACK };

class SquareBoard {
public:
    SquareBoard(std::size_t _n, const std::vector<int>& _board):
        n(_n),
        board(_board),
        color(std::vector<Color>(n*n, GRAY)) {
    }

    void set_color(int i, int j, Color c) {
        color[i*n + j] = c;
    }

    auto get_value(int i, int j) const {
        return board[i*n + j];
    }

    auto get_color(int i, int j) const {
        return color[i*n + j];
    }

    auto get_size() const {
        return n;
    }

    bool is_black(int i, int j) const {
        return get_color(i, j) == BLACK;
    }

    bool is_gray(int i, int j) const {
        return get_color(i, j) == GRAY;
    }

private:
    std::size_t n;
    std::vector<int> board;
    std::vector<Color> color;
};


std::ostream& operator<<(std::ostream& os, const SquareBoard& board) {
    auto n = board.get_size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            os << board.get_value(i, j) << ' ';
        }
        os << '\n';
    }

    std::cout << "\nColors: WHITE(0), GRAY(1), BLACK(2)\n";

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            os << board.get_color(i, j) << ' ';
        }
        os << '\n';
    }

    return os;
}

using Coordinate = std::pair<int, int>;

auto horizontal_and_vertical_neighbors(int i, int j, std::size_t n) {
    std::vector<Coordinate> neighbors;

    if (i > 0) {
        neighbors.push_back({i - 1, j});
    }
    if (j > 0) {
        neighbors.push_back({i, j - 1});
    }
    if (i < n - 1) {
        neighbors.push_back({i + 1, j});
    }
    if (j < n - 1) {
        neighbors.push_back({i, j + 1});
    }

    return neighbors;
}

auto diagonal_neighbors(int i, int j, std::size_t n) {
    std::vector<int> neighbors;

    if (i < n - 1) {
        if (j > 0) {
            neighbors.push_back((i + 1)*n + (j - 1));
        }
        if (j < n - 1) {
            neighbors.push_back((i + 1)*n + (j + 1));
        }
    }
    if (i > 0) {
        if (j > 0) {
            neighbors.push_back((i - 1)*n + (j - 1));
        }
        if (j < n - 1) {
            neighbors.push_back((i - 1)*n + (j + 1));
        }
    }

    // Join all edge cells to a single black cell at (n, n)
    if (i == 0 or j == 0 or i == n - 1 or j == n - 1) {
        neighbors.push_back(n * n);
    }

    return neighbors;
}

void find_starting_pattern_colors(
    const SquareBoard board,
    std::vector<Coordinate>& blacks,
    std::vector<Coordinate>& whites) {

    auto n = board.get_size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // Same value in adjacent cells in a row
            if (j < n - 1 and board.get_value(i, j) == board.get_value(i, j + 1)) {
                // Every other same value in this row must be black
                for (int k = 0; k < n; ++k) {
                    if (k != j and k != j + 1 and board.get_value(i, k) == board.get_value(i, j)) {
                        blacks.push_back({i, k});
                    }
                }
            }

            // Same value in adjacent cells in a column
            if (i < n - 1 and board.get_value(i, j) == board.get_value(i + 1, j)) {
                // Every other same value in this column must be black
                for (int k = 0; k < n; ++k) {
                    if (k != i and k != i + 1 and board.get_value(i, j) == board.get_value(k, j)) {
                        blacks.push_back({k, j});
                    }
                }
            }

            // Sandwiched between 2 same values in a row
            if (j < n - 2 and board.get_value(i, j) == board.get_value(i, j + 2)) {
                whites.push_back({i, j + 1});
            }

            // Sandwiched between 2 same values in a column
            if (i < n - 2 and board.get_value(i, j) == board.get_value(i + 2, j)) {
                whites.push_back({i + 1, j});
            }
        }
    }
}

bool is_black_loop(const SquareBoard& board, int i, int j) {
    auto n = board.get_size();
    std::vector<bool> discovered(n * n + 1, false);

    std::function<bool(int, int)> _dfs = [&](int u, int p) {
        if (discovered[u]) { // LOOP
            return true;
        }

        discovered[u] = true;
        for (const auto v: diagonal_neighbors(u/n, u%n, n)) if (v != p) {
            if (v == n * n or board.is_black(v/n, v%n)) {
                if (_dfs(v, u)) {
                    return true;
                }
            }
        }

        return false;
    };

    return _dfs(i*n + j, -1);
}

void force_whites_that_avoid_isolation(
    const SquareBoard board,
    std::vector<Coordinate>& whites) {

    auto n = board.get_size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board.is_gray(i, j) and is_black_loop(board, i, j)) {
                whites.push_back({i, j});
            }
        }
    }
}

void solve(SquareBoard& board) {
    auto n = board.get_size();

    std::vector<Coordinate> blacks, whites;
    find_starting_pattern_colors(board, blacks, whites);

    do {
        while (not blacks.empty()) {
            auto [i, j] = blacks.back(); blacks.pop_back();
            board.set_color(i, j, BLACK);
            for (const auto& neighbor: horizontal_and_vertical_neighbors(i, j, n)) {
                whites.push_back(neighbor);
            }
        }

        while (not whites.empty()) {
            auto [i, j] = whites.back(); whites.pop_back();
            board.set_color(i, j, WHITE);
            for (int k = 0; k < n; ++k) {
                // Same value in same row without color
                if (board.is_gray(i, k) and board.get_value(i, j) == board.get_value(i, k)) {
                    blacks.push_back({i, k});
                }
                // Same value in same column without color
                if (board.is_gray(k, j) and board.get_value(i, j) == board.get_value(k, j)) {
                    blacks.push_back({k, j});
                }
            }
        }

        if (blacks.empty() and whites.empty()) {
            force_whites_that_avoid_isolation(board, whites);
        }
    } while (not blacks.empty() or not whites.empty());
}

int main() {
    std::size_t n; std::cin >> n;

    std::vector<int> puzzle(n * n);
    for (int i = 0; i < n*n; ++i) {
        std::cin >> puzzle[i];
    }
    auto b = SquareBoard(n, puzzle);

    solve(b);

    std::cout << b << '\n';
    return 0;
}
