#pragma once
#ifndef HUMAN_H
#define HUMAN_H
#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

class Human {
protected:
	string name;
	int HP;
	int damage;
	char avavtar;
	int color;

public:
	Human(string name, int HP, int damage, char avavtar, int color)
		: name{}, HP{ HP }, damage{ damage }, avavtar{ avavtar }, color{ color } {}
	virtual ~Human() {}
	virtual string getName() const = 0;
	virtual int getHP() const = 0;
	virtual int getDamage() const = 0;
	virtual char getAvatar() const = 0;
	virtual int getColor() const = 0;

	virtual void setHP(int HP) = 0;
};

#endif // HUMAN_H

class GG : public Human {
public:
	GG() : Human("GG", 10, 1, '@', 0xCC) {}
	virtual string getName() const { return name; }
	virtual int getHP() const { return HP; }
	virtual int getDamage() const {	return damage; }
	virtual char getAvatar() const { return avavtar; }
	virtual int getColor() const { return color; }

	virtual void setHP(int HP) { this->HP = HP;	}
};