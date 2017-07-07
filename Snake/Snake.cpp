#include "windows.h"
#include "fstream"
#include "conio.h"
#include "time.h"
#include "stdio.h"
#include "iostream"
using namespace std;

#define sleepTime 150
/*
可控范围纵向1-19；
横向2-60（偶数项） ； 
*/

int defaultX = 42;
int defaultY = 10;
int curScore = 0;
int bestScore = 0;
int moveCounter = 0;
bool STATE_UP = false;
bool STATE_DOWN = false;
bool STATE_LEFT = true;		//默认向左行走 
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
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄  
	CONSOLE_SCREEN_BUFFER_INFO bInfo; // 窗口缓冲区信息  
	GetConsoleScreenBufferInfo(hOut, &bInfo );// 获取窗口缓冲区信息  
	SetConsoleTitle("贪吃蛇 C版"); // 设置窗口的标题  
	COORD size = {80, 20};//不能小于默认大小（80，25）  
	SetConsoleScreenBufferSize(hOut,size); // 重新设置缓冲区大小*/  
	SMALL_RECT rc = {0,0, 100, 64}; //不能大于缓冲区大小  
	SetConsoleWindowInfo(hOut,true ,&rc);// 重置窗口大小  
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; //第一个参数光标百分比大小。第二个参数是否可见。  
	SetConsoleCursorInfo(hOut, &cursor_info);//设置控制台光标  
}  

void gotoxy(int x,int y)//X表示横坐标，Y表示纵坐标。  
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
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	for(int i = 0; i < 19; i++)
		printf("■\t\t\t\t\t\t\t      ■\n");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n\n");
	printf("|--------------------------------------------------------------|\n");
	gotoxy(0,23);
	printf("|");
	gotoxy(4,23);
	printf("得分：%d",curScore);		//得分监控 
	gotoxy(23,23);
	printf("当前状态：初始");	//按键监控 
	gotoxy(63,23);
	printf("|");
	gotoxy(0,24);
	printf("|");
	gotoxy(0,25);
	printf("|");
	gotoxy(0,26);
	printf("|");
	gotoxy(4,25);
	printf("长度：%d",snakeLength) ;
	gotoxy(23,26);
	printf("等级：LV %d",level);
	gotoxy(23,25);
	printf("豆豆位置:(%d，%d)",rand_P.x,rand_P.y);
	gotoxy(4,24);
	printf("最佳：%d",bestScore);		//最佳监控 
	gotoxy(23,24);
	printf("位置监控:(%d，%d)",snake[0].x,snake[0].y);	//步数监控
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
		printf("■");	
	}
}

void DrawFootprint()
{
	gotoxy(23,23);
	
	if(STATE_UP)
	{
		printf("状态监控：向上");	//按键监控 
	}
	else if(STATE_DOWN)
	{
		printf("状态监控：向下");	//按键监控
	}
	else if(STATE_LEFT)
	{
		printf("状态监控：向左");	//按键监控
	}
	else if(STATE_RIGHT)
	{
		printf("状态监控：向右");	//按键监控
	}
	
	if(curScore > bestScore)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY|FOREGROUND_GREEN);
		gotoxy(4,26);
		printf("新纪录!");	//位置监控
	}
	gotoxy(4,23);
	printf("得分：%d",curScore);		//得分监控 
	gotoxy(4,24);
	printf("最佳：%d",curScore > bestScore? curScore:bestScore);		//最佳监控 
	gotoxy(4,25);
	printf("长度：%d",snakeLength) ;
	gotoxy(23,25);
	printf("豆豆位置:(%d，%d)",rand_P.x,rand_P.y);
	gotoxy(23,24);
	printf("位置监控:(%d，%d)\n",snake[0].x,snake[0].y);	//位置监控
	gotoxy(23,26);
	printf("等级：LV %d",level);

	
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
	printf("■");
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
	//吃到方块检测
	if(isEaten)
	{
	srand((unsigned)time(NULL));
	rand_P.x = (rand()%29 + 1)*2;
	rand_P.y = rand()%18 + 1;
	gotoxy(rand_P.x,rand_P.y);
	printf("■");
	isEaten = false;
	}
	
	if(!isEaten)
	{
		gotoxy(rand_P.x,rand_P.y);
		printf("■");	
	}
	
	if((snake[0].x == rand_P.x)&&(snake[0].y == rand_P.y))
	{
		snakeLength++;
		curScore += scoreWeight;
		isEaten = true; 
		gotoxy(0,0);
		printf("■");
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
		//碰撞检测 
		STATE_ALIVE = CollisionCheck();
		//转向检测 
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
		
		//更新方向控制 
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
		Sleep(sleepTime - sleepTimeWeight);	//停顿0.25秒 
	}
	
	gotoxy(23,23);
	printf("状态监控：死亡");	//按键监控
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
	STATE_LEFT = true;		//默认向左行走 
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
