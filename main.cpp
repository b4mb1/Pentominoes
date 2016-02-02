#include <iostream>
#include <string>
#include "InputFactory.h"
using namespace std;

const int INF = 10000;

class item {
  public:
    item *up, *down, *right, *left;
    item *col;
    int x, y;
};

class column : public item {
  public:
    string name;
    int size;
    column();
    column(string in_name, item *in_right, item *in_left);
};

column::column(string in_name, item *in_right, item *in_left) {
  name = in_name;
  right = in_right;
  left = in_left;
  up = this;
  down = this;
  size = 0;
};

column::column() {
  name = "problem matrix";
  left = this;
  right = this;
  up = NULL;
  down = NULL;
  size = 0;
};

void make_column(column *h) {
  string in_name;
  cin >> in_name;
  column *c = new column(in_name, h, h->left);
  h->left->right = c;
  h->left = c;
};

column* read_problem() {
  column *h = new column();
  column *current_column;
  int n, m, k;
  bool first_in_row;
  item *first;
  item *to_insert;
  cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    make_column(h);
  };
  for (int i = 0; i < n; ++i) {
    first_in_row = true;
    current_column = h;
    for (int j = 0; j < m; ++j) {
      current_column = (column *) current_column->right;
      cin >> k;
      if (k) {
        if (first_in_row) {
          first_in_row = false;
          first = new item();
          current_column->size += 1;
          first->col = current_column;
          first->x = i;
          first->y = j;
          // link up and down
          first->up = current_column->up;
          first->down = current_column;
          current_column->up->down = first;
          current_column->up = first;
          // link left and right
          first->right = first;
          first->left = first;
        } else {
          to_insert = new item();
          current_column->size += 1;
          to_insert->col = current_column;
          to_insert->x = i;
          to_insert->y = i;
          // link up and down
          to_insert->up = current_column->up;
          to_insert->down = current_column;
          current_column->up->down = to_insert;
          current_column->up = to_insert;
          // link left and right
          to_insert->right = first;
          to_insert->left = first->left;
          first->left->right = to_insert;
          first->left = to_insert;
        };
      };
    };
  };
  return h;
};

void debug_print(column *h) {
  column *worker = (column *) h->right;
  while (worker != h) {
    cout << worker->name << " " << worker->size<< endl;
    worker = (column *) worker->right;
  };
};

void print_current_solution(int k, item **solution) {
  cout << "printing solution" << endl;
  for (int i = 0; i < k; ++i) {
    cout << solution[i]->x + 1 << endl;
  };
  cout << endl;
};

void cover_column(column *c) {
  c->right->left = c->left;
  c->left->right = c->right;
  for (item *i = c->down; i != c; i = i->down) {
    for (item *j = i->right; j != i; j = j->right) {
      j->down->up = j->up;
      j->up->down = j->down;
      ((column *) j->col)->size -= 1;
    };
  };
};

void uncover_column(column *c) {
  for (item *i = c->up; i != c; i = i->up) {
    for (item *j = i->left; j != i; j = j->left) {
      ((column *) j->col)->size += 1;
      j->down->up = j;
      j->up->down = j;
    };
  };
  c->right->left = c;
  c->left->right = c;
};

void search(int k, column *h, item **solution){
  if (h->right == h) {
    print_current_solution(k, solution);
  } else {
    // choose column object c to minimize branching factor
    column *c;
    int s = INF;
    for (column *j = (column *) h->right; j != h ; j = (column *) j->right) {
      if (j->size < s) {
        c = j;
        s = j->size;
      };
    };
    cover_column(c);
    for (item *r = c->down; r != c; r = r->down) {
      solution[k] = r;
      for (item *j = r->right; j != r; j=j->right) {
        cover_column((column *) j->col);
      };
      search(k+1, h, solution);
      r = solution[k];
      c = (column *) r->col;
      for (item *j = r->left; j != r; j=j->left) {
        uncover_column((column *) j->col);
      };
    };
    uncover_column(c);
  };
};

int main() {
  if (0) {
    InputFactory inp_maker;
    inp_maker.parseInput("pentominoes.txt", "board.txt");
  } else {
    column *h = read_problem();
    // debug_print(h);
    item **solution;
    search(0, h, solution);
  }
};
