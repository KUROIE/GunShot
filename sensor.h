#pragma once

#include "ev3dev.h"

using namespace ev3dev;

class Touch{
private:
	touch_sensor *T1 = new touch_sensor(INPUT_1);
	touch_sensor *T2 = new touch_sensor(INPUT_4);
public:
	int get_left();
	int get_right();
};

int Touch::get_left()
{
	return T1->value();
}

int Touch::get_right()
{
	return T2->value();
}