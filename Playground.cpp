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
        bool get_open() { return open; };
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
    void remove_first();
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

void AI_brain::remove_first()
{
    int* new_pathX = new int[pathX_length-1];
    int* new_pathY = new int[pathY_length-1];
    if (pathX_length > 0 && pathY_length > 0)
    {
        if (pathX_length == pathY_length)
        {
            for (int i = 0; i < pathX_length - 1; i++)
            {
                new_pathX[i] = pathX[i + 1];
                new_pathY[i] = pathY[i + 1];
            }
        }
        else
        {
            for (int i = 0; i < pathX_length - 1; i++)
            {
                new_pathX[i] = pathX[i + 1];
            }
            for (int z = 0; z < pathY_length - 1; z++)
            {
                new_pathY[z] = pathY[z + 1];
            }
        }
    }

    delete[] pathX;
    delete[] pathY;
    pathX = new_pathX;
    pathY = new_pathY;

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



class Goto_Door
{
    public:
        int go_to_door(Door* door_obj, AI_brain brain)
        {
            int brain_owner_posx = brain.get_owner()->get_posx();
            int brain_owner_posy = brain.get_owner()->get_posy();
            int door_posx = door_obj->get_posx();
            int door_posy = door_obj->get_posy();
            brain.bresenham_pathfinding(brain_owner_posx,
                                        brain_owner_posy,
                                        door_posx, door_posy);
            while (brain.get_pathx()[0] != door_posx && brain.get_pathy()[0] != door_posy)
            {
                brain.move_owner();
                brain.remove_first();
            }
            return 1;
        }
};

class Open_Door
{
public:
    int open_door(Door* door_obj)
    {
        door_obj->set_open();
        return 1;
    }
};

// Result 0 = Failure, 1 = Success, 3 = undefined
class Condition_Door_Closed
{
private:
    Goto_Door child_node_goto;
    Open_Door child_node_open;
public:
    void check_the_door(Door* door_obj, AI_brain brain)
    {
        int result_goto = 3;
        int result_open = 3;
        if (door_obj->get_open() == false)
        {
            result_goto = child_node_goto.go_to_door(door_obj, brain);
        }
        if (result_goto == 1)
        {
            result_open = child_node_open.open_door(door_obj);
        }
        if (result_goto == 1 && result_open == 1)
        {
            std::cout << "Success \n";
        }
    }
};

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

void clear_object(int** level_map, int object_x, int object_y)
{
    level_map[object_x][object_y] = 0;
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
    Condition_Door_Closed open_door_tree;

        insert_object(Level1.get_level(), Goblin1.get_posx(), Goblin1.get_posy(), Goblin1.get_img_id());
        insert_object(Level1.get_level(), WoodenDoor.get_posx(), WoodenDoor.get_posy(), WoodenDoor.get_img_id());
        display_level(Level1.get_level());
        clear_object(Level1.get_level(), Goblin1.get_posx(), Goblin1.get_posy());
        clear_object(Level1.get_level(), WoodenDoor.get_posx(), WoodenDoor.get_posy());

        open_door_tree.check_the_door(&WoodenDoor, Brain1);
        insert_object(Level1.get_level(), Goblin1.get_posx(), Goblin1.get_posy(), Goblin1.get_img_id());
        insert_object(Level1.get_level(), WoodenDoor.get_posx(), WoodenDoor.get_posy(), WoodenDoor.get_img_id());
        display_level(Level1.get_level());

};
