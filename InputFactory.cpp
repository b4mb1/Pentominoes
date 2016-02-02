#import <iostream>
#import <fstream>
#import <vector>
#import <string>
#import "InputFactory.h"
using namespace std;

struct Node {
    int dx;
    int dy;
    Node(int x, int y): dx(x), dy(y){ };
};

struct Board {
    bool* content;
    int w;
    int h;
    Board(bool* content, int w, int h): content(content), w(w), h(h){ };
};

class Shape {
public:
    // attributes
    int number;
    vector<Node> elems;

    // methods
    bool checkIfFits(Board &t, int x, int y);

};

bool Shape::checkIfFits(Board &board, int x, int y) {
    bool resu = true;
    for (auto &current_tile : elems) {
        if ((x+current_tile.dx >= board.w ) ||
           (y+current_tile.dy >= board.h ) ||
           (board.content[(y+current_tile.dy)*board.w + x+current_tile.dx])) {
            resu = false;
        }
    }
    return resu;
};

class TableHead {
    vector<string> header;
    vector< vector<bool> > rows;
};

void debugWrite( vector<Shape> pentominoes ) {
    for ( auto &shape : pentominoes ) {
        cout << endl << "printing next shape!" << endl;
        for (auto &node : shape.elems) {
            cout << node.dy << node.dx << endl;
        }
    }
    
};

void InputFactory::parseInput(string pentomino_filename, 
                              string board_filename) {
    // get pentomino shapes
    int shapeNo = 12;
    ifstream myfile(pentomino_filename);
    bool figureMakingMode = true;
    string line;
    int line_counter = 0;
    Shape* currentShape = NULL;
    vector<Shape> pentominoes = vector<Shape>();
    while ( figureMakingMode && getline(myfile, line) ) {
        if (line == "# end") {
            figureMakingMode = false;
        } else {
            if (line.length() > 0 && line[0] == '#') {
                if (currentShape != NULL) {
                    pentominoes.push_back(*currentShape);
                };
                currentShape = new Shape();
                line_counter = -1;
                currentShape->number = stoi(line.substr(1,line.length()-1));
            } else {
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] != ' ') {
                        currentShape->elems.push_back(Node(i, line_counter));
                    };
                };
            };
        };
        line_counter++;
    };
    myfile.close();
    ifstream board_file(board_filename);
    getline(board_file, line);
    int board_height = stoi(line);
    getline(board_file, line);
    int board_width = stoi(line);
    bool* board_content = new bool[board_height * board_width];
    for (int i = 0; i < board_height ; i++) {
        getline(board_file, line);
        for (int j = 0; j < board_width; j++) {
            board_content[j + board_width*i ] = (line[j] == '1');
        };
    };
    line_counter = 0;
    Board board = Board(board_content, board_width, board_height);
    for ( auto &shape : pentominoes ) {
        for (int y = 0; y < board_height ; y++) {
            for (int x = 0; x < board_width; x++) {
                if (shape.checkIfFits(board, x, y)) {
                    bool * arrayToPrint = new bool[board_width * board_height + shapeNo];
                    for (auto &node : shape.elems) {
                        arrayToPrint[(y+node.dy)*board_width + node.dx + x] = true;
                    }
                    arrayToPrint[board_width*board_height + shape.number - 1] = true;
                    for (int i = 0; i < board_width * board_height + shapeNo; i++){
                        cout << arrayToPrint[i] << ' ';
                    }
                    cout << endl;
                    line_counter++;
                };
            };
        };
    };
    cout << line_counter << " lines written" << endl;
    cout << board_width * board_height + shapeNo << " is table width" << endl;
    // we need to write the header
    for (int i = 0; i < board_height ; i++) {
        for (int j = 0; j < board_width; j++) {
            cout << "t_" << i << "," << j << ' ';
        };
    };
    for (int i = 0; i < shapeNo; i++) {
        cout << "elemNo_" << shapeNo << ' ';
    };
    cout << endl;
};
