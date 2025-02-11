#include <iostream>

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########" ;

const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_PLAYER,
	OBJ_PLAYER_ON_GOAL,

	OBJ_UNKNOWN
};

void Initialize(Object* State, int w, int h, const char* stageData);
void Draw(const Object* State, int w, int h); // !! const??, 
void Update(Object* s, char input, int w, int h);
bool Checkclear(const Object* s,int w, int h); // !! const

int main()
{
	Object* state = new Object[gStageWidth * gStageHeight];
	Initialize(state, gStageWidth, gStageHeight, gStageData);

	while (true)
	{
		Draw(state, gStageWidth, gStageHeight);

		if (Checkclear(state, gStageWidth, gStageHeight))
		{
			break;
		}

		std::cout << "a:left d:right w:up s:down. command?" << std::endl;
		char input;
		std::cin >> input;

		Update(state, input, gStageWidth, gStageHeight);
	}

	std::cout << "Congratualations! you Win!" << std::endl;

	delete[] state; // 동적 할당 때 쌍으로 같이 만들기
	state = nullptr;

	while (true)
	{

	}
	return 0;
}

void Initialize(Object* State, int w, int h, const char* stageData)
{
	const char* d = stageData;
	int x = 0;
	int y = 0;

	while (*d != '\0')
	{
		Object t;

		switch (*d)
		{
		case '#':
			t = OBJ_WALL;
			break;
		case ' ':
			t = OBJ_SPACE;
			break;
		case 'o':
			t = OBJ_BLOCK;
			break;
		case 'O':
			t = OBJ_BLOCK_ON_GOAL;
			break;
		case '.':
			t = OBJ_GOAL;
			break;
		case 'p':
			t = OBJ_PLAYER;
			break;
		case 'P':
			t = OBJ_PLAYER_ON_GOAL;
			break;
		case '\n': // 맨끝 열에 닿으면 다음행 첫 열로 이동
			x = 0; // 
			++y; // 
			t = OBJ_UNKNOWN;
			break;
		default:
			t = OBJ_UNKNOWN;
			break;
		}
		++d;
		if (t != OBJ_UNKNOWN)
		{
			State[y * gStageWidth + x] = t; // 배열에 타일 값 끼워넣기
			++x; // 바로 오른쪽 타일로 이동
		}
	}
}

void Draw(const Object* State, int w, int h)
{
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			Object o = State[y * gStageWidth + x]; // 해당 타일 속성 복사해오기
			std::cout << font[o];
		}
		std::cout << std::endl;
	}
}

void Update(Object* s, char input, int w, int h)
{
	int dx = 0;
	int dy = 0;
	switch (input)
	{
	case 'a':
		dx = -1;
		break;
	case 'd':
		dx = 1;
		break;
	case 'w':
		dy = -1;
		break;
	case 's':
		dy = 1;
		break;
	}

	int i = -1;
	for (i = 0; i < w * h; i++)
	{
		if(s[i] == OBJ_PLAYER || s[i] == OBJ_PLAYER_ON_GOAL) // ??
		{
			break;
		}
	}
	int x = i % w;
	int y = i / w;

	int tx = x + dx; // 이동 후 좌표
	int ty = y + dy;

	if (tx < 1 || ty < 1 || tx >= w || ty >= h) // 맵을 나가면 에러 알림
	{
		std::cerr << "Invalid player position" << std::endl;
		return;
	}

	int p = y * w + x;
	int tp = ty * w + tx;
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)
	{
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_PLAYER_ON_GOAL : OBJ_PLAYER; // 이동할 위치의 변화
		s[p] = (s[p] == OBJ_PLAYER_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; // 원래 위치의 변화
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) // 밀린 상자 위치가 맵 밖이면 무시
		{
			return;
		}

		int tp2 = (ty + dy) * w + (tx + dx);
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)
		{
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK; // 이동뒤 블록 위치의 변화
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_PLAYER_ON_GOAL : OBJ_PLAYER;
			s[p] = (s[p] == OBJ_PLAYER_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}

	}
}

bool Checkclear(const Object* s, int w, int h)
{
	for (int i = 0; i < w * h; i++)
	{
		if (s[i] == OBJ_BLOCK) // 멀쩡한 블록이 존재하면 0 돌려주기
		{
			return false;
		}
	}
	return true; // 블록이 전부 골위치에 들어갔으면 1돌려주기
}
