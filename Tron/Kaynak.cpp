#include <iostream>
#include <conio.h> // konsol input output
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <time.h>

#pragma comment(lib, "winmm.lib") // yine bir cesit library ama include la yazilmiyor

enum yonler { UP = 'w', DOWN = 's', LEFT = 'a', RIGHT = 'd', up = 'u', down = 'j', left = 'h', right = 'k' }; // okunulabilirlik icin

constexpr auto heigth = 179; // sahanin boyutunu kolayca degistirebilmek icin
constexpr auto width = 316;

void HideCursor(bool showFlag) // cursor cirkin gozukdugunden gozukmesin
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void ScreenReset(int x, int y) // konsol da alt alta, yan yana yazmak yerine benim dedigim yere bas
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void Movement(int& snake1X, int& snake1Y, int& mem1, bool& Condition, int& snake2X, int& snake2Y, int& mem2)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	bool player1 = false, player2 = false;
	static int mem1X = 0, mem1Y = 1, mem2X = 0, mem2Y = -1; // onceki gidilen yolu hatirlayabilmek icin static
	if (Condition) mem1X = 0, mem1Y = 1, mem2X = 0, mem2Y = -1, Condition = false; // olduyse saga hareketi yenile
	static int c = 0;
	clock_t start = clock();
	while (!_kbhit())
	{
	Ignore:;
		while (!_kbhit())
		{
			if ((((double)clock() - start) / CLOCKS_PER_SEC) >= 0.04) // hareket hizi
			{
				if (player1 == false && player2 == false)
				{
					snake1X += mem1X; // tusa basilmazsa onceki turun yonunu uygula
					snake1Y += mem1Y;
					snake2X += mem2X;
					snake2Y += mem2Y;
					return;
				}
				else if (player1 == false && player2 == true)
				{
					snake1X += mem1X; // tusa basilmazsa onceki turun yonunu uygula
					snake1Y += mem1Y;
					return;
				}
				else if (player1 == true && player2 == false)
				{
					snake2X += mem2X;
					snake2Y += mem2Y;
					return;
				}
				else if (player1 == true && player2 == true)
				{
					return;
				}
			}
		}
	}
	c = _getch();
	if (c == mem1 || c == mem2) goto Ignore;
	switch (c)
	{
	case UP:
		if (mem1 == DOWN) goto Ignore;
		--snake1X;
		mem1X = -1; //
		mem1Y = 0; // tusa basilmazsa diye bu tur yapilanlari kullanabilmek icin kaydet
		mem1 = UP; //
		player1 = true;
		ScreenReset(snake1Y * 2, snake1X);
		SetConsoleTextAttribute(hConsole, 33);
		std::cout << "  ";
		goto Ignore;
		break;
	case DOWN:
		if (mem1 == UP) goto Ignore;
		++snake1X;
		mem1X = 1;
		mem1Y = 0;
		mem1 = DOWN;
		player1 = true;
		ScreenReset(snake1Y * 2, snake1X);
		SetConsoleTextAttribute(hConsole, 33);
		std::cout << "  ";
		goto Ignore;
		break;
	case LEFT:
		if (mem1 == RIGHT) goto Ignore;
		--snake1Y;
		mem1Y = -1;
		mem1X = 0;
		mem1 = LEFT;
		player1 = true;
		ScreenReset(snake1Y * 2, snake1X);
		SetConsoleTextAttribute(hConsole, 33);
		std::cout << "  ";
		goto Ignore;
		break;
	case RIGHT:
		if (mem1 == LEFT) goto Ignore;
		++snake1Y;
		mem1Y = 1;
		mem1X = 0;
		mem1 = RIGHT;
		player1 = true;
		ScreenReset(snake1Y * 2, snake1X);
		SetConsoleTextAttribute(hConsole, 33);
		std::cout << "  ";
		goto Ignore;
		break;
	case up:
		if (mem2 == down) goto Ignore;
		--snake2X;
		mem2X = -1;
		mem2Y = 0;
		mem2 = up;
		player2 = true;
		ScreenReset(snake2Y * 2, snake2X);
		SetConsoleTextAttribute(hConsole, 16);
		std::cout << "  ";
		goto Ignore;
		break;
	case down:
		if (mem2 == up) goto Ignore;
		++snake2X;
		mem2X = 1;
		mem2Y = 0;
		mem2 = down;
		player2 = true;
		ScreenReset(snake2Y * 2, snake2X);
		SetConsoleTextAttribute(hConsole, 16);
		std::cout << "  ";
		goto Ignore;
		break;
	case left:
		if (mem2 == right) goto Ignore;
		--snake2Y;
		mem2X = 0;
		mem2Y = -1;
		mem2 = left;
		player2 = true;
		ScreenReset(snake2Y * 2, snake2X);
		SetConsoleTextAttribute(hConsole, 16);
		std::cout << "  ";
		goto Ignore;
		break;
	case right:
		if (mem2 == left) goto Ignore;
		++snake2Y;
		mem2X = 0;
		mem2Y = 1;
		mem2 = right;
		player2 = true;
		ScreenReset(snake2Y * 2, snake2X);
		SetConsoleTextAttribute(hConsole, 16);
		std::cout << "  ";
		goto Ignore;
		break;
	default:
		goto Ignore;
		break;
	}
}

void Snake(std::vector<int>& kuyruk1x, std::vector<int>& kuyruk1y, int snake1x, int snake1y,
	std::vector<int>& kuyruk2x, std::vector<int>& kuyruk2y, int snake2x, int snake2y)
{
	kuyruk1x.push_back(snake1x);
	kuyruk1y.push_back(snake1y);
	kuyruk2x.push_back(snake2x);
	kuyruk2y.push_back(snake2y);
}

int main()
{
	mciSendString(L"open C:\\Users\\KAAN\\source\\repos\\Tron\\fight.mp3", NULL, 0, NULL);
	mciSendString(L"setaudio C:\\Users\\KAAN\\source\\repos\\Tron\\fight.mp3 volume to 20", NULL, 0, NULL);
	mciSendString(L"play C:\\Users\\KAAN\\source\\repos\\Tron\\fight.mp3 repeat", NULL, 0, NULL);
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, 0); // tam ekran yap
	std::ios_base::sync_with_stdio(false);
	std::cout.tie(NULL); // daha hizli cout
	bool Condition = false, greendead = false;
	int headmem1x, headmem1y, headmem2x, headmem2y;
	std::vector<int> kuyruk1x, kuyruk1y, kuyruk2x, kuyruk2y;
	std::string saha = "";
restart: // olunce bastan basla
	greendead = false;
	kuyruk1x.clear(), kuyruk1y.clear(); // yeniden baslandiysa onceki kuyrugu sil
	kuyruk2x.clear(), kuyruk2y.clear();
	int snake1x = heigth / 2, snake1y = 5, mem1 = RIGHT;
	int snake2x = heigth / 2, snake2y = width - 6, mem2 = left; //baslangic konumu ve yonu
	HideCursor(false); // cursor gozukmesin
	ScreenReset(0, 0);
	if (saha == "")
		for (int i = 0; i < heigth; ++i) // sahayi ciz
		{
			for (int j = 0; j < width; ++j)
			{
				if (i == 0 || i == heigth - 1 || j == 0 || j == width - 1)
				{
					SetConsoleTextAttribute(hConsole, 128);
					saha += "# ";
				}
				else if (i == snake1x && snake1y == j)
				{
					SetConsoleTextAttribute(hConsole, 33);
					saha += "  ";
				}
				else if (i == snake2x && snake2y == j)
				{
					SetConsoleTextAttribute(hConsole, 16);
					saha += "  ";
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 128);
					saha += "  "; // bos alan
				}
			}
			saha += '\n';
		}
	std::cout << saha;
	while (true)
	{
		headmem2x = snake2x, headmem2y = snake2y;
		headmem1x = snake1x, headmem1y = snake1y; // su anki kafa konumu
		Movement(snake1x, snake1y, mem1, Condition, snake2x, snake2y, mem2);
		Snake(kuyruk1x, kuyruk1y, headmem1x, headmem1y, kuyruk2x, kuyruk2y, headmem2x, headmem2y);
		if (snake1x == snake2x && snake1y == snake2y)
		{
			mciSendString(L"play C:\\Users\\KAAN\\source\\repos\\snake\\roblox.mp3", NULL, 0, NULL); // olum sesi
			Condition = true;
			SetConsoleTextAttribute(hConsole, 128);
			ScreenReset(width - 5, heigth / 2);
			std::cout << "Berabere!";
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			while (_kbhit()) _getch();
			goto restart;
		}
		for (int l = kuyruk1x.size(); l--;)
		{
			if ((kuyruk1x[l] == snake1x && kuyruk1y[l] == snake1y)
				|| (snake1x == 0 || snake1x == heigth - 1 || snake1y == 0 || snake1y == width - 1)
				|| (kuyruk2x[l] == snake1x && kuyruk2y[l] == snake1y))
			{
				greendead = true;
			}
		}
		for (int l = kuyruk2x.size(); l--;)
		{
			if ((kuyruk2x[l] == snake2x && kuyruk2y[l] == snake2y)
				|| (snake2x == 0 || snake2x == heigth - 1 || snake2y == 0 || snake2y == width - 1)
				|| (kuyruk1x[l] == snake2x && kuyruk1y[l] == snake2y))
			{
				if (greendead)
				{
					mciSendString(L"play C:\\Users\\KAAN\\source\\repos\\snake\\roblox.mp3", NULL, 0, NULL); // olum sesi
					Condition = true;
					SetConsoleTextAttribute(hConsole, 128);
					ScreenReset(width - 5, heigth / 2);
					std::cout << "Berabere!";
					std::this_thread::sleep_for(std::chrono::milliseconds(3000));
					while (_kbhit()) _getch();
					goto restart;
				}
				mciSendString(L"play C:\\Users\\KAAN\\source\\repos\\snake\\roblox.mp3", NULL, 0, NULL);
				Condition = true;
				SetConsoleTextAttribute(hConsole, 130);
				ScreenReset(width - 5, heigth / 2);
				std::cout << "Yesil Kazandi!";
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				while (_kbhit()) _getch();
				goto restart;
			}
		}
		if (greendead)
		{
			mciSendString(L"play C:\\Users\\KAAN\\source\\repos\\snake\\roblox.mp3", NULL, 0, NULL);
			Condition = true;
			SetConsoleTextAttribute(hConsole, 137);
			ScreenReset(width - 5, heigth / 2);
			std::cout << "Mavi Kazandi!";
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			while (_kbhit()) _getch();
			goto restart;
		}
		ScreenReset(snake1y * 2, snake1x);
		SetConsoleTextAttribute(hConsole, 33);
		std::cout << "  ";
		ScreenReset(snake2y * 2, snake2x);
		SetConsoleTextAttribute(hConsole, 16);
		std::cout << "  ";
	}
}