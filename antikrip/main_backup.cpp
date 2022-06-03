#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

//Размер окна
int window_width = 1280,
    window_height = 720;

//Меню игры и сама игра
bool GameActive = false;

void menu(RenderWindow& window) {
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("111.png");
	menuTexture2.loadFromFile("222.png");
	menuTexture3.loadFromFile("333.png");
	aboutTexture.loadFromFile("images/about.png");
	menuBackground.loadFromFile("start_menu.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 30);
	menu2.setPosition(100, 90);
	menu3.setPosition(100, 150);
	menuBg.setPosition(345, 0);

	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			else if (menuNum == 2) { 
				window.draw(about); 
				window.display(); 
				while (!Keyboard::isKeyPressed(Keyboard::Escape)); 
			}
			else if (menuNum == 3) { 
				window.close(); 
				isMenu = false; 
			}

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
	return;
}

class Object {
public:
	Texture texture;
	Sprite sprite;

	float x = 0.,
		  y = 0.,
		  velocity_x, velocity_y,
		  width = 0.,
		  height = 0.,
		  mass = 0.;
	bool moving = false,
	     onGround,
	     can_collide = false;
	FloatRect rect;
	float currentFrame = 0;

	Object(string filename) {
		texture.loadFromFile(filename);
		sprite.setTexture(texture);
		sprite.setOrigin(width / 2, height / 2);
		rect = FloatRect(0, 0, 45, 67);

		width = texture.getSize().x;
		height = texture.getSize().y;

		x = sprite.getPosition().x;
		y = sprite.getPosition().y;

		velocity_x = 0;
		velocity_y = 0;
	}

	void update(float time) {
		
		if (onGround == false) {
			velocity_y = (velocity_y + 0.05 * time)*mass;
			rect.top += velocity_y * time;
		}
		onGround = false;
		if (rect.top >= 440) onGround = true;
		if (velocity_y < 0) {
			velocity_y = (velocity_y + 0.05*time)*mass;
			rect.top += velocity_y*time;
		}
		currentFrame += 0.05 * time;
		if (currentFrame > 3) currentFrame -= 3;
		Move(rect.left,rect.top);
		return;
	}

	void keys(RenderWindow& window, Event& event, bool& GameActive, float& time) {
		if (GameActive) {
			//Vector2i mouse_position = Mouse::getPosition(window);
			if (event.type == sf::Event::MouseButtonPressed) {
			//	if (event.mouseButton.button == Mouse::Left && sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
			//		onGround = false;
			//		velocity_x = mouse_position.x - 100;
			//		velocity_y = mouse_position.y - 100;
			//		Move(mouse_position.x - 100, mouse_position.y - 100);
			//		cout << "OBJ CAUGHT!\n" << "vel.x = " << mouse_position.x << "\nvel.y = " << mouse_position.y << "\nsprite.x = " << x << "\nsprite.y = " << y << "\ntime = " << time << endl;
					//cout << "OBJ CAUGHT!\n" << "vel.x = " << velocity_x << "\nvel.y = " << velocity_y << "\nsprite.x = " << x << "\nsprite.y = " << y << endl;
			//	}
			}
			else if (event.type == Event::KeyPressed) {
				switch (event.key.code) {
				//Выход
				case Keyboard::Escape: {
					GameActive = false;
					menu(window);
					//window.close(); 
					break; 
				}
				//Вправо
				case Keyboard::D: {
					if (!(rect.left > window_width - rect.width * 4)) rect.left += 10;
					velocity_x = 2;
					sprite.setTextureRect(IntRect(45 * int(currentFrame), 0, 45, 67));
					break;
				}
				//Влево
				case Keyboard::A: {
					if (!(rect.left < 0)) rect.left -= 10;
					velocity_x = -2;
					sprite.setTextureRect(IntRect(45 * int(currentFrame) + 45, 0, -45, 67));
					break;
				}
				//Прыжок
				case Keyboard::W: {
					if (onGround) {
						onGround = false;
						velocity_y = -3;
					}
					break;
				}
				}
			}
			else if (event.type == Event::KeyReleased) {
				// Если отпустил клавишу вправо
				if (event.key.code == Keyboard::D) {
					sprite.setTextureRect(IntRect(45 * 1, 0, 45, 67));
				}
				// Если отпустил клавишу влево
				else if (event.key.code == Keyboard::A) {
					sprite.setTextureRect(IntRect(45 * 1 + 45, 0, -45, 67));
				}
			}
		}
		else if (event.type == Event::KeyPressed) {
			//Начать игру
			if (event.key.code == Keyboard::Enter) {
				GameActive = true;
				sprite.setTextureRect(IntRect(45 * 1, 0, 45, 67));
				Move(rect.left, rect.top);
			}
		}
		return;
	}


	void Move(float new_x, float new_y) {
		sprite.setPosition(new_x, new_y);
		x = new_x;
		y = new_y;
	};
	void setsprite(string filename, float , int xp, int yp) {
		texture.loadFromFile(filename);
		sprite.setTexture(texture);
	};
	void setScale(float x_scale, float y_scale) {
		sprite.setScale(x_scale, y_scale);
		width *= x_scale;
		height *= y_scale;
	};

};

//-----------------------------------------------------------------------------------------------------------------------------
vector <Object> objects;
Object background("background.png");
Object player("use_characters.png");
Object background_menu("start_menu.png");
Object sun("sun.png");

 //Все текстовые блоки
void NameGame(Text& GameTitel, RenderWindow& window) {
	GameTitel.setString("Sun");//задает строку тексту
	GameTitel.setPosition(window_width / 2.8, 0);//задаем позицию текста, центр камеры
	GameTitel.setStyle(Text::Bold);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	window.draw(GameTitel);//рисую этот текст
	return;
}

void StartGame(Text& EnterStart, RenderWindow& window) {
	EnterStart.setString("Press ENTER to Sun");//задает строку тексту
	EnterStart.setPosition(window_width / 3, window_height / 2);//задаем позицию текста, центр камеры
	window.draw(EnterStart);//рисую этот текст
	return;
}

void Keys_help(Text& Keys, RenderWindow& window) {
	Keys.setString(" Keys: \n Left - A \n Right - D \n Jump - W \n Exit - ESP");//задает строку тексту
	Keys.setPosition(50, window_height / 1.7);//задаем позицию текста, центр камеры
	window.draw(Keys);//рисую этот текст
	return;
}

	////////////////////////////////////////////////////
int main()
{
	setlocale(LC_ALL, "rus");
	RenderWindow window(VideoMode(window_width, window_height), " SFML game ", Style::Titlebar);

	menu(window);//вызов меню

	objects.push_back(background); //0
	objects.push_back(player);//1
	objects.push_back(background_menu); //2
	objects.push_back(sun); //3

	//Номера объектов
	int bg = 0,         //Задний фон
		plr = 1,        //Игрок
		bg_menu = 2,   //Задний фон меню
		sun_best = 3;    

	//Солнце
	objects[sun_best].Move(window_width / 2- objects[sun_best].width/2, 0);
	objects[sun_best].setScale(1, 1);
	
	//Персонаж
	objects[plr].setScale(4, 4);
	objects[plr].mass = 1;

	//Добавление текста 
	Font font;//шрифт 
	font.loadFromFile("font.otf");//передаем нашему шрифту файл шрифта
	Text GameTitel("", font, 200), EnterStart("", font, 50), Keys("", font, 25);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	GameTitel.setStyle(Text::Bold);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	EnterStart.setStyle(Text::Bold);
	Keys.setStyle(Text::Bold);

	//Звуки 
	Music music_theme, music_game;//создаем объект музыки
	music_theme.openFromFile("theme.ogg");//загружаем файл
	music_game.openFromFile("game.ogg");//загружаем файл
	music_theme.play();//воспроизводим музыку

	Clock clock;//перeменная времени
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();//дать прошедшее время в микросек
		clock.restart();//перезагружает время
		time /= 8000;//скорость игры

		if (time > 60) time = 60;
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			objects[plr].keys(window, event, GameActive,time);
		}
		
		Vector2i mouse_position = Mouse::getPosition(window);
		if (event.type == sf::Event::MouseButtonPressed) {
			cout << " PRESSED!\n";
			if (event.mouseButton.button == Mouse::Left && objects[plr].sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
				//objects[plr].onGround = false;
				objects[plr].velocity_x = mouse_position.x - 10;
				objects[plr].velocity_y = mouse_position.y - 10;
				objects[plr].Move(mouse_position.x - 10, mouse_position.y - 10);
				cout << "OBJ CAUGHT!\n" << "vel.x = " << mouse_position.x << "\nvel.y = " << mouse_position.y << "\nsprite.x = " << objects[plr].x << "\nsprite.y = " << objects[plr].y << "\ntime = " << time << endl;
			}
		}
		// Вызов обновления и физики
		if (GameActive) {
			music_theme.stop();
			objects[bg_menu].setScale(0, 0);
			//objects[plr].update(time);
			window.clear();
			
			// Отрисовка всех объектов в векторе
			for (int i = 0; i < int(objects.size()); i++) window.draw(objects[i].sprite);

		}
		//Отрисовка текста игры
		else if (!GameActive) {
			music_game.play();
			window.draw(objects[bg_menu].sprite);
			NameGame(GameTitel, window);
			StartGame(EnterStart, window);
			Keys_help(Keys, window);
		}
		// Отобразить все
		window.display();
	}
	return 0;
}