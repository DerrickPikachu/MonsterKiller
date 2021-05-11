#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <climits>

using namespace std;


// The declaration of the classes are all here
class Character;
class Player;
class Monster;
class Equipment;
class Game;
class Pikachu;
class ThunderSword;
class TADerrickChin;


/* The Method Pointer class with template.
 * This is the general form of method pointer(because of template)
 * The method type I used here is:
 * T functionName(T*) */
template<typename T>
class CallBack {
public:
    void (T::*function)(T*);

    explicit CallBack(void (T::*func)(T*)) {
        function = func;
    }
};


/* The base class of all the other class
 * This class is very important for this project(or game)
 * You don't need to derived this class(except for class Player)
 * I have defined another class that is more useful and easy to derived*/
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

    void damaged(int damage) {
        roundDamage += damage;
    }

    void defended(int defend) {
        roundDefend += defend;
    }

    void enhanceHp(int hp) {
        this->hp += hp;
    }

    void enhanceMp(int mp) {
        this->mp += mp;
    }

    void enhanceAtk(int atk) {
        this->attack += atk;
    }

    void enhanceDfc(int dfc) {
        this->defence += dfc;
    }

    void enhanceMAtk(int mAtk) {
        this->magicAttack += mAtk;
    }

    void enhanceSkCost(int skCost) {
        this->skillCost += skCost;
    }

    virtual void showInfo() {
        cout << "hp\t\tmp\t\tattack\t\tdefence\t\tmagic attack\t\tskill cost" << endl;
        cout << hp << "\t\t" << mp << "\t\t" << attack << "\t\t\t" << defence << "\t\t\t" << magicAttack << "\t\t\t" << skillCost << endl;
    }

    // function doAction is very important and will take the desired action for user.
    virtual void doAction(int action, Character* other) {
        // This command will call function among the above three action function
        (this->*(actions[action - 1].function))(other);
    }

    virtual bool isAlive() {
        return hp > 0;
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

/* Here is the class which derived by the specified equipment
 * You can see the example in the sample.cpp
 * If you want to give the equipment to the player,
 * first you need to create a class derived from this class
 * and after defeating the monster you can use Player::equip to give the equipment to player*/
class Equipment {
public:
    virtual void equipEffect(Player* player, Monster* monster) {}
};


/* The Player class is for the user
 * you must complete the definition of this class
 * I have define some attribute and important method to you
 * If you don't really understand what the method will effect,
 * please don't modify the code which I define for you*/
class Player : public Character {
private:
    vector<Equipment*> playerEquip;

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

    /* This method is trying to iteratively launch the equipment effect
     * If you don't understand the code inside the method, please don't modify it*/
    void launchEquipmentEffect(Monster* monster) {
        for (auto& equip : playerEquip) {
            equip->equipEffect(this, monster);
        }
    }

    /* This method is used to equip the equipment*/
    void equip(Equipment* equipment) {
        playerEquip.push_back(equipment);
    }
};


/* Monster is the base class of all monsters
 * Every monster "must" derived from this class
 * and this is the most important part of this homework
 * as the result, you should clearly understand what is Inheritance and Polymorphism*/
class Monster : public Character {
protected:
    string name;
    Equipment* equipment;

    void actionAttack(Character* other) override {
        cout << "Monster attack" << endl;
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

    void showInfo() override {
        cout << "Monster Name: " << name << endl;
        cout << "hp\t\tmp\t\tattack\t\tdefence\t\tmagic attack\t\tskill cost" << endl;
        cout << hp << "\t\t" << mp << "\t\t" << attack << "\t\t\t" << defence << "\t\t\t" << magicAttack << "\t\t\t" << skillCost << endl;
    }

    virtual void selectAction(Character* other) {
        cout << "Monster choose an action" << endl;
        int validActions = this->isMpEnough()? 3 : 2;
        int action = rand() % validActions + 1;
        this->doAction(action, other);
    }

    virtual void reward(Player* player) {}
};


class ThunderSword : public Equipment {
public:
    void equipEffect(Player* player, Monster* monster) override {
        cout << "Thunder wrapping around the sword!" << endl;
        cout << "Player get more power!" << endl;
        cout << "atk + 5" << endl;
        player->enhanceAtk(5);
    }
};

class TestEquip : public Equipment {
public:
    void equipEffect(Player* player, Monster* monster) override {
        cout << "TA equipment!" << endl;
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

    void reward(Player* player) override {
        player->equip(equipment);
    }
};

class TADerrickChin : public Monster {
protected:
    void actionAttack(Character* other) override {
        cout << "TA choose attack" << endl;
        cout << "TA honor punch!!!!" << endl;
        int damage = (int)(attack * 1.5);
        other->damaged(damage);
        cout << "TA make " << damage << " damage" << endl;
    }

    void actionDefence(Character* other) override {
        cout << "TA choose defence" << endl;
        cout << "TA feel tired" << endl;
        int defend = (int)(defence * 0.5);
        this->defended(defend);
        cout << "TA get defence " << defend << endl;
    }

    void actionSkill(Character* other) override {
        cout << "TA choose to use skill" << endl;
        cout << "TA tell you that you will fail this course!!!" << endl;
        int damage = (int)(magicAttack * 2);
        other->damaged(damage);
        cout << "TA make " << damage << " damage" << endl;
    }
public:
    TADerrickChin(int hp, int mp, int atk, int dfc, int mAtk, int skCost) : Monster(hp, mp, atk, dfc, mAtk, skCost) {
        name = "TADerrickChin";
        equipment = new TestEquip;
    }

    void reward(Player* player) {
        player->equip(equipment);
    }
};

/* The Game is about controlling the game flow, monitoring the game state and providing the hole game experience
 * There is only one thing you should modify here, that is inside the constructor
 * You must create your monster here
 * Just mention above, DO NOT modify the game flow if you don't really understand what is the game going on*/
class Game {
private:
    Player* player;
    vector<Monster*> monsters;

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

    /* You must create your monster here*/
    // Constructor
    Game() {
        // Create player
        player = new Player(100, 50, 15, 30, 35, 25);

        // Create monster
        monsters.push_back(new Pikachu(50, 50, 20, 10, 30, 25));
        monsters.push_back(new TADerrickChin(200, 50, 30, 30, 30, 10));
//        monsters.push_back(new Monster(100, 100, 100, 100, 100, 100));
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
            Monster* currentMonster = this->monsters[monsterId];

            while (player->isAlive() && currentMonster->isAlive()) {
                cout << "-------------------------------" << endl;
                // Show monster and player information
                showMonsterInfo(monsterId);
                showPlayerInfo();

                // Player choose an action
                int action = chooseBattleAction();

                // Battle start, both character do their action
                cout << "Battle!!" << endl;
                cout << "Player action" << endl;
                player->doAction(action, currentMonster);
                cout << "-------------" << endl
                     << "Monster action" << endl;
                currentMonster->selectAction(player);
                cout << "-------------" << endl
                     << "equipment effect" << endl;
                player->launchEquipmentEffect(currentMonster);
                cout << "-------------" << endl;

                // Compute the round damage and result
                player->computeRoundResult();
                currentMonster->computeRoundResult();
            }

            if (player->isAlive() && !currentMonster->isAlive()) {
                cout << "You defeat the Monster" << endl;
                currentMonster->reward(player);
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
