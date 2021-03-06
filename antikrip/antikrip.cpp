#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class Object {
public:
	sf::Texture texture;
	sf::Sprite image;
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
	float mass = 0;
	sf::Vector2f velocity;
	bool moving = false;
	bool can_collide = false;
	bool onGround = false;

	Object(string filename) {
		texture.loadFromFile(filename);
		image.setTexture(texture);
		width = texture.getSize().x;
		height = texture.getSize().y;
		image.setOrigin(width / 2, height / 2);

		x = image.getPosition().x;
		y = image.getPosition().y;

		velocity = sf::Vector2f();
		velocity.x = 0.;
		velocity.y = 0.;
	};
	~Object() {};
	void Move(float new_x, float new_y) {
		image.setPosition(new_x, new_y);
		x = new_x;
		y = new_y;
	};
	void setImage(string filename) {
		texture.loadFromFile(filename);
		image.setTexture(texture);
	};
	void setScale(float x_scale, float y_scale) {
		image.setScale(x_scale, y_scale);
		width *= x_scale;
		height *= y_scale;
	};

};

int window_width = 1280;
int window_height = 720;
bool GameActive = false;

vector <Object> objects;
Object background("background.jpg");
Object player("antikrip.png");
Object background_menu("start_menu.jpg");
Object sun("sun.png");
Object wall("tower.png"); 
Object platform("creep.png");
Object hitpoint_wall("1.png");
Object hitpoint_player("playerhp.png");

void update(bool right, bool left, bool up) {
	// падение игрока
	if (objects[1].velocity.y < 550) {
		objects[1].velocity.y = objects[1].mass * (objects[1].velocity.y + 9.8 * 0.03);
		objects[1].Move(objects[1].velocity.x, objects[1].mass * (objects[1].velocity.y + 9.8 * 0.03));
		
	}
	// вправо
	if (right && (objects[1].velocity.x < window_width - objects[1].width / 2 - 2)) {

		objects[1].setScale(4, 4);
		objects[1].width = 128;
		objects[1].velocity.x += 8;
		objects[1].Move(objects[1].velocity.x + 5, objects[1].velocity.y);
	}
	// влево
	if (left && (objects[1].velocity.x > objects[1].width / 2 + 2)) {

		objects[1].setScale(-4, 4);
		objects[1].width = -128;
		objects[1].velocity.x -= 8;
		objects[1].Move(objects[1].velocity.x - 5, objects[1].velocity.y);
	}
	// прыжок
	if (up) {
		if ((objects[1].velocity.y > 540) || (objects[1].velocity.y > 489 && objects[1].velocity.y < 491) || (objects[1].velocity.y >350 && objects[1].velocity.y < 370)) {
			objects[1].velocity.y -= 300;
			objects[1].Move(objects[1].velocity.x, objects[1].velocity.y - 300);
		}
	}
	return;
}
void attack(double difrnce, int& j) {
	j++;
	if (j == 2) objects[6].setImage("2.png");
	else if (j == 3) objects[6].setImage("3.png");
	else if (j == 4) objects[6].setImage("4.png");
	else if (j == 5) objects[6].setImage("5.png");
	else {
		objects[4].velocity.x = 0;
		objects[4].velocity.y = 1000;
		objects[4].Move(0, 1000);
		j = -10;
	}
	return;
}
void game();

void menu() {
	RenderWindow window_menu(VideoMode(window_width, window_height), "SFML menu", Style::Titlebar);
	Texture menuTexture1, menuTexture2, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("111.png");
	menuTexture2.loadFromFile("333.png");
	aboutTexture.loadFromFile("about.png");
	menuBackground.loadFromFile("start_menu.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 30);
	menu2.setPosition(100, 90);
	menuBg.setPosition(0, 0);
	about.setPosition(200, 100);
	Music music_theme;//создаем объект музыки
	music_theme.openFromFile("theme.ogg");//загружаем файл
	music_theme.play();//воспроизводим музыку
	while (window_menu.isOpen())
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu2.setColor(Color::White);
		menuNum = 0;
		window_menu.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window_menu))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window_menu))) { menu2.setColor(Color::Blue); menuNum = 2; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) {
				isMenu = false; //если нажали первую кнопку, то выходим из меню 
				music_theme.stop();
				window_menu.close();
				game();
			}
			if (menuNum == 2) {
				window_menu.close();
				isMenu = false;
			}

		}

		window_menu.draw(menuBg);
		window_menu.draw(menu1);
		window_menu.draw(menu2);

		window_menu.display();
	}
	return;
}

void game() {
	Vector2i mouse_position;
	int j = 1;
	bool right = false, left = false, up = false, mouse = false;
	double difrnce = abs(objects[4].velocity.x + objects[4].width / 2 - objects[1].velocity.x + objects[1].width / 2);
	SoundBuffer hit_buffer;
	hit_buffer.loadFromFile("hit.ogg");
	Sound hit_sound;
	hit_sound.setBuffer(hit_buffer);

	SoundBuffer nohit_buffer;
	nohit_buffer.loadFromFile("nohit.ogg");
	Sound nohit_sound;
	nohit_sound.setBuffer(nohit_buffer);

	SoundBuffer stun_buffer;
	stun_buffer.loadFromFile("stun.ogg");
	Sound stun_sound;
	stun_sound.setBuffer(stun_buffer);

	RenderWindow window_game(VideoMode(window_width, window_height), "SFML game", Style::Titlebar);
	//Солнце
	objects[3].Move(window_width / 2, sun.height / 2);

	// Фон
	objects[2].setScale(0, 0);
	objects[0].Move(window_width / 2, window_height / 2);

	//Персонаж
	objects[1].setScale(4, 4);
	objects[1].mass = 1;
	objects[1].velocity.x = 500;
	objects[1].velocity.y = 550;
	objects[1].Move(500, 550);
	// Стена
	objects[4].velocity.x = 300;
	objects[4].velocity.y = 550;
	objects[4].Move(300, 550);

	// Платформа
	objects[5].velocity.x = 900;
	objects[5].velocity.y = 550; 
	objects[5].Move(900, 550);

	// Хитпоинты стены
	objects[6].velocity.x = objects[4].velocity.x;
	objects[6].velocity.y = objects[4].velocity.y - 50;
	objects[6].Move(objects[4].velocity.x, objects[4].velocity.y - 50);

	// Хитпоинты игрока
	objects[7].velocity.x = objects[1].velocity.x;
	objects[7].velocity.y = objects[1].velocity.y - 100;
	objects[7].Move(objects[1].velocity.x, objects[1].velocity.y - 100);

	//Добавление текста 
	Font font;//шрифт 
	font.loadFromFile("font.otf");//передаем нашему шрифту файл шрифта
	Text GameTitel("", font, 200), EnterStart("", font, 50), Keys("", font, 25);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	GameTitel.setStyle(Text::Bold);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	EnterStart.setStyle(Text::Bold);
	Keys.setStyle(Text::Bold);

	//Звуки 
	Music music_game;//создаем объект музыки
	music_game.openFromFile("game.ogg");//загружаем файл
	music_game.play();//воспроизводим музыку
	while (window_game.isOpen()) {
		//printf("x = %f, xw = %f\n", objects[1].velocity.x, objects[4].velocity.x);
		Event event;
		while (window_game.pollEvent(event)) {
			mouse_position = Mouse::getPosition(window_game);
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					mouse = true;
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					mouse = false;
				}
			}
			if (event.type == Event::KeyPressed) {
				//Выход
				if (event.key.code == Keyboard::Escape) {
					window_game.close();
					music_game.stop();
					menu();
				}
				//Вправо
				else if (event.key.code == Keyboard::D) {
					right = true;
				}
				//Влево
				else if (event.key.code == Keyboard::A) {
					left = true;
				}
				//Прыжок
				else if (event.key.code == Keyboard::W) {
					up = true;
				}
				else if (event.key.code == Keyboard::E) {
					difrnce = objects[1].velocity.x - objects[4].velocity.x - 170;
					if (difrnce > 50 || j == -10) {
						nohit_sound.play();
					}
					if (j == 5) {
						stun_sound.play();
					}
					if (difrnce <= 50 && j != -10) {
						hit_sound.play();
						attack(difrnce, j);
					}
					
				}
			}
		}
		// Хитпоинты стены
		objects[6].velocity.x = objects[4].velocity.x;
		objects[6].velocity.y = objects[4].velocity.y - 200;
		objects[6].Move(objects[4].velocity.x, objects[4].velocity.y - 200);
		// Хитпоинты игрока
		objects[7].velocity.x = objects[1].velocity.x;
		objects[7].velocity.y = objects[1].velocity.y - 200;
		objects[7].Move(objects[1].velocity.x, objects[1].velocity.y - 200);
		// перетаскивание игрока
		if (mouse && objects[1].image.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
			objects[1].velocity.x = mouse_position.x;
			objects[1].velocity.y = mouse_position.y;
		}
		// границы окна
		if (objects[1].velocity.x < 64) {
			objects[1].velocity.x = 68;
		}
		if (objects[1].velocity.x > window_width) {
			objects[1].velocity.x = window_width - objects[1].width;
		}
		if (objects[1].velocity.y < 64) {
			objects[1].velocity.y = 68;
		}
		// коллизия стены
		if (objects[1].image.getGlobalBounds().intersects(objects[4].image.getGlobalBounds())) {
			if (objects[1].velocity.x > objects[4].velocity.x) {
				objects[1].velocity.x = objects[4].velocity.x + objects[4].width + 30;
			}
			if (objects[1].velocity.x < objects[4].velocity.x) {
				objects[1].velocity.x = objects[4].velocity.x - objects[4].width - 30;
			}
		}

		// коллизия платформы
		if (objects[5].image.getGlobalBounds().intersects(objects[1].image.getGlobalBounds())) {
			if (objects[1].velocity.y < objects[5].velocity.y - 112) {
				if (mouse && objects[1].image.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
					objects[1].velocity.x = mouse_position.x;
					objects[1].velocity.y = mouse_position.y;
				}
				else {
					objects[1].velocity.y = objects[5].velocity.y - objects[5].height - 27;
				}
			}
			else {
				if ((objects[1].velocity.x < objects[5].velocity.x)) {
					objects[1].velocity.x = objects[5].velocity.x - objects[5].width - 30;
				}
				if ((objects[1].velocity.x > objects[5].velocity.x)) {
					objects[1].velocity.x = objects[5].velocity.x + objects[5].width + 30;
				}
			}
		}
		update(right, left, up);
		right = false;
		left = false;
		up = false;
		window_game.clear();
		// бэкграунд
		window_game.draw(objects[0].image);
		window_game.draw(objects[3].image);
		for (int i = 0; i < int(objects.size()); i++) if (i != 0 && i != 3) window_game.draw(objects[i].image);
		window_game.display();
	}
	return;
}


int main()
{
	setlocale(LC_ALL, "rus");
	objects.push_back(background); //0
	objects.push_back(player);//1
	objects.push_back(background_menu); //2
	objects.push_back(sun); //3
	objects.push_back(wall); //4
	objects.push_back(platform); //5
	objects.push_back(hitpoint_wall); //6
	objects.push_back(hitpoint_player); //7
	for (int i = 0; i < 30; i++) cout << endl;
	menu();//вызов меню
	return 0;
}