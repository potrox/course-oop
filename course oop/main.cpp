#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

class Cell {
public:
    enum CellType { Empty, Mine, Number };

    Cell() : type(Empty), isRevealed(false) {}

    virtual void reveal() = 0;
    virtual void display() const = 0;

    bool isMine() const {
        return type == Mine;
    }

    bool isNumber() const {
        return type == Number;
    }

    bool isRevealedCell() const {
        return isRevealed;
    }

    void setRevealed() {
        isRevealed = true;
    }

protected:
    CellType type;
    bool isRevealed;
};

class EmptyCell : public Cell {
public:
    EmptyCell() : Cell() {}

    void reveal() override {
        setRevealed();
    }

    void display() const override {
        if (isRevealed) {
            std::cout << " ";
        }
        else {
            std::cout << "-";
        }
    }
};

class MineCell : public Cell {
public:
    MineCell() : Cell() {
        type = Mine;
    }

    void reveal() override {
        setRevealed();
    }

    void display() const override {
        if (isRevealed) {
            std::cout << "*";
        }
        else {
            std::cout << "-";
        }
    }
};

class NumberCell : public Cell {
public:
    NumberCell(int number) : Cell(), number(number) {
        type = Number;
    }

    void reveal() override {
        setRevealed();
    }

    void display() const override {
        if (isRevealed) {
            std::cout << number;
        }
        else {
            std::cout << "-";
        }
    }

private:
    int number;
};

class MinesweeperGame {
public:
    MinesweeperGame(int rows, int cols, int mineCount)
        : rows(rows), cols(cols), mineCount(mineCount), isGameOver(false) {
        initializeBoard();
        placeMines();
        calculateNumbers();
    }

    void displayBoard() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                board[i][j]->display();
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    void play(int row, int col) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            std::cout << "Invalid move. Try again." << std::endl;
            return;
        }

        Cell* selectedCell = board[row][col];
        if (!selectedCell->isRevealedCell()) {
            selectedCell->reveal();
            if (selectedCell->isMine()) {
                isGameOver = true;
                std::cout << "Game Over! You hit a mine." << std::endl;
            }
            else if (selectedCell->isNumber()) {
                std::cout << "Cell is a number. Keep playing." << std::endl;
            }
            else {
                std::cout << "Cell is empty. Keep playing." << std::endl;
            }
        }
        else {
            std::cout << "Cell already revealed. Try again." << std::endl;
        }
    }

    bool gameOver() const {
        return isGameOver;
    }

private:
    int rows;
    int cols;
    int mineCount;
    bool isGameOver;
    std::vector<std::vector<Cell*>> board;

    void initializeBoard() {
        board.resize(rows, std::vector<Cell*>(cols, nullptr));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                board[i][j] = new EmptyCell();
            }
        }
    }

    void placeMines() {
        srand(static_cast<unsigned>(time(nullptr)));
        int minesPlaced = 0;

        while (minesPlaced < mineCount) {
            int randRow = rand() % rows;
            int randCol = rand() % cols;

            if (!board[randRow][randCol]->isMine()) {
                delete board[randRow][randCol];
                board[randRow][randCol] = new MineCell();
                ++minesPlaced;
            }
        }
    }

    void calculateNumbers() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (!board[i][j]->isMine()) {
                    int mineCount = countAdjacentMines(i, j);
                    if (mineCount > 0) {
                        delete board[i][j];
                        board[i][j] = new NumberCell(mineCount);
                    }
                }
            }
        }
    }

    int countAdjacentMines(int row, int col) const {
        int mineCount = 0;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newRow = row + i;
                int newCol = col + j;

                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
                    board[newRow][newCol]->isMine()) {
                    ++mineCount;
                }
            }
        }

        return mineCount;
    }
};

int main() {
    MinesweeperGame game(10, 10, 10);

    while (!game.gameOver()) {
        game.displayBoard();
        std::cout << "Enter row and column (e.g., 1 2): ";
        int row, col;
        std::cin >> col >> row;
        game.play(row-1, col-1);
    }

    return 0;
}