/*
 * Course: COMP 206
 * Assignment: Assignment 4
 * Name: Jai Srinivasan Sivaraj
 * Student ID: 3806075
 */

#include <iostream>
#include <map> 
#include <fstream> 
#include <sstream> 
#include <vector> 
#include <algorithm>
using namespace std;

static string trim(const string &value)
{
    size_t start = value.find_first_not_of(" \t\r\n"); 
    if (start == string::npos) 
        return "";
    size_t end = value.find_last_not_of(" \t\r\n"); 
    return value.substr(start, end - start + 1); 
}

static string toLower(const string &value)
{
    string result = value;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

static bool findItemCaseInsensitive(const vector<string> &items, const string &target, string &found)
{
    string lowerTarget = toLower(target);
    for (auto &item : items)
    {
        if (toLower(item) == lowerTarget)
        {
            found = item;
            return true;
        }
    }
    return false;
}

class Location
{
    private:
        string name, desc;
        map<string,string> exits; 
        vector<string> item;
        vector<string> charac;
    public:

        Location() {} 
        Location(string n, string d, map<string,string> exitString,vector<string> itms, vector<string> ch)
        {
            name = n;
            desc = d;
            exits = exitString;
            item = itms;
            charac  = ch;
        }

        string GetName(){return name;}
        string GetDesc(){return desc;}
        map<string, string>& GetExits(){return exits;} 
        vector<string>& GetItem(){return item;}
        vector<string>& GetCharac(){return charac;}
};

class Items
{
    private:
        string name,desc,effect;
        string takeable,score_val;
    
    public:
        Items(){}
        Items(string n,string d,string t,string e ,string sc)
        {
            name = n;
            desc = d;
            takeable = t;
            effect = e;
            score_val = sc;
        }
        string GetName(){return name;}
        string GetDesc(){return desc;}
        string GetTakeable(){return takeable;}
        string GetEffect(){return effect;}
        string GetScore(){return score_val;}
};

class Characters
{
    private:
    string room,name,desc,hostile,message;

    public:
    Characters(){}
    Characters(string r, string n, string d, string h, string m)
    {
        room = r;
        name = n;
        desc = d;
        hostile = h;
        message = m;
    }   

    string getRoom(){return room;}
    string getName(){return name;}
    string getDesc(){return desc;}
    string getHostile(){return hostile;}
    string getMessage(){return message;}
};

class Inventory
{
    private:
        vector<string> items;

    public:
        bool has(const string &target, string &found)
        {
            return findItemCaseInsensitive(items, target, found);
        }

        void add(const string &name){ items.push_back(name); }

        void remove(const string &name)
        {
            auto it = find(items.begin(), items.end(), name);
            if (it != items.end())
                items.erase(it);
        }

        bool empty(){ return items.empty(); }

        void print()
        {
            if (items.empty())
                cout << "You are not carrying anything." << endl;
            else
            {
                cout << "You are carrying:";
                for (auto &itm : items)
                    cout << " " << itm;
                cout << endl;
            }
        }
};

class Control
{
    public:
        struct Command
        {
            string verb, target;
        };

        Command parse(const string &input)
        {
            Command c;
            stringstream input_ss(input);
            input_ss >> c.verb;
            getline(input_ss, c.target);
            c.target = trim(c.target);
            c.verb = toLower(c.verb);
            return c;
        }
};

class Actions
{
    public:
        void pick(map<string,Location> &WorldMap, map<string,Items> &WorldItem, string curr_room, Inventory &inventory, bool &hasJamTart, int &current_score, const string &target)
        {
            auto &roomItems = WorldMap[curr_room].GetItem(); 
            string foundRoomItem;
            bool inRoom = findItemCaseInsensitive(roomItems, target, foundRoomItem);
            string foundInvItem;
            bool inInv = inventory.has(target, foundInvItem);

            if(inRoom) 
            {
                if(!inInv) 
                {
                    auto it_room = find(roomItems.begin(),roomItems.end(),foundRoomItem);
                    inventory.add(foundRoomItem); 
                    roomItems.erase(it_room); 
                    
                    int pts = 0;
                    try { pts = stoi(WorldItem[toLower(foundRoomItem)].GetScore()); } catch(...) {}
                    current_score += pts;
                    cout << "You picked up " << foundRoomItem << ". (+" << pts << " points)\n";
                    
                    if(toLower(foundRoomItem) == "jam tart") hasJamTart = true; 
                }
                else{ cout<<"You already have '"<<foundRoomItem<<"' in your inventory"<<endl; }
            }
            else{ cout<<"Item not present in the room"<<endl; }
        }

        void drop(map<string,Location> &WorldMap, map<string,Items> &WorldItem, string curr_room, Inventory &inventory, int &current_score, const string &target)
        {
            auto &roomItems = WorldMap[curr_room].GetItem(); 
            string foundInvItem;
            bool inInv = inventory.has(target, foundInvItem);

            if(inInv)
            {
                inventory.remove(foundInvItem); 
                roomItems.push_back(foundInvItem); 
                
                int pts = 0;
                try { pts = stoi(WorldItem[toLower(foundInvItem)].GetScore()); } catch(...) {}
                current_score -= pts;
                cout << "You dropped " << foundInvItem << ". (-" << pts << " points)\n";
            }
            else{cout<<"Item: '"<<target<<"' not present in inventory"<<endl;}
        }

        void use(map<string,Items> &WorldItem, map<string,Characters> &WorldChar, map<string,Location> &WorldMap,
                 string curr_room, Inventory &inventory, string &char_size, bool &life, bool &hostileDefeated, const string &target)
        {
            string foundItem;
            if (!inventory.has(target, foundItem))
            {
                cout << "You're not carrying that." << endl;
                return;
            }

            string key = toLower(foundItem);
            if (WorldItem.find(key) == WorldItem.end())
            {
                cout << "You can't use " << foundItem << "." << endl;
                return;
            }

            Items &itemObj = WorldItem[key];
            string effect = toLower(itemObj.GetEffect());

            if (effect == "shrink")
            {
                if (char_size == "NORMAL")
                    char_size = "SMALL";
                else if (char_size == "BIG")
                    char_size = "NORMAL";
                else if (char_size == "SMALL")
                {
                    char_size = "DEATH";
                    life = false;
                }
                cout << "You used " << itemObj.GetName() << ". Your size is now " << char_size << ".\n";
            }
            else if (effect == "grow")
            {
                if (char_size == "NORMAL")
                    char_size = "BIG";
                else if (char_size == "SMALL")
                    char_size = "NORMAL";
                else if (char_size == "BIG")
                {
                    char_size = "DEATH";
                    life = false;
                }
                cout << "You used " << itemObj.GetName() << ". Your size is now " << char_size << ".\n";
            }
            else if (effect == "key")
            {
                cout << "Key can be used to open doors from The Hall of Door" << endl;
            }
            else if (effect == "weapon")
            {
                auto &roomCharsRef = WorldMap[curr_room].GetCharac();
                string hostileFound;
                bool foundHostile = false;
                for (auto &c : roomCharsRef)
                {
                    if (toLower(WorldChar[c].getHostile()) == "yes")
                    {
                        hostileFound = c;
                        foundHostile = true;
                        break;
                    }
                }
                
                if (foundHostile)
                {
                    hostileDefeated = true;
                    cout << "You brandish the " << itemObj.GetName() << " at " << hostileFound << "! They back away, stunned." << endl;
                }
                else
                {
                    cout << "There is no one here to use the weapon on." << endl;
                }
            }
            else
            {
                cout << itemObj.GetDesc() << "\n";
            }
        }

        void talk(map<string,Characters> &WorldChar, vector<string> &roomChars, int &mockTurtleVisits, bool hostileDefeated, const string &target)
        {
            string foundChar;
            bool present = findItemCaseInsensitive(roomChars,target,foundChar);

            if(!present)
            {
                cout<<"There is no one here by that name."<<endl;
            }
            else
            {
                cout<<foundChar<<":"<<WorldChar[foundChar].getMessage()<<endl;
                if(toLower(foundChar)== "mock turtle")
                {
                    mockTurtleVisits++;
                }

                if(toLower(WorldChar[foundChar].getHostile()) == "yes" && !hostileDefeated)
                {
                    cout << "You feel a chill of danger just being near them." << endl;
                }
            }
        }

        void prove(string curr_room, bool &hasProvedKnaveInnocent)
        {
            if(curr_room == "The Courtroom")
            {
                hasProvedKnaveInnocent = true;
                cout << "You present your evidence to the court. The Knave of Hearts is declared innocent!" << endl;
            }
            else
            {
                cout << "There is nothing to prove here." << endl;
            }
        }

        void move(map<string,Location> &WorldMap, map<string,Characters> &WorldChar, string &curr_room,
                  bool hostileDefeated, int &hostileMeetings, bool &life, 
                  int mockTurtleVisits, bool hasProvedKnaveInnocent, bool hasJamTart, const string &direction)
        {
            auto &exitsRef = WorldMap[curr_room].GetExits();
            if (exitsRef.find(direction) == exitsRef.end())
            {
                cout << "You cannot go that way." << endl;
                return;
            }

            string next_room = exitsRef[direction];

            if (next_room == "The Beautiful Garden")
            {
                if (mockTurtleVisits < 2 || !hasProvedKnaveInnocent || !hasJamTart)
                {
                    cout << "\nThe heavy iron gate to The Beautiful Garden is locked fast!" << endl;
                    cout << "You feel you haven't completed your quest in Wonderland yet." << endl;
                    cout << "(Hint: You must carry the Jam Tart, speak with the Mock Turtle twice, and prove the Knave innocent first.)" << endl;
                    return; 
                }
            }

            curr_room = next_room;

            if (!hostileDefeated)
            {
                for (auto &c : WorldMap[curr_room].GetCharac()) 
                {
                    if (toLower(WorldChar[c].getHostile()) == "yes") 
                    {
                        hostileMeetings++; 
                        cout << "\nYou come face to face with " << c << "! (encounter " << hostileMeetings << "/2)" << endl;
                        if (hostileMeetings >= 2)
                        {
                            life = false;
                            cout << c << " has had enough of you. OFF WITH YOUR HEAD!" << endl;
                        }
                        break;
                    }
                }
            }
        }
};

class Game
{
    private:
        string char_size = "NORMAL";
        bool life = true;
        string curr_room = "Halls of Door";
        int mockTurtleVisits = 0; 
        bool hasProvedKnaveInnocent = false; 
        bool hasJamTart = false; 
        int hostileMeetings = 0; 
        bool hostileDefeated = false;
        int current_score = 0;

        map<string,Location> WorldMap; 
        map<string,Items> WorldItem; 
        map<string,Characters> WorldChar; 

        Inventory inventory;
        Control control;
        Actions actions;

        bool loadLocations();
        bool loadItems();
        bool loadCharacters();

    public:
        bool loadAll();
        void run();
};

bool Game::loadLocations()
{
    ifstream ifs1("location.txt");

    if(!ifs1){cout<<"Error: location.txt does not exist."<<endl; return false;}

    string line1;
    while(getline(ifs1,line1)) 
    {
        if (trim(line1).empty()) continue; 

        map<string,string> exit_u; 
        vector<string> itms; 
        vector<string> characterss;

        stringstream ss1(line1); 
        string room_n,room_d,exits_str,item;
        getline(ss1,room_n,'|');
        getline(ss1,room_d,'|');
        getline(ss1,exits_str,'|');
        getline(ss1,item);
        room_n = trim(room_n); 
        room_d = trim(room_d);

        stringstream items1(item);
        stringstream exits1(exits_str);
        string key,value;
        while(getline(exits1,key,',')) 
        {   
            stringstream pair(key);
            getline(pair,key,':');
            getline(pair,value);
            key = toLower(trim(key));   
            value = trim(value);
            exit_u[key]= value; 
        }     

        while(getline(items1,item,','))
        {
            if (!trim(item).empty())
                itms.push_back(trim(item));
        }
        Location loc(room_n,room_d,exit_u,itms,characterss); 
        WorldMap[room_n] = loc; 
    }
    return true;
}

bool Game::loadItems()
{
    ifstream ifs2("items.txt");
    if(!ifs2){cout<<"Error: items.txt does not exist."<<endl; return false;}

    map<string, vector<string>> roomItems; 
    string line2;
    while(getline(ifs2,line2)) 
    {
        if (trim(line2).empty()) continue; 

        stringstream ss1(line2);
        string room_name,item_name,desc,takeable,effect,score_val;
        getline(ss1, room_name, '|');
        getline(ss1, item_name, '|');
        getline(ss1, desc, '|');
        getline(ss1, takeable, '|');
        getline(ss1, effect, '|');
        getline(ss1, score_val);

        string itemKey = toLower(trim(item_name));
        Items itm(trim(item_name), trim(desc), trim(takeable), trim(effect), trim(score_val));
        WorldItem[itemKey] = itm;
        roomItems[trim(room_name)].push_back(trim(item_name));
    }

    for (auto &pair : roomItems) 
    {
        if (WorldMap.find(pair.first) != WorldMap.end())
            WorldMap[pair.first].GetItem() = pair.second;
    }
    return true;
}

bool Game::loadCharacters()
{
    ifstream ifs3("characters.txt");
   
    if(!ifs3){cout<<"Error: characters.txt does not exist."<<endl; return false;}

    map<string,vector<string>> characters; 
    string line3;
    while(getline(ifs3,line3))
    {
        if (trim(line3).empty()) continue; 

        stringstream ss(line3);
        string rname,cname,description,hostile,dialogue;
        getline(ss,rname,'|');
        getline(ss,cname,'|');
        getline(ss, description,'|');
        getline(ss,hostile,'|');
        getline(ss, dialogue);

        Characters charac(trim(rname),trim(cname),trim(description),trim(hostile),trim(dialogue));
        WorldChar[trim(cname)] = charac; 
        characters[trim(rname)].push_back(trim(cname)); 
    }

    for(auto &pair : characters) 
    {
        if(WorldMap.find(pair.first)!= WorldMap.end()) 
        {
            WorldMap[pair.first].GetCharac() = pair.second;
        } 
    }
    return true;
}

bool Game::loadAll()
{
    bool ok = true;
    ok &= loadLocations();
    ok &= loadItems();
    ok &= loadCharacters();
    return ok;
}

void Game::run()
{
    cout << "=======================================================================" << endl;
    cout << "                    ALICE IN WONDERLAND ADVENTURE                      " << endl;
    cout << "=======================================================================" << endl;
    cout << "You have fallen down the rabbit hole into a world of wonder and danger!" << endl;
    cout << "To win the game and escape, you must complete ALL THREE tasks before" << endl;
    cout << "entering 'The Beautiful Garden':" << endl;
    cout << endl;
    cout << "  [WIN CONDITIONS]" << endl;
    cout << "   1. Pick up and carry the 'Jam Tart' in your inventory." << endl;
    cout << "   2. Use 'TALK Mock Turtle' at least twice." << endl;
    cout << "   3. Go to 'The Courtroom' and type 'PROVE' to declare the Knave innocent." << endl;
    cout << "   4. Navigate to 'The Beautiful Garden' once all 3 tasks are complete." << endl;
    cout << endl;
    cout << "  [LOSS CONDITIONS]" << endl;
    cout << "   - SIZE DEATH: Shrinking below SMALL or growing above BIG." << endl;
    cout << "   - HOSTILE ENCOUNTER: Entering rooms with hostile characters (like the" << endl;
    cout << "     Queen of Hearts) 2 times without using a weapon to neutralize them." << endl;
    cout << endl;
    cout << "  [COMMANDS]" << endl;
    cout << "   - Movement  : N, S, E, W (or GO N, GO S, etc.)" << endl;
    cout << "   - Interaction: PICK <item>, DROP <item>, USE <item>, TALK <character>" << endl;
    cout << "   - Actions    : PROVE, LOOK, INVENTORY, SCORE, QUIT" << endl;
    cout << "=======================================================================" << endl;

    while(life)
    {
        cout<<"\n";
        cout<<"Room Name: "<<WorldMap[curr_room].GetName()<<" | Current Score: "<<current_score<<endl;
        cout<<"Room Description: "<<WorldMap[curr_room].GetDesc()<<endl;
        cout<<endl;
        auto exits = WorldMap[curr_room].GetExits();  
        cout<<"Direction : Room Name"<<endl;
        for(const auto& [dir,dest] :exits) 
        {
            cout<<dir<<" : "<<dest<<endl;
        }
        cout<<endl;

        auto &roomItems = WorldMap[curr_room].GetItem();
        if(!roomItems.empty())
        {
            cout<<"Items in room: "<<endl;
            for(size_t i=0;i<roomItems.size();i++)
            {
                cout<<"Item Name: "<<roomItems[i]<<endl;
                cout<<"Item Description: "<<WorldItem[toLower(roomItems[i])].GetDesc()<<endl;
                cout<<endl;
            }   
        }   
        cout<<endl;

        auto &roomChars = WorldMap[curr_room].GetCharac();
        if(!roomChars.empty())
        {
            cout<<"Characters in room: "<<endl;
            for(size_t i=0;i<roomChars.size();i++)
            {
                cout<<"Character Name: "<<roomChars[i]<<endl;
                cout<<"Character Description: "<<WorldChar[roomChars[i]].getDesc()<<endl;
                cout<<endl;
            }
        }
        cout << "\nEnter command (N/S/E/W, GO <dir>, PICK <item>, DROP <item>, USE <item>, TALK <char>, PROVE, LOOK, INVENTORY, SCORE, QUIT): ";
        string input;
        if (!getline(cin, input))
            break; 
        input = trim(input);
        if (input.empty())
            continue; 

        Control::Command cmd = control.parse(input);
        string command_lc = cmd.verb;
        string target = cmd.target;

        if (command_lc == "quit" || command_lc == "exit")
        {
            cout << "Thanks for playing." << endl;
            break;
        }

        if (command_lc == "look")
        {
            continue;
        }

        if (command_lc == "inventory")
        {
            inventory.print();
            continue;
        }
        
        if (command_lc == "score")
        {
            cout << "Your current score is: " << current_score << endl;
            continue;
        }

        string direction = command_lc;
        if (direction == "go" && !target.empty())
        {
            string first;
            stringstream tgt_ss(target);
            tgt_ss >> first;
            direction = toLower(first);
        }

        if (direction == "n" || direction == "s" || direction == "e" || direction == "w")
        {
            actions.move(WorldMap, WorldChar, curr_room, hostileDefeated, hostileMeetings, life, 
                         mockTurtleVisits, hasProvedKnaveInnocent, hasJamTart, direction);
        }
        else if(command_lc == "pick")
        {
            actions.pick(WorldMap, WorldItem, curr_room, inventory, hasJamTart, current_score, target);
        }
        else if(command_lc == "drop")
        {
            actions.drop(WorldMap, WorldItem, curr_room, inventory, current_score, target);
        }
        else if(command_lc == "use")
        {
            actions.use(WorldItem, WorldChar, WorldMap, curr_room, inventory, char_size, life, hostileDefeated, target);
        }
        else if(command_lc == "talk")
        {
            actions.talk(WorldChar, roomChars, mockTurtleVisits, hostileDefeated, target);
        }
        else if(command_lc == "prove")
        {
            actions.prove(curr_room, hasProvedKnaveInnocent);
        }
    
        else 
        {
            cout << "Unknown command. Try N, S, E, W, PICK, DROP, USE, TALK, PROVE, INVENTORY, SCORE, or QUIT." << endl;
        }

        if (curr_room == "The Beautiful Garden" && mockTurtleVisits>=2 && hasProvedKnaveInnocent && hasJamTart) 
        {
            cout << "\n============================================\n";
            cout << "You have completed the quest." << endl;
            cout << "You met Mock Turtle twice, proved the Knave was innocent, and found the Jam Tart." << endl;
            cout << "The garden opens to you. You win!" << endl;
            cout << "Final Score: " << current_score << endl;
            cout << "============================================\n";
            break;
        }
    }

    if (!life)
    {
        cout << "\n============================================\n";
        cout << "Game Over. Better luck next time!" << endl;
        cout << "Final Score: " << current_score << endl;
        cout << "============================================\n";
    }
}

int main()
{
    Game game;
   
    if (game.loadAll()) 
    {
        game.run();
    }
    else
    {
        cout << "Failed to load game data files. Exiting." << endl;
    }
    return 0;
}