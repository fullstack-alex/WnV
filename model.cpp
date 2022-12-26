#include <iostream>
#include <typeinfo>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include<bits/stdc++.h>
using namespace std;

class Position
{
    public:
        int x;
        int y;

        Position()
        {}

        Position(int x, int y)
        {
            this->x = x;
            this->y = y;
        }

        bool equals(Position position)
        {
            return x == position.x && y == position.y;
        }

        void print()
        {
            cout << "<" << x << "," << y << ">\n";
        }
};

enum mapObjectType
{
    earth,
    water,
    tree,
    character
};

class MapObject
{
    public:
        Position position;
        bool isWalkable;
        mapObjectType type;

        MapObject(mapObjectType type, Position position)
        {
            this->position = position;
            this->type = type;
            isWalkable = type == earth;
        }
};

class Character: public MapObject
{
    public:
        int hp;
        int strength;
        int defense;
        int medicine;

        Character(mapObjectType type, Position position) : MapObject(type, position)
        {}
};

enum movement   //seems unnecessary but its easier to change movements in the future (if needed)
{
    upMovement,
    downMovement,
    leftMovement,
    rightMovement,
    upleftMovement,
    uprightMovement,
    downleftMovement,
    downrightMovement
};

enum supportingRace
{
    vampire,
    werewolf
};

class Vampire : public Character
{
    public:
        movement moves[8] {upMovement, downMovement, leftMovement, rightMovement, upleftMovement, uprightMovement, downleftMovement, downrightMovement};

        Vampire(mapObjectType type, Position position) : Character(type, position)
        {}
    };

class Werewolf : public Character
{
    public:
        movement moves[4] {upMovement, downMovement, leftMovement, rightMovement};

        Werewolf(mapObjectType type, Position position) : Character(type, position)
        {}
};

class Avatar : public Character
{
    public:
        movement moves[4] {upMovement, downMovement, leftMovement, rightMovement};
        int potions = 1;
        string supportingRace;

        Avatar(string supportingRace, mapObjectType type, Position position) : Character(type, position)
        {
            this->supportingRace = supportingRace;
        }
};

int getRandomNumberInRange(int start, int finish)
{
    return start + ( rand() % ( finish - start + 1 ) );
}

// enum mapObjectType
// {
//     earth,
//     water,
//     tree,
//     character
// };

// class MapObject
// {
//     public:
//         int x;
//         int y;
//         bool isWalkable;
//         mapObjectType type;

//         MapObject(mapObjectType type, int x, int y)
//         {
//             this->x = x;
//             this->y = y;
//             this->type = type;
//             isWalkable = type == earth;
//         }
// };

class Map
{
    public:
        int x;
        int y;
        vector<MapObject> mapElements;

        Map(int x, int y) // Constructor
        {     
            if(x <= 0 || y <= 0)
            {
                cout << "Wrong map dimensions.\n Please try again." << endl;
                exit(1);
            }

            this->x = x;
            this->y = y;
            fillMap();
        }

        void fillMap()
        {
            for (int i = 0; i < x; i++)
            {
                for (int j = 0; j < y; j++)
                {
                    int randomNumber = getRandomNumberInRange(0, 100);
                    Position position;
                    position.x = i;
                    position.y = j;

                    if (randomNumber <= 80)
                    {
                        mapElements.push_back(MapObject(earth, position));
                    }
                    else if (randomNumber <= 90)
                    {
                        mapElements.push_back(MapObject(water, position));
                    }
                    else
                    {
                        mapElements.push_back(MapObject(tree, position));
                    }
                }
            }
        }

        void print(bool showCoordinates = false)
        {
            for (int i = 0; i < (x * y); i++)
            {
                if (showCoordinates)
                {
                    cout << "<" << mapElements[i].position.x << "," << mapElements[i].position.y << ">  ";  //test only
                }
                else
                {
                    cout << mapElements[i].type;
                }

                if ((i + 1) % x == 0)
                {
                    cout << endl;
                }
            }
        }
}; 

bool isSupportingVampire(string supportingRace)
{
    return supportingRace == typeid(Vampire).name();
}

bool isSupportingWerewolf(string supportingRace)
{
    return supportingRace == typeid(Werewolf).name();
}

class Game
{
    public:
        Map map;
        Avatar avatar;
        vector<Vampire> vampireTeam;
        vector<Werewolf> werewolfTeam;
        
        Game(int x, int y, string avatarSupportingRace) : map(x,y), avatar(avatarSupportingRace, character, Position(x, 0))
        {
            for (int i = 0; i < (x*y) / 15; i++)
            {
                Vampire vampire(character, getValidPosition());
                vampire.medicine = getRandomNumberInRange(0,2);
                vampireTeam.push_back(vampire);

                Werewolf werewolf(character, getValidPosition());
                werewolf.medicine = getRandomNumberInRange(0,2);
                werewolfTeam.push_back(werewolf);
            }
        }

        void print()
        {
            cout << "\n\n";
            if (isSupportingVampire(avatar.supportingRace))
            {
                cout << "You support Vampires.";
            }
            else if (isSupportingWerewolf(avatar.supportingRace))
            {
                cout << "You support Werewolves.";
            }
            cout << "\n\n";
            map.print();
            cout  << endl << endl << "Available inputs: (case-insensitive)" << endl <<"W - UP   S - DOWN    A - LEFT    D - RIGHT" << endl << "EXIT - QUIT GAME" << endl << endl;
        }

        Position getValidPosition()
        {
            Position position;
            bool isPositionTaken = true;
            while (isPositionTaken)
            {
                int randomPosX = getRandomNumberInRange(0, map.x);
                int randomPosY = getRandomNumberInRange(0, map.y);

                position = Position(randomPosX, randomPosY);

                isPositionTaken = this->isPositionTaken(getAllValidPositions(), position);
            }

            position.print();
            return position;
        }

        bool isPositionTaken(vector<Position> positions, Position position)
        {
            for(int i = 0; i < positions.size(); i++)
            {
                if (positions[i].equals(position))
                {
                    return true;
                }
            }

            return false;
        }

        vector<Position> getAllValidPositions()
        {
            vector<Position> positions;

            for (int i = 0; i < map.x; i++)
            {
                for (int j = 0; j < map.y; j++)
                {
                    positions.push_back(Position(i,j));
                }
            }

            for (int i = 0; i < vampireTeam.size(); i++)
            {
                for (int j = 0; j < positions.size(); j++)
                {
                    if (vampireTeam[i].position.equals(positions[j]))
                    {
                        positions.erase(positions.begin() + j);
                    }
                }
            }

            for (int i = 0; i < werewolfTeam.size(); i++)
            {
                for (int j = 0; j < positions.size(); j++)
                {
                    if (werewolfTeam[i].position.equals(positions[j]))
                    {
                        positions.erase(positions.begin() + j);
                    }
                }
            }

            for (int i = 0; i < map.mapElements.size(); i++)
            {
                for (int j = 0; j < positions.size(); j++)
                {
                    if (map.mapElements[i].position.equals(positions[j]) && map.mapElements[i].type != earth)
                    {
                        positions.erase(positions.begin() + j);
                    }
                }
            }

            return positions;
        }
};


int getIntFromArgument(char* arg)
{
    istringstream iss(arg);
    int intNumber;
    if (iss >> intNumber)
    {
        return intNumber;
    }
    return 0;
}

movement getAction(string input)
{
    if (input == "w")
    {
        return upMovement;
    } 
    else if (input == "s")
    {
        return downMovement;
    }
    else if (input == "a")
    {
        return leftMovement;
    }
    else if (input == "d")
    {
        return rightMovement;
    }
}

int main(int argc, char** argv)
{
    system("clear");
    srand(time(NULL));
    bool isPlaying = true;
    string input = "standby";
    string avatarSupportingRace;

    if (argc != 3) 
    {
        cout << "Wrong arguments.\n";
        exit(1); // terminate with error
    }

    
    bool isSupportPhase = true;
    while (isSupportPhase)
    {
        cout << "Would you like to support Vampires or Werewolves?" << endl << "Type \"vampires\" or \"werewolves\" respectively. (case-insensitive)" << endl;
        cin >> avatarSupportingRace;
        transform(avatarSupportingRace.begin(), avatarSupportingRace.end(), avatarSupportingRace.begin(), ::tolower);
        if (avatarSupportingRace == "vampires")
        {
            avatarSupportingRace = typeid(Vampire).name();
            isSupportPhase = false;
        }
        else if (avatarSupportingRace == "werewolves")
        {
            avatarSupportingRace = typeid(Werewolf).name();
            isSupportPhase = false;
        }
        else
        {
            system("clear");
            cout << "Try again." << endl << endl;
        }
    }

    Game game(getIntFromArgument(argv[1]), getIntFromArgument(argv[2]), avatarSupportingRace);

    system("clear");
    game.print();

    while(isPlaying)
    {
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if(input == "exit") {
            // system("stty cooked");
            exit(0);
        } 
        else if (input == "pause")
        {
            system("clear");
        }
        else if (input != "standby")
        {
            system("clear");

            if (input == "w" || input == "s" || input == "a" || input == "d")
            {
                cout << "Arrow is pressed\n\n";
            }
            game.print();
            input = "standby";
        }
    }

    return 0;
}