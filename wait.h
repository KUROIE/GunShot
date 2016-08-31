#include "motor.h"
#include "sensor.h"
#include <pthread.h>
#include <thread>

using namespace std;

namespace GameWait{

	int prt = 0;
	bool pushleft = false, pushright = false;

	void* ReadyMusic(void *uid)
	{
		system("clear");
		ev3dev::sound::play("/home/robot/suck.wav", false);
		pthread_exit((void*)uid);
	}


	float lefttime = 0, righttime = 0;
	
	//눌렀을때 삑삑이
	void *Beep(void*uid)
	{
		sound::tone(3200, 350, true);
		pthread_exit((void*)uid);
	}

	//쏘기 시작 신호
	void *PushBeep(void*uid)
	{
		sound::tone(4000, 1200, true);
		pthread_exit((void*)uid);
	}

	//종료 신호
	void *EndBeep(void *uid)
	{
		sound::tone(2000, 1200, true);
		pthread_exit((void*)uid);
	}

	//타이머 삑삑이
	void *EndBP(void *uid)
	{
		sound::tone(3400, 1100, true);
		pthread_exit((void*)uid);
	}

	//누를떄까지 대기
	void *pushwait(void*uid)
	{
		Touch *dtoch = new Touch();
		//왼쪽 오른쪽 타임
		unsigned long times = 0, times2 = 0;
		pthread_t tp1, tp2;
		system("clear");
		do{
			//오른쪽 눌렀을때의 반응!
			if (pushright == false && dtoch->get_right()) 
			{ 
				pushright = true;
				pthread_create(&tp1, NULL, &Beep, (void*)2);
			}
			//왼쪽 눌렀을때의 반응
			if (pushleft == false && dtoch->get_left())
			{ 
				pushleft = true;
				pthread_create(&tp2, NULL, &Beep, (void*)3);
			}
			this_thread::sleep_for(chrono::milliseconds(1));
			if (pushleft != true) times++;
			if (pushright != true) times2++;
			if (pushleft == true && pushright == true) break;
		} while (true);
		lefttime = times / 1000;
		righttime = times2 / 1000;
		pthread_exit((void*)uid);
	}
	
	//랜덤값 생성
	int GotRandom(int MAX = 100, int MIN = 0)
	{
		int RT = 0;
		while (true)
		{
			RT = rand() % MAX + MIN;
			if (RT >= MIN && RT <= MAX) break;
		}
		return RT;
	}

	void Waitr(bool bp)
	{
		unsigned long ready = 0;
		motors *mtr = new motors();
		while (true)
		{
			if (ready >= (prt)) break;
			if (bp == true && ready % 500 == 0) {
				pthread_t ebp;
				pthread_create(&ebp, NULL, &EndBP, (void*)10);
			}
			this_thread::sleep_for(chrono::milliseconds(1));
			ready++;
			if (pushleft == true)
			{
				mtr->LeftMove(-50, -60);
				break;
			}
			if (pushright == true)
			{
				mtr->RightMove(-50, -60);
				break;
			}
		}
	}

	bool Exit = false;

	/*
	이미 게임을 시작했을경우 정지는 불가능함
	게임이 시작되기전에 취소버튼을 누를시 취소가능함
	아직 맨 초기버전이라 퀄리티가 최하에 기능 부족
	급하게 제작했으므로 아직 버그가 많음
	*/
	void Start()
	{
		//무한 반복
		do
		{
			//준비됨의 버튼누르기
			while (!button::enter.pressed()) {
				//취소 또는 정지
				if (button::back.pressed())
				{
					Exit = true;
					break;
				}
			}
			while (button::enter.pressed());
			sound::tone(1600, 250, true);
			//현재 상태 복구
			motors *mtr = new motors();
			if (lefttime < righttime) mtr->RightMove(50, 60);
			else if (lefttime > righttime) mtr->LeftMove(50, 60);
			pushleft = pushright = false;
			lefttime = righttime = 0;
			prt = 0;
			//랜덤값생성
			prt = GotRandom(5000, 3000);
			float laptime = (prt / 1000);
			//버튼 누르기 대기 시작
			pthread_t push;
			pthread_create(&push, NULL, &pushwait, (void*)1);
			//대기시간 동안 대기 시작
			Waitr(true);
			//둘다 미리안눌렀을떄 석양이진다 소리내고
			pthread_t shoot, shootstart;
			if (pushleft != true || pushright != true)
			{
				pthread_create(&shoot, NULL, &ReadyMusic, (void*)4);
				//쏘기준비 && 추가 대기
				prt = GotRandom(500, 200);
				Waitr(false);
				//쏘기 신호주고
				if (pushleft != true || pushright != true)
				{
					pthread_t stt;
					pthread_create(&stt, NULL, &PushBeep, (void*)11);
					//측정한값 빼고 현재 자신이 누른속도 
					motors *mtr = new motors();
					while ((!pushleft) || (!pushright));
					if (lefttime < righttime)
					{
						mtr->RightMove(-50, -60);
						printf("%.2f", lefttime - laptime);
					}
					if (lefttime > righttime)
					{
						mtr->LeftMove(-50, -60);
						printf("%.2f", righttime - laptime);
					}
				}
			}
		} while (!Exit);
	}
}