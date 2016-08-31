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
	
	//�������� �����
	void *Beep(void*uid)
	{
		sound::tone(3200, 350, true);
		pthread_exit((void*)uid);
	}

	//��� ���� ��ȣ
	void *PushBeep(void*uid)
	{
		sound::tone(4000, 1200, true);
		pthread_exit((void*)uid);
	}

	//���� ��ȣ
	void *EndBeep(void *uid)
	{
		sound::tone(2000, 1200, true);
		pthread_exit((void*)uid);
	}

	//Ÿ�̸� �����
	void *EndBP(void *uid)
	{
		sound::tone(3400, 1100, true);
		pthread_exit((void*)uid);
	}

	//���������� ���
	void *pushwait(void*uid)
	{
		Touch *dtoch = new Touch();
		//���� ������ Ÿ��
		unsigned long times = 0, times2 = 0;
		pthread_t tp1, tp2;
		system("clear");
		do{
			//������ ���������� ����!
			if (pushright == false && dtoch->get_right()) 
			{ 
				pushright = true;
				pthread_create(&tp1, NULL, &Beep, (void*)2);
			}
			//���� ���������� ����
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
	
	//������ ����
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
	�̹� ������ ����������� ������ �Ұ�����
	������ ���۵Ǳ����� ��ҹ�ư�� ������ ��Ұ�����
	���� �� �ʱ�����̶� ����Ƽ�� ���Ͽ� ��� ����
	���ϰ� ���������Ƿ� ���� ���װ� ����
	*/
	void Start()
	{
		//���� �ݺ�
		do
		{
			//�غ���� ��ư������
			while (!button::enter.pressed()) {
				//��� �Ǵ� ����
				if (button::back.pressed())
				{
					Exit = true;
					break;
				}
			}
			while (button::enter.pressed());
			sound::tone(1600, 250, true);
			//���� ���� ����
			motors *mtr = new motors();
			if (lefttime < righttime) mtr->RightMove(50, 60);
			else if (lefttime > righttime) mtr->LeftMove(50, 60);
			pushleft = pushright = false;
			lefttime = righttime = 0;
			prt = 0;
			//����������
			prt = GotRandom(5000, 3000);
			float laptime = (prt / 1000);
			//��ư ������ ��� ����
			pthread_t push;
			pthread_create(&push, NULL, &pushwait, (void*)1);
			//���ð� ���� ��� ����
			Waitr(true);
			//�Ѵ� �̸��ȴ������� ���������� �Ҹ�����
			pthread_t shoot, shootstart;
			if (pushleft != true || pushright != true)
			{
				pthread_create(&shoot, NULL, &ReadyMusic, (void*)4);
				//����غ� && �߰� ���
				prt = GotRandom(500, 200);
				Waitr(false);
				//��� ��ȣ�ְ�
				if (pushleft != true || pushright != true)
				{
					pthread_t stt;
					pthread_create(&stt, NULL, &PushBeep, (void*)11);
					//�����Ѱ� ���� ���� �ڽ��� �����ӵ� 
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