#include "windows.h"
#include "fstream"
#include "conio.h"
#include "time.h"
#include "stdio.h"
#include "iostream"
using namespace std;

#define sleepTime 150
/*
�ɿط�Χ����1-19��
����2-60��ż��� �� 
*/

int defaultX = 42;
int defaultY = 10;
int curScore = 0;
int bestScore = 0;
int moveCounter = 0;
bool STATE_UP = false;
bool STATE_DOWN = false;
bool STATE_LEFT = true;		//Ĭ���������� 
bool STATE_RIGHT = false;
bool STATE_ALIVE = true;
bool isEaten = true;
int level = 0; 
int scoreWeight = 1;
int sleepTimeWeight = 0;

ifstream getcache;
ofstream storecache;

int snakeLength = 3;
struct Snake
{
	int x;
	int y;
}snake[100],rand_P{1,1};

void console()  
{  
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// ��ȡ��׼����豸���  
	CONSOLE_SCREEN_BUFFER_INFO bInfo; // ���ڻ�������Ϣ  
	GetConsoleScreenBufferInfo(hOut, &bInfo );// ��ȡ���ڻ�������Ϣ  
	SetConsoleTitle("̰���� C��"); // ���ô��ڵı���  
	COORD size = {80, 20};//����С��Ĭ�ϴ�С��80��25��  
	SetConsoleScreenBufferSize(hOut,size); // �������û�������С*/  
	SMALL_RECT rc = {0,0, 100, 64}; //���ܴ��ڻ�������С  
	SetConsoleWindowInfo(hOut,true ,&rc);// ���ô��ڴ�С  
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; //��һ���������ٷֱȴ�С���ڶ��������Ƿ�ɼ���  
	SetConsoleCursorInfo(hOut, &cursor_info);//���ÿ���̨���  
}  

void gotoxy(int x,int y)//X��ʾ�����꣬Y��ʾ�����ꡣ  
{  
	HANDLE app;  
	COORD  pos;  
	pos.X=x;  
	pos.Y=y;  
	app=GetStdHandle(STD_OUTPUT_HANDLE);  
	SetConsoleCursorPosition(app,pos);  
}  

void Print_Info()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
	gotoxy(0,0);
	printf("����������������������������������������������������������������\n");
	for(int i = 0; i < 19; i++)
		printf("��\t\t\t\t\t\t\t      ��\n");
	printf("����������������������������������������������������������������\n\n");
	printf("|--------------------------------------------------------------|\n");
	gotoxy(0,23);
	printf("|");
	gotoxy(4,23);
	printf("�÷֣�%d",curScore);		//�÷ּ�� 
	gotoxy(23,23);
	printf("��ǰ״̬����ʼ");	//������� 
	gotoxy(63,23);
	printf("|");
	gotoxy(0,24);
	printf("|");
	gotoxy(0,25);
	printf("|");
	gotoxy(0,26);
	printf("|");
	gotoxy(4,25);
	printf("���ȣ�%d",snakeLength) ;
	gotoxy(23,26);
	printf("�ȼ���LV %d",level);
	gotoxy(23,25);
	printf("����λ��:(%d��%d)",rand_P.x,rand_P.y);
	gotoxy(4,24);
	printf("��ѣ�%d",bestScore);		//��Ѽ�� 
	gotoxy(23,24);
	printf("λ�ü��:(%d��%d)",snake[0].x,snake[0].y);	//�������
	gotoxy(63,24);
	printf("|");
	gotoxy(63,25);
	printf("|");
	gotoxy(63,26);
	printf("|");
	gotoxy(0,27);
	printf("|--------------------------------------------------------------|\n");

}

InitFootprint()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	for(int i = 0; i < snakeLength; i++)
	{
		gotoxy(snake[i].x,snake[i].y);
		printf("��");	
	}
}

void DrawFootprint()
{
	gotoxy(23,23);
	
	if(STATE_UP)
	{
		printf("״̬��أ�����");	//������� 
	}
	else if(STATE_DOWN)
	{
		printf("״̬��أ�����");	//�������
	}
	else if(STATE_LEFT)
	{
		printf("״̬��أ�����");	//�������
	}
	else if(STATE_RIGHT)
	{
		printf("״̬��أ�����");	//�������
	}
	
	if(curScore > bestScore)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY|FOREGROUND_GREEN);
		gotoxy(4,26);
		printf("�¼�¼!");	//λ�ü��
	}
	gotoxy(4,23);
	printf("�÷֣�%d",curScore);		//�÷ּ�� 
	gotoxy(4,24);
	printf("��ѣ�%d",curScore > bestScore? curScore:bestScore);		//��Ѽ�� 
	gotoxy(4,25);
	printf("���ȣ�%d",snakeLength) ;
	gotoxy(23,25);
	printf("����λ��:(%d��%d)",rand_P.x,rand_P.y);
	gotoxy(23,24);
	printf("λ�ü��:(%d��%d)\n",snake[0].x,snake[0].y);	//λ�ü��
	gotoxy(23,26);
	printf("�ȼ���LV %d",level);

	
	gotoxy(snake[snakeLength-1].x,snake[snakeLength-1].y);
	printf("  ");
	
	for(int i = snakeLength - 2; i >= 0; i--)
	{
		snake[i+1].x = snake[i].x;
		snake[i+1].y = snake[i].y;
	}
	if(STATE_UP)
	{
		snake[0].y = snake[0].y - 1;
	}
	else if(STATE_DOWN)
	{
		snake[0].y = snake[0].y + 1;
	}
	else if(STATE_LEFT)
	{
		snake[0].x = snake[0].x - 2;
	}
	else if(STATE_RIGHT)
	{
		snake[0].x = snake[0].x + 2;
	}
	gotoxy(snake[0].x,snake[0].y);
	printf("��");
}

bool CollisionCheck()
{
	if((snake[0].x == 2) && STATE_LEFT)
	{
		return false;
	}
	else if((snake[0].y == 1) && STATE_UP)
	{
		return false;
	}
	else if((snake[0].x == 60) && STATE_RIGHT)
	{
		return false;
	}
	else if((snake[0].y == 19) && STATE_DOWN)
	{
		return false;
	}
	else return true;
} 

void BlockIsEaten()
{
	//�Ե�������
	if(isEaten)
	{
	srand((unsigned)time(NULL));
	rand_P.x = (rand()%29 + 1)*2;
	rand_P.y = rand()%18 + 1;
	gotoxy(rand_P.x,rand_P.y);
	printf("��");
	isEaten = false;
	}
	
	if(!isEaten)
	{
		gotoxy(rand_P.x,rand_P.y);
		printf("��");	
	}
	
	if((snake[0].x == rand_P.x)&&(snake[0].y == rand_P.y))
	{
		snakeLength++;
		curScore += scoreWeight;
		isEaten = true; 
		gotoxy(0,0);
		printf("��");
	} 
		
}

void LevelUpCheck()
{
	if(curScore < 10)
	{
		level = 0;
	}
	else if(curScore < 50)
	{
		level = 1;
		scoreWeight = 5;
		sleepTimeWeight = 20;
	}
	else if(curScore < 100)
	{
		level = 2;
		scoreWeight = 10;
		sleepTimeWeight = 40;
	}
	else if(curScore < 200)
	{
		level = 3;
		scoreWeight = 20;
		sleepTimeWeight = 80;
	}
	else if(curScore < 400)
	{
		level = 4;
		scoreWeight = 30;
		sleepTimeWeight = 120;
	}
	else
	{
		level = 5;
		scoreWeight = 40;
		sleepTimeWeight += 1;
	}
}

void Game_Entrance()
{
	InitFootprint();
	while(STATE_ALIVE)
	{
		//��ײ��� 
		STATE_ALIVE = CollisionCheck();
		//ת���� 
		if(STATE_LEFT && (snake[0].x > 2))
		{
			moveCounter++;
			DrawFootprint();
		}
		else if(STATE_UP && (snake[0].y > 1))
		{
			moveCounter++;
			DrawFootprint();
		}
		else if(STATE_RIGHT && (snake[0].x < 60))
		{
			moveCounter++;
			DrawFootprint();
		}
		else if(STATE_DOWN && (snake[0].y < 19))
		{ 
			moveCounter++;
			DrawFootprint();
		}
		
		//���·������ 
		if(GetAsyncKeyState(VK_LEFT)&0x8000 && !STATE_RIGHT)
		{
			STATE_UP = false;
			STATE_DOWN = false;
			STATE_LEFT = true;
			STATE_RIGHT = false;
		} 
		if(GetAsyncKeyState(VK_DOWN)&0x8000 && !STATE_UP)
		{
			STATE_UP = false;
			STATE_DOWN = true;
			STATE_LEFT = false;
			STATE_RIGHT = false;
		}
		if(GetAsyncKeyState(VK_RIGHT)&0x8000 && !STATE_LEFT)
		{
			STATE_UP = false;
			STATE_DOWN = false;
			STATE_LEFT = false;
			STATE_RIGHT = true;
		}
		if(GetAsyncKeyState(VK_UP)&0x8000 && !STATE_DOWN)
		{
			STATE_UP = true;
			STATE_DOWN = false;
			STATE_LEFT = false;
			STATE_RIGHT = false;
		}
		
		BlockIsEaten();
		LevelUpCheck();
		Sleep(sleepTime - sleepTimeWeight);	//ͣ��0.25�� 
	}
	
	gotoxy(23,23);
	printf("״̬��أ�����");	//�������
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE);
	gotoxy(24,10);
	printf("  GAME OVER  ");
	gotoxy(20,11);
	printf("  PRESS R TO RESTART  ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
}

void Init()
{
	console();
	snake[0].x = defaultX;
	snake[0].y = defaultY;
	snake[1].x = defaultX + 2;
	snake[1].y = defaultY;
	snake[2].x = defaultX + 4;
	snake[2].y = defaultY;
	snakeLength = 3;
	STATE_UP = false;
	STATE_DOWN = false;
	STATE_LEFT = true;		//Ĭ���������� 
	STATE_RIGHT = false;
	STATE_ALIVE = true;
	isEaten = true;
	level = 0; 
	scoreWeight = 1;
	sleepTimeWeight = 0;
	curScore = 0;
}

int main()
{
	char ch;
	restart:;
	getcache.open("info.txt");
	getcache>>bestScore;
	getcache.close();
	Init();
	Print_Info();
	Game_Entrance();
	storecache.open("info.txt");
	storecache<<(curScore > bestScore? curScore:bestScore);
	storecache.close();
	
	cin>>ch;
	
	if((ch == 'R') || (ch == 'r')) goto restart;
	
	gotoxy(0,27);
	return 0;
}
