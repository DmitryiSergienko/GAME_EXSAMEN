#pragma once
#ifndef TRAP_H
#define TRAP_H
#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

class Trap {
protected:
	string name;
	char icon;
	int damage;
	int numTrack;
	int color;

public:
	Trap(const std::string& name, char icon, int damage, int numTrack, int color)
		: name{ name }, icon{ icon }, damage{ damage }, numTrack{ numTrack }, color{ color } {}
	virtual ~Trap(){}
	virtual string getName() const = 0;
	virtual char getIcon() const = 0;
	virtual int getDamage() const = 0;
	virtual int getNumTrack() const = 0;
	virtual int getColor() const = 0;
};

#endif // TRAP_H

class fakeDollar : public Trap {
public:
	fakeDollar() : Trap("trapFakeDollar", 'S', 1, 15, 0x67) {}
	virtual string getName() const { return name; }
	virtual char getIcon() const { return icon;	}
	virtual int getDamage() const { return damage; }
	virtual int getNumTrack() const { return numTrack; }
	virtual int getColor() const { return color; }
};
class water : public Trap {
public:
	water() : Trap("water", 'W', 1, 15, 0x11) {}
	virtual string getName() const { return name; }
	virtual char getIcon() const { return icon; }
	virtual int getDamage() const { return damage; }
	virtual int getNumTrack() const { return numTrack; }
	virtual int getColor() const { return color; }
};
class mine : public Trap {
public:
	mine() : Trap("mine", 'M', 1, 25, 0x22) {}
	virtual string getName() const { return name; }
	virtual char getIcon() const { return icon; }
	virtual int getDamage() const { return damage; }
	virtual int getNumTrack() const { return numTrack; }
	virtual int getColor() const { return color; }
};
class bridge : public Trap {
public:
	bridge() : Trap("bridge", 'T', 1, 15, 0x77) {}
	virtual string getName() const { return name; }
	virtual char getIcon() const { return icon; }
	virtual int getDamage() const { return damage; }
	virtual int getNumTrack() const { return numTrack; }
	virtual int getColor() const { return color; }
};