//
// Created by velkejondra on 1/21/21.
//
#include "../src/Board.h"
#include "../src/MoveGenerator.h"
#include "../src/debug.h"
#include "../src/defs.h"

using namespace std;

int INDEX = 0;

void print_dot_graph(int depht, Board &board) {
    MoveGenerator gen = MoveGenerator();
    int index = INDEX;
    if (depht == 0) {
        return;
    }
    for (auto move : gen.getLegalMoves(board)) {
        ++INDEX;
        Board next_board = board;
        next_board.MakeMove(move);
        cout << INDEX << "[label= \"" << next_board.GetMoveNotation() << "\"]" << ";\n";
        cout << index << " ->" << INDEX << ";" << "\n";
        print_dot_graph(depht - 1, next_board);
    }
}

void make_dot_graph(int depht) {
    cout << "digraph G {" << "\n";
    cout << R"=(0[label="start pozice"])=" << "\n";
    Board board = Board();
    print_dot_graph(3, board);
    cout << "}" << "\n";

}


int main() {
    make_dot_graph(3);

};

