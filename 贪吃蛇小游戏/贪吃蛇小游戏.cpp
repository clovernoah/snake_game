#define NODE_WIDTH 40
#define WIDTH 800          // 窗口宽度
#define HEIGHT 600         // 窗口高度
#define MAX_SNAKE_LENGTH 100  // 蛇最大长度
#include <easyx.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

enum direction
{
	eUp,
	eDown,
	eLeft,
	eRight
};

typedef struct
{
	int x;
	int y;
}node;

void paintGrid()
{
	for (int y = 0; y < HEIGHT; y += NODE_WIDTH)
	{
		line(0, y, WIDTH, y);
	}
	for (int x = 0; x < WIDTH; x += NODE_WIDTH)
	{
		line(x, 0, x, HEIGHT);
	}
}

void paintSnake(node* snake, int n, int direction)
{
	int left, top, right, bottom;
	for (int i = 0; i < n; i++)
	{
		left = snake[i].x * NODE_WIDTH;
		top = snake[i].y * NODE_WIDTH;
		right = (snake[i].x + 1) * NODE_WIDTH;
		bottom = (snake[i].y + 1) * NODE_WIDTH;
		setlinecolor(BLACK);
		rectangle(left, top, right, bottom);
		setlinecolor(WHITE);
	}

	setfillcolor(BLACK);
	switch (direction)
	{
	case eUp:
		solidrectangle(snake[0].x * NODE_WIDTH + 9, snake[0].y * NODE_WIDTH + 9, snake[0].x * NODE_WIDTH + 11, snake[0].y * NODE_WIDTH + 11);
		solidrectangle(snake[0].x * NODE_WIDTH + 29, snake[0].y * NODE_WIDTH + 9, snake[0].x * NODE_WIDTH + 31, snake[0].y * NODE_WIDTH + 11);
		break;
	case eDown:
		solidrectangle(snake[0].x * NODE_WIDTH + 9, snake[0].y * NODE_WIDTH + 29, snake[0].x * NODE_WIDTH + 11, snake[0].y * NODE_WIDTH + 31);
		solidrectangle(snake[0].x * NODE_WIDTH + 29, snake[0].y * NODE_WIDTH + 29, snake[0].x * NODE_WIDTH + 31, snake[0].y * NODE_WIDTH + 31);
		break;
	case eLeft:
		solidrectangle(snake[0].x * NODE_WIDTH + 9, snake[0].y * NODE_WIDTH + 9, snake[0].x * NODE_WIDTH + 11, snake[0].y * NODE_WIDTH + 11);
		solidrectangle(snake[0].x * NODE_WIDTH + 9, snake[0].y * NODE_WIDTH + 29, snake[0].x * NODE_WIDTH + 11, snake[0].y * NODE_WIDTH + 31);
		break;
	case eRight:
		solidrectangle(snake[0].x * NODE_WIDTH + 29, snake[0].y * NODE_WIDTH + 9, snake[0].x * NODE_WIDTH + 31, snake[0].y * NODE_WIDTH + 11);
		solidrectangle(snake[0].x * NODE_WIDTH + 29, snake[0].y * NODE_WIDTH + 29, snake[0].x * NODE_WIDTH + 31, snake[0].y * NODE_WIDTH + 31);
		break;
	}
	setfillcolor(WHITE);

}

node snakeMove(node* snake, int length, int direction)
{
	node tail = snake[length - 1];
	for (int i = length - 1; i > 0; i--)
	{
		snake[i] = snake[i - 1];
	}
	node newHead;
	newHead = snake[0];
	if (direction == eUp)
	{
		newHead.y--;
	}
	else if (direction == eDown)
	{
		newHead.y++;
	}
	else if (direction == eLeft)
	{
		newHead.x--;
	}
	else
	{
		newHead.x++;
	}
	snake[0] = newHead;

	int maxX = WIDTH / NODE_WIDTH;
	int maxY = HEIGHT / NODE_WIDTH;
	if (snake[0].x < 0)
		snake[0].x += maxX;
	else if (snake[0].y < 0)
		snake[0].y += maxY;
	else if (snake[0].x >= maxX)
		snake[0].x -= maxX;
	else if (snake[0].y >= maxY)
		snake[0].y -= maxY;

	return tail;
}

void changeDirection(enum direction* pD)
{
	if (_kbhit() != 0)
	{
		char c = _getch();
		switch (c)
		{
		case 'w':
			if (*pD != eDown)
				*pD = eUp;
			break;
		case 's':
			if (*pD != eUp)
				*pD = eDown;
			break;
		case 'a':
			if (*pD != eRight)
				*pD = eLeft;
			break;
		case 'd':
			if (*pD != eLeft)
				*pD = eRight;
			break;
		}

	}
}

node createFood(node* snake, int length)
{
	node food;
	while (1) {
		food.x = rand() % (WIDTH / NODE_WIDTH);
		food.y = rand() % (HEIGHT / NODE_WIDTH);
		bool overlap = false;
		for (int i = 0; i < length; i++) {
			if (snake[i].x == food.x && snake[i].y == food.y) {
				overlap = true;
				break;
			}
		}
		if (!overlap) break;
	}
	return food;
}

void paintFood(node food)
{
	int left, top, right, bottom;
	left = food.x * NODE_WIDTH;
	top = food.y * NODE_WIDTH;
	right = (food.x + 1) * NODE_WIDTH;
	bottom = (food.y + 1) * NODE_WIDTH;
	setfillcolor(YELLOW);
	solidrectangle(left, top, right, bottom);
	setfillcolor(WHITE);
}

bool isGameOver(node* snake, int length)
{
	for (int i = 1; i < length; i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return true;
	}
	return false;
}


int main()
{
	printf("按任意键进入游戏...\n");
	printf("（温馨提示：切换为英文输入法，并在运行控制台窗口中输入wsad实现移动）\n");
	_getch();
	system("cls");

	initgraph(WIDTH, HEIGHT);
	setbkcolor(RGB(164, 225, 202));
	cleardevice();

	node snake[MAX_SNAKE_LENGTH] = { {5,7},{4,7},{3,7},{2,7},{1,7} };
	int length = 5;
	enum direction d = eRight;
	srand(unsigned int(time(NULL)));
	node food = createFood(snake, length);

	while (1)
	{
		cleardevice();
		paintGrid();
		paintFood(food);
		paintSnake(snake, length, d);
		Sleep(100);

		changeDirection(&d);
		node lastTail = snakeMove(snake, length, d);
		if (snake[0].x == food.x && snake[0].y == food.y)
		{
			if (length < MAX_SNAKE_LENGTH)
			{
				snake[length] = lastTail;
				length++;
			}
			food = createFood(snake, length);
		}
		if (isGameOver(snake, length) == true)
		{
			break;
		}
	}

	closegraph();
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("game over!!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("你个菜鸡!!!!!!!!!!!!\n");
	printf("\n");
	system("pause");

	return 0;
}

