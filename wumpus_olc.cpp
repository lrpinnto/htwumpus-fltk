//g++ -o wumpus wumpus.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Wumpus : public olc::PixelGameEngine
{
public:
	Wumpus()
	{
		sAppName = "Hunt the Wumpus";
	}

	struct sButton
	{
		olc::vi2d vPos;
		olc::vi2d vSize;
		std::string sText;

		bool clicked(const olc::vi2d& vMousePos)
		{
			return vMousePos.x >= vPos.x &&
			vMousePos.x < (vPos.x + vSize.x) &&
			vMousePos.y >= vPos.y &&
			vMousePos.y < (vPos.y + vSize.y);
		}

		void Draw(olc::PixelGameEngine* pge)
		{
			pge->FillRect(vPos,vSize,olc::BLUE);
			pge->DrawRect(vPos,vSize,olc::WHITE);

			olc::vi2d vTextSize = pge->GetTextSizeProp(sText);
			pge->DrawStringProp(vPos + (vSize - vTextSize) / 2, sText, olc::WHITE);
		}
	};

	sButton btnShoot;
	sButton btnMove;
	
private:
	olc::vi2d center {ScreenWidth()/2,ScreenHeight()/2};

	olc::vi2d current;

	const olc::Pixel player_color {255,255,254};
	
	std::vector<olc::vi2d> points_inner;
	std::vector<olc::vi2d> points_middle;
	std::vector<olc::vi2d> points_outer;

	std::vector<olc::Pixel> colors {
		olc::GREY, olc::DARK_GREY, olc::VERY_DARK_GREY,
		olc::RED, olc::DARK_RED, olc::VERY_DARK_RED,
		olc::YELLOW, olc::DARK_YELLOW, olc::VERY_DARK_YELLOW,
		olc::GREEN, olc::DARK_GREEN, olc::VERY_DARK_GREEN,
		olc::CYAN, olc::DARK_CYAN, olc::VERY_DARK_CYAN,
		olc::BLUE, olc::DARK_BLUE, olc::VERY_DARK_BLUE,
		olc::MAGENTA, olc::DARK_MAGENTA, olc::VERY_DARK_MAGENTA,
		olc::WHITE, olc::BLACK, olc::BLANK
	};
	olc::Sprite *sprIsom = nullptr;

public:
	bool OnUserCreate() override
	{
		//Create Buttons
		btnShoot.vPos = {ScreenWidth()*(1/4.0),ScreenHeight()*(9/10.0)};
		btnShoot.vSize = {50, 15};
		btnShoot.sText = "Shoot";

		btnMove.vPos = {ScreenWidth()*(2/4.0),ScreenHeight()*(9/10.0)};
		btnMove.vSize = {50, 15};
		btnMove.sText = "Move";

		//Creating Map layout
		const double smallest_division {(2*M_PI)/5};
		const double offset {smallest_division/2};

		for (int i = 0; i < 5; i++)
		{
			points_inner.push_back({static_cast<int>(center.x+25*cos((i*smallest_division))),static_cast<int>(center.y+25*sin(i*smallest_division))});
		}
		for (int i = 0; i < 5; i++)
		{
			points_middle.push_back({static_cast<int>(center.x+50*cos((i*smallest_division)+offset)),static_cast<int>(center.y+50*sin((i*smallest_division)+offset))});
		}
		for (int i = 0; i < 5; i++)
		{
			points_outer.push_back({static_cast<int>(center.x+75*cos((i*smallest_division)+offset)),static_cast<int>(center.y+75*sin((i*smallest_division)+offset))});
		}
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Erase previous frame
		Clear(olc::Pixel(0,128,148));

		// Get Mouse in world
		olc::vi2d vMouse = { GetMouseX(), GetMouseY() };

		//Buttons
		btnShoot.Draw(this);
		btnMove.Draw(this);

		// Map layout
		FillTriangle(center,points_inner[4],points_inner[0],colors[15]);
		DrawTriangle(center,points_inner[4],points_inner[0]);
		for (int i = 0 ; i<points_inner.size()-1; i++)
		{
			FillTriangle(center,points_inner[i],points_inner[i+1],colors[i]);
			DrawTriangle(center,points_inner[i],points_inner[i+1]);
		}
		FillTriangle(points_inner[4],points_inner[0],points_middle[4],colors[16]);
		FillTriangle(points_inner[0],points_middle[0],points_middle[4],colors[17]);
		DrawTriangle(points_inner[4],points_inner[0],points_middle[4]);
		DrawTriangle(points_inner[0],points_middle[0],points_middle[4]);
		for (int i = 0 ; i<points_middle.size()-1; i++)
		{
			FillTriangle(points_inner[i],points_inner[i+1],points_middle[i],colors[5+i]);
			FillTriangle(points_inner[i+1],points_middle[i+1],points_middle[i],colors[10+i]);
			DrawTriangle(points_inner[i],points_inner[i+1],points_middle[i]);
			DrawTriangle(points_inner[i+1],points_middle[i+1],points_middle[i]);
		}

		DrawLine(points_outer[4],points_outer[0]);
		DrawLine(points_outer[4],points_middle[4]);
		for (int i = 0 ; i<points_outer.size()-1; i++)
		{
			//FillRect();
			DrawLine(points_outer[i],points_outer[i+1]);
			DrawLine(points_outer[i],points_middle[i]);
		}

		if (GetMouse(0).bPressed)
		{
			if (btnShoot.clicked(GetMousePos()))
			{
				std::cout<<"SHOOT\t";
			}

			if (btnMove.clicked(GetMousePos()))
			{
				std::cout<<"MOVE\t";
			}
			
		}
		return true;
	}
};

int main()
{
	Wumpus game;
	if (game.Construct(256, 240, 4, 4))
		game.Start();

	return 0;
}
