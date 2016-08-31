#include "motor.h"
#include "wait.h"

int main()
{
	motors *mtr = new motors();
	mtr->LeftMove(50, 60);
	mtr->RightMove(50, 60);
	GameWait::Start();
	return 0;
}