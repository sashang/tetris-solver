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
    shape_map.resize(16);
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            shape_map[j*4 + i] = Coordinate({i, j});
        }
    }
}

class Shape
{
    private:
        static char _current_colour;
        vector<Coordinate> _layout;
        char _colour;
    
        static void NextColour()
        {
            if (_current_colour == 'r')
                _current_colour = 'b';
            else if (_current_colour == 'b')
                _current_colour = 'g';
            else if (_current_colour == 'g')
                _current_colour = 'y';
            else if (_current_colour == 'y')
                _current_colour = 'r';
        }

    public:
        Shape(const string& line) :
            _colour(_current_colour)
        {
            stringstream ss(line);
            int a, b, c, d;
            ss >> a >> b >> c >> d;
            _layout.push_back(shape_map[a]);
            _layout.push_back(shape_map[b]);
            _layout.push_back(shape_map[c]);
            _layout.push_back(shape_map[d]);
            NextColour();
        }


        void Rotate90()
        {
            for (auto& elem : _layout)
            {
                int temp = elem[0];
                elem[0] = -1 * elem[1];
                elem[1] =  temp;
            }
        }
        
        const Coordinate& GetCoord(int i) const
        {
            return _layout[i];
        }

        char GetColour() const
        {
            return _colour;
        }

        void Print() const
        {
            for (auto elem : _layout)
            {
                cout << "(" << elem[0] << "," << elem[1] << ") ";
            }
        }
};

char Shape::_current_colour = 'r';

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
            bool fits = false;
            for (j = 0; j < _height; j++)
            {
                for (i = 0; i < _width; i++)
                {
                    fits = false;
                    for (k = 0; k < 4; k++)
                    {
                        //get block from shape
                        Coordinate c(s.GetCoord(k));

                        //check if it lies outside the grid
                        int l = c[0];
                        int m = c[1];
                        cout << "l = " << l << " m =  " << m << endl;
                        if (l < 0 || l > _width || m < 0 || m > _height)
                        {
                            fits = false;
                            break;
                        }
                        
                        cout << "in bounds" << endl;

                        //check that this grid spot is empty
                        if (_grid[l][m] != '*')
                        {
                            fits = false;
                            break;
                        }
                        
                        cout << "spot is empty" << endl;

                        fits = true;
                    }

                    if (fits)
                    {
                        //if we get here the shape fits so mark the spaces in 
                        //the grid as occupied.
                        for (k = 0; k < 4; k++)
                        {
                            //get block from shape
                            Coordinate c(s.GetCoord(k));
                            int l = c[0];
                            int m = c[1];
                            _grid[l][m] = s.GetColour();
                        }
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

bool find_tesselation(Grid& grid, const vector<Shape>& shapes)
{
    for (const auto& shape : shapes)
    {
        if (!grid.Insert(shape))
        {
            return false;
        }
    }
    return true;
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
    if (find_tesselation(grid, shapes))
        cout << "Solution found:" << endl;
    else
        cout << "No solution:" << endl;
    grid.Print();
}
