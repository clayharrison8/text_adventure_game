#include <string>
#include <map>
#include <memory>
#include <list>

using namespace std;
using std::string;

class Zombie
{
public:
    Zombie();

    // Creates object for room to be put in map
    Zombie(string id, string desc, string n, string e, string s, string w, list<string> obj, list <string> enemies);

    void killEnemy(string enemy);

    int randomNumber();

    // Calculates chances enemy is attacked based on enemies currently in room
    bool enemyExists();

    bool playerAttacked();

    // Simulates enemy attack and player health is reduced
    void enemyAttack();

    // Adds contents of json to a map, which is used to hold info about rooms
	void init(string map);

    // Lists all items player currently holds
    void printPlayerItems();

    void takeItem(string item);

    // Prints details of a room
    void printDesc();

    // Main method. Used for reading user input
    void play();

protected:
    string currentRoom;
    // Keeps track of bullets player has left
    int ammoCount;
    // Number of enemies left to kill
    int enemyCount;
    int health;
    bool playerAlive;

    // Info about room
    string id; 
    string desc;
    string n; 
	string e;
    string s;
	string w;

    // Keeps track of objects and enemies currently in room  
    list <string> obj;
    list <string> enemies;

    // Keeping data about room states
    std::map<string, shared_ptr<Zombie>> rooms;
    list <string> playerItems;

};

