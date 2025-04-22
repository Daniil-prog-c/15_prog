#include <cassert>
#include <iostream>
#include <cmath> 
#include "Random.h"

constexpr int g_consoleLines{ 25 };


class Tile {
private:
    int m_numT{};
public:
    Tile() = default;

    Tile(int num) : m_numT{ num } {

    }

    int getNum() { return m_numT; }


    friend std::ostream& operator<<(std::ostream& out, Tile& tile) {
        if (tile.getNum() > 9) 
            out << " " << tile.getNum() << " ";
        else if (tile.getNum() > 0) 
            out << "  " << tile.getNum() << " ";
        else if (tile.getNum() == 0) 
            out << "    ";
        return out;
    }

    bool isEmpty() {
        return (m_numT == 0);
    }

    friend bool operator== (Tile& t1, int t2) {

        return (t1.m_numT == t2);
    };

};




class Direction {

public:
    enum Direct {
        up,
        down,
        left,
        right,
        maxDirections,
    };

    Direction(Direct direct) : m_direct{direct}
    {
    }

    Direct getType() const {
        return m_direct;
    }


    Direction operator-() const
    {
        switch (m_direct)
        {
        case up:    return Direction{ down };
        case down:  return Direction{ up };
        case left:  return Direction{ right };
        case right: return Direction{ left };
        default:    break;
        }

        assert(0 && "Unsupported direction was passed!");
        return Direction{ up };
    }


    static Direction getRandomDirection()
    {
        Direct random{ static_cast<Direct>(Random::get(0, Direct::maxDirections - 1)) };
        return Direction{ random };
    }

private:
    Direct m_direct{};

};


std::ostream& operator<<(std::ostream& stream, Direction dir)
{
    switch (dir.getType())
    {
    case Direction::up:     return (stream << "up");
    case Direction::down:   return (stream << "down");
    case Direction::left:   return (stream << "left");
    case Direction::right:  return (stream << "right");
    default:                break;
    }

    assert(0 && "Unsupported direction was passed!");
    return (stream << "unknown direction");
}

namespace UserInput {


    bool isValidCommand(char ch)
    {
        return ch == 'w'
            || ch == 'a'
            || ch == 's'
            || ch == 'd'
            || ch == 'q';
    }

    void ignoreLine()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    char getCharacter()
    {
        char operation{};
        std::cin >> operation;
        ignoreLine(); 
        return operation;
    }

    char user_command() {
        char vvod{};
        while (!isValidCommand(vvod))
            vvod = getCharacter();

        return vvod;

    }

    Direction charToDirection(char ch)
    {
        switch (ch)
        {
        case 'w': return Direction{ Direction::up };
        case 's': return Direction{ Direction::down };
        case 'a': return Direction{ Direction::left };
        case 'd': return Direction{ Direction::right };
        }

        assert(0 && "Unsupported direction was passed!");
        return Direction{ Direction::up };
    }

}

class Point {
private:
    int m_x{};
    int m_y{};
public:
    Point(int x, int y) : m_x{ x }, m_y{ y } 
    {
    }

    friend bool operator== (const Point& p1, const Point& p2);
    friend bool operator!= (const Point& p1, const Point& p2);

    int getX() { return m_x; }
    int getY() { return m_y; }

    Point getAdjacentPoint(Direction dir) 
    {

        switch (dir.getType())
        {
        case Direction::up: return    Point{ m_x-1,   m_y };
        case Direction::down: return  Point{ m_x+1,   m_y };
        case Direction::left: return  Point{ m_x,   m_y-1 };
        case Direction::right: return Point{ m_x,   m_y+1 };
        }
        assert(0 && "Unsupported direction was passed!");
        return *this;

    }

};

bool operator== (const Point& p1, const Point& p2)
{
    return (p1.m_x == p2.m_x &&
        p1.m_y == p1.m_y);
}

bool operator!= (const Point& p1, const Point& p2)
{
    return (p1.m_x != p2.m_x ||
        p1.m_y != p2.m_y);
}





class Board {
private:
    static constexpr int s_size{ 4 };
    Tile m_arr[s_size][s_size];
public:
    //Board() = default;

    Board() {
        for (int i = 0; i < s_size; i++) {
            for (int j = 0; j < s_size; j++) {
                if (((j + 1) + i * 4) != 16)
                    m_arr[i][j] = (j + 1) + i * 4;
                else
                    m_arr[i][j] = 0;

            }
        }
    }


    friend std::ostream& operator<<(std::ostream& out, Board board) {

        for (int i{ 0 }; i < g_consoleLines; i++)
            std::cout << '\n';

        for (std::size_t i = 0; i < s_size; i++) {
            for (std::size_t j = 0; j < s_size; j++) {
                out << board.m_arr[i][j];

            }
            out << "\n";
        }

        return out;
    }




    bool moveTile(Direction dir) {

        for (int i = 0; i < s_size; i++) {
            for (int j = 0; j < s_size; j++) {

                if (m_arr[i][j] == 0) {

                    
                    Point point_user_ini{ i,j };
                    Point point_user_fin{ point_user_ini.getAdjacentPoint(-dir) };

                    if (point_user_fin.getX() < s_size && point_user_fin.getX() >= 0) {
                        if (point_user_fin.getY() < s_size && point_user_fin.getY() >= 0) {

                            std::swap(m_arr[i][j], m_arr[point_user_fin.getX()][point_user_fin.getY()]);


                        }
                    }

                    return true;
                }

            }
        }


    }

    bool winMove() {
        int k{ 0 };
        for (int i = 0; i < s_size; i++) {
            for (int j = 0; j < s_size; j++) {

                if (m_arr[i][j] == (j + 1) + i * 4) {
                    k += 1;
                }

            }
        }
        if (k == 15) return true;
        return false;

    }

};



int main()
{
   Board board{};
   for (int i = 0; i < 200; i++) {
       board.moveTile(Direction::getRandomDirection());
   }
    
    std::cout << board;

    std::cout << "Enter a command: ";
    while (true)
    {
        char ch{ UserInput::user_command() };

        // Handle non-direction commands
        if (ch == 'q')
        {
            std::cout << "\n\nBye!\n\n";
            return 0;
        }

        // Handle direction commands
        Direction dir{ UserInput::charToDirection(ch) };

        bool userM{ board.moveTile(dir) };

        if(userM)
        std::cout << board;

       if (board.winMove()) {
            std::cout << "\n\nYou won!\n\n";
            return 0;
        }
       // std::cout << "You entered direction: " << dir << '\n';
    }


    return 0;
}