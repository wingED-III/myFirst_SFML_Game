#include<SFML\Graphics.hpp>
#include<SFML\Audio.hpp>
#include<math.h>
#include<vector>
#include<cstdlib>
#include<iostream>
#include<time.h>
#include<sstream>
#include<string.h>
#include<fstream>
#include<deque>
#include<windows.h>

#define M_PI 3.14159265358979323846;

using namespace sf;
using namespace std;

class Prototype
{
public:
	RectangleShape Shape;
	unsigned int HP;
	float x = Shape.getPosition().x, y = Shape.getPosition().y;
	int width, height;

};

class Player :public Prototype
{
public:
	Texture Pskin, Pskin2;
	Sprite Psprite, Psprite2;
	float stamina = 100,SpdUp=12.5f;
	float playerSpeed = 5.f,regenrate=1.5;
	int changeAct = 0, forceDirect = 1;
	unsigned int force = 0;
	Time regenTime, ShieldT,ssTime;
	CircleShape Shield, Forceball;
	bool ShieldState = 0, ForceState = 0, SS = 0;
	RectangleShape line;
	SoundBuffer forForce;
	Sound Forcesound;

	Player::Player()
	{
		width = 40; height = 40;
		Shape.setSize(Vector2f(width, height));
		Shape.setFillColor(Color::Cyan);
		HP = 100;
		Pskin.loadFromFile("player stand sprite.png");
		Psprite.setTexture(Pskin);
		Psprite.setTextureRect(IntRect(30, 30, 100, 100));

		Pskin2.loadFromFile("player slash sprite.png");
		Psprite2.setTexture(Pskin2);
		Psprite2.setTextureRect(IntRect(30, 30, 100, 100));

		Shield.setRadius(25);
		Shield.setFillColor(Color::Color(0, 0, 0, 0));
		Shield.setOutlineColor(Color::Yellow);
		Shield.setOutlineThickness(5);

		Forceball.setRadius(40);
		Forceball.setFillColor(Color::Color(133, 234, 252, 128));

		line.setSize(Vector2f(30, 100));
		line.setFillColor(Color::Green);
		line.setPosition(-100.f, -100.f);


		forForce.loadFromFile("forcepush eff.wav");
		Forcesound.setBuffer(forForce);
	}
	int Player::pmove(RenderWindow & window, RectangleShape & player, int & Pface, Sprite &Psprite)
	{
		if (Keyboard::isKeyPressed(Keyboard::Left) && player.getPosition().x > 40)
		{
			player.move(-playerSpeed, 0.f);// player.move(x.f,y.f)
			Psprite.setPosition(player.getPosition().x - 50, player.getPosition().y - 10);
			Psprite.setTextureRect(IntRect(280, 20, 100, 100));
			Pface = -1; changeAct = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) && player.getPosition().x + 40 < window.getSize().x - 45)
		{
			player.move(playerSpeed, 0.f);
			Psprite.setPosition(player.getPosition());
			Psprite.setTextureRect(IntRect(30, 30, 100, 100));
			Pface = 1; changeAct = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) && player.getPosition().y > 50)
		{
			player.move(0.f, -playerSpeed);
			Psprite.setPosition(player.getPosition().x, player.getPosition().y - 25);
			Psprite.setTextureRect(IntRect(130, 5, 60, 100));
			Pface = 2; changeAct = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down) && player.getPosition().y + 40 < window.getSize().y - 40)
		{
			player.move(0.f, playerSpeed);
			Psprite.setPosition(player.getPosition().x - 10, player.getPosition().y);
			Psprite.setTextureRect(IntRect(220, 30, 60, 100));
			Pface = -2; changeAct = 0;
		}

		return Pface;
	}
	bool Player::attack(RectangleShape &player, Clock &C1, RectangleShape &line, int &Pface)
	{
		if (Keyboard::isKeyPressed(Keyboard::Space) && stamina >= 20)
		{
			if (Pface == 1)
			{
				line.setRotation(0);
				line.setPosition(player.getPosition().x + 35, player.getPosition().y - 25);
				Psprite2.setPosition(player.getPosition());
				Psprite2.setTextureRect(IntRect(30, 30, 100, 100));

			}
			else if (Pface == -1)
			{
				line.setRotation(0);
				line.setPosition(player.getPosition().x - 23, player.getPosition().y - 30);
				Psprite2.setPosition(player.getPosition().x - 50, player.getPosition().y - 10);
				Psprite2.setTextureRect(IntRect(280, 20, 100, 100));
			}
			else if (Pface == 2)
			{
				line.setRotation(90);
				line.setPosition(player.getPosition().x + 60, player.getPosition().y - 28);
				Psprite2.setPosition(player.getPosition().x, player.getPosition().y - 25);
				Psprite2.setTextureRect(IntRect(130, 5, 60, 100));
			}
			else if (Pface == -2)
			{
				line.setRotation(90);
				line.setPosition(player.getPosition().x + 70, player.getPosition().y + 33);
				Psprite2.setPosition(player.getPosition().x - 10, player.getPosition().y);
				Psprite2.setTextureRect(IntRect(220, 30, 60, 100));
			}
			C1.restart();
			changeAct = 1;
			return 1;
		}
	}
	void Player::useforce(int &Pface)
	{
		if (Keyboard::isKeyPressed(Keyboard::F))
		{
			if (force == 5 && ForceState == 0)
			{
				ForceState = 1; forceDirect = Pface;
				Forceball.setPosition(Shape.getPosition().x, Shape.getPosition().y);
				force = 0;
				Forcesound.play();
			}
		}
	}
	void Player::Update(Time & watch)
	{
		x = Shape.getPosition().x;
		y = Shape.getPosition().y;
		Shield.setPosition(x - 8, y - 8);
		HP += SS;
		if (stamina < 100)
			if (watch.asMilliseconds() - regenTime.asMilliseconds() >= 100)
			{
				stamina += regenrate;
				regenTime = watch;
			}
			else if (stamina == 100)
				regenTime = watch;
		if (SS == 1)
		{
			regenrate = 2;
			playerSpeed = SpdUp;
		}
		else
		{	
			playerSpeed = 5;
			regenrate = 1.5;
		}
		if (force > 5)
			force = 5;
		if (HP > 100)
			HP = 100;
	}
	void Player::Forcemove(Window &window)
	{
		const float FORCESPEED = 20;
		float Fx = Forceball.getPosition().x, Fy = Forceball.getPosition().y;
		if (ForceState == 1)
			if (forceDirect == 1)
				Forceball.move(FORCESPEED, 0);
			else if (forceDirect == -1)
				Forceball.move(-FORCESPEED, 0);
			else if (forceDirect == 2)
				Forceball.move(0, -FORCESPEED);
			else if (forceDirect == -2)
				Forceball.move(0, FORCESPEED);
		if (Fx < 40 || Fx > window.getSize().x - 45 || Fy < 50 || Fy>  window.getSize().y - 40)
		{
			Forceball.setPosition(-100, -100);
			ForceState = 0;
		}
	}
};

class Trooper1 :public Prototype
{
public:
	Texture Enemyskin, SpecialSkin;
	Sprite EnemySprite;
	Vector2f NormalizedDirection, Direction;
	float movespeed = 1;
	bool VIP = 0;

	Trooper1::Trooper1()
	{
		height = 35; width = 35;
		Shape.setSize(Vector2f(width, height));
		HP = 25;
		SpecialSkin.loadFromFile("enemyGreen stand sprite.png");
		Enemyskin.loadFromFile("enemy stand sprite.png");
		EnemySprite.setTexture(Enemyskin);
		EnemySprite.setTextureRect(IntRect(0, 0, 50, 50));
	}
	void Trooper1::enemyMove(float Px, float Py, RenderWindow& window)
	{
		if (Shape.getPosition().x < Px && Shape.getPosition().x + 35 < window.getSize().x - 45)
			Shape.move(movespeed, 0);
		else if (Shape.getPosition().x > Px && Shape.getPosition().x > 40)
			Shape.move(-movespeed, 0);

		if (Shape.getPosition().y > Py && Shape.getPosition().y > 50)
			Shape.move(0, -movespeed);
		else if (Shape.getPosition().y < Py  && Shape.getPosition().y + 70 < window.getSize().y - 40)
			Shape.move(0, movespeed);
		else if (Shape.getPosition().y == Py)
			Shape.move(0, 0);

	}
	void Trooper1::Setsprite(int angle)
	{
		x = Shape.getPosition().x;
		y = Shape.getPosition().y;
		if (-45 <= angle && angle <= 45) //right
		{
			EnemySprite.setTextureRect(IntRect(0, 0, 50, 50));
			EnemySprite.setPosition(Shape.getPosition().x, Shape.getPosition().y - 5);
		}
		else if (45 < angle && angle < 135) //down
		{
			EnemySprite.setTextureRect(IntRect(104, 0, 45, 50));
			EnemySprite.setPosition(Shape.getPosition().x, Shape.getPosition().y - 5);
		}
		else if (-135 < angle && angle < -45)
		{
			EnemySprite.setPosition(Shape.getPosition().x, Shape.getPosition().y - 10);
			EnemySprite.setTextureRect(IntRect(55, 0, 41, 50));
		}
		else
		{
			EnemySprite.setPosition(Shape.getPosition().x - 10, Shape.getPosition().y - 5);
			EnemySprite.setTextureRect(IntRect(152, 0, 50, 50));
		}
	}
	int Trooper1::EnemyHit(RectangleShape & line, CircleShape &bullet, float way, CircleShape &Force)
	{
		if (Shape.getGlobalBounds().intersects(line.getGlobalBounds()))
		{
			HP = 0;
			line.setPosition(-100.f, -100.f);
			return 1;
		}
		else if (Shape.getGlobalBounds().intersects(bullet.getGlobalBounds()) && way == -1)
		{
			HP = 0;
			line.setPosition(-100.f, -100.f);
			return 2;
		}
		else if (Shape.getGlobalBounds().intersects(Force.getGlobalBounds()))
		{
			HP = 0;
			return 3;
		}
		else
			return 0;
	}
	int Trooper1::AngletoPayer(RectangleShape &player)
	{
		Vector2f  playerXY, enemyXY;
		int Ang;
		playerXY = Vector2f(player.getPosition().x, player.getPosition().y);
		enemyXY = Vector2f(Shape.getPosition().x, Shape.getPosition().y);
		Direction = playerXY - enemyXY;
		NormalizedDirection = Direction / sqrt(pow(Direction.x, 2) + pow(Direction.y, 2));
		Ang = atan2(NormalizedDirection.y, NormalizedDirection.x) * 180 / M_PI;
		return Ang;
	}
	void Trooper1::collision(RectangleShape &otherenemy)
	{
		if (otherenemy.getGlobalBounds().intersects(Shape.getGlobalBounds()))
		{
			if (y >= otherenemy.getPosition().y && y < otherenemy.getPosition().y + height - 5)
			{
				if (x < otherenemy.getPosition().x)
					Shape.setPosition(otherenemy.getPosition().x - width, y);
				if (x > otherenemy.getPosition().x)
					Shape.setPosition(otherenemy.getPosition().x + width, y);
			}
			else
			{
				if (y < otherenemy.getPosition().y)
					Shape.setPosition(x, otherenemy.getPosition().y - height);
				if (y >= otherenemy.getPosition().y)
					Shape.setPosition(x, otherenemy.getPosition().y + height);
			}
		}
	}
};

class Bullet
{
public:
	CircleShape Bshape;
	float way = 1;
	Vector2f Bvelocity;
	Time firetime;
	Bullet()
	{
		{
			Bshape.setRadius(5);
			Bshape.setOutlineThickness(3);
			Bshape.setOutlineColor(Color::Blue);
		}
	}
};

class element
{
public:
	Music menutheme, saberloop, JediTemple, Deathsong;
	Music Victory,chapter2,BossMusic;
	SoundBuffer Laser1;
	Sound fire1;
	SoundBuffer slash;
	Sound slashSound;
	SoundBuffer start;
	Sound startsound;
	SoundBuffer impact;
	Sound impactsound;
	SoundBuffer hit;
	Sound hitsound;
	SoundBuffer bosshitBuff;
	Sound	bosshit;

	Font scoreFont;
	int Score = 0;
	element()
	{
		scoreFont.loadFromFile("AGENCYB.TTF");
		JediTemple.openFromFile("jediTempleCut.wav");
		menutheme.openFromFile("menu theme.wav");
		saberloop.openFromFile("lightsaber loop.wav");
		Deathsong.openFromFile("Jedideath.wav");
		Victory.openFromFile("Victory (starwars main theme cut).wav");
		chapter2.openFromFile("chapter2.wav");
		BossMusic.openFromFile("boss music.wav");		
		saberloop.setVolume(50);

		Laser1.loadFromFile("clone laser1.wav"); // import Sound
		fire1.setBuffer(Laser1);
		slash.loadFromFile("swing lightsaber.wav");
		slashSound.setBuffer(slash);
		start.loadFromFile("pull lightsaber.wav");
		startsound.setBuffer(start);
		impact.loadFromFile("impactlaser.wav");
		impactsound.setBuffer(impact);
		hit.loadFromFile("saberhit.wav");
		hitsound.setBuffer(hit);
		bosshitBuff.loadFromFile("hit_marker_Sound.wav");
		bosshit.setBuffer(bosshitBuff);
	}

};

class MultiSound
{
private:

	SoundBuffer Buffer1;
	deque<Sound> sounddd;

public:
	Sound soundX;

	void Load(std::string filename)
	{
		Buffer1.loadFromFile(filename);
	}

	void Update(void)
	{
		for (int i = 0; i < sounddd.size(); ++i)
		{
			if (sounddd[i].getStatus() == Sound::Stopped)
			{
				sounddd.erase(sounddd.begin() + i);
				--i;
			}
		}
	}

	void Play(void)
	{
		sounddd.push_back(soundX);
		sounddd.back().play();
	}

	unsigned int Count()
	{
		sounddd.size();
	}
};

class TrooperBlue :public Trooper1
{
public:
	float value = 1.5;
	TrooperBlue()
	{
		Shape.setSize(Vector2f(35, 35));
		HP = 25;
		Enemyskin.loadFromFile("enemyBlue stand sprite.png");
		EnemySprite.setTexture(Enemyskin);
		EnemySprite.setTextureRect(IntRect(0, 0, 50, 50));
	}
};

class TrooperRed :public Trooper1
{
public:
	bool crew = 0;
	int movepoint;
	TrooperRed()
	{
		Shape.setSize(Vector2f(35, 35));
		HP = 25;
		Enemyskin.loadFromFile("enemyRed stand sprite.png");
		EnemySprite.setTexture(Enemyskin);
		EnemySprite.setTextureRect(IntRect(0, 0, 50, 50));
	}
	int walk()
	{
		if (x < 400 && y> 600)
			return 0;
		if (x > 1200 && y > 600)
			return 1;
	}

};

class BOSS
{
public:
	int HP = 1000;
	float x = Bsprite.getPosition().x, y = Bsprite.getPosition().y;
	Texture bosstext;
	Sprite Bsprite;
	Vector2f Barrel[4];
	int bossspeed = 2, movestate = 0;
	BOSS()
	{
		bosstext.loadFromFile("Boss sprite .png");
		Bsprite.setTexture(bosstext);
	}
	void Update(CircleShape &ball)
	{
		if (ball.getGlobalBounds().intersects(Bsprite.getGlobalBounds()))
		{
			ball.setPosition(800, 1500);
			HP -= 75;
			//cout << HP << endl;
		}
		x = Bsprite.getPosition().x,
			y = Bsprite.getPosition().y;
		setBarrelsVector();
		bossmove();
	}
	void setBarrelsVector()
	{
		Barrel[0].x = x + 15;
		Barrel[0].y = y + 110;
		Barrel[1].x = x + 150;
		Barrel[1].y = y + 110;
		Barrel[2].x = x + 490;
		Barrel[2].y = y + 110;
		Barrel[3].x = x + 600;
		Barrel[3].y = y + 100;
	}
	void bossmove()
	{
		if (x == 100)
			movestate = 1;
		if (x == 900)
			movestate = 0;
		if (x > 100 && movestate == 0)
			Bsprite.move(-bossspeed, 0);
		else if (x < 900 && movestate == 1)
			Bsprite.move(bossspeed, 0);
	}
};

class ITEM
{
public:
	Texture itemtex;
	Sprite itembox;
	int dropped = 1;
	Time randomTime, pickTime;
	int randomCD = 0, interDrop = 7000;
	ITEM()
	{
		itemtex.loadFromFile("randbox.png");
		itembox.setTexture(itemtex);
	}
	void DropCheck(unsigned int &HP, Time &watch)
	{
		if (HP <= 70 && dropped == 1)
		{
			randomTime = watch;
			randomCD = 1 + rand() % 7;
			dropped = 2;
		}
	}
	bool Chosen(Time &watch)
	{
		if (watch.asSeconds() - randomTime.asSeconds() >= randomCD && dropped == 2)
		{
			dropped = 3;
			return true;
		}
		else
			return false;
	}
	void Drop(Vector2f place)
	{
		if (dropped == 3)
			itembox.setPosition(place);

	}
	bool pickUP(RectangleShape &player)
	{
		if (player.getGlobalBounds().intersects(itembox.getGlobalBounds()))
		{
			itembox.setPosition(-111, -111);
			return true;
		}
		else
			return false;
	}
	void effect(unsigned int &HP, bool &shieldstate, bool &SS, Time &watch)
	{
		int chance = 1 + rand() % 100;
		if (chance <= 50)
		{
			HP += 50;
			//cout << "HP\n";
		}
		else
		{
			if (chance <= 75)
			{
				shieldstate = 1;
				//cout << "Shield\n";
			}
			else
			{
				SS = 1;
				//cout << "SS\n";
			}
		}
		dropped = 0;
		pickTime = watch;
	}
	void Update(bool &shieldstate, bool &SS, Time &watch)
	{
		if (dropped == 0 && watch.asMilliseconds() - pickTime.asMilliseconds() > interDrop)
		{
			shieldstate = 0;
			SS = 0;
			dropped = 1;
			//cout << "Time Up\n";
		}
	}
};

class Wall
{
public:
	RectangleShape Shape;
	float hieght, width;
	Wall()
	{
		Shape.setFillColor(Color::Black);
	}
	void sizeto(float x, float y)
	{
		Shape.setSize(Vector2f(x, y));
		width = x;
		hieght = y;
	}
	void collide_player(RectangleShape &player)
	{
		if (Shape.getGlobalBounds().intersects(player.getGlobalBounds()))
		{
			if (player.getPosition().y >= Shape.getPosition().y &&  player.getPosition().y < Shape.getPosition().y + hieght - 5)
			{
				if (player.getPosition().x < Shape.getPosition().x)
					player.setPosition(Shape.getPosition().x - 40, player.getPosition().y);
				if (player.getPosition().x > Shape.getPosition().x)
					player.setPosition(Shape.getPosition().x + width, player.getPosition().y);
			}
			else
			{
				if (player.getPosition().y < Shape.getPosition().y)
					player.setPosition(player.getPosition().x, Shape.getPosition().y - 40);
				if (player.getPosition().y >= Shape.getPosition().y)
					player.setPosition(player.getPosition().x, Shape.getPosition().y + hieght);
			}
		}
	}
};

Vector2f reflectAngle();
void stat_bar(Font &scoreFont, RenderWindow &window, int &scoreX, int &levelX, unsigned int &hpX, float &stamina, unsigned int &force);
int Mainmenu();
bool GameOverScreen(unsigned int &playerHP);
bool Scorescreen();
void Getscore(int &Newscore, string &name);
string Rank(int &Newscore);
int setIcon(RenderWindow &window);
int chapter2();
int chapter1();
int chapter3();
void showXY(RenderWindow &window);
int Exitbox(RenderWindow &window);
int Credits_Howtoplay(int &Next);

int GlobalScore = 0;

int main()
{
	srand(time(NULL));
	int Next, Return;
	while (true)
	{
		Next = Mainmenu();
		if (Next == 0)
		{
			Return = chapter1();
			if (Return == 0)
				Next = 5;
			else if (Return == 2)
				Return = chapter2();
			if (Return == 0)
				Next = 5;
			else if (Return == 3)
				Return = chapter3();
		}

		else if (Next == 1 || Next == 3)
		{
			Credits_Howtoplay(Next);
			Next = 5;
		}
		else if (Next == 2)
		{
			Scorescreen();
			Next = 5;
		}
		else if (Next == 4)
			return 0;
	}
}

Vector2f reflectAngle()
{
	int a = rand() % 100, x = 300;
	if (a >= 40)
		return Vector2f(0, 0);
	else if (a <= 20)
		return Vector2f(0, x);
	else if (20 < a && a < 40)
		return Vector2f(0, -x);
}

void stat_bar(Font &scoreFont, RenderWindow &window, int &scoreX, int &levelX, unsigned int &hpX, float &stamina, unsigned int &force)
{
	stringstream sscore, slevel;
	sscore << scoreX;
	slevel << levelX;

	Text ScoreT("Score: " + sscore.str(), scoreFont, 30);
	Text HP_T("HP:", scoreFont, 30);
	Text Stamina_T("Stamina:", scoreFont, 30);
	Text Force_T("Force:", scoreFont, 30);
	Text level_T("Leve: " + slevel.str(), scoreFont, 30); // T from Text
	Text Myname("60010106 Kheamachat Silawan", scoreFont, 24);
	Text Hints("", scoreFont, 30);

	if (levelX == 1)
		Hints.setString("[Eliminate enemies before Escape]");
	else if (levelX == 2)
		Hints.setString("[Use Force to Break a Blocker]");
	else if (levelX == 3)
		Hints.setString("[Destroy The Aircraft]");
	ScoreT.setPosition(1300, 2);
	HP_T.setPosition(75, 2);
	Stamina_T.setPosition(215, 2);
	level_T.setPosition(695, 2);
	Force_T.setPosition(425, 2);
	Myname.setFillColor(Color::Black);
	Myname.setPosition(1300, 870); window.draw(Myname);
	Hints.setPosition(815, 2);

	RectangleShape staminaBar;
	staminaBar.setSize(Vector2f(stamina, 10));
	staminaBar.setFillColor(Color::Cyan);
	staminaBar.setPosition(310, 20);

	RectangleShape HPBar;
	HPBar.setSize(Vector2f(hpX, 10));
	HPBar.setFillColor(Color::Red);
	HPBar.setPosition(110, 20);

	RectangleShape MaxBar[2];
	for (size_t i = 0; i < 2; i++)
	{
		MaxBar[i].setSize(Vector2f(100, 10));
		MaxBar[i].setFillColor(Color::Color(142, 142, 142));
	}
	RectangleShape ForceBox[5], MaxBox[5];
	for (size_t i = 0; i < 5; i++)
	{
		ForceBox[i].setSize(Vector2f(20, 20));
		ForceBox[i].setFillColor(Color::Yellow);
		ForceBox[i].setPosition(490 + i * 25, 15);
		MaxBox[i].setSize(Vector2f(20, 20));
		MaxBox[i].setFillColor(Color::Color(142, 142, 142));
		MaxBox[i].setPosition(490 + i * 25, 15);
		window.draw(MaxBox[i]);
	}
	MaxBar[0].setPosition(110, 20);
	MaxBar[1].setPosition(310, 20);
	GlobalScore = scoreX;
	for (int i = 0; i < force; i++)
		window.draw(ForceBox[i]);
	window.draw(MaxBar[0]);
	window.draw(MaxBar[1]);
	window.draw(HPBar);
	window.draw(staminaBar);
	window.draw(Stamina_T);
	window.draw(ScoreT);
	window.draw(Myname);
	window.draw(HP_T);
	window.draw(Force_T);
	window.draw(level_T);
	window.draw(Hints);
}

int Mainmenu()
{
	RenderWindow Menuwindow(VideoMode(1600, 900), "The Order66/Menu", Style::Close | Style::Fullscreen);
	Menuwindow.setFramerateLimit(60); Menuwindow.setMouseCursorVisible(false);
	setIcon(Menuwindow);
	Texture textureMenu;
	textureMenu.loadFromFile("New menu BG.png");
	Sprite Background(textureMenu);

	element stuff;

	Font logof;
	logof.loadFromFile("STJEDISE.TTF");
	Text LogoT("tHE oRDeR66", logof, 84);
	LogoT.setFillColor(Color::Black);
	LogoT.setOutlineColor(Color::Red);
	LogoT.setOutlineThickness(6);
	LogoT.setPosition(450, 640);
	Text Myname("60010106 Kheamachat Silawan", stuff.scoreFont, 24);
	Myname.setPosition(1300, 5); 
	Text menutext[5];
	for (int i = 0; i < 5; i++)
	{
		menutext[i].setFont(logof);
		menutext[i].setCharacterSize(48);
		menutext[i].setFillColor(Color::Black);
		menutext[i].setOutlineThickness(4);
	}
	menutext[0].setString("start");
	menutext[1].setString("how to play");
	menutext[2].setString("high score");
	menutext[3].setString("credits");
	menutext[4].setString("quit");

	menutext[0].setPosition(75, 820);
	menutext[1].setPosition(320, 820);
	menutext[2].setPosition(750, 820);
	menutext[3].setPosition(1100, 820);
	menutext[4].setPosition(1400, 820);
	GlobalScore = 0;
	int selected = 0;
	Clock clockMenu;
	Time tcount;
	element things;
	things.menutheme.play();
	things.menutheme.setLoop(true);
	while (Menuwindow.isOpen())
	{
		Event evnt;	 //event
		while (Menuwindow.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed || (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Escape))
			{
				Menuwindow.close();
				return 4;
			}
			if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Right)
				selected++;
			if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Left)
				selected--;
			if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Return || evnt.key.code == Keyboard::Space)
			{
				Menuwindow.close();
				return selected;
			}
		}
		if (selected > 4)
			selected = 4;
		if (selected < 0)
			selected = 0;

		tcount = clockMenu.getElapsedTime();

		Menuwindow.clear();
		Menuwindow.draw(Background);
		Menuwindow.draw(LogoT);
		for (int i = 0; i < 5; i++)
		{
			if (i != selected)
			{
				menutext[i].setOutlineColor(Color::White);
				menutext[i].setOutlineThickness(4);
			}
			Menuwindow.draw(menutext[i]);
		}
		if (tcount.asMilliseconds() <= 250)
		{
			menutext[selected].setOutlineColor(Color::Yellow);
			menutext[selected].setOutlineThickness(6);
		}
		else if (tcount.asMilliseconds() > 250 && tcount.asMilliseconds() <= 500)
		{
			menutext[selected].setOutlineColor(Color::White);
			menutext[selected].setOutlineThickness(3);
		}
		else if (tcount.asMilliseconds() > 500)
		{
			clockMenu.restart();
		}
		Menuwindow.draw(Myname);
		Menuwindow.display();
	}
}
bool GameOverScreen(unsigned int &playerHP)
{
	RenderWindow ScreenWindow(VideoMode(1600, 900), "The Order66/DeathScreen", Style::Close | Style::Fullscreen);
	ScreenWindow.setFramerateLimit(60); ScreenWindow.setMouseCursorVisible(false);
	setIcon(ScreenWindow);
	Font scoreText;
	Font jedifont;
	Texture BG;
	Sprite Background;

	scoreText.loadFromFile("CASTELAR.TTF");
	Text ScoreT("	You died!\nYour Score:" + to_string(GlobalScore), scoreText, 72);
	ScoreT.setPosition(445, 250);
	string name = "";
	element stuff;
	Text nameT("", scoreText, 56);
	nameT.setPosition(435, 450);
	if (playerHP <= 0)
		stuff.Deathsong.play();
	else
	{
		stuff.Victory.play();
		jedifont.loadFromFile("STJEDISE.TTF");
		BG.loadFromFile("lightspeed.jpg");
		Background.setTexture(BG);
		ScoreT.setString("	victory\nYour Score : " + to_string(GlobalScore));
		ScoreT.setFont(jedifont);
		ScoreT.setFillColor(Color::Black);
		ScoreT.setOutlineColor(Color::Yellow);
		ScoreT.setOutlineThickness(5);
		nameT.setFont(jedifont);
		nameT.setFillColor(Color::Black);
		nameT.setOutlineColor(Color::Yellow);
		nameT.setOutlineThickness(3);
	}
	while (ScreenWindow.isOpen())
	{
		Event evnt;
		while (ScreenWindow.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed || (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Escape))
			{
				ScreenWindow.close();
				return true;
			}
			else if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Return)
			{
				name = Rank(GlobalScore) + name;
				Getscore(GlobalScore, name);
				ScreenWindow.close();
				return false;
			}
			if (evnt.type == Event::TextEntered)
				if (evnt.text.unicode < 128 && evnt.text.unicode > 31)
					name += static_cast<char>(evnt.text.unicode);
			if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::BackSpace)
				name.clear();
		}
		nameT.setString(Rank(GlobalScore) + name);
		ScreenWindow.clear();
		ScreenWindow.draw(Background);
		ScreenWindow.draw(nameT);
		ScreenWindow.draw(ScoreT);
		ScreenWindow.display();
	}
}

bool Scorescreen()
{
	RenderWindow ScreenWindow(VideoMode(1600, 900), "The Order66/ScoreScreen", Style::Close | Style::Fullscreen);
	ScreenWindow.setFramerateLimit(60); ScreenWindow.setMouseCursorVisible(false);
	setIcon(ScreenWindow);

	Font Scorefont;
	Scorefont.loadFromFile("STJEDISE.TTF");

	int score[5];
	ifstream InscoreFile;
	InscoreFile.open("Top5Score.txt");
	InscoreFile >> score[0] >> score[1] >> score[2] >> score[3] >> score[4];
	InscoreFile.close();

	string playernames[5];
	ifstream namefile;
	namefile.open("Top5Name.txt");
	namefile >> playernames[0] >> playernames[1] >> playernames[2] >> playernames[3] >> playernames[4];
	namefile.close();

	Text ScoreT[5], NamesT[5];
	for (int i = 0; i < 5; i++)
	{
		ScoreT[i].setString(to_string(score[i]));
		ScoreT[i].setFont(Scorefont);
		ScoreT[i].setCharacterSize(36);
		ScoreT[i].setPosition(1200, 270 + 100 * i);

		NamesT[i].setString(to_string(i + 1) + ")" + playernames[i]);
		NamesT[i].setFont(Scorefont);
		NamesT[i].setCharacterSize(36);
		NamesT[i].setPosition(300, 270 + 100 * i);
	}

	Text Head("ToP 5 Players", Scorefont, 84);
	Head.setFillColor(Color::Color(0, 0, 0, 0));
	Head.setOutlineColor(Color::Yellow);
	Head.setOutlineThickness(1);
	Head.setPosition(475, 70);

	element stuff;
	stuff.menutheme.setLoop(true);
	stuff.menutheme.play();

	while (ScreenWindow.isOpen())
	{
		Event evnt;
		while (ScreenWindow.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed || (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Escape))
			{
				ScreenWindow.close();
				return true;
			}
			else if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Return)
			{
				ScreenWindow.close();
				return false;
			}
		}

		ScreenWindow.clear();
		for (size_t i = 0; i < 5; i++)
		{
			ScreenWindow.draw(ScoreT[i]);
			ScreenWindow.draw(NamesT[i]);
		}
		ScreenWindow.draw(Head);
		ScreenWindow.display();
	}
}
void Getscore(int &Newscore, string &name)
{
	int score[6];
	ifstream InscoreFile;
	InscoreFile.open("Top5Score.txt");
	InscoreFile >> score[5] >> score[4] >> score[3] >> score[2] >> score[1];
	InscoreFile.close();
	score[0] = Newscore;

	string playernames[6];
	ifstream namefile;
	namefile.open("Top5Name.txt");
	namefile >> playernames[5] >> playernames[4] >> playernames[3] >> playernames[2] >> playernames[1];
	namefile.close();
	playernames[0] = name;

	int temp;
	string stemp;
	for (int a = 0; a < 6; a++)
	{
		for (int b = 0; b < 6; b++)
		{
			if (score[a] < score[b])
			{
				temp = score[a];
				score[a] = score[b];
				score[b] = temp;

				stemp = playernames[a];
				playernames[a] = playernames[b];
				playernames[b] = stemp;
			}
		}
	}
	ofstream scoreFile;
	scoreFile.open("Top5Score.txt");
	for (int i = 5; i > 0; i--)
		scoreFile << score[i] << endl;
	scoreFile.close();

	ofstream nameFile;
	nameFile.open("Top5Name.txt");
	for (int i = 5; i > 0; i--)
		nameFile << playernames[i] << endl;
	nameFile.close();
}
string Rank(int &Newscore)
{
	string rank[4] = { "Youngling.","Padawan.","Knight.","Master." };
	int i, grade[3] = { 4000,7500,15000 };
	for (i = 0; i < 3; i++)
	{
		if (Newscore <= grade[i])
			break;
	}
	return rank[i];
}

int setIcon(RenderWindow& window)
{
	Image icon;
	icon.loadFromFile("icon66.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	return 0;
}

int chapter1()
{
	RenderWindow window(VideoMode(1600, 900), "The Order66/Chapter 1", Style::Close | Style::Fullscreen);
	window.setFramerateLimit(60); window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	setIcon(window);
	Texture texturelv0;
	texturelv0.loadFromFile("New chapter edit.png");
	Sprite Background(texturelv0);

	element stuff;
	MultiSound laserimpact;

	Bullet b;
	vector<Bullet>Bs;
	Trooper1 enemyT1;
	vector<Trooper1>T1s;
	ITEM item;

	Time passtime = milliseconds(0), duration = milliseconds(800), watch, Tspawn, countdown;
	Clock attackClock, clock1, CDclock;

	float Bulletspeed = 10;
	int Pface = 1, LevelX = 1, px, py, MaxEnemy = 2, enemycount = 0, Po_i[7], WaveTime = 90, Interspawn = 2000;
	int dis = 300;

	laserimpact.soundX.setBuffer(stuff.impact);
	item.itembox.setPosition(-111, -111);
	Player player;
	player.Shape.setPosition(640, 360);
	player.Psprite.setPosition(player.Shape.getPosition().x, player.Shape.getPosition().y);
	player.Forceball.setPosition(3000, 3000);

	RectangleShape door[4], standPo[8], exit(Vector2f(100, 60)), BlackBox(Vector2f(110, 40));

	exit.setPosition(750, 862);
	exit.setFillColor(Color::Color(249, 95, 64));
	BlackBox.setFillColor(Color::Black);

	for (int i = 0; i < 8; i++)
	{
		standPo[i].setSize(Vector2f(30, 30));
		if (i < 4)
		{
			door[i].setSize(Vector2f(20, 20));
			door[i].setFillColor(Color::Color(102, 51, 0));
		}
	}
	{
		door[0].setPosition(15, 190);
		door[1].setPosition(15, 540);
		door[2].setPosition(1560, 400);
		door[3].setPosition(1560, 720);
	}

	Text Time_T("Time: " + to_string(WaveTime), stuff.scoreFont, 36);
	Time_T.setPosition(745, 45);
	BlackBox.setPosition(Time_T.getPosition().x - 3, Time_T.getPosition().y);
	stuff.JediTemple.setLoop(true);
	stuff.JediTemple.play();
	stuff.saberloop.setLoop(true);
	stuff.saberloop.play();
	stuff.startsound.play();
	while (window.isOpen())
	{
		Event evnt;	 //event
		watch = clock1.getElapsedTime();

		while (window.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed || (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Escape))
			{
				stuff.JediTemple.pause();
				stuff.saberloop.pause();
				if (Exitbox(window) == 1)
				{
					window.close();
					return 0;
				}
				else
				{
					stuff.JediTemple.play();
					stuff.saberloop.play();
				}
			}
		}
		passtime = attackClock.getElapsedTime();
		countdown = CDclock.getElapsedTime();
		px = player.Shape.getPosition().x;
		py = player.Shape.getPosition().y;

		if (WaveTime > 0 && countdown.asSeconds() >= 1)
		{
			WaveTime--;
			Time_T.setString("Time: " + to_string(WaveTime));
			CDclock.restart();
		}
		else if (WaveTime == 0)
		{
			exit.setSize(Vector2f(100, 70));
			exit.setPosition(750, 852);
			exit.setFillColor(Color::Color(162, 224, 76));
			MaxEnemy = 0;
			if (exit.getGlobalBounds().intersects(player.Shape.getGlobalBounds()))
			{
				window.close();
				return 2;
			}
		}
		if (WaveTime == 75)
			MaxEnemy = 3;
		else if (WaveTime == 60)
		{
			Interspawn = 1000;
			MaxEnemy = 4;
		}
		else if (WaveTime == 45)
			MaxEnemy = 5;
		if (GlobalScore > 5500 && WaveTime > 0)
			MaxEnemy = 7;

		{
			standPo[0].setPosition(px + dis, py);
			standPo[1].setPosition(px + dis, py + dis);
			standPo[2].setPosition(px, py + dis);
			standPo[3].setPosition(px - dis, py + dis);
			standPo[4].setPosition(px - dis, py);
			standPo[5].setPosition(px - dis, py - dis);
			standPo[6].setPosition(px, py - dis);
			standPo[7].setPosition(px + dis, py - dis);
		}
		//showXY(window);
		if (enemycount < MaxEnemy && watch.asMilliseconds() - Tspawn.asMilliseconds()> Interspawn)
		{
			T1s.push_back(Trooper1(enemyT1));
			if (item.Chosen(watch))
			{
				T1s[T1s.size() - 1].EnemySprite.setTexture(enemyT1.SpecialSkin);
				T1s[T1s.size() - 1].VIP = 1;
				cout << "Chose!\n";
			}
			T1s[T1s.size() - 1].Shape.setPosition(door[rand() % 4].getPosition());
			enemycount++;
			Tspawn = clock1.getElapsedTime();
			Po_i[T1s.size() - 1] = rand() % 8;
		}

		for (size_t i = 0; Bs.size() < T1s.size(); i++)
		{
			Bs.push_back(Bullet(b));
			Bs[Bs.size() - 1].Bshape.setPosition(T1s[T1s.size() - 1].Shape.getPosition() + Vector2f(17.5, 17.5));
		}

		Pface = player.pmove(window, player.Shape, Pface, player.Psprite);
		player.useforce(Pface); player.Forcemove(window);
		if (player.attack(player.Shape, attackClock, player.line, Pface))
		{
			passtime += attackClock.getElapsedTime();
			if (stuff.slashSound.getStatus() != stuff.slashSound.Playing)
				stuff.slashSound.play();
		}

		for (size_t i = 0; i < T1s.size(); i++)
		{
			T1s[i].enemyMove(standPo[Po_i[i]].getPosition().x, standPo[Po_i[i]].getPosition().y, window);
			T1s[i].Setsprite(T1s[i].AngletoPayer(player.Shape));
			for (size_t j = 0; j < T1s.size(); j++)
				T1s[i].collision(T1s[j].Shape);
			if (Bs[i].Bshape.getPosition().x > window.getSize().x - 30 || Bs[i].Bshape.getPosition().x < 30 || Bs[i].Bshape.getPosition().y < 40 || Bs[i].Bshape.getPosition().y > window.getSize().y - 35
				|| (Bs[i].Bshape.getGlobalBounds().intersects(player.Shield.getGlobalBounds()) && player.ShieldState == 1))
			{
				Bs[i].Bshape.setPosition(2700, 2700);
				Bs[i].way = 1;
				if (watch.asSeconds() - Bs[i].firetime.asSeconds() >= 1 + rand() % 3)
					Bs[i].Bshape.setPosition(T1s[i].Shape.getPosition() + Vector2f(17.5, 17.5));
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.line.getGlobalBounds()))
			{
				Bs[i].way = -1;
				T1s[i].NormalizedDirection = (T1s[i].Direction + reflectAngle()) / sqrt(pow(T1s[i].Direction.x, 2) + pow(T1s[i].Direction.y, 2));
				Bs[i].Bvelocity = 2 * Bulletspeed*Bs[i].way *T1s[i].NormalizedDirection;
				laserimpact.Play();
				Bs[i].Bshape.move(Bs[i].Bvelocity);
				if (player.stamina >= 20)
					player.stamina -= 20;
				player.force++;
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.Forceball.getGlobalBounds()))
			{
				Bs[i].way = -1;
				T1s[i].NormalizedDirection = (T1s[i].Direction + reflectAngle()) / sqrt(pow(T1s[i].Direction.x, 2) + pow(T1s[i].Direction.y, 2));
				Bs[i].Bvelocity = 2 * Bulletspeed*Bs[i].way *T1s[i].NormalizedDirection;
				Bs[i].Bshape.move(Bs[i].Bvelocity);
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.Shape.getGlobalBounds()))
			{
				Bs[i].Bshape.setPosition(2700, 2700);
				if (player.HP > 0)
					player.HP -= 10;
			}
			if (Bs[i].Bshape.getGlobalBounds().intersects(T1s[i].Shape.getGlobalBounds()) == true && Bs[i].way == 1)
			{
				Bs[i].Bvelocity = Bulletspeed*Bs[i].way*T1s[i].NormalizedDirection;
				stuff.fire1.play();
				Bs[i].firetime = watch;
			}
			for (size_t k = 0; k < Bs.size(); k++)
			{
				int waytodie = T1s[i].EnemyHit(player.line, Bs[k].Bshape, Bs[k].way, player.Forceball);
				if (waytodie == 1)
				{
					if (player.stamina >= 20)
						player.stamina -= 20;
					player.force++;
				}
				else if (waytodie == 2)
				{
					Bs.erase(Bs.begin() + k);
					GlobalScore += 50;
				}
				else if (waytodie == 3)
					GlobalScore += 50;
				if (T1s[i].HP <= 0)
				{
					if (T1s[i].VIP == true)
						item.Drop(T1s[i].Shape.getPosition());
					stuff.hitsound.play();
					GlobalScore += 100;
					T1s.erase(T1s.begin() + i);
					if (enemycount == 3)
						Tspawn = clock1.getElapsedTime();
					enemycount--;
					break;
				}
			}
		}
		if (item.pickUP(player.Shape))
			item.effect(player.HP, player.ShieldState, player.SS, watch);
		player.Update(watch);
		laserimpact.Update();
		item.DropCheck(player.HP, watch);
		item.Update(player.ShieldState, player.SS, watch);
		/*	cout << item.randomCD << "	Random CD" << endl;
		cout << item.randomTime.asSeconds() << "	Random Time" << endl;
		cout << item.pickTime.asSeconds() << "	pick Time" << endl;
		cout << item.dropped << "	drop stat" << endl;*/

		window.clear();
		window.draw(Background);
		//for (int i = 0; i < 4; i++)
		//	window.draw(door[i]);
		//	window.draw(player.Shape); 
		if (player.changeAct == 0)
		{
			window.draw(player.Psprite);
			player.line.setPosition(-100.f, -100.f);
		}
		else
			window.draw(player.Psprite2);
		for (size_t i = 0; i < Bs.size(); i++)
		{
			Bs[i].Bshape.move(Bs[i].Bvelocity);
			window.draw(Bs[i].Bshape);
		}
		for (size_t i = 0; i < T1s.size(); i++)
		{
			//window.draw(T1s[i].Shape);
			window.draw(T1s[i].EnemySprite);
		}
		if (passtime > duration)
		{
			player.changeAct = 0;
			player.line.setPosition(-100.f, -100.f);
		}
		//window.draw(player.line);
		//for (int i = 0; i < 8; i++)
		//	window.draw(standPo[i]);
		window.draw(player.Forceball);
		if (player.ShieldState == 1)
			window.draw(player.Shield);
		window.draw(BlackBox);
		window.draw(Time_T);
		window.draw(exit);
		window.draw(item.itembox);
		stat_bar(stuff.scoreFont, window, GlobalScore, LevelX, player.HP, player.stamina, player.force);
		window.display();
		if (player.HP == 0)
		{
			stuff.JediTemple.stop();
			stuff.saberloop.stop();
			window.close();
			break;
		}
	}
	return GameOverScreen(player.HP);
}

int chapter2()
{
	RenderWindow window(VideoMode(1600, 900), "The Order66/Chapter 2", Style::Close | Style::Fullscreen);
	window.setFramerateLimit(60); window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	setIcon(window);
	Texture texturelv0;
	texturelv0.loadFromFile("Newmapchapter2.png");
	Sprite Background(texturelv0);

	element stuff;
	MultiSound laserimpact;

	Bullet b;
	vector<Bullet>Bs;
	TrooperBlue enemyPrototype;
	vector<TrooperBlue>T2s;
	ITEM item;
	Wall walls[5];
	Time passtime = milliseconds(0), duration = milliseconds(800), watch, Tspawn;
	Clock attackClock, clock1, CDclock;

	float Bulletspeed = 10;
	int Pface = 1, LevelX = 2, MaxEnemy = 6, enemycount = 0, Interspawn = 1500;
	int dis = 350, gamestate = 0, HPBlock = 750, Po_i[7];


	laserimpact.soundX.setBuffer(stuff.impact);
	item.itembox.setPosition(-111, -111);
	Player player;
	player.Shape.setPosition(160, 70);
	player.Psprite.setPosition(player.Shape.getPosition().x, player.Shape.getPosition().y);
	player.Forceball.setPosition(3000, 3000);
	enemyPrototype.Shape.setPosition(3000, 2555);
	RectangleShape door[4], standPo[8], exit(Vector2f(100, 60)), BlockerHPBar(Vector2f(750, 20)), maxBlockHP(Vector2f(750, 20));

	for (int i = 0; i < 8; i++)
	{
		standPo[i].setSize(Vector2f(30, 30));
		if (i < 4)
		{
			door[i].setFillColor(Color::Magenta);
			door[i].setSize(Vector2f(20, 20));
			door[i].setFillColor(Color::Color(102, 51, 0));
		}
		if (i < 5)
			walls[i].sizeto(30, 100);
	}
	{
		exit.setPosition(1550, 440);
		exit.setFillColor(Color::Color(162, 224, 76));
		maxBlockHP.setPosition(450, 870);
		maxBlockHP.setFillColor(Color::Color(66, 66, 66));
		BlockerHPBar.setPosition(450, 870);
		BlockerHPBar.setFillColor(Color::Color(0, 95, 163));
		door[0].setPosition(1450, 50);
		door[1].setPosition(1450, 840);
		door[2].setPosition(200, 45);
		door[3].setPosition(200, 840);
		walls[4].sizeto(340, 10);

		walls[3].sizeto(40, 180);
		walls[3].Shape.setFillColor(Color::Color(8, 141, 75));

		walls[0].Shape.setPosition(200, 405);
		walls[1].Shape.setPosition(660, 38);
		walls[2].Shape.setPosition(660, 761);
		walls[3].Shape.setPosition(1520, 385);
		walls[4].Shape.setPosition(1000, 465);


		standPo[0].setPosition(150, 150);
		standPo[1].setPosition(150, 685);
		standPo[2].setPosition(500, 160);
		standPo[3].setPosition(500, 685);
		standPo[4].setPosition(1412, 335);
		standPo[5].setPosition(1412, 535);
	}
	stuff.saberloop.setLoop(true);
	stuff.saberloop.play();
	stuff.startsound.play();
	stuff.chapter2.setLoop(true);
	stuff.chapter2.play();
	while (window.isOpen())
	{
		Event evnt;	 //event
		watch = clock1.getElapsedTime();

		while (window.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed || (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Escape))
			{
				stuff.chapter2.pause();
				stuff.saberloop.pause();
				if (Exitbox(window) == 1)
				{
					window.close();
					return 0;
				}
				else
				{
					stuff.saberloop.play();
					stuff.chapter2.play();
				}
			}
		}

		passtime = attackClock.getElapsedTime();
		{
			standPo[6].setPosition(player.x - 100, 200);
			standPo[7].setPosition(player.x + 100, 700);
		}

		if (enemycount < MaxEnemy && watch.asMilliseconds() - Tspawn.asMilliseconds()> Interspawn)
		{
			T2s.push_back(TrooperBlue(enemyPrototype));
			if (item.Chosen(watch))
			{
				T2s[T2s.size() - 1].EnemySprite.setTexture(enemyPrototype.SpecialSkin);
				T2s[T2s.size() - 1].VIP = 1;
			}
			T2s[T2s.size() - 1].Shape.setPosition(door[rand() % 4].getPosition());
			enemycount++;
			Tspawn = clock1.getElapsedTime();
		}


		for (size_t i = 0; Bs.size() < T2s.size(); i++)
		{
			Bs.push_back(Bullet(b));
			Bs[Bs.size() - 1].Bshape.setPosition(T2s[T2s.size() - 1].Shape.getPosition() + Vector2f(17.5, 17.5));
		}

		//showXY(window);

		Pface = player.pmove(window, player.Shape, Pface, player.Psprite);
		player.useforce(Pface); player.Forcemove(window);
		if (player.attack(player.Shape, attackClock, player.line, Pface))
		{
			passtime += attackClock.getElapsedTime();
			if (stuff.slashSound.getStatus() != stuff.slashSound.Playing)
				stuff.slashSound.play();
		}

		for (size_t i = 0; i < T2s.size(); i++)
		{
			T2s[i].Setsprite(T2s[i].AngletoPayer(player.Shape));
			T2s[i].enemyMove(standPo[i].getPosition().x, standPo[i].getPosition().y, window);
			if (Bs[i].Bshape.getPosition().x > window.getSize().x - 35 || Bs[i].Bshape.getPosition().x < 35 || Bs[i].Bshape.getPosition().y < 35 || Bs[i].Bshape.getPosition().y > window.getSize().y - 35
				|| (Bs[i].Bshape.getGlobalBounds().intersects(player.Shield.getGlobalBounds()) && player.ShieldState == 1))
			{
				Bs[i].Bshape.setPosition(2700, 2700);
				Bs[i].way = 1;
				if (watch.asSeconds() - Bs[i].firetime.asSeconds() >= 1.5 + rand() % 3)
					Bs[i].Bshape.setPosition(T2s[i].Shape.getPosition() + Vector2f(17.5, 17.5));
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.line.getGlobalBounds()))
			{
				Bs[i].way = -1;
				T2s[i].NormalizedDirection = (T2s[i].Direction + reflectAngle()) / sqrt(pow(T2s[i].Direction.x, 2) + pow(T2s[i].Direction.y, 2));
				Bs[i].Bvelocity = 2 * Bulletspeed*Bs[i].way *T2s[i].NormalizedDirection;
				laserimpact.Play();
				Bs[i].Bshape.move(Bs[i].Bvelocity);
				if (player.stamina >= 20)
					player.stamina -= 20;
				player.force++;
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.Forceball.getGlobalBounds()))
			{
				Bs[i].way = -1;
				T2s[i].NormalizedDirection = (T2s[i].Direction + reflectAngle()) / sqrt(pow(T2s[i].Direction.x, 2) + pow(T2s[i].Direction.y, 2));
				Bs[i].Bvelocity = 2 * Bulletspeed*Bs[i].way *T2s[i].NormalizedDirection;
				Bs[i].Bshape.move(Bs[i].Bvelocity);
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.Shape.getGlobalBounds()))
			{
				Bs[i].Bshape.setPosition(2700, 2700);
				if (player.HP > 0)
					player.HP -= 10;
			}

			if (Bs[i].Bshape.getGlobalBounds().intersects(T2s[i].Shape.getGlobalBounds()) == true && Bs[i].way == 1)
			{
				Bs[i].Bvelocity = Bulletspeed*Bs[i].way*T2s[i].NormalizedDirection;
				stuff.fire1.play();
				Bs[i].firetime = watch;
			}
			for (int j = 0; j < 5; j++)
			{
				if (Bs[i].Bshape.getGlobalBounds().intersects(walls[j].Shape.getGlobalBounds()))
					Bs[i].Bshape.setPosition(2700, 2700);
				walls[j].collide_player(T2s[i].Shape);
			}
			for (size_t k = 0; k < Bs.size(); k++)
			{
				int waytodie = T2s[i].EnemyHit(player.line, Bs[k].Bshape, Bs[k].way, player.Forceball);
				if (waytodie == 1)
				{
					if (player.stamina >= 20)
						player.stamina -= 20;
					player.force++;
				}
				else if (waytodie == 2)
				{
					Bs.erase(Bs.begin() + k);
					GlobalScore += 50;
				}
				else if (waytodie == 3)
					GlobalScore += 50;
				if (T2s[i].HP <= 0)
				{
					if (T2s[i].VIP == true)
					{
						item.Drop(T2s[i].Shape.getPosition());
						GlobalScore += 100;
					}
					else
						GlobalScore += 150;
					T2s.erase(T2s.begin() + i);
					stuff.hitsound.play();
					enemycount--;
					break;
				}
			}
		}
		for (int i = 0; i < 5; i++)
		{
			walls[i].collide_player(player.Shape);
			if (walls[i].Shape.getGlobalBounds().intersects(player.Forceball.getGlobalBounds()))
			{
				player.Forceball.setPosition(-555, -555);
				if (i == 3)
					HPBlock -= 75;
			}
		}
		BlockerHPBar.setSize(Vector2f(HPBlock, 20));

		if (HPBlock <= 0)
		{
			HPBlock = 0;
			walls[3].Shape.setPosition(-666, -666);
		}
		if (exit.getGlobalBounds().intersects(player.Shape.getGlobalBounds()))
		{
			window.close();
			return 3;
		}
		if (item.pickUP(player.Shape))
			item.effect(player.HP, player.ShieldState, player.SS, watch);
		player.Update(watch);
		laserimpact.Update();
		item.DropCheck(player.HP, watch);
		item.Update(player.ShieldState, player.SS, watch);

		window.clear();
		window.draw(Background);
		window.draw(maxBlockHP);
		window.draw(BlockerHPBar);
		for (int i = 0; i < 8; i++)
		{
			//window.draw(standPo[i]);
			if (i < 5)
				window.draw(walls[i].Shape);
			//if (i < 4)
			//	window.draw(door[i]);
		}
		//window.draw(player.Shape);
		if (player.changeAct == 0)
		{
			window.draw(player.Psprite);
			player.line.setPosition(-100.f, -100.f);
		}
		else
			window.draw(player.Psprite2);
		for (size_t i = 0; i < Bs.size(); i++)
		{
			Bs[i].Bshape.move(Bs[i].Bvelocity);
			window.draw(Bs[i].Bshape);
		}
		for (size_t i = 0; i < T2s.size(); i++)
		{
			//window.draw(T2s[i].Shape);
			window.draw(T2s[i].EnemySprite);
		}
		if (passtime > duration)
		{
			player.changeAct = 0;
			player.line.setPosition(-100.f, -100.f);
		}
		//window.draw(player.line);
		window.draw(player.Forceball);
		if (player.ShieldState == 1)
			window.draw(player.Shield);
		window.draw(exit);
		window.draw(item.itembox);
		stat_bar(stuff.scoreFont, window, GlobalScore, LevelX, player.HP, player.stamina, player.force);
		window.display();
		if (player.HP == 0)
		{
			stuff.chapter2.stop();
			stuff.saberloop.stop();
			window.close();
			break;
		}
	}
	return GameOverScreen(player.HP);
}

int chapter3()
{
	RenderWindow window(VideoMode(1600, 900), "The Order66/Chapter 3", Style::Close | Style::Fullscreen);
	window.setFramerateLimit(60); window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	setIcon(window);
	Texture texturelv0;
	texturelv0.loadFromFile("mapchapter3.png");
	Sprite Background(texturelv0);

	element stuff;
	MultiSound laserimpact,bosshit;

	Bullet b;
	vector<Bullet>Bs;
	TrooperRed enemyPrototype;
	vector<TrooperRed>Reds;
	ITEM item;
	Wall walls[4];
	Time passtime = milliseconds(0), duration = milliseconds(800), watch, Tspawn;
	Clock attackClock, clock1, CDclock;
	BOSS boss;
	

	float Bulletspeed = 15;
	int Pface = 1, LevelX = 3, MaxEnemy = 1, enemycount = 0, Interspawn = 3500;
	bool setup = 0;
	laserimpact.soundX.setBuffer(stuff.impact);
	bosshit.soundX.setBuffer(stuff.bosshitBuff);
	item.itembox.setPosition(-111, -111);
	Player player;
	player.Shape.setPosition(640, 700);
	player.Psprite.setPosition(player.Shape.getPosition().x, player.Shape.getPosition().y);
	player.Forceball.setPosition(3000, 3000);
	player.SpdUp = 5;

	RectangleShape door[2], standPo[2], BossmaxHP(Vector2f(boss.HP, 20)), Bar(Vector2f(boss.HP, 20));
	for (int i = 0; i < 3; i++)
	{
		walls[i].Shape.setPosition(300 + 400 * i, 600);
		walls[i].Shape.setFillColor(Color::Color(83, 83, 83));
		walls[i].sizeto(160, 50);
		if (i < 2)
		{
			standPo[i].setSize(Vector2f(30, 30));
			door[i].setSize(Vector2f(20, 20));
		}
	}
	{
		walls[0].Shape.setPosition(0, 1800);
		walls[2].Shape.setPosition(0, 1800);
		BossmaxHP.setFillColor(Color::Color(63, 63, 63));
		BossmaxHP.setPosition(300, 95);
		Bar.setFillColor(Color::Red);
		Bar.setPosition(300, 95);
		walls[3].sizeto(1600, 100);
		door[0].setPosition(40, 680);
		door[1].setPosition(1550, 680);
		standPo[1].setPosition(1450, 680);
		standPo[0].setPosition(140, 680);
		walls[3].Shape.setPosition(0, 460);
		boss.Bsprite.setPosition(300, 150);
	}

	stuff.saberloop.setLoop(true);
	stuff.saberloop.play();
	stuff.startsound.play();
	stuff.BossMusic.play();
	stuff.BossMusic.setLoop(true);
	while (window.isOpen())
	{
		Event evnt;	 //event
		watch = clock1.getElapsedTime();

		while (window.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed || (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Escape))
			{
				stuff.saberloop.pause();
				stuff.BossMusic.pause();
				if (Exitbox(window) == 1)
				{
					window.close();
					return 0;
				}
				else
				{
					stuff.saberloop.play();
					stuff.BossMusic.play();
				}
			}
		}
		passtime = attackClock.getElapsedTime();

		if (enemycount < MaxEnemy && watch.asMilliseconds() - Tspawn.asMilliseconds()> Interspawn)
		{
			Reds.push_back(TrooperRed(enemyPrototype));
			Reds.back().movepoint = rand() % 2;
			if (item.Chosen(watch))
			{
				Reds.back().EnemySprite.setTexture(enemyPrototype.SpecialSkin);
				Reds.back().VIP = 1;
			}
			Reds.back().Shape.setPosition(door[Reds.back().movepoint].getPosition());
			enemycount++;
		}

		for (size_t i = 0; Bs.size() < Reds.size(); i++)
		{
			Bs.push_back(Bullet(b));
			Bs[Bs.size() - 1].Bshape.setPosition(Reds[Reds.size() - 1].Shape.getPosition() + Vector2f(17.5, 17.5));
		}

		Pface = player.pmove(window, player.Shape, Pface, player.Psprite);
		player.useforce(Pface);
		player.Forcemove(window);
		if (player.attack(player.Shape, attackClock, player.line, Pface))
		{
			passtime += attackClock.getElapsedTime();
			if (stuff.slashSound.getStatus() != stuff.slashSound.Playing)
				stuff.slashSound.play();
		}


		for (size_t i = 0; i < Reds.size(); i++)
		{
			if (Reds[i].crew == true)
				Reds[i].Shape.setPosition(boss.Barrel[i]);
			if (i == 4)
				Reds[i].enemyMove(standPo[Reds[i].movepoint].getPosition().x, standPo[Reds[i].movepoint].getPosition().y, window);
			Reds[i].Setsprite(Reds[i].AngletoPayer(player.Shape));
			Bs[i].Bvelocity = Bulletspeed*Bs[i].way*Reds[i].NormalizedDirection;
			for (size_t j = 0; j < Reds.size(); j++)
				Reds[i].collision(Reds[j].Shape);
			if (Bs[i].Bshape.getPosition().x > window.getSize().x - 30 || Bs[i].Bshape.getPosition().x < 30 || Bs[i].Bshape.getPosition().y < 40 || Bs[i].Bshape.getPosition().y > window.getSize().y - 35
				|| (Bs[i].Bshape.getGlobalBounds().intersects(player.Shield.getGlobalBounds()) && player.ShieldState == 1))
			{
				Bs[i].Bshape.setPosition(2700, 2700);
				Bs[i].way = 1;
				if (watch.asSeconds() - Bs[i].firetime.asSeconds() >= 1+ (rand() % 4) / 2)
					Bs[i].Bshape.setPosition(Reds[i].Shape.getPosition() + Vector2f(17.5, 17.5));
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.line.getGlobalBounds()))
			{
				Bs[i].way = -1;
				Reds[i].NormalizedDirection = (Reds[i].Direction + reflectAngle()) / sqrt(pow(Reds[i].Direction.x, 2) + pow(Reds[i].Direction.y, 2));
				Bs[i].Bvelocity = 2 * Bulletspeed*Bs[i].way *Reds[i].NormalizedDirection;
				laserimpact.Play();
				Bs[i].Bshape.move(Bs[i].Bvelocity);
				if (player.stamina >= 25)
					player.stamina -= 25;
				player.force++;
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(boss.Bsprite.getGlobalBounds()) && Bs[i].way == -1)
			{
				Bs[i].Bshape.setPosition(2700, 2700);
				boss.HP -= 10;
				bosshit.Play();
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.Forceball.getGlobalBounds()))
			{
				Bs[i].way = -1;
				Reds[i].NormalizedDirection = (Reds[i].Direction + reflectAngle()) / sqrt(pow(Reds[i].Direction.x, 2) + pow(Reds[i].Direction.y, 2));
				Bs[i].Bvelocity = 2 * Bulletspeed*Bs[i].way *Reds[i].NormalizedDirection;
				Bs[i].Bshape.move(Bs[i].Bvelocity);
			}
			else if (Bs[i].Bshape.getGlobalBounds().intersects(player.Shape.getGlobalBounds()))
			{
				Bs[i].Bshape.setPosition(2700, 2700);
				if (player.HP > 0)
					player.HP -= 10;
			}
			if (Bs[i].Bshape.getGlobalBounds().intersects(Reds[i].Shape.getGlobalBounds()) == true && Bs[i].way == 1)
			{
				stuff.fire1.play();
				Bs[i].firetime = watch;
			}
			for (int j = 0; j < 3; j++)
			{
				if (Bs[i].Bshape.getGlobalBounds().intersects(walls[j].Shape.getGlobalBounds()))
					Bs[i].Bshape.setPosition(2700, 2700);
			}
			for (size_t k = 0; k < Bs.size(); k++)
			{
				int waytodie = Reds[i].EnemyHit(player.line, Bs[k].Bshape, Bs[k].way, player.Forceball);
				if (waytodie == 1)
				{
					if (player.stamina >= 20)
						player.stamina -= 20;
					player.force++;
				}
				else if (waytodie == 2)
				{
					Bs.erase(Bs.begin() + k);
					GlobalScore += 50;
				}
				else if (waytodie == 3)
					GlobalScore += 50;
				if (Reds[i].HP <= 0)
				{
					if (Reds[i].VIP == true)
						item.Drop(Reds[i].Shape.getPosition());
					stuff.hitsound.play();
					GlobalScore += 200;
					Reds.erase(Reds.begin() + i);
					enemycount--;
					Tspawn = clock1.getElapsedTime();
					break;
				}
			}
		}
		if (item.pickUP(player.Shape))
			item.effect(player.HP, player.ShieldState, player.SS, watch);
		player.Update(watch);
		laserimpact.Update();
		bosshit.Update();
		item.DropCheck(player.HP, watch);
		item.Update(player.ShieldState, player.SS, watch);
		boss.Update(player.Forceball);
		Bar.setSize(Vector2f(boss.HP, 20));
		if (boss.HP <= 0)
		{
			boss.HP = 0;
			GlobalScore += 5000;
			Bs.clear();
			Reds.clear();
			break;
		}
		if (setup == 0)
		{
			for (int i = 0; i < 4; i++)
			{
				Reds.push_back(TrooperRed(enemyPrototype));
				Reds[i].Shape.setPosition(boss.Barrel[i]);
				Reds[i].crew = true;
			}
			setup = 1;
		}
		for (int i = 0; i < 3; i++)
		{
			if (walls[i].Shape.getGlobalBounds().intersects(player.Forceball.getGlobalBounds()))
				player.Forceball.setPosition(-3000, -3000);
		}

		window.clear();
		window.draw(Background);
		for (int i = 0; i < 4; i++)
		{
			walls[i].collide_player(player.Shape);
			if (i < 3)
				window.draw(walls[i].Shape);
			/*if (i < 2)
			{
				window.draw(standPo[i]);
				window.draw(door[i]);
			}*/
		}

		//	window.draw(player.Shape); 
		if (player.changeAct == 0)
		{
			window.draw(player.Psprite);
			player.line.setPosition(-100.f, -100.f);
		}
		else
			window.draw(player.Psprite2);
		for (size_t i = 0; i < Bs.size(); i++)
		{
			Bs[i].Bshape.move(Bs[i].Bvelocity);
			window.draw(Bs[i].Bshape);
		}
		for (size_t i = 0; i < Reds.size(); i++)
		{
			//window.draw(T1s[i].Shape);
			if (i == 4)
				window.draw(Reds[i].EnemySprite);
		}
		if (passtime > duration)
		{
			player.changeAct = 0;
			player.line.setPosition(-100.f, -100.f);
		}
		//window.draw(player.line);
		window.draw(player.Forceball);
		if (player.ShieldState == 1)
			window.draw(player.Shield);
		window.draw(BossmaxHP);
		window.draw(Bar);
		window.draw(boss.Bsprite);
		window.draw(item.itembox);
		stat_bar(stuff.scoreFont, window, GlobalScore, LevelX, player.HP, player.stamina, player.force);
		window.display();
		if (player.HP == 0)
			break;
	}
	stuff.BossMusic.pause();
	stuff.saberloop.stop();
	window.close();
	return GameOverScreen(player.HP);
}
void showXY(RenderWindow &window)
{
	float x = Mouse::getPosition(window).x, y = Mouse::getPosition(window).y;
	cout << "X:" << x << "	Y:" << y << endl;
}

int Exitbox(RenderWindow &window)
{
	element stuff;
	Text YesNo[3];
	string words[3] = { "Exit?","Yes","No" };
	for (int i = 0; i < 3; i++)
	{
		YesNo[i].setFont(stuff.scoreFont);
		YesNo[i].setCharacterSize(48);
		YesNo[i].setString(words[i]);
	}
	YesNo[0].setStyle(Text::Bold);
	YesNo[0].setPosition(750, 360);
	YesNo[1].setPosition(705, 435);
	YesNo[2].setPosition(820, 435);

	RectangleShape whitebox(Vector2f(240, 170));
	whitebox.setOutlineColor(Color::Red);
	whitebox.setOutlineThickness(10);
	whitebox.setPosition(665, 340);
	int exit = 1;
	Event evnt;
	while (true)
	{
		while (window.pollEvent(evnt))
		{
			if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Return || evnt.key.code == Keyboard::Space)
				return exit;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
			exit = 2;
		if (Keyboard::isKeyPressed(Keyboard::Left))
			exit = 1;

		for (int i = 0; i < 3; i++)
		{
			YesNo[i].setFillColor(Color::White);
			YesNo[i].setOutlineColor(Color::Black);
			YesNo[i].setOutlineThickness(5);
			YesNo[exit].setFillColor(Color::Green);
		}
		window.draw(whitebox);
		for (int i = 0; i < 3; i++)
			window.draw(YesNo[i]);
		window.display();
	}
}

int Credits_Howtoplay(int & Next)
{
	RenderWindow ScreenWindow(VideoMode(1600, 900), "The Order66/ScoreScreen", Style::Close | Style::Fullscreen);
	ScreenWindow.setFramerateLimit(60); ScreenWindow.setMouseCursorVisible(false);
	setIcon(ScreenWindow);
	Texture TextCredit, TextHow;
	TextCredit.loadFromFile("credit.png");
	TextHow.loadFromFile("how2play.PNG");
	Sprite Background;
	if (Next < 3)
		Background.setTexture(TextHow);
	else
		Background.setTexture(TextCredit);

	element stuff;
	stuff.menutheme.setLoop(true);
	stuff.menutheme.play();
	while (ScreenWindow.isOpen())
	{
		Event evnt;
		while (ScreenWindow.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed || (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Escape))
			{
				ScreenWindow.close();
				return true;
			}
			else if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Return)
			{
				ScreenWindow.close();
				return false;
			}
		}

		ScreenWindow.clear();
		ScreenWindow.draw(Background);
		ScreenWindow.display();
	}
}



