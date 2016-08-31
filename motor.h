#pragma once

#include "ev3dev.h"

using namespace ev3dev;

class motors{
private:
	large_motor leftmotor = large_motor(OUTPUT_A);
	large_motor rightmotor = large_motor(OUTPUT_D);
public:
	unsigned long Get_Left();
	unsigned long Get_Right();

	void LeftMove(int Speed, int Degress);
	void RightMove(int Speed, int Degress);
	void LeftReSet();
	void RightReSet();

	~motors(){
	}
};

//왼쪽 엔코더값 불러오기
unsigned long motors::Get_Left(){
	return (unsigned long)leftmotor.position();
}

//오른쪽 엔코더값 불러오기
unsigned long motors::Get_Right(){
	return (unsigned long)rightmotor.position();
}

//왼쪽 모터 움직이기 엔코더값 움직이기
void motors::LeftMove(int Speed, int Degress)
{
	while (!leftmotor.connected());
	LeftReSet();
	leftmotor.set_duty_cycle_sp(Speed);
	leftmotor.run_forever();
	while (true)
	{
		if (Degress >= 0 && Degress <= leftmotor.position()) break;
		if (Degress <= 0 && Degress >= leftmotor.position()) break;
	}
	leftmotor.set_duty_cycle_sp(Speed*(-1));
	leftmotor.set_duty_cycle_sp(0);
}

//오른쪽 모터 움직이기 엔코더값
void motors::RightMove(int Speed, int Degress)
{
	while (!rightmotor.connected());
	RightReSet();
	rightmotor.set_duty_cycle_sp(Speed);
	rightmotor.run_forever();
	while (true)
	{
		if (Degress >= 0 && Degress <= rightmotor.position()) break;
		if (Degress <= 0 && Degress >= rightmotor.position()) break;
	}
	rightmotor.set_duty_cycle_sp(Speed*(-1));
	rightmotor.set_duty_cycle_sp(0);
}

void motors::LeftReSet()
{
	leftmotor.reset();
}

void motors::RightReSet()
{
	rightmotor.reset();
}