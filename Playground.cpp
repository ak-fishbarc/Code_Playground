#include <iostream>
#include <algorithm>

class Door
{
    private:
        int posx, posy, img_id;
        bool open = false;

    public:
        void set_values(int, int, int);
        void set_open() {
            open = true;
            img_id = 2;
        };
        int get_posx() { return posx; };
        int get_posy() { return posy; };
        int get_img_id() { return img_id; };
};

void Door::set_values(int x, int y, int img_id_no)
{
    posx = x;
    posy = y;
    img_id = img_id_no;
};

class Goblin
{
    private:
        int posx, posy, img_id;

    public:
        void set_values(int, int, int);
        void set_posx(int x) {
            posx = x;
        };
        void set_posy(int y) {
            posy = y;
        };
        int get_posx() { return posx; };
        int get_posy() { return posy; };
        int get_img_id() { return img_id; };
};

void Goblin::set_values(int x, int y, int img_id_no)
{
    posx = x;
    posy = y;
    img_id = img_id_no;
}


class AI_brain
{
    Goblin* owner;
    int* pathX;
    int* pathY;
    int pathX_length;
    int pathY_length;

    public:
    void bresenham_pathfinding(int, int, int, int);
    void set_owner(Goblin* own) { owner = own; };
    Goblin* get_owner() { return owner; };
    void move_owner();
    int* get_pathx() { return pathX; };
    int* get_pathy() { return pathY; };
    int length_pathx() { return pathX_length; };
    int length_pathy() { return pathY_length; };
};

void AI_brain::move_owner()
{
    owner->set_posx(pathX[0]);
    owner->set_posy(pathY[0]);
}

void AI_brain::bresenham_pathfinding(int startx, int starty, int endx, int endy)
{
    int nextX = startx;
    int nextY = starty;
    int distanceToX = endx - startx;
    int distanceToY = endy - starty;
    int directionX, directionY;
    if (distanceToX < 0) directionX = (-1); else directionX = 1;
    if (distanceToY < 0) directionY = (-1); else directionY = 1;
    int step = 0;
    
    distanceToX = abs(distanceToX);
    distanceToY = abs(distanceToY);
    int maxDistance = std::max(distanceToX, distanceToY);

    double remainder = (maxDistance / 2);

    pathX = new int[maxDistance];
    pathY = new int[maxDistance];
    
    if (distanceToX > distanceToY)
    {
        
        for (int i = 0; i < maxDistance; i++)
        {
            nextX += directionX; remainder += distanceToY;
            if (remainder >= distanceToX)
            {
                nextY += directionY;
                remainder -= distanceToX;
            }
            pathX[step] = nextX;
            pathY[step] = nextY;
            step++;
        }
    }
    else
    {
        for (int i = 0; i < maxDistance; i++)
        {
            
            nextY += directionY; remainder += distanceToX;
            if (remainder >= distanceToY)
            {
                nextX += directionX;
                remainder -= distanceToY;
            }
            pathX[step] = nextX;
            pathY[step] = nextY;
            step++;
        }
    }
    pathX_length = step;
    pathY_length = step;
}

class LevelCreator
{
    private:
        int rows, columns;
        int** level;

    public:
        void set_values(int, int);
        int** get_level() { return level; };
        int** create_level();
};

void LevelCreator::set_values(int x, int y)
{
    rows = x;
    columns = y;
};

int** LevelCreator::create_level()
{
    level = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        level[i] = new int[columns];
        for (int z = 0; z < columns; z++)
        {
            level[i][z] = 0;
        }
    }
    return level;
};


void insert_object(int** level_map, int object_x, int object_y, int object_img_id)
{
    level_map[object_x][object_y] = object_img_id;
};


void display_level(int** level_map)
{
    for (int i = 0; i < 10; i++)
    {
        for (int z = 0; z < 10; z++)
        {
            if (i == 0 || i == 9 || z == 0 || z == 9)
            {
                std::cout << "^";
            }
            else if (level_map[i][z] == 0)
            {
                std::cout << " ";
            }
            else if (level_map[i][z] == 1)
            {
                std::cout << "#";
            }
            else if (level_map[i][z] == 2)
            {
                std::cout << " |";
            }
            else if (level_map[i][z] == 5)
            {
                std::cout << "g";
            }
        }
        std::cout << "\n";
    }
}


int main()
{
    LevelCreator Level1;
    Level1.set_values(15, 15);
    Level1.create_level();

    Door WoodenDoor;
    WoodenDoor.set_values(5, 5, 1);

    Goblin Goblin1;
    Goblin1.set_values(8, 8, 5);
    AI_brain Brain1;
    Brain1.set_owner(&Goblin1);
    Brain1.bresenham_pathfinding(Goblin1.get_posx(), Goblin1.get_posy(), WoodenDoor.get_posx(), WoodenDoor.get_posy());

    insert_object(Level1.get_level(), Goblin1.get_posx(), Goblin1.get_posy(), Goblin1.get_img_id());
    insert_object(Level1.get_level(), WoodenDoor.get_posx(), WoodenDoor.get_posy(), WoodenDoor.get_img_id());
    display_level(Level1.get_level());
    Brain1.move_owner();
    insert_object(Level1.get_level(), Goblin1.get_posx(), Goblin1.get_posy(), Goblin1.get_img_id());
    display_level(Level1.get_level());
};
