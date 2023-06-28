#include <stdio.h>
#include <stdlib.h>
#include <algorithm> 
#define NPLAYERS 2
#define PLAYER0 'O'
#define PLAYER1 'X'
#define STANDARD_NROWS 10
#define STANDARD_NCOLS 3

struct board_entry {
    int player_id;
    char val;
    int row, col;

    board_entry() {
        this->player_id = -1;
        this->val = '-';
    }

    void init(int r, int c) {
        this->row = r;
        this->col = c;
    }

    void assign(int player) {
        this->player_id = player;
        if (player == 0) {
            this->val = PLAYER0;
        } else if (player == 1) {
            this->val = PLAYER1;
        }
    }
};

struct board {
    /*grid of the board is accessed grid[row][column]*/
    int nrows;
    int ncols;
    board_entry** grid;
    int current_move;
    bool is_done;

    board(){}

    board(int nrows, int ncols) {
        this->current_move = 0;
        this->is_done = false;
        this->nrows = nrows;
        this->ncols = ncols;
        grid = new board_entry*[nrows];
        for(int i = 0; i < nrows; i++) {
            grid[i] = new board_entry[ncols];
        }

        for (int r = 0; r < nrows; r++) {
            for (int c = 0; c < ncols; c++) {
                grid[r][c].init(r, c);
            }
        }
    }

    void print() {
        for (int i = 0; i < nrows; i++) {
            for (int j = 0; j < ncols; j++) {
                printf("%c ", grid[i][j].val);
            }
            printf("\n");
        }
    }

    bool fast_check_winner(int player, int row, int col) {
        // check below
        if (row - 3 >= 0) {
            if (this->grid[row][col].player_id == player && 
                this->grid[row - 1][col].player_id == player && 
                this->grid[row - 2][col].player_id == player && 
                this->grid[row - 3][col].player_id == player) {
                    return true;
            }
        }

        int left_bound = std::max(col - 3, 0);
        int right_bound = std::min(col + 3, this->ncols - 1);
        // check across
        for (int i = left_bound; i <= right_bound - 3; i++) {
            if (this->grid[row][i].player_id == player && 
                this->grid[row][i + 1].player_id == player && 
                this->grid[row][i + 2].player_id == player && 
                this->grid[row][i + 3].player_id == player) {
                    return true;
            }
        }

        int upper_bound = std::min(row + 3, this->nrows - 1);
        int lower_bound = std::max(row - 3, 0);
        for (int i = left_bound + 3; right_bound; i++) {
            for (int j = lower_bound; j < upper_bound - 3; j++) {

            }
        }
        // check diagonal up

        // for (int i = lower_bound; i <= upper_bound - 3; i++) {
        //     for (int j = left_bound; j <= right_bound - 3; j++) {
        //         if (this->grid[j][i].player_id == player &&
        //             this->grid[j + 1][i + 1].player_id == player &&
        //             this->grid[j + 2][j + 3].player_id == player &&
        //             this->grid[j + 4][j + 4].player_id == player) {
        //                 return true;
        //         }
        //     }
        // }

        // check diagonal down

        return true;
    }

    // checks for a winner after a drop into the entry at row,col
    bool check_winner(int player, int row, int col) {
        // todo: optimize to check only where the most recent play was

        // horizontal
        for (int j = 0; j < this->nrows - 3; j++) {
            for (int i = 0; i < this->ncols; i++) {
                if (this->grid[j][i].player_id == player && 
                    this->grid[j+1][i].player_id == player && 
                    this->grid[j+2][i].player_id == player && 
                    this->grid[j+3][i].player_id == player) {
                        return true;
                }           
            }
        }

        // vertical
        for (int i = 0; i < this->nrows - 3; i++) {
            for (int j = 0; j < this->ncols; j++) {
                if (this->grid[i][j].player_id == player && 
                    this->grid[i+1][j].player_id == player && 
                    this->grid[i+2][j].player_id == player && 
                    this->grid[i+3][j].player_id == player) {
                        return true;
                }           
            }
        }

        // diagonal up 
        for (int i = 3; i < this->ncols; i++) {
            for (int j = 0; j < this->nrows - 3; j++) {
                if (this->grid[j][i].player_id == player && 
                    this->grid[j+1][i-1].player_id == player && 
                    this->grid[j+2][i-2].player_id == player&& 
                    this->grid[j+3][i-3].player_id == player) {
                        return true;
                }
            }
        }

        // diagonal down
        for (int i = 3; i < this->ncols; i++) {
            for (int j = 3; j < this->nrows; j++) {
                if (this->grid[j][i].player_id == player && 
                    this->grid[j-1][i-1].player_id == player && 
                    this->grid[j-2][i-2].player_id == player && 
                    this->grid[j-3][i-3].player_id == player) {
                        return true;
                }
            }
        }

        return false;
    }

    bool check_full() {
        for (int i = 0; i < this->ncols; i++) {
            if (grid[0][i].val == '-') {
                return false;
            }
        }
        return true;
    }

    void drop(int n) {
        // n is the column to drop into
        if (n < 1 || n > this->ncols) {
            printf("Woah there! You're outside the board!\n");
            return;
        }

        if (grid[0][n-1].val == PLAYER0 || grid[0][n-1].val == PLAYER1 ) {
            printf("....that column is full!\n");
            return;
        }

        for (int r = nrows - 1; r >= 0; r--) {
            if (grid[r][n-1].val == '-') {
                grid[r][n-1].assign(current_move % NPLAYERS);
                if (check_winner(current_move % NPLAYERS, r, n-1)) {
                    this->print();
                    printf("***PLAYER %i WON!***\n", current_move % NPLAYERS);
                    this->is_done = true;
                    return;
                }

                if (this->check_full()) {
                    this->print();
                    printf("***BOARD IS FULL! GAME TIED***\n");
                    this->is_done = true;
                    return;
                }

                current_move++;
                return;
            }
        }
    }

    ~board() {}
};

int main(int argc, char *argv[]) {
    board new_board;
    if (argc > 1) {

        new_board = board(atoi(argv[1]), atoi(argv[2]));
    } else {
        new_board = board(STANDARD_NROWS, STANDARD_NCOLS);;
    }

    int col;
    while (!new_board.is_done) {
        new_board.print();
        printf("Player %i, choose a column: ", new_board.current_move % NPLAYERS);
        scanf("%i", &col); 
        new_board.drop(col);
    }
}