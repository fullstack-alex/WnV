#include <stdlib.h>
#include <bits/stdc++.h>
#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

bool isPlaying = true;

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

int getRandomNumberInRange(int start, int finish)
{
    return start + ( rand() % ( finish - start + 1 ) );
}

enum action
{
    attack,
    heal,
    obtain
};

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
            return this->x == position.x && this->y == position.y;
        }

        void print()
        {
            cout << "<" << x << "," << y << ">\n";
        }

        bool isValid(int limitX, int limitY)
        {
            return this->x >= 0 && this->y >= 0 && this->x <= limitX - 1 && this->y <= limitY - 1;
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
        mapObjectType type;

        MapObject()
        {}
        MapObject(mapObjectType type)
        {
            this->type = type;
        }
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

class Character: public MapObject
{
    public:
        vector<movement> moves;

        Character() : MapObject(character)
        {}
};

class Attackable
{
    public:
        int hp;
        int strength;
        int defense;
        int medicine;
        bool isAlive = true;
};

enum supportingRace
{
    vampire,
    werewolf
};

class Vampire : public Character, public Attackable
{
    public:
        Vampire()
        {
            moves = {upMovement, downMovement, leftMovement, rightMovement, upleftMovement, uprightMovement, downleftMovement, downrightMovement};
        }
};

class Werewolf : public Character, public Attackable
{
    public:
        Werewolf()
        {
            moves = {upMovement, downMovement, leftMovement, rightMovement};
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

class Avatar : public Character
{
    public:
        int potions = 1;
        string supportingRace;

        Avatar(string supportingRace)
        {
            this->supportingRace = supportingRace;
        moves = {upMovement, downMovement, leftMovement, rightMovement};
        }
};

class Map
{
    public:
        int x;
        int y;
        vector<MapObject> elements;

        Map(int x, int y) // Constructor
        {     
            if(x <= 0 || y <= 0)
            {
                cout << "Wrong map dimensions.\n Please try again." << endl;
                exit(1);
            }

            this->x = x;
            this->y = y;
        }
        
        int getEarthSize()
        {
            int earthSize = 0;

            for (int i = 0; i < elements.size(); i++)
            {
                if(elements[i].type == earth)
                {
                    earthSize++;
                }
            }

            return earthSize;
        }

        void printElements(bool showCoordinates = false)
        {
            for (int i = 0; i < (x * y); i++)
            {
                if (showCoordinates)
                {
                    elements[i].position.print();
                }
                else
                {
                    try
                    {
                        cout << elements[i].type;
                    }
                    catch(...)
                    {
                        cout << " ";
                    }
                }

                if ((i + 1) % x == 0)
                {
                    cout << endl;
                }
            }
        }

        bool isPosObstacle(Position pos)
        {
            for (int i = 0; i < elements.size(); i++)
            {
                if (elements[i].position.equals(pos))
                {
                    if (elements[i].type != earth)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            return false;
        }

        mapObjectType getElementTypeInPos(Position pos)
        {
            mapObjectType type;
            
            for (int i = 0; i < elements.size(); i++)
            {
                if (elements[i].position.equals(pos))
                {
                    type = elements[i].type;
                }
            }

            return type;
        }
}; 

class Game
{
    public:
        Map map;
        Avatar avatar;
        vector<Vampire> vampireTeam;
        vector<Werewolf> werewolfTeam;
        Position potionPosition;
        bool isDay;
        
        Game(int x, int y, string avatarSupportingRace) : map(x,y), avatar(avatarSupportingRace)
        {
            for (int i = 0; i < (x*y) / 15; i++)
            {
                Vampire vampire;
                vampire.hp = 10;
                vampire.strength = getRandomNumberInRange(1,3);
                vampire.defense = getRandomNumberInRange(1,2);
                vampire.medicine = getRandomNumberInRange(0,2);
                vampireTeam.push_back(vampire);

                Werewolf werewolf;
                werewolf.hp = 10;
                werewolf.strength = getRandomNumberInRange(1,3);
                werewolf.defense = getRandomNumberInRange(1,2);
                werewolf.medicine = getRandomNumberInRange(0,2);
                werewolfTeam.push_back(werewolf);
            }
        }

        void placeMapElements()
        {
            for (int i = 0; i < map.x; i++)
            {
                for (int j = 0; j < map.y; j++)
                {
                    int randomNumber = getRandomNumberInRange(0, 100);
                    MapObject element;
                    element.position.x = i;
                    element.position.y = j;

                    if (randomNumber <= 80)
                    {
                        element.type = earth;
                        map.elements.push_back(element);
                    }
                    else if (randomNumber <= 90)
                    {
                        element.type = water;
                        map.elements.push_back(element);
                    }
                    else
                    {
                        element.type = tree;
                        map.elements.push_back(element);
                    }
                }
            }
        }

        void placeObjects()
        {
            int potionsToPlace = 1;
            int avatarsToPlace = 1;
            int vampiresToPlace = vampireTeam.size();
            int werewolvesToPLace = werewolfTeam.size();

            int objectsSize = potionsToPlace + avatarsToPlace + vampiresToPlace + werewolvesToPLace;
            int availableEarth = map.getEarthSize();

            cout << "Initial object size " << objectsSize << endl;

            for (int i = 0; i < map.elements.size(); i++)
            {
                if (map.elements[i].type == earth)
                {
                    Position position(map.elements[i].position.x, map.elements[i].position.y);
                    int chanceToPlaceItem = getRandomNumberInRange(0, 5);   

                    if (availableEarth == objectsSize) chanceToPlaceItem = 5;   //if the available space of the map is equal to the number of the remaining objects, make sure that the object will be placed

                    if (chanceToPlaceItem == 5) //decide if an object will be placed
                    {
                        //decide which object will be placed
                        int chanceForObjectType = getRandomNumberInRange(0, 3);

                        if (chanceForObjectType == 0)
                        {
                            if (potionsToPlace > 0 )
                            {
                                potionPosition = position;
                                potionsToPlace--;
                            }
                            else if (avatarsToPlace > 0)
                            {
                                avatar.position = position;
                                avatarsToPlace--;
                            }
                            else if (vampiresToPlace > 0)
                            {
                                vampireTeam[vampiresToPlace - 1].position = position;
                                vampiresToPlace--;
                            }
                            else if (werewolvesToPLace > 0)
                            {
                                werewolfTeam[werewolvesToPLace - 1].position = position;
                                werewolvesToPLace--;
                            }
                        }
                        else if (chanceForObjectType == 1)
                        {
                            if (avatarsToPlace > 0 )
                            {
                                avatar.position = position;
                                avatarsToPlace--;
                            }
                            else if (potionsToPlace > 0)
                            {
                                potionPosition = position;
                                potionsToPlace--;
                            }
                            else if (vampiresToPlace > 0)
                            {
                                vampireTeam[vampiresToPlace - 1].position = position;
                                vampiresToPlace--;
                            }
                            else if (werewolvesToPLace > 0)
                            {
                                werewolfTeam[werewolvesToPLace - 1].position = position;
                                werewolvesToPLace--;
                            }
                        }
                        else if ( chanceForObjectType == 2)
                        {
                            if (vampiresToPlace > 0 )
                            {
                                vampireTeam[vampiresToPlace - 1].position = position;
                                vampiresToPlace--;
                            }
                            else if (potionsToPlace > 0)
                            {
                                potionPosition = position;
                                potionsToPlace--;
                            }
                            else if (avatarsToPlace > 0)
                            {
                                avatar.position = position;
                                avatarsToPlace--;
                            }
                            else if (werewolvesToPLace > 0)
                            {
                                werewolfTeam[werewolvesToPLace - 1].position = position;
                                werewolvesToPLace--;
                            }
                        }
                        else if (chanceForObjectType == 3)
                        {
                            if (werewolvesToPLace > 0 )
                            {
                                werewolfTeam[werewolvesToPLace - 1].position = position;
                                werewolvesToPLace--;
                            }
                            else if (potionsToPlace > 0)
                            {
                                potionPosition = position;
                                potionsToPlace--;
                            }
                            else if (avatarsToPlace > 0)
                            {
                                avatar.position = position;
                                avatarsToPlace--;
                            }
                            else if (vampiresToPlace > 0)
                            {
                                vampireTeam[vampiresToPlace - 1].position = position;
                                vampiresToPlace--;
                            }
                        }
                        objectsSize--;
                    }

                    availableEarth--;
                }

                if (objectsSize == 0) break;
            }

            cout<< "Object size: " << objectsSize << endl;
        }

        int getActiveVampires()
        {
            int aliveVampires = 0;
            for (int i = 0; i < vampireTeam.size(); i++)
            {
                if (vampireTeam[i].isAlive) aliveVampires++;
            }
            return aliveVampires;
        }

        int getActiveWerewolves()
        {
            int aliveWerewolves = 0;
            for (int i = 0; i < werewolfTeam.size(); i++)
            {
                if (werewolfTeam[i].isAlive) aliveWerewolves++;
            }
            return aliveWerewolves;
        }

        void print()
        {
            if (isDay)
            {
                cout << "\nDayTime\n\n";
            }
            else
            {
                cout << "\nNightTime\n\n";
            }
            cout << "\nTree is symbolized as \"+\", water is symbolized as \"~\", earth is symbolized as \"-\"\n";
            cout << "Avatar is \"V\" or \"W\", vampires are \"v\", werevolves are \"w\" and potions are \"p\"\n\n";
            if (isSupportingVampire(avatar.supportingRace))
            {
                cout << "You support Vampires.";
            }
            else if (isSupportingWerewolf(avatar.supportingRace))
            {
                cout << "You support Werewolves.";
            }
            cout << "\n\n";
            this->printObjects();
            cout << getAvailableActionsMessage();
        }

        bool isPosObject(Position pos)
        {
            if (potionPosition.equals(pos)) return true;
            else if (avatar.position.equals(pos)) return true;
            else if (findPosInVampires(pos)) return true;
            else if (findPosInWerewolves(pos)) return true;

            return false;
        }

        vector<movement> getAvailableMoves(Position pos, vector<movement> characterMoves)
        {
            vector<movement> moves;

            for(int i = 0; i < characterMoves.size(); i++)
            {
                Position nextPosition = getPositionFromMove(pos, characterMoves[i]);
                
                if (nextPosition.isValid(map.x, map.y) && !map.isPosObstacle(nextPosition) && !isPosObject(nextPosition))
                {
                    moves.push_back(characterMoves[i]);
                }
            }

            return moves;
        }

        Position getPositionFromMove(Position currentPos, movement move)
        {
            Position nextPosition(currentPos);

            if (move == upMovement)
            {
                nextPosition = Position(currentPos.x - 1, currentPos.y);
            }
            else if (move == downMovement)
            {
                nextPosition = Position(currentPos.x + 1, currentPos.y);
            }
            else if (move == leftMovement)
            {
                nextPosition = Position(currentPos.x, currentPos.y - 1);
            }
            else if (move == rightMovement)
            {
                nextPosition = Position(currentPos.x, currentPos.y + 1);
            }
            else if (move == upleftMovement)
            {
                nextPosition = Position(currentPos.x - 1, currentPos.y - 1);
            }
            else if (move == uprightMovement)
            {
                nextPosition = Position(currentPos.x - 1, currentPos.y + 1);
            }
            else if (move == downleftMovement)
            {
                nextPosition = Position(currentPos.x + 1, currentPos.y - 1);
            }
            else if (move == downrightMovement)
            {
                nextPosition = Position(currentPos.x + 1, currentPos.y + 1);
            }
            
            return nextPosition;
        }

        bool moveAvatar(string input)
        {
            vector<movement> availableMoves = getAvailableMoves(avatar.position, avatar.moves);

            if (input == "w" && find(availableMoves.begin(), availableMoves.end(), upMovement) != availableMoves.end())
            {
                avatar.position = getPositionFromMove(avatar.position, upMovement);
                return true;
            } 
            else if (input == "s" && find(availableMoves.begin(), availableMoves.end(), downMovement) != availableMoves.end())
            {
                avatar.position = getPositionFromMove(avatar.position, downMovement);
                return true;
            }
            else if (input == "a" && find(availableMoves.begin(), availableMoves.end(), leftMovement) != availableMoves.end())
            {
                avatar.position = getPositionFromMove(avatar.position, leftMovement);
                return true;
            }
            else if (input == "d" && find(availableMoves.begin(), availableMoves.end(), rightMovement) != availableMoves.end())
            {
                avatar.position = getPositionFromMove(avatar.position, rightMovement);
                return true;
            }

            return false;
        }

        Position getCharacterNextPosition(Position currentPos, vector<movement> legalMoves)
        {
            vector<movement> availableMoves = getAvailableMoves(currentPos, legalMoves);

            if (availableMoves.size() <= 0) return currentPos;

            int randomMoveIndex = getRandomNumberInRange(0, availableMoves.size() - 1);
            return getPositionFromMove(currentPos, availableMoves[randomMoveIndex]);
        }

        void endTurn()
        {
            //actions of NPCs
            for (int i = 0; i < vampireTeam.size(); i++)
            {
                if (vampireTeam[i].isAlive)
                {
                    int chanceToMove = getRandomNumberInRange(0, 100);
                    if (chanceToMove < 50)
                    {
                        vampireTeam[i].position = getCharacterNextPosition(vampireTeam[i].position, vampireTeam[i].moves);
                    }

                    if(isAllyVampire(vampireTeam[i].position, heal, vampireTeam[i].strength) && vampireTeam[i].medicine > 0)
                    {
                        if (actionAtPos(getPositionOfNeighborVampire(vampireTeam[i].position, heal, vampireTeam[i].strength), heal, true, vampireTeam[i].strength))
                        {
                            vampireTeam[i].medicine--;
                        }
                    }
                    
                    if (isAllyWereowolf(vampireTeam[i].position, attack, vampireTeam[i].strength))
                    {
                        actionAtPos(getPositionOfNeighborWerewolf(vampireTeam[i].position, attack, vampireTeam[i].strength), attack, false, vampireTeam[i].strength);
                    }
                }
            }

            for (int i = 0; i < werewolfTeam.size(); i++)
            {
                if (werewolfTeam[i].isAlive)
                    {
                    int chanceToMove = getRandomNumberInRange(0, 100);
                    if (chanceToMove < 50)
                    {
                        werewolfTeam[i].position = getCharacterNextPosition(werewolfTeam[i].position, werewolfTeam[i].moves);
                    }

                    if(isAllyVampire(werewolfTeam[i].position, heal, werewolfTeam[i].strength) && werewolfTeam[i].medicine > 0)
                    {
                        if (actionAtPos(getPositionOfNeighborVampire(werewolfTeam[i].position, heal, werewolfTeam[i].strength), heal, false, werewolfTeam[i].strength))
                        {    
                            werewolfTeam[i].medicine--;
                        }
                    }
                    
                    if (isAllyWereowolf(werewolfTeam[i].position, attack, werewolfTeam[i].strength))
                    {
                        actionAtPos(getPositionOfNeighborWerewolf(werewolfTeam[i].position, attack, werewolfTeam[i].strength), attack, true, werewolfTeam[i].strength);
                    }
                }
            }

            isDay = !isDay;
        }

        void usePotion()
        {
            if (avatar.potions > 0)
            {
                if (isSupportingVampire(avatar.supportingRace) && isDay)
                {
                    for (int i = 0; i < vampireTeam.size(); i++)
                    {
                        if (vampireTeam[i].isAlive) vampireTeam[i].hp = 10;
                    }

                    avatar.potions--;
                }
                else if (isSupportingWerewolf(avatar.supportingRace) && !isDay)
                {
                    for (int i = 0; i < werewolfTeam.size(); i++)
                    {
                        if (werewolfTeam[i].isAlive) werewolfTeam[i].hp = 10;
                    }

                    avatar.potions--;
                }
            }
            else
            {
                cout << "\nNo available potions.";
            }
        }

        string getAvailableActionsMessage()
        {
            string moveMessage = "PASS - PASS   ";
            vector<movement> availableMoves = getAvailableMoves(avatar.position, avatar.moves);

            if (find(availableMoves.begin(), availableMoves.end(), upMovement) != availableMoves.end())
            {
                moveMessage += "W - UP   ";
            } 
            if (find(availableMoves.begin(), availableMoves.end(), downMovement) != availableMoves.end())
            {
                moveMessage += "S - DOWN   ";
            }
            if (find(availableMoves.begin(), availableMoves.end(), leftMovement) != availableMoves.end())
            {
                moveMessage += "A - LEFT   ";
            }
            if (find(availableMoves.begin(), availableMoves.end(), rightMovement) != availableMoves.end())
            {
                moveMessage += "D - RIGHT   ";
            }

            string message = "\n\nAvailable inputs: (case-insensitive)\n" + moveMessage;

            if (avatar.potions != 0) 
            {
                message += "\nPOTION - HEAL ALL YOUR ALLIES";
            }

            if (isPotionClose())
            {
                message += "\n OBTAIN = OBTAIN THE POTION";
            }

            message += "\nPAUSE - PAUSE GAME\nEXIT - QUIT GAME\n\n";

            return message;
        }

        bool isPotionClose()
        {
            vector<Position> nearPositions = {Position(avatar.position.x - 1, avatar.position.y), Position(avatar.position.x + 1, avatar.position.y), Position(avatar.position.x, avatar.position.y - 1), Position(avatar.position.x, avatar.position.y + 1)};

                for (int i = 0; i < nearPositions.size(); i++)
                {
                    if (potionPosition.equals(nearPositions[i]))
                    {
                        return true;
                    }
                }
            return false;
        }

        private:

        void printObjects()
        {
            for (int i = 0; i < map.x; i++)
            {
                for (int j = 0; j < map.y; j++)
                {
                    Position currentPosition = Position(i, j);
                    if (potionPosition.equals(currentPosition)) cout << "P"; 
                    else if (avatar.position.equals(currentPosition))
                    {
                        if (isSupportingVampire(avatar.supportingRace))
                        {
                            cout << "V";
                        }
                        else if (isSupportingWerewolf(avatar.supportingRace))
                        {
                            cout << "W";
                        }
                    }
                    else if (vampireTeam.size() > 0 && findPosInVampires(currentPosition))
                    {
                        cout << "v";
                    }
                    else if (werewolfTeam.size() > 0 && findPosInWerewolves(currentPosition))
                    {
                        cout << "w";
                    }
                    else
                    {
                        if (map.getElementTypeInPos(currentPosition) == earth)
                        {
                            cout << "-";
                        }
                        else if (map.getElementTypeInPos(currentPosition) == water)
                        {
                            cout << "~";
                        }
                        else if (map.getElementTypeInPos(currentPosition) == tree)
                        {
                            cout << "+";  
                        }
                    }
                }

                cout << endl;
            }
        }

        bool actionAtPos(Position pos, action act, bool isTargetVampire, int strength)
        {
            if (act != obtain && isTargetVampire)
            {
                for (int i = 0; i < vampireTeam.size(); i++)
                {
                    if (vampireTeam[i].position.equals(pos))
                    {
                        if (act == heal)
                        {
                            vampireTeam[i].hp = 10;
                        }
                        else if (act == attack && vampireTeam[i].defense < strength)
                        {
                            vampireTeam[i].hp -= strength - vampireTeam[i].defense;
                            if (vampireTeam[i].hp <= 0)
                            {
                                vampireTeam[i].isAlive = false;
                                if (getActiveVampires() <= 0) 
                                {
                                    cout << "Vampires Won!";
                                    isPlaying = false;
                                }
                            } 
                        }
                    }
                }
            }
            else if (act != obtain && !isTargetVampire)
            {
                for (int i = 0; i < werewolfTeam.size(); i++)
                {
                    if (werewolfTeam[i].position.equals(pos))
                    {
                        if (act == heal)
                        {
                            werewolfTeam[i].hp = 10;
                        }
                        else if (act == attack && werewolfTeam[i].defense < strength)
                        {
                            werewolfTeam[i].hp -= strength - werewolfTeam[i].defense;
                            if (werewolfTeam[i].hp <= 0) 
                            {
                                werewolfTeam[i].isAlive = false;
                                if (getActiveWerewolves() <= 0)
                                {
                                    cout << "Werewolves Won!";
                                    isPlaying = false;
                                }
                            } 
                        }
                    }
                }
            }
            else if (act == obtain)
            {

            }
        }

        bool isAllyVampire(Position position, action act, int strength)
        {
            vector<Position> nearPositions = {Position(position.x - 1, position.y), Position(position.x + 1, position.y), Position(position.x, position.y - 1), Position(position.x, position.y + 1)};

            for (int i = 0; i < vampireTeam.size(); i++)
            {
                for (int j = 0; j < nearPositions.size(); j++)
                {
                    if (vampireTeam[i].position.equals(nearPositions[j]) && vampireTeam[i].isAlive)
                    {
                        if (act == heal && vampireTeam[i].hp < 10)
                        {
                            return true;
                        }
                        else if (act == attack && vampireTeam[i].strength <= strength)
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool isAllyWereowolf(Position position, action act, int strength)
        {
            vector<Position> nearPositions = {Position(position.x - 1, position.y), Position(position.x + 1, position.y), Position(position.x, position.y - 1), Position(position.x, position.y + 1)};

            for (int i = 0; i < werewolfTeam.size(); i++)
            {
                for (int j = 0; j < nearPositions.size(); j++)
                {
                    if (werewolfTeam[i].position.equals(nearPositions[j]) && werewolfTeam[i].isAlive)
                    {
                        if (act == heal && werewolfTeam[i].hp < 10)
                        {
                            return true;
                        }
                        else if (act == attack && werewolfTeam[i].strength <= strength)
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        Position getPositionOfNeighborVampire(Position position, action act, int strength)
        {
            vector<Position> nearPositions = {Position(position.x - 1, position.y), Position(position.x + 1, position.y), Position(position.x, position.y - 1), Position(position.x, position.y + 1)};

            for (int i = 0; i < vampireTeam.size(); i++)
            {
                for (int j = 0; j < nearPositions.size(); j++)
                {
                    if (vampireTeam[i].position.equals(nearPositions[j]))
                    {
                        if (act == heal && vampireTeam[i].hp < 10)
                        {
                            return vampireTeam[i].position;
                        }
                        else if (act == attack && vampireTeam[i].strength <= strength)
                        {
                            return vampireTeam[i].position;
                        }
                    }
                }
            }
        }

        Position getPositionOfNeighborWerewolf(Position position, action act, int strength)
        {
            vector<Position> nearPositions = {Position(position.x - 1, position.y), Position(position.x + 1, position.y), Position(position.x, position.y - 1), Position(position.x, position.y + 1)};

            for (int i = 0; i < werewolfTeam.size(); i++)
            {
                for (int j = 0; j < nearPositions.size(); j++)
                {
                    if (werewolfTeam[i].position.equals(nearPositions[j]))
                    {
                        if (act == heal && werewolfTeam[i].hp < 10)
                        {
                            return werewolfTeam[i].position;
                        }
                        else if (act == attack && werewolfTeam[i].strength <= strength)
                        {
                            return werewolfTeam[i].position;
                        }
                    }
                }
            }
        }

        bool findPosInVampires(Position position)
        {
            for (int i = 0; i < vampireTeam.size(); i++)
            {
                if (vampireTeam[i].position.equals(position) && vampireTeam[i].isAlive)
                {
                    return true;
                }
            }

            return false;
        }

        bool findPosInWerewolves(Position position)
        {
            for (int i = 0; i < werewolfTeam.size(); i++)
            {
                if (werewolfTeam[i].position.equals(position) && werewolfTeam[i].isAlive)
                {
                    return true;
                }
            }

            return false;
        }
};

int main(int argc, char** argv)
{
    system("clear");
    srand(time(NULL));
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
    game.placeMapElements();
    game.placeObjects();

    system("clear");
    game.print();

    bool isGamePaused = false;
    while(isPlaying)
    {
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if(input == "exit") 
        {
            isPlaying = false;
        } 

        if(isGamePaused)
        {
           if (input == "resume")
            {
                isGamePaused = false;
            } 
            else
            {
                system("clear");
                cout << "\nActive Vampires: " << game.getActiveVampires() << "\nActive Werewolves: " << game.getActiveWerewolves() << "\nAvailable potions: " << game.avatar.potions << "\n\nAvailable inputs: (case-insensitive)\nRESUME - CONTINUE PLAYING" << endl << "EXIT - QUIT GAME\n\n";
            }
        }
        
        if(!isGamePaused)
        {
            if (input == "pause")
            {
                isGamePaused = true;
                system("clear");
                cout << "\nActive Vampires: " << game.getActiveVampires() << "\nActive Werewolves: " << game.getActiveWerewolves() << "\nAvailable potions: " << game.avatar.potions << "\n\nAvailable inputs: (case-insensitive)\nRESUME - CONTINUE PLAYING" << endl << "EXIT - QUIT GAME\n\n";
            }
            else if (input != "standby")
            {
                system("clear");

                if (input == "w" || input == "s" || input == "a" || input == "d" || input == "pass")
                {
                    if (game.moveAvatar(input) || input == "pass")
                    {
                        game.endTurn();
                    }
                }
                else if (input == "potion" && game.avatar.potions > 0)
                {
                    game.usePotion();
                }
                else if (input == "obtain" && game.isPotionClose())
                {
                    game.potionPosition = Position(-100,-100);
                    game.avatar.potions++;
                }

                game.print();
                input = "standby";
            }
        }
    }

    return 0;
}