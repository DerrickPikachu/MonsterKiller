#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <climits>

using namespace std;

class Character;
class Player;
class Monster;
class Equipment;
class Game;
class Pikachu;
class ThunderSword;


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
    int skillCost;
    int roundDamage;
    int roundDefend;
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
    Character(int hp, int mp, int atk, int dfc, int mAtk, int skCost){
        this->hp = hp;
        this->mp = mp;
        this->attack = atk;
        this->defence = dfc;
        this->magicAttack = mAtk;
        this->skillCost = skCost;
        this->roundDamage = 0;
        this->roundDefend = 0;

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

    void damaged(int damage) {
        roundDamage += damage;
    }

    void defended(int defend) {
        roundDefend += defend;
    }

    virtual void computeRoundResult() {
        int trueDamage = roundDamage - roundDefend;
        hp -= (trueDamage < 0)? 0 : trueDamage;
        roundDamage = 0;
        roundDefend = 0;
    }

    virtual bool isMpEnough() {
        return mp >= skillCost;
    }
};

class Equipment {
public:
    virtual void equipEffect(Player* player) {}
};

class Player : public Character {
private:
    vector<Equipment> playerEquip;

protected:
    void actionAttack(Character* other) override {
        cout << "player choose attack" << endl;
        other->damaged(attack);
        cout << "player make damage: " << attack << endl;
    }

    void actionDefence(Character* other) override {
        cout << "player choose defence" << endl;
        this->defended(defence);
        cout << "player get defence: " << defence << endl;
    }

    void actionSkill(Character* other) override {
        cout << "player choose to use skill" << endl;
        other->damaged(magicAttack);
        mp -= skillCost;
        cout << "player cost " << skillCost << " mp to use skill" << endl
             << "player make damage: " << magicAttack << endl;
    }

public:
    Player(int hp, int mp, int atk, int dfc, int mAtk, int skCost) : Character(hp, mp, atk, dfc, mAtk, skCost){}
    ~Player()= default;
    friend Equipment;

//    void showInfo() override {
//        cout << "hp\t\tmp\t\tattack\t\tdefence\t\tmagic attack" << endl;
//        cout << hp << "\t\t" << mp << "\t\t" << attack << "\t\t\t" << defence << "\t\t\t" << magicAttack << endl;
//        // You can apply some information about the player skill
//    }

    void launchEquipmentEffect() {
        for (auto& equip : playerEquip) {
            equip.equipEffect(this);
        }
    }

    void equip(Equipment* equipment) {
        playerEquip.push_back(*equipment);
    }
};


class Monster : public Character {
protected:
    string name;
    Equipment* equipment;

    void actionAttack(Character* other) override {
        cout << "Monster attack" << endl;
//        other->damaged(attack);
    }

    void actionDefence(Character* other) override {
        cout << "Monster defence" << endl;
    }

    void actionSkill(Character* other) override {
        cout << "Monster skill" << endl;
    }
public:
    Monster(int hp, int mp, int atk, int dfc, int mAtk, int skCost) : Character(hp, mp, atk, dfc, mAtk, skCost) {
        srand(time(NULL));
    }
    ~Monster()= default;

    virtual void selectAction(Character* other) {
        cout << "Monster choose an action" << endl;
        int validActions = this->isMpEnough()? 3 : 2;
        int action = rand() % validActions + 1;
        this->doAction(action, other);
    }
};


class ThunderSword : public Equipment {
public:
    void equipEffect(Player* player) override {
        cout << "Thunder wrapping around the sword!" << endl;
        cout << "Player get more power!" << endl;
        cout << "atk + 5" << endl;
    }
};


class Pikachu : public Monster {
protected:
    void actionAttack(Character* other) override {
        cout << "Pikachu choose attack" << endl;
        int damage = (int)(attack * 0.6 + magicAttack * 0.4);
        other->damaged(damage);
        cout << "Pikachu attack damage: " << damage << endl;
    }

    void actionDefence(Character* other) override {
        cout << "Pikachu choose defence" << endl;
        int defend = (int)(defence * 0.8 + hp * 0.2);
        this->defended(defend);
        cout << "Pikachu defence value: " << defend << endl;
    }

    void actionSkill(Character* other) override {
        cout << "Pikachu choose to use skill" << endl;
        cout << "Volt Attack!!" << endl;

        int damageToOpponent = (int)(magicAttack * 1.4);
        int damageToPikachu = (int)(attack * 0.3);
        other->damaged(damageToOpponent);
        this->damaged(damageToPikachu);
        mp -= skillCost;

        cout << "Pikachu use " << skillCost << " mp to use skill" << endl;
        cout << "damage to other: " << damageToOpponent << endl
             << "damage to Pikachu: " << damageToPikachu << endl;
    }

public:
    Pikachu(int hp, int mp, int atk, int dfc, int mAtk, int skCost) : Monster(hp, mp, atk, dfc, mAtk, skCost) {
        name = "Pikachu";
        equipment = new ThunderSword;
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

            // User input
            string tem;
            cin >> tem;

            // Input check
            if (tem.size() == 1 && isdigit(tem[0])) {
                action = tem[0] - '0';
            } else {
                action = -1;
            }

            // MP check
            if (!player->isMpEnough() && action == 3) {
                cout << "You don't have enough mp!" << endl;
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

    void showPlayerInfo() {
        cout << "Your information detail" << endl;
        player->showInfo();
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
        player = new Player(100, 50, 15, 30, 35, 25);

        // Create monster
        monsters.push_back(new Pikachu(100, 50, 20, 10, 50, 25));
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
                // Show monster and player information
                showMonsterInfo(monsterId);
                showPlayerInfo();

                // Player choose an action
                int action = chooseBattleAction();

                // Battle start, both character do their action
                cout << "Battle!!" << endl;
                player->doAction(action, currentMonster);
                ((Monster *)currentMonster)->selectAction(player);

                // Compute the round damage and result
                player->computeRoundResult();
                currentMonster->computeRoundResult();
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
