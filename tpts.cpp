#include <iostream>
#include <unistd.h>
#include <exception>
#include <list>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <set>
#include <algorithm>

using namespace std;
bool animate = false;
bool debug = false;
typedef std::array<int, 2> Coordinate;

//Maps the enumerated positions on the imaginary grid to coordinates
vector<Coordinate> shape_map;

vector<vector<int>> iso_square = {{0, 1, 4, 5}};

//l shape
vector<vector<int>> iso_l_shape = 
    {
        {0, 1, 2, 4}, 
        {0, 1, 5, 9},
        {2, 4, 5, 6},
        {0, 4, 8, 9},
    };

//reverse l shape
vector<vector<int>> iso_rl_shape = 
    {
        {0, 1, 2, 6}, 
        {1, 5, 8, 9},
        {0, 4, 5, 6},
        {0, 1, 4, 8},
    };

vector<vector<int>> iso_z_shape = 
    {
        {0, 1, 5, 6}, 
        {1, 4, 5, 8},
    };

//reverse z shape
vector<vector<int>> iso_rz_shape = 
    {
        {1, 2, 4, 5}, 
        {0, 4, 5, 9},
    };

vector<vector<int>> iso_bar = 
    {
        {0, 1, 2, 3}, 
        {0, 4, 8, 12},
    };

vector<vector<int>> iso_t_shape = 
    {
        {0, 1, 2, 5}, 
        {1, 4, 5, 9},
        {1, 4, 5, 6},
        {0, 4, 5, 8},
    };


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

class ShapeCreationFailed :
    public exception
{
    private:
        std::string _shape;
        std::string _msg;

    public:
        ShapeCreationFailed(const std::string& shape) :
            _shape(shape)
        {
            _msg = "failed to match shape ";
            _msg += _shape;
            _msg += " with a template";
        }

        virtual const char* what() const noexcept
        {
            return _msg.c_str();
        }
};

class Shape
{
    private:
        static char _current_colour;
        vector<Coordinate> _layout;
        char _colour;
        vector<vector<int>>* _type;
        vector<vector<int>>::iterator _it_rotation;
        int _iso_idx;
    
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
            _colour(_current_colour),
            _iso_idx(0)
        {
            stringstream ss(line);
            int a, b, c, d;
            ss >> a >> b >> c >> d;
            _layout.push_back(shape_map[a]);
            _layout.push_back(shape_map[b]);
            _layout.push_back(shape_map[c]);
            _layout.push_back(shape_map[d]);
            //find matching shape
            vector<int> v = {a,b,c,d};
            vector<vector<int>>::iterator it;
            if ((it = find(iso_square.begin(), iso_square.end(), v)) != iso_square.end())
                _type = &iso_square;
            else if ((it = find(iso_t_shape.begin(), iso_t_shape.end(), v)) != iso_t_shape.end())
                _type = &iso_t_shape;
            else if ((it = find(iso_l_shape.begin(), iso_l_shape.end(), v)) != iso_l_shape.end())
                _type = &iso_l_shape;
            else if ((it = find(iso_rl_shape.begin(), iso_rl_shape.end(), v)) != iso_rl_shape.end())
                _type = &iso_rl_shape;
            else if ((it = find(iso_z_shape.begin(), iso_z_shape.end(), v)) != iso_z_shape.end())
                _type = &iso_z_shape;
            else if ((it = find(iso_rz_shape.begin(), iso_rz_shape.end(), v)) != iso_rz_shape.end())
                _type = &iso_rz_shape;
            else if ((it = find(iso_bar.begin(), iso_bar.end(), v)) != iso_bar.end())
                _type = &iso_bar;
            else
            {
                std::stringstream ss;
                ss << a << " " << b << " " << c << " " << d;
                throw ShapeCreationFailed(ss.str());
            }
            _it_rotation = it;
            NextColour();
        }

        void Rotate()
        {
            _it_rotation++;

            if (_it_rotation == _type->end())
                _it_rotation = _type->begin();

            int a, b ,c ,d;
            a = (*_it_rotation)[0];
            b = (*_it_rotation)[1];
            c = (*_it_rotation)[2];
            d = (*_it_rotation)[3];
            _layout.clear();
            _layout.push_back(shape_map[a]);
            _layout.push_back(shape_map[b]);
            _layout.push_back(shape_map[c]);
            _layout.push_back(shape_map[d]);
        }

        int GetRotations() { return _type->size(); }
        
        const Coordinate& GetCoord(int i) const
        {
            const Coordinate& c(_layout[i]);
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

        bool CheckHoles(int i, int j, int depth)
        {
            if (depth == 0)
                return false;
            int x, y;
            for (x = i; i < _width-1; x++)
            {
                for (y = j; j < _height-1; y++)
                {
                    if (_grid[x][y] == '*')
                    {
                        //check surroundxng locatxons for another empty spot
                        if (_grid[x+1][y] != '*' && _grid[x-1][y] != '*' &&
                            _grid[x][y+1] != '*' && _grid[x][y-1] != '*')
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        void Remove(vector<Coordinate>& coords)
        {
            int x, y, i;

            for (i = 0; i < coords.size(); i++)
            {
                x = coords[i][0];
                y = coords[i][1];
                _grid[x][y] = '*';
            }
        }

        bool Insert(Shape& s, vector<Coordinate>& insertion_coords)
        {
            int i,j;
            int k = 0;
            bool fits = false;
            for (j = 0; j < _height; j++)
            {
                for (i = 0; i < _width; i++)
                {
                    //find the 1st empty block
                    if (_grid[i][j] == '*')
                    {
                        int rotate_count = 1;
                        do 
                        {
                            int l = i;
                            int m = j;
                            insertion_coords.push_back(Coordinate({i, j}));
                            _grid[l][m] = s.GetColour();
                            for (k = 1; k < 4; k++)
                            {
                                //get block from shape
                                Coordinate a(s.GetCoord(k -1));
                                Coordinate b(s.GetCoord(k));
                                int dx = b[0] - a[0];
                                int dy = b[1] - a[1];
                                cout << "dx = " << dx << "dy = " << dy << endl;
                                //check if it lies outside the grid
                                l += dx;
                                m += dy;
                                if (l < 0 || l > _width - 1  || m < 0 || m > _height - 1)
                                {
                                    Remove(insertion_coords);
                                    insertion_coords.clear();
                                    fits = false;
                                    break;
                                }

                                //check that this grid spot is empty
                                if (_grid[l][m] != '*')
                                {
                                    Remove(insertion_coords);
                                    insertion_coords.clear();
                                    fits = false;
                                    break;
                                }

                                fits = true;
                                insertion_coords.push_back(Coordinate({l ,m}));
                                _grid[l][m] = s.GetColour();
                            }
                            if (fits)
                            {
                                return true;
                            }
                            cout << "rotating inner: " << i << " " << j << endl;
                            s.Rotate();
                            rotate_count++;
                        } while (rotate_count < s.GetRotations());
                    }
                }
            }
            //no empty block
            return false;
        }

        void Print() const
        {
            //since we are writing to stdout we print rows at the top of
            //the grid first
            for (int j = _height - 1 ; j >= 0; j--)
            {
                for (int i = 0; i < _width; i++)
                {
                    cout << _grid[i][j] << ' ';
                }
                cout << endl;
            }
        }
};



bool find_space(Grid& grid, vector<Shape> shapes)
{
    vector<Shape>::iterator it;
    if (shapes.empty())
    {
        return true;
    }
    
    for (it = shapes.begin(); it != shapes.end(); ++it)
    {
        int rotate_count = 1;
        do 
        {
            vector<Coordinate> insertion_coords;
            if (grid.Insert(*it, insertion_coords))
            {
                if (animate)
                {
                    grid.Print();
                    cout << endl;
                    sleep(1);
                }

                vector<Shape>::iterator it1;
                vector<Shape> remaining;
                for (it1 = shapes.begin(); it1 != shapes.end(); ++it1)
                {
                    if (it1 != it)
                    {
                        remaining.push_back(*it1);
                    }
                }
                if (find_space(grid, remaining))
                {
                    return true;
                }
                //about to try the next inserting the next shape. Therefore the previous
                //shape and the path it took must have failed. Therefore remove it from
                //the grid.
                grid.Remove(insertion_coords);
            }
            else
            {
                grid.Remove(insertion_coords);
            }
            it->Rotate();
            rotate_count++;
        } while (rotate_count < it->GetRotations());
    }
    return false;
}

void process_cmdline(int argc, char* argv[])
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            string arg(argv[1]);
            if (arg == "-a" || arg == "--animate")
            {
                animate = true;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    string line;
    int width = 0, height = 0;
    uint32_t count = 0;
    vector<Shape> shapes;

    try
    {
        process_cmdline(argc, argv);
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
        bool result = find_space(grid, shapes);
        if (result)
            cout << "Solution found:" << endl;
        else
            cout << "No solution:" << endl;
        grid.Print();
        if (result)
            return 0;
        else
            return 1;
    }
    catch (const ShapeCreationFailed& ex)
    {
        cout << ex.what() << endl;
        return 1;
    }
}

