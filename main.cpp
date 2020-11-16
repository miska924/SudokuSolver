#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


using vpii = vector<pair<int, int> >;
using vi = vector<int>;
using vvi = vector<vi>;
using vvvi = vector<vvi>;
using vvvvi = vector<vvvi>;
using vvvvvi = vector<vvvvi>;

bool VERBOSE = false;
const int N = 9;
const int BLOCK = 3;

void print(const vvi &field) {
  for (int i = 0; i < N; ++i) {
    if (i && i % BLOCK == 0) {
      for (int j = 0; j < N; ++j) {
        if (j && j % BLOCK == 0) {
          cout << "-+";
        }
        cout << "--";
      }
      cout << "-\n";
    }
    for (int j = 0; j < N; ++j) {
      if (j && j % BLOCK == 0) {
        cout << " |";
      }
      if (field[i][j] == 0) {
        cout << " _";
      } else {
        cout << " " << field[i][j];
      }
    }
    cout << "\n";
    cout.flush();
  }
}

vi possible(const vvi &field, int x, int y) {
  vector<bool> can(N + 1, true);

  for (int k = 0; k < N; ++k) {
    can[field[x][k]] = can[field[k][y]] = false;
  }
  
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (x / BLOCK == i / BLOCK && y / BLOCK == j / BLOCK) {
        can[field[i][j]] = false;
      }
    }
  }

  vi res;
  for (int i = 1; i <= N; ++i) {
    if (can[i]) {
      res.push_back(i);
    }
  }
  return res;
}

bool check(vvi &field, int x, int y) {
  vi can = possible(field, x, y);
  if (can.size() == 1) {
    field[x][y] = can[0];
    return true;
  }
  return false;
}

int left(const vvi &field) {
  int res = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (!field[i][j]) {
        res++;
      }
    }
  }
  return res;
}

void clear(vvi &field, const vpii &history) {
  for (const auto &p: history) {
    field[p.first][p.second] = 0;
  }
}

bool in(const int& x, const vi& container) {
  for (const int& item: container) {
    if (x == item) {
      return true;
    }
  }
  return false;
}

bool correct(const vvi& field) {
  for (int i = 0; i < N; i += 3) {
    for (int j = 0; j < N; j += 3) {
      vector<int> cnt(N); 
      vector<int> can(N);
      for (int k = 0; k < 3; ++k) {
        for (int p = 0; p < 3; ++p) {
          cnt[field[i + k][j + p]]++;
          for (int n = 1; n <= N; ++n) {
            if (!field[i + k][j + p]) {
              can[n] += in(n, possible(field, i + k, j + p));
            }
          }
        }
      }
      for (int j = 1; j < N; ++j) {
        if (cnt[j] > 1) {
          return false;
        }
      }
    }
  }
  for (int i = 0; i < N; ++i) {
    vector<int> cnt0(N); 
    vector<int> cnt1(N); 
    for (int j = 0; j < N; ++j) {
      cnt0[field[i][j]]++;
      cnt1[field[j][i]]++;
    }
    for (int j = 1; j < N; ++j) {
      if (cnt0[j] > 1 || cnt1[j] > 1) {
        return false;
      }
    }
  }
  return true;
}

int cnt(const vvi& field, int x, int y) {
  int res = 0;
  for (int i = 0; i < N; ++i) {
    if (field[i][y]) {
      res++;
    }
    if (i != y && field[x][i]) {
      res++;
    }
  }
  return res;
}

bool solve(vvi &field, int &max_req, int req = 0) {
  if (req == 0) {
    max_req = 0;
  }
  max_req = max(max_req, req);

  bool changed = true;
  vpii history;
  while (changed && correct(field)) {
    changed = false;
    for (int i = 0; i < N && !changed; ++i) {
      for (int j = 0; j < N && !changed; ++j) {
        if (!field[i][j]) {
          if (check(field, i, j)) {
            history.push_back({i, j});
            changed = true;
          }
        }
      }
    }
    for (int i = 0; i < N; ++i) {
      vector<int> pos0(N, -1);
      vector<int> pos1(N, -1);
      for (int j = 0; j < N; ++j) {
        vector<int> can0 = possible(field, i, j);
        vector<int> can1 = possible(field, j, i);
        for (int n = 0; n < N; ++n) {
          if (field[i][j] == 0 && in(n, can0)) {
            if (pos0[n] == -1) {
              pos0[n] = j;
            } else if (pos0[n] > -1) {
              pos0[n] = -2;
            }
          }
          if (field[j][i] == 0 && in(n, can1)) {
            if (pos1[n] == -1) {
              pos1[n] = j;
            } else if (pos1[n] > -1) {
              pos1[n] = -2;
            }
          }
        }
      }
      for (int n = 0; n < N; ++n) {
        if (pos0[n] > -1) {
          field[i][pos0[n]] = n;
          history.push_back({i, pos0[n]});
          changed = true;
        }
        if (pos1[n] > -1) {
          field[pos1[n]][i] = n;
          history.push_back({pos1[n], i});
          changed = true;
        }
      }
    }

    if (changed) {
      if (VERBOSE) {
        system("clear");
        print(field);
        cout << endl << endl;
        system("sleep 0.1");
      }
    } else {
      bool end = false;
      for (size_t k = 2; k <= N && !end; ++k) {
        //for (int kk = N * 2 - 2; kk >= 0; --kk) {
          for (int i = 0; i < N && !end; ++i) {
            for (int j = 0; j < N && !end; ++j) {
              if (!field[i][j]) {// && cnt(field, i, j) >= kk) {
                vi can = possible(field, i, j);
                if (can.size() == k) {
                  if (can.empty()) {
                    clear(field, history);
                    return false;
                  }
                  for (auto g: can) {
                    field[i][j] = g;

                    if (!correct(field)) {
                      field[i][j] = 0;
                      continue;
                    }
                    
                    if (VERBOSE) {
                      system("clear");
                      print(field);
                      cout << endl << endl;
                    }
                    
                    if (solve(field, max_req, req + 1)) {
                      return true;
                    }
                    field[i][j] = 0;
                  }
                  end = true;
                }
              }
            }
          }
        //}
      }
    }
  }
  if (!left(field) && correct(field)) {
    return true;
  }
  clear(field, history);
  return false;
}

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == string("-v")) {
      VERBOSE = true;
    }
  }
  int left = N * N;
  vvi field(N, vi(N, 0));
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cin >> field[i][j];
      if (field[i][j]) {
        left--;
      }
    }
  }
  
  if (VERBOSE) {
    print(field);
  }

  int mx_req;
  bool solved = solve(field, mx_req);
  system("clear");
  print(field);
  if (!solved){
    cout << "THERE IS NO SOLUTION" << endl;
  }
  cout << "MAXIMUM REQURSION: " << mx_req << endl;
}
