#include <iostream>
#include "Zombie.h"
#include <string>
#include <memory>
#include <vector>
#include <list>

#include <fstream>
#include <vector>
#include "json.hpp"
#include <stdlib.h>

using namespace std;
using json = nlohmann::json;
std::map<std::string, shared_ptr<Zombie>>::const_iterator it;

Zombie::Zombie()
{
}

Zombie::Zombie(string roomId, string description, string north, string east, string south, string west, list<string> objects, list<string> enemyList)
{
    id = roomId;
    desc = description;
    n = north;
    e = east;
    s = south;
    w = west;
    obj = objects;
    enemies = enemyList;
}

void Zombie::printDesc()
{
    it = rooms.find(currentRoom);
    if (it != rooms.end())
    {
        cout << "Description: " << it->second->desc << endl;
        if (it->second->obj.empty())
        {
            cout << "No Items" << endl;
        }
        else
        {
            cout << "Items: " << endl;
            for (auto v : it->second->obj)
                cout << v << endl;
        }

        if (it->second->enemies.empty())
        {
            cout << "No Enemies" << endl;
        }
        else
        {
            cout << "Enemies: " << endl;
            for (auto v : it->second->enemies)
                cout << v << endl;
        }
    }
}

void Zombie::printPlayerItems()
{
    cout << "Player items: " << endl;
    for (auto v : playerItems)
        cout << v << endl;
}

int Zombie::randomNumber()
{
    int random;
    srand(time(NULL));

    random = rand() % 100 + 1;
    return random;
}

void Zombie::enemyAttack()
{
    it = rooms.find(currentRoom);
    if (it != rooms.end())
    {
        // Enemy at front of list will attack (assuming enemy at front is closest to player)
        string enemy = it->second->enemies.front();

        // Amount of health reduced dependant on type of enemy
        if (enemy == "clicker")
        {
            cout << "Attacked by clicker. Health down 40" << endl;
            ;
            health -= 40;
        }
        else if (enemy == "runner")
        {
            cout << "Attacked by runner. Health down 20" << endl;
            health -= 20;
        }

        if (health <= 0)
        {
            cout << "You are attacked and killed" << endl;
            cout << "Game over" << endl;
            playerAlive = false;
        }
    }
}

bool Zombie::playerAttacked()
{
    // Chances of being attacked
    if (enemyExists())
    {
        enemyAttack();
        return true;
    }
    else
    {
        return false;
    }
}

bool Zombie::enemyExists()
{
    double chances = 30;
    it = rooms.find(currentRoom);
    if (it != rooms.end())
    {
        if (!it->second->enemies.empty())
        {
            // Increases chances of being attacked based on number of enemies and type of enemy
            for (auto v : it->second->enemies)
            {
                if (v == "clicker")
                {
                    chances *= 1.2;
                }
                else if (v == "runner")
                {
                    chances *= 1.3;
                }
            }

            int random = randomNumber();

            // Chances of being attacked based on enemy
            if (random <= chances)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}

void Zombie::killEnemy(string enemy)
{
    it = rooms.find(currentRoom);
    if (it != rooms.end())
    {
        list<string> enemyList = it->second->enemies;
        bool enemyfound = (std::find(enemyList.begin(), enemyList.end(), enemy) != enemyList.end());

        if (enemyfound)
        {
            bool gunFound = (std::find(playerItems.begin(), playerItems.end(), "gun") != playerItems.end());

            if (gunFound && ammoCount != 0)
            {
                it->second->enemies.remove(enemy);
                enemyCount -= 1;
                ammoCount -= 1;
                cout << enemy << " killed" << endl
                     << endl;

                // 30% chance of picking up ammo after killing enemy
                if (randomNumber() >= 70)
                {
                    cout << "Ammo picked up" << endl;
                    ammoCount += 2;
                }

                if (enemyCount == 0)
                {
                    cout << "Congratulations, you have killed all enemies" << endl;
                }
            }
            else
            {
                cout << "You didnt have a gun/ammo and were killed as a result" << endl;
                playerAlive = false;
            }
        }
        else
        {
            cout << "Enemy does not exist" << endl;
        }
    }
}

void Zombie::takeItem(string item)
{
    it = rooms.find(currentRoom);
    if (it != rooms.end())
    {
        list<string> objects = it->second->obj;
        bool found = (std::find(objects.begin(), objects.end(), item) != objects.end());

        // Checking item exists
        if (found)
        {
            // Attacks player, if they survive/no enemies, they will take item
            playerAttacked();
            if (playerAlive != false)
            {
                cout << "Took " << item << endl;
                it->second->obj.remove(item);
                playerItems.push_back(item);

                // Each ammo will contain 2 bullets
                if (item == "ammo" || item == "gun")
                {
                    ammoCount += 2;
                }
            }
        }
        else
        {
            cout << "Item does not exist" << endl;
        }
    }
}

void Zombie::play()
{
    // a is first word input
    string a;
    // b is second word input
    string b;
    cout << "You are in " << currentRoom << endl;
    printDesc();
    cout << endl;

    while (a != "exit" && playerAlive != false && enemyCount != 0)
    {
        cin >> a;

        if (a == "go")
        {
            cin >> b;

            if (b != "north" && b != "east" && b != "south" && b != "west")
            {
                cout << "Can't go in that direction" << endl;
            }
            else
            {

                it = rooms.find(currentRoom);
                if (it != rooms.end())
                {
                    if (b == "north")
                    {
                        if (it->second->n != "")
                        {
                            cout << endl;
                            playerAttacked();

                            if (playerAlive != false)
                            {
                                currentRoom = it->second->n;
                                cout << "You are in " << currentRoom << endl;
                                printDesc();
                            }
                        }
                        else
                        {
                            cout << "No room in north direction" << endl;
                        }
                    }

                    if (b == "east")
                    {
                        if (it->second->e != "")
                        {
                            cout << endl;
                            playerAttacked();
                            if (playerAlive != false)
                            {
                                string word = e;
                                currentRoom = it->second->e;
                                cout << "You are in " << currentRoom << endl;
                                printDesc();
                            }
                        }
                        else
                        {
                            cout << "No room in east direction" << endl;
                        }
                    }

                    if (b == "south")
                    {
                        if (it->second->s != "")
                        {
                            cout << endl;
                            playerAttacked();

                            if (playerAlive != false)
                            {
                                currentRoom = it->second->s;
                                cout << "You are in " << currentRoom << endl;
                                printDesc();
                            }
                        }
                        else
                        {
                            cout << "No room in south direction" << endl;
                        }
                    }

                    if (b == "west")
                    {
                        if (it->second->w != "")
                        {
                            cout << endl;
                            playerAttacked();

                            if (playerAlive != false)
                            {
                                currentRoom = it->second->w;
                                cout << "You are in " << currentRoom << endl;
                                printDesc();
                            }
                        }
                        else
                        {
                            cout << "No room in west direction" << endl;
                        }
                    }
                }
            }
        }

        else if (a == "list")
        {
            cin >> b;
            if (b == "items")
            {
                printPlayerItems();
            }
            else if (b == "ammo")
            {
                cout << "Ammo Count: " << ammoCount << endl
                     << endl;
            }
            else if (b == "health")
            {
                cout << "Health: " << health << endl
                     << endl;
            }
        }

        else if (a == "take")
        {
            cin >> b;
            takeItem(b);
        }

        else if (a == "kill")
        {
            cin >> b;
            killEnemy(b);
        }

        else if (a == "eat")
        {
            cin >> b;
            if (b == "snack")
            {

                bool snackFound = (std::find(playerItems.begin(), playerItems.end(), "snack") != playerItems.end());
                if (snackFound)
                {
                    playerItems.remove("snack");

                    // Each snack will add 30 to player health
                    if (health + 30 > 100)
                    {
                        health = 100;
                        cout << "Health increased " << endl;
                    }
                    else
                    {
                        health += 30;
                        cout << "Health increased " << endl;
                    }
                }
                else
                {
                    cout << "Snack not found" << endl;
                }
            }
        }
    }
}

void Zombie::init(string map)
{
    playerAlive = true;
    health = 100;
    ifstream fin(map);
    json j; // object that represents the json data
    fin >> j;

    string initialRoom = j["player"]["initialroom"].get<string>();
    list<string> items = j["player"]["obj"];

    playerItems = items;
    currentRoom = initialRoom;
    enemyCount = 0;

    bool gunFound = (std::find(playerItems.begin(), playerItems.end(), "gun") != playerItems.end());
    // Assuming gun has two bullets seems
    if (gunFound)
    {
        ammoCount = 2;
    }
    else {
        ammoCount = 0;
    }

    // Loops through rooms array and adds it to rooms map
    for (int i = 0; i < j["rooms"].size(); i++)
    {

        string id = j["rooms"][i]["id"];
        string desc = j["rooms"][i]["desc"];
        string n = j["rooms"][i]["n"];
        string e = j["rooms"][i]["e"];
        string s = j["rooms"][i]["s"];
        string w = j["rooms"][i]["w"];
        list<string> obj = j["rooms"][i]["obj"];
        list<string> enemies = j["rooms"][i]["enemies"];

        enemyCount += enemies.size();

        shared_ptr<Zombie> str(new Zombie(id, desc, n, e, s, w, obj, enemies));
        rooms.insert({id, str});
    }
}

int main(int argc, char *argv[])
{
    Zombie z;
    string map;

    map = argv[1];

    z.init(map);
    z.play();

    return 0;
}