#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <climits>

using namespace std;

template<typename T>
class CallBack {
public:
    void (T::*function)(T*);

    explicit CallBack(void (T::*func)(T*)) {
        function = func;
    }
};


class Character {
protected:
    int hp;
    int mp;
    int attack;
    int defence;
    int magicAttack;
    vector<CallBack<Character>> actions;

    virtual void actionAttack(Character* other) {
        cout << "you choose attack" << endl;
    }

    virtual void actionDefence(Character* other) {
        cout << "you choose defence" << endl;
    }

    virtual void actionSkill(Character* other) {
        cout << "you choose skill" << endl;
    }
public:
    Character(int hp, int mp, int atk, int dfc, int mAtk){
        this->hp = hp;
        this->mp = mp;
        this->attack = atk;
        this->defence = dfc;
        this->magicAttack = mAtk;

        CallBack<Character> action1(&Character::actionAttack);
        CallBack<Character> action2(&Character::actionDefence);
        CallBack<Character> action3(&Character::actionSkill);
        actions.push_back(action1);
        actions.push_back(action2);
        actions.push_back(action3);
    }
    ~Character()= default;

    virtual void showInfo() {
        cout << "hp\t\tmp\t\tattack\t\tdefence\t\tmagic attack" << endl;
        cout << hp << "\t\t" << mp << "\t\t" << attack << "\t\t\t" << defence << "\t\t\t" << magicAttack << endl;
    }

    // function doAction is very important and will take the desired action for user.
    virtual void doAction(int action, Character* other) {
        // This command will call function among the above three action function
        (this->*(actions[action - 1].function))(other);
    }

    virtual bool isAlive() {
        return hp > 0;
    }

    void beDamaged(int damage) {
        this->hp -= damage;
    }
};


class Player : public Character {
protected:
    void actionAttack(Character* other) override {
        cout << "player attack" << endl;
        other->beDamaged(attack);
    }

    void actionDefence(Character* other) override {
        cout << "player defence" << endl;
    }

    void actionSkill(Character* other) override {
        cout << "player skill" << endl;
    }
public:
    Player(int hp, int mp, int atk, int dfc, int mAtk) : Character(hp, mp, atk, dfc, mAtk){}
    ~Player()= default;

//    void showInfo() override {
//        cout << "hp\t\tmp\t\tattack\t\tdefence\t\tmagic attack" << endl;
//        cout << hp << "\t\t" << mp << "\t\t" << attack << "\t\t\t" << defence << "\t\t\t" << magicAttack << endl;
//        // You can apply some information about the player skill
//    }
};


class Monster : public Character {
protected:
    string name;

    void actionAttack(Character* other) override {
        cout << "Monster attack" << endl;
        other->beDamaged(attack);
    }

    void actionDefence(Character* other) override {
        cout << "Monster defence" << endl;
    }

    void actionSkill(Character* other) override {
        cout << "Monster skill" << endl;
    }
public:
    Monster(int hp, int mp, int atk, int dfc, int mAtk) : Character(hp, mp, atk, dfc, mAtk) {
        srand(time(NULL));
    }
    ~Monster()= default;

    virtual void selectAction(Character* other) {
        cout << "Monster choose an action" << endl;
        int action = rand() % 3 + 1;
        cout << action << endl;
        this->doAction(action, other);
        cout << "bbbb" << endl;
    }
};


class Game {
private:
    Player* player;
    vector<Character*> monsters;

    const vector<string> status = {"Game Over", "Playing"};

    void showStartInfo() {
        cout << "Game start!!" << endl;
        cout << "Welcome to Monster Killer!" << endl;
        cout << "You are a warrior who take the mission to kill the monster" << endl;
        cout << "There are some powerful monsters. You need to try your best to kill them!!" << endl;
        cout << "Here is your information detail" << endl;
        player->showInfo();
    }

    int chooseBattleAction() {
        int action = INT_MAX;
        while (action < 1 || action > 3) {
            if (action != INT_MAX) {
                cout << "error action!" << endl;
            }
            cout << "Select your action:" << endl
                 << "1. attack" << endl
                 << "2. defence" << endl
                 << "3. skill" << endl;
            string tem;
            cin >> tem;
            if (tem.size() == 1 && isdigit(tem[0])) {
                action = tem[0] - '0';
            } else {
                action = -1;
            }
        }
        return action;
    }

    bool isMonstersAllDie() {
        for (auto & monster : monsters) {
            if (monster->isAlive()) {
                return false;
            }
        }
        return true;
    }

    void showMonsterInfo(int monsterId) {
        cout << "The current monster Information" << endl;
        monsters[monsterId]->showInfo();
    }

    void showResult() {
        if (player->isAlive()) {
            // Player win the game
            cout << "Congratulation! You Win!" << endl;
        } else {
            // Player lose the game
            cout << "You die! Game Over!" << endl;
        }
    }
public:
    // Constructor
    Game() {
        // Create player
        player = new Player(100, 50, 50, 50, 70);

        // Create monster
        monsters.push_back(new Monster(100, 100, 100, 100, 100));
//        monsters.push_back(new Monster(100, 100, 100, 100, 100));
//        monsters.push_back(new Monster(100, 100, 100, 100, 100));
    }

    // Check the current game status
    string gameState() {
        if (player->isAlive() && !isMonstersAllDie()) {
            return status[1];
        } else {
            return status[0];
        }
    }

    void start() {
        showStartInfo();
        int monsterId = 0;

        while (gameState() == status[1]) {
            Character* currentMonster = this->monsters[monsterId];

            while (player->isAlive() && currentMonster->isAlive()) {
                cout << "-------------------------------" << endl;
                showMonsterInfo(monsterId);

                // Player's turn
                cout << "your turn!" << endl
                     << "player status:" << endl;
                player->showInfo();
                int action = chooseBattleAction();
                player->doAction(action, currentMonster);

                // Monster's turn
                if (currentMonster->isAlive()) {
                    cout << "Monster's turn!" << endl;
                    ((Monster *) currentMonster)->selectAction(player);
                }
            }

            monsterId++;
        }

        showResult();
    }
};


int main() {
    Game monsterKiller;
    monsterKiller.start();
    return 0;
}
