#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <random>

using namespace std;
typedef long long ll;

//Card section

class Solitare {
private:
    class Card {
    private:
        char _letter;
        int _numer;
        bool _opened;
        static unordered_map<char, int> _numeric;
    public:
        Card(char c) : _letter(c), _numer(_numeric[c]), _opened(false) {}
        char get_letter() {
            return _letter;
        }
        int get_numer() {
            return _numer;
        }
        bool is_open() {
            return _opened;
        }
        void open() {
            _opened = true;
        }
        bool is_next(const Card other) {
            return other._numer - _numer == 1;
        }
        bool operator<(const Card other) const {
            return _numer < other._numer;
        }
    };

    vector<vector<Card> > _solitare;

public:
    Solitare() = default;
    void Init(string name);
    void MakeLog(string s = "\n\n\n") {
        ofstream out("log", ios::app | ios::ate);
        for(int i = 0; i < _solitare.size(); ++i) {
            for (int j = 0; j < _solitare[i].size(); ++j)
                out << _solitare[i][j].get_letter();
            out << '\n';
        }
        if(!s.empty()) {
            out << s << '\n';
        }
        out.close();
    }
    bool Move() {

        vector<int> stack_index(_solitare.size());
        vector<int> min_potential(_solitare.size());
        vector<int> max_potential(_solitare.size());
        for(int i = 0, ind; i < _solitare.size(); ++i) {
            ind = _solitare[i].size() - 1;
            if(ind != -1) {
                min_potential[i] = _solitare[i][ind].get_numer();
                while (ind > 0 && _solitare[i][ind - 1].is_open()
                       && _solitare[i][ind].is_next(_solitare[i][ind - 1]))
                    --ind;
                max_potential[i] = _solitare[i][ind].get_numer();
                stack_index[i] = ind;

                if(max_potential[i] - min_potential[i] + 1 == _solitare[i].size()
                        && _solitare.size() == 9) {
                    _solitare[i].clear();
                    MakeLog();
                    return true;
                }

            }
            else {
                //TODO;
            }
        }
        bool fl = false;
        for(int i = 0; i < _solitare.size() && !fl; ++i) {
            for(int j = 0; j < _solitare.size() && !fl; ++j) {
                if(i != j) {
                    if(max_potential[i] >= min_potential[j] - 1 &&
                        min_potential[i] <= min_potential[j] - 1 &&
                        max_potential[j] > max_potential[i]) {
                        fl = true;

                        vector<Card> buf;
                        while(_solitare[i].back().get_numer() < max_potential[j]
                            && _solitare[i].back().is_open() && _solitare[i].size() > stack_index[i]) {
                            buf.push_back(_solitare[i].back());
                            _solitare[i].pop_back();
                        }
                        if(!_solitare[i].empty() && !_solitare[i].back().is_open())
                            _solitare[i].back().open();
                        while(!buf.empty()) {
                            _solitare[j].push_back(buf.back());
                            buf.pop_back();
                        }
                    }
                }
            }
        }
        if(!fl) {
            //TODO
        }
        MakeLog();
        return fl;
    }
    bool Play() {
        MakeLog();
        while(Move()) {
            for(int i = 0; i < _solitare.size(); ++i) {
                if (_solitare[i].empty()) {
                    MakeLog("No solution here\n-----\n");
                    return true;
                }
            }
        }
        MakeLog("Win!\n-----\n");
        return false;
    }
    void Shuffle() {
        srand(time(NULL));
        string s = "test";
        ofstream out(s);
        string str = "6789TJQKA";
        str = str + str + str + str + str + str + str + str;
        for(int i = 0; i < str.size(); ++i) {
            swap(str[i], str[rand() % min(i + 1, 9)]);
        }
        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 9; ++j) {
                out << str.back();
                str.pop_back();
            }
            out << '\n';
        }
        out.close();
    }
};
unordered_map<char, int> Solitare::Card::_numeric = {
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'T', 10},
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
};

void Solitare::Init(string name) {
    string line;

    ifstream in(name);
    vector<vector<Card> > new_solitare;
    if (in.is_open())
    {
        vector<Card> buf;
        while (getline(in, line))
        {
            if(line.size() != 9) {
                break;
            }
            for(char c : line)
                buf.push_back(Card(c));
            new_solitare.push_back(buf);
            buf.clear();
        }
    }
    else {
        //TODO
    }
    in.close();
    if(new_solitare.size() == 8) {
        for(int i = 0; i < new_solitare.size(); ++i) {
            new_solitare[i].back().open();
        }
        _solitare = new_solitare;
    }
    else {
        //TODO
    }
}
// end of Card section

int main() {
    Solitare Solv;
    for(int i = 0; i < 100; ++i) {
        Solv.Shuffle();
        Solv.Init("test");
        bool fl = Solv.Play();
        if(fl)
            break;
    }
    return 0;
}
