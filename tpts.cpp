#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <set>

using namespace std;

typedef std::array<int, 2> Coordinate;

//Maps the enumerated positions on the imaginary grid to coordinates
vector<Coordinate> shape_map;

void generate_shape_map()
{
    //12 positions on the imaginary grid. Map them to xy coordinates
    shape_map.resize(12);
    for (int i = 0; i < 12; i++)
    {
        switch (i)
        {
            case 0:
                shape_map[0] = Coordinate({-1,2});
                break;
            case 1:
                shape_map[1] = Coordinate({1,2});
                break;
            case 2:
                shape_map[2] = Coordinate({-2,1});
                break;
            case 3:
                shape_map[3] = Coordinate({-1,1});
                break;
            case 4:
                shape_map[4] = Coordinate({1,1});
                break;
            case 5:
                shape_map[5] = Coordinate({2,1});
                break;
            case 6:
                shape_map[6] = Coordinate({-2,-1});
                break;
            case 7:
                shape_map[7] = Coordinate({-1,-1});
                break;
            case 8:
                shape_map[8] = Coordinate({1,-1});
                break;
            case 9:
                shape_map[9] = Coordinate({2,-1});
                break;
            case 10:
                shape_map[10] = Coordinate({-1,-2});
                break;
            case 11:
                shape_map[11] = Coordinate({1,-2});
                break;
        }
    }
}

class Shape
{
    private:
        vector<Coordinate> layout;

    public:
        Shape(const string& line)
        {
            stringstream ss(line);
            int a, b, c, d;
            ss >> a >> b >> c >> d;
            layout.push_back(shape_map[a]);
            layout.push_back(shape_map[b]);
            layout.push_back(shape_map[c]);
            layout.push_back(shape_map[d]);

        }

        void Rotate90()
        {
            for (auto& elem : layout)
            {
                int temp = elem[0];
                elem[0] = -1 * elem[1];
                elem[1] =  temp;
            }
        }
        
        const Coordinate& GetCoord(int i) const
        {
            return layout[i];
        }

        void Print() const
        {
            for (auto elem : layout)
            {
                cout << "(" << elem[0] << "," << elem[1] << ") ";
            }
        }
};

class Grid
{
    private:
        vector<vector<char>> _grid;
        int _width;
        int _height;

    public:
        Grid(int width, int height) :
            _width(width), _height(height)
        {
            _grid.resize(_width);
            for (auto& e : _grid)
            {
                e.resize(_height);
                for (auto& e1 : e)
                {
                    e1 = '*';
                }
            }
        }

        bool Insert(const Shape& s)
        {
            int i,j;
            int k = 0;
            for (i = 0; i < _width; i++)
            {
                for (j = 0; j < _height; j++)
                {
                    if (_grid[i][j] == '*')
                    {
                        Coordinate c(s.GetCoord(k));
                        
                    }
                }
            }
        }

        void Print() const
        {
            for (int j = 0; j < _height; j++)
            {
                for (int i = 0; i < _width; i++)
                {
                    cout << _grid[i][j] << ' ';
                }
                cout << endl;
            }
        }
};


void print_shapes(const vector<Shape>& shapes)
{
    for (const auto& elem : shapes)
    {
        elem.Print();
        cout << endl;
    }
}

void search(const vector<Shape>& shapes)
{

}

int main(int argc, char* argv[])
{
    string line;
    int width = 0, height = 0;
    uint32_t count = 0;
    vector<Shape> shapes;

    generate_shape_map();
    while (getline(cin, line))
    {
        //1st line is the grid dimensions
        if (count == 0)
        {
            stringstream ss(line);
            ss >> width >> height;
        }
        else
        {
            Shape s(line);
            shapes.push_back(s);
        }
        count++;
    }

    Grid grid(width, height);
    grid.Print();
}
