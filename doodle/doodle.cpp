#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using namespace sf;

struct point { int x, y; };
class MovePlatform {
private:
	float x, y, w, h;
	bool left = false; // если true то движется влево
	String File;
	Image image;
	Texture texture;

public:
	Sprite sprite; // отрисованный персонаж
	MovePlatform(float x_pos, float y_pos, float w_pl, float h_pl) { // конструктор
		x = x_pos;
		y = y_pos;
		h = h_pl;
		w = w_pl;
		image.loadFromFile("images/moveplat.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));

		sprite.setPosition(x, y);
	}
	void setPos(float x_pos, float y_pos) { //задаёт нужную позицию
		x = x_pos;
		y = y_pos;
		sprite.setPosition(x_pos, y_pos);
	}
	void updatePlatform(float x_pers, float y_pers, float dy, float time, float h_pers, float w_pers, float jump) {

		if (jump && y_pers < 300) { //позиция, при которой движется карта
			y += dy;
		}

		if (y > 1000) { //новая позиция при выходе за карту
			y = -1000;
			image.loadFromFile("images/moveplat.png");
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		if (!left) { //движение
			if (x + w >= 532) {
				left = true;
			}
			x += 0.3 * time;
		}
		else {
			if (x <= 0) {
				left = false;
			}
			x -= 0.3 * time;
		}

		sprite.setPosition(x, y);
	}
	int isCollusion(float x_pers, float y_pers, float h_pers, float w_pers, float jump) { //проверяем коллизию с персонажем
		if (y > 0 && !jump && (y_pers + h_pers * 0.9 > y && y_pers + h_pers * 0.9 < h + y) && ((x <= x_pers + w_pers * 0.333 && x_pers + w_pers * 0.333 <= x + w) || (x <= x_pers + w_pers * 0.666 && x_pers + w_pers * 0.666 <= x + w))) {
			return 1;

		}
		else {
			return 0;
		}
	}

};
class BrokenPlatform {
private:
	float x, y, w, h;

	String File;
	Image image;
	Texture texture;
	int state = 0;
public:
	bool hasJetpack = false; // привязан ли джетпак
	Sprite sprite;
	BrokenPlatform(String F, float X, float Y, float W, float H) {
		File = F;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);
	}
	void setPlatform(float pos_x, float pos_y) { // позиция
		x = pos_x;
		y = pos_y;
		sprite.setPosition(pos_x, pos_y);
	}
	void getPlat(float* pos_x, float* pos_y, float* width, float* height) { // получение параметрпов класса
		*pos_x = x;
		*pos_y = y;
		*width = w;
		*height = h;
	}
	void updatePlatform(float x_pers, float y_pers, float dy, float time, float h_pers, float w_pers, float jump) {
		if (state == 0) {// если стейт == 0 то платформа стоит, если 1 то она ломается
			if (jump && y_pers < 300) {
				y += dy;// проверка передвижения карты
			}

			if (y > 0 && !jump && (y_pers + h_pers * 0.9 > y && y_pers + h_pers * 0.9 < h + y) && ((x <= x_pers + w_pers * 0.333 && x_pers + w_pers * 0.333 <= x + w) || (x <= x_pers + w_pers * 0.666 && x_pers + w_pers * 0.666 <= x + w))) {
				image.loadFromFile("images/brokenplat2.png"); // проверка колизии и загрузка нового изображения
				texture.loadFromImage(image);
				sprite.setTexture(texture);
				sprite.setTextureRect(IntRect(0, 0, w, h));
				state = 1;// платформа обранилась

			}

		}
		else if (state == 1) {// прибавляем движение дудлера
			if (jump && y_pers < 300) {
				y += dy;
			}
			y += 0.5 * time;

		}
		if (y > 1000) {// если ушла вниз возраждаем платформу
			y = -600;
			x = rand() % 534;
			state = 0;
			image.loadFromFile("images/brokenplat1.png");
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		sprite.setPosition(x, y);// задаем позицию
	}
};
class Jetpack {
private:
	float x, y, w, h;
	Image image;
	Texture texture;
public:
	Sprite sprite;
	Jetpack(float pos_x, float pos_y) { //конструктор как везде
		w = 40;
		h = 59;
		x = pos_x;
		y = pos_y;
		image.loadFromFile("images/jetbase.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);
	}
	int isCollusion(float pos_x, float pos_y, float h_pers, float w_pers) { // проверка соприкосновения с джетпаком
		if (!((pos_x + w_pers < x) || (pos_x > x + w)) && !((pos_y + h_pers < y) || (pos_y >= y + h))) {

			return true;
		}
		else {

			return false;
		}
	}
	void setJetpack(float pos_x, float pos_y) { //смена позиции джетпака
		x = pos_x;
		y = pos_y;
		sprite.setPosition(pos_x, pos_y);
	}
};

class Enemy {
private:
	float x, y, speed = 1, w, h;

	Image image;
	Texture texture;

public:
	Sprite sprite;
	Enemy(float pos_x, float pos_y) { //конструктор
		w = 139;
		h = 85;
		x = pos_x;
		y = pos_y;
		image.loadFromFile("images/besson.png");
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);
	}
	void setPos(float pos_x, float pos_y) { // смена позиции
		x = pos_x;
		y = pos_y;
		sprite.setPosition(x, y);
	}
	void getPos(float* pos_x, float* pos_y) { // получение позиции
		*pos_x = x;
		*pos_y = y;
	}
	int isCollusion(float x_pers, float y_pers, float h_pers, float w_pers, int jump) { // проверка коллизии, если сверху, то возвращаем 1, если не сверху,то 2, если нет коллизии ,то 0
		if (y > 0 && !jump && (y_pers + h_pers > y && y_pers + h_pers < h + y) && ((x <= x_pers && x_pers <= x + w) || (x <= x_pers + w_pers && x_pers + w_pers <= x + w))) {
			x = rand() % 534;
			setPos(x, -5000);
			return 1;
		}
		if (!((x_pers + w_pers < x) || (x_pers > x + w)) && !((y_pers + h_pers < y) || (y_pers >= y + h))) {
			return 2;
		}
		return 0;
	}
	int isDead(int x_g, int y_g, int w_g, int h_g) { // проверка, не попала ли пуля во врага
		if ((x_g <= x + w && x_g >= x - w) && (y_g <= y + h && y_g >= y - h) || y > 1000) {

			x = rand() % 534;
			y = -5000;
			setPos(x, y);

			return 1;
		}
		return 0;
	}

};

class Gun {
private:
	float x, y, speed = 1, w, h;

	Image image;
	Texture texture;

public:
	Sprite sprite;
	Gun(float pos_x, float pos_y) { // конструктор
		w = 40;
		h = 33;
		x = pos_x;
		y = pos_y;
		image.loadFromFile("images/bullet.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);

	}

	float get_y() {
		return y;
	}
	float get_x() {
		return x;
	}
	void update(float time) { // полёт
		y -= speed * time;
		sprite.setPosition(x, y);

	}
	~Gun() {

	}
};

class Guns {
private:
	std::vector<Gun*> Arr; //создаём вектор пуль
public:
	void update(float time, Enemy* enem) {
		for (int i = 0; i
			< Arr.size(); i++) {
			Arr[i]->update(time);
			if (Arr[i]->get_y() < -10) {
				Arr[i]->~Gun();
				Arr.erase(Arr.begin() + i);
				i--;
			}// удаляем элемент,если он вышел за пределы карты
			else if (enem->isDead(Arr[i]->get_x(), Arr[i]->get_y(), 40, 33) == 1) {
				Arr[i]->~Gun();
				Arr.erase(Arr.begin() + i);
				i--;
				enem->setPos(rand() % 534, -5000); // удаляем элемент, если столкнулся с монстром
			}
		}
	}
	void fire(float pos_x, float pos_y) {
		Gun* n = new Gun(pos_x + 60, pos_y); //создаём пулю и добавляем в вектор пуль
		Arr.push_back(n);
	}
	int getSize() { // забираем длину
		return Arr.size();
	}
	Gun* getItem(int i) { // получам нужный экземпляр
		return Arr[i];
	}
};

class Score {
private:
	int maxValue; // максимальное значение
	float value; // текущее значение
public:
	Score() {
		value = 0;
		maxValue = 0;
	}
	void setScore(float val) { // проверяем и записываем если надо счётчик
		value -= val;
		if (value > maxValue) {
			maxValue = value;

		}

	}
	int getScore() { // возвращаем значение
		return maxValue;
	}

};

class Platform {
private:
	float x, y, w, h;

	String File;
	Image image;
	Texture texture;

public:
	bool hasJetpack = false;
	Sprite sprite;//СЃС„РјР» СЃРїСЂР°Р№С‚
	Platform(String F, float X, float Y, float W, float H) {
		File = F;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);
	}
	void setPlatform(float pos_x, float pos_y) { // позиция
		x = pos_x;
		y = pos_y;
		sprite.setPosition(pos_x, pos_y);
	}
	void getPlat(float* pos_x, float* pos_y, float* width, float* height) { //получение данных
		*pos_x = x;
		*pos_y = y;
		*width = w;
		*height = h;
	}
	void setJetpack(bool is) { // присваиваем платформе джетпак
		hasJetpack = is;
	}
};

class Player {
private:
	bool dead = false; // жив персонаж или нет
public:
	float x, y, w, h, dx, dy, speed = 0, speed_Y = 0, jump = false, timeLast = 0;
	float current = 0;
	int dir = 0;
	int left = 0;
	int jetFire = 0;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, float X, float Y, float W, float H) {
		w = W; h = H; //задаём ширину и высоту
		File = F;
		image.loadFromFile("images/" + File);// загружаем изображение
		image.createMaskFromColor(Color(41, 33, 59));//убираем синий цвет
		texture.loadFromImage(image);// задаём текстуру
		sprite.setTexture(texture);// задаём спрайт
		x = X; y = Y;// координаты
		sprite.setTextureRect(IntRect(0, 0, w, h)); // форма спрайта
	}
	void setSpeed(float val) { // задаём скорость
		speed_Y = val;
	}
	void update(float time, Platform** platforms, Score* score, Enemy* enem, Jetpack* jet, BrokenPlatform* broke, MovePlatform* movePlat) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		float dx = 0, dy = 0;
		if (timeLast > 2000) { // проверка времени и обновление персонажа при полёте на ракете
			current = 0;
			image.loadFromFile("images/Doodle.png");
			image.createMaskFromColor(Color(41, 33, 59));
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		if (current == 0) { // если персонаж в состоянии прыжков
			timeLast = 0;
			if (!jump) { // физика прыжка
				dy += speed_Y * time + 0.002 * time * time / 2;
				speed_Y += 0.002 * time;
				//timeLast += time;
			}
			else {
				dy += speed_Y * time - 0.002 * time * time / 2;
				speed_Y += 0.002 * time;
				if (speed_Y > 0) {
					jump = false;
				}
			}

			int collusionMove = movePlat->isCollusion(x, y, h, w, jump); // проверка коллизии с движущейся платформой
			if (collusionMove) {
				jump = true;
				speed_Y = -1;
			}

		}
		else if (current == 1) {// полёт на ракете

			if ((int(timeLast) / 400) % 2 == 0) { // смена изображения для красоты
				image.loadFromFile("images/jetpack1.png");
				image.createMaskFromColor(Color(41, 33, 59));
				texture.loadFromImage(image);
				sprite.setTexture(texture);
				sprite.setTextureRect(IntRect(0, 0, w, h));
			}
			else {
				image.loadFromFile("images/jetpack2.png");
				image.createMaskFromColor(Color(41, 33, 59));
				texture.loadFromImage(image);
				sprite.setTexture(texture);
				sprite.setTextureRect(IntRect(0, 0, w, h));
			}
			speed_Y = -1.5;


			dy = time * speed_Y;
			jump = true;
			timeLast += time;
		}
		broke->updatePlatform(x, y, -dy, time, h, w, jump); // обновление
		movePlat->updatePlatform(x, y, -dy, time, h, w, jump);// обновление
		score->setScore(dy); // передаём изменения по у в счёт

		for (int i = 0; i < 20; i++) { // проверка коллизии для каждой платформы
			float x_pl, y_pl, width_pl, height_pl;
			platforms[i]->getPlat(&x_pl, &y_pl, &width_pl, &height_pl);
			if (y_pl > 0 && !jump && (y + h * 0.9 - y_pl > 0 && y + h * 0.9 - y_pl < height_pl) && ((x_pl <= x + w * 0.333 && x + w * 0.333 <= x_pl + width_pl) || (x_pl <= x + w * 0.666 && x + w * 0.666 <= x_pl + width_pl))) {
				\
					speed_Y = -1;
				y = y_pl - h;
				jump = true;
				break;
			}
		}
		if (y < 300 && jump) {// если координата дудлера меньше чем 300пкс от начала, то двигается враг
			float x_en, y_en;
			enem->getPos(&x_en, &y_en);
			enem->setPos(x_en, y_en - dy);
		}
		if (!current) { // если мы не летим на ракете, то проверяем коллизию с монстром
			int colEnemy = enem->isCollusion(x, y, h, w, jump);
			if (colEnemy == 1) {
				speed_Y = -1.5;
			}
			if (colEnemy == 2) {
				dead = true;
			}
		}


		for (int i = 0; i < 20; i++) {
			float x_pl, y_pl, width_pl, height_pl;
			platforms[i]->getPlat(&x_pl, &y_pl, &width_pl, &height_pl);
			if (y < 300 && jump) { // если координата дудлера меньше чем 300пкс от начала, то двигаются платформы
				float y_new = y_pl - dy;
				platforms[i]->setPlatform(x_pl, y_new);
				if (i == 19) {
					dy = 0;
				}
			}
			if (y_pl > 800) { // если платформа за картой, то меняем ей координаты
				float x_new = rand() % 532;
				float y_new = -1 * rand() % 200;
				platforms[i]->setPlatform(x_new, y_new);
				if (i == 0) {
					platforms[0]->setJetpack(false);
				}

			}
		}

		for (int i = 0; i < 20; i++) {
			float min = 1000;
			int k, l;
			for (int j = 0; j < 20; j++) {
				float x_pl_1, y_pl_1, x_pl_2, y_pl_2, w_plat, h_plat;
				platforms[i]->getPlat(&x_pl_1, &y_pl_1, &w_plat, &h_plat);
				platforms[j]->getPlat(&x_pl_2, &y_pl_2, &w_plat, &h_plat);
				if (abs(y_pl_1 - y_pl_2) <= 40) {
					min = abs(y_pl_1 - y_pl_2);
					break;

				}
				else if (abs(y_pl_1 - y_pl_2) < min)
				{
					min = abs(y_pl_1 - y_pl_2);
					k = i;
					l = j;
				}

			}

			if (min > 40) {
				float x_pl_1, y_pl_1, x_pl_2, y_pl_2, w_platk, h_platk;
				platforms[k]->getPlat(&x_pl_1, &y_pl_1, &w_platk, &h_platk);
				platforms[l]->getPlat(&x_pl_2, &y_pl_2, &w_platk, &h_platk);
				if (abs(y_pl_1 - y_pl_2) >= 40) {
					if (y_pl_1 > y_pl_2) {
						platforms[l]->setPlatform(x_pl_2, y_pl_1 - 40);
					}
					if (y_pl_1 < y_pl_2) {
						platforms[k]->setPlatform(x_pl_1, y_pl_2 - 40);
					}
				}

			}
		}
		y += dy; // движение по у
		switch (dir)// движение по х
		{
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		}
		x += dx * time;

		speed = 0;

		if (x + 0.2 * w > 532) {
			x = w * -0.3;
		}
		if (x + 0.8 * w < 0) {
			x = 532 - w * 0.7;
		}

		float p_x, p_y, heigth, width;
		platforms[0]->getPlat(&p_x, &p_y, &heigth, &width); // получаем 1ю платформу

		if (score->getScore() % 5000 < 1000 && !(platforms[0]->hasJetpack) && p_y <= 0) { //если выполняются условия, то располагаем джетпак на ней
			platforms[0]->setJetpack(true);
			jet->setJetpack(p_x, p_y - 55);
		}
		if (platforms[0]->hasJetpack) { // если на платформе джетпак, то смещаем его вместе с ней, иначе убираем за карту
			jet->setJetpack(p_x, p_y - 55);
			if (jet->isCollusion(x, y, h, w) && current != 1) {
				timeLast = 0;
				current = 1;
				w = 135; h = 116;
			}
		}
		else {
			jet->setJetpack(-1000, -1000);
		}
		if (left) { // в зависимости от последней клавижи разворачиваем спрайт
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		else {
			sprite.setTextureRect(IntRect(w, 0, -w, h));
		}
		sprite.setPosition(x, y); // выводим спрайт в позицию x y, посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
	}
	void fire_tex() { // при выстреле меняем текстуру
		w = 148;
		h = 125;
		image.loadFromFile("images/Hit.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);

	}
	void non_fire_tex() {// при выстреле меняем текстуру
		w = 148;
		h = 125;
		image.loadFromFile("images/Doodle.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
	void setLeft(int l) { // разворот персонажа
		left = l;
	}

	int isDeadP() { // проверям смерть игрока
		if (y + h > 850 || dead) return 1;
		else return 0;
	}
};


void menu(RenderWindow& window, int& Axe) { // функция главного меню
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/ButtonPlay.png");
	menuTexture3.loadFromFile("images/ButtonExit.png");
	menuBackground.loadFromFile("images/Main-Screen.png");
	Sprite menu1(menuTexture1), menu3(menuTexture3), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(0, 0);
	menu3.setPosition(100, 150);
	menuBg.setPosition(0, 0);


	while (isMenu) // пока меню активно
	{
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 250, 300, 50).contains(Mouse::getPosition(window))) { menuNum = 1; }
		if (IntRect(200, 400, 300, 50).contains(Mouse::getPosition(window))) { menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { isMenu = false; }//если нажали первую кнопку, то выходим из меню
			if (menuNum == 3) { window.close(); isMenu = false; Axe = 1; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu3);
		window.display();
	}

}

void menuEnd(RenderWindow& window, int& sc) { // меню после смерти
	Texture menuEndTexture1, menuEndTexture2, menuEndTexture3, menuEndBackground;
	menuEndTexture1.loadFromFile("images/backtomenu.png");
	menuEndBackground.loadFromFile("images/gameover.png");
	Sprite menuEnd1(menuEndTexture1), menuEnd2(menuEndTexture2), menuEndBg(menuEndBackground);
	bool isEndMenu = 1;
	int menuNum = 0;
	menuEnd1.setPosition(0, 0);
	//menuEnd2.setPosition(100, 150);
	menuEndBg.setPosition(0, 0);

	ofstream fout;
	fout.open("score.txt", ios_base::app);
	fout << sc << endl;
	int maxScore = 0, temp;
	fout.close();
	ifstream fin("score.txt");
	while (!fin.eof())
	{
		fin >> temp;
		if (temp > maxScore)
		{
			maxScore = temp;
			continue;
		}
	}
	cout << maxScore;
	fin.close();

	while (isEndMenu)
	{
		Font Score;
		Score.loadFromFile("DoodleJump.ttf");
		Text texScore("", Score, 50);
		texScore.setFillColor(sf::Color::Black);
		texScore.setString("your score: " + std::to_string(sc));
		texScore.setPosition(80, 420);

		Font MaxScore;
		Score.loadFromFile("DoodleJump.ttf");
		Text texMaxScore("", Score, 30);
		texMaxScore.setFillColor(sf::Color::Black);
		texMaxScore.setString("high score: " + std::to_string(maxScore));
		texMaxScore.setPosition(140, 690);

		menuNum = 0;

		if (IntRect(160, 540, 300, 50).contains(Mouse::getPosition(window))) { menuNum = 1; }
		if (IntRect(200, 400, 300, 50).contains(Mouse::getPosition(window))) { menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { isEndMenu = false; }
		}

		window.draw(menuEndBg);
		window.draw(texScore);
		window.draw(texMaxScore);
		window.draw(menuEnd1);
		window.display();
	}

}

bool StartGame() // главная функция игры
{
	int* Axe = 0;
	bool keySpace = false;
	RenderWindow window(sf::VideoMode(532, 850), "Doodle Jump");
	menu(window, *Axe);//вызов меню


	Texture bktexture;
	bktexture.loadFromFile("images/background.png");
	Sprite bksprite;
	bksprite.setTexture(bktexture);
	bksprite.setTextureRect(IntRect(0, 0, 532.0, 850.0));
	bksprite.setPosition(0, 0); //выводим спрайт в позицию x y

	Font font;//С€СЂРёС„С‚
	font.loadFromFile("DoodleJump.ttf");
	Text text("", font, 35);
	text.setFillColor(sf::Color::Black);
	Clock clock;
	// инициализация экземпялров классов
	Clock clock_fire;
	Score score;
	Guns g;
	Jetpack jet(0, -10);
	Enemy en(100, -1000);
	Player p("Doodle.png", 100, 100, 148.0, 125.0);//cоздаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	Platform** plat = new Platform * [20];

	for (int i = 0; i < 20; i++) {
		float pos_x, pos_y;
		pos_x = rand() % 532;
		pos_y = rand() % 850;
		plat[i] = new Platform("platform.png", pos_x, pos_y, 68, 14);
	}
	//
	BrokenPlatform broke("brokenplat1.png", 200, rand() % 534, 61, 14);
	MovePlatform movePlat(1000, -24, 61, 14);
	//
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds(); // получаем время прошедшее за вызов цикла
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) { // управление персонажем
			p.dir = 1; p.speed = 0.3;
			p.setLeft(1);

		}
		if ((Keyboard::isKeyPressed(Keyboard::Space))) { // выстрел

			if (keySpace == false) {
				g.fire(p.x, p.y);
				p.fire_tex();
				keySpace = true;
			}
		}


		if (event.key.code == sf::Keyboard::Space && (event.type == Event::KeyReleased)) { // выстрел отжат
			if (keySpace == true) {
				keySpace = false;
				p.non_fire_tex();
			}
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) { // управление персонажем
			p.dir = 0; p.speed = 0.3;
			p.setLeft(0);
		}
		// методы обновления классов с нужными параметрами
		p.update(time, plat, &score, &en, &jet, &broke, &movePlat);
		g.update(time, &en);

		if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//если таб, то перезагружаем игру
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры

		window.clear();

		text.setString("Score: " + std::to_string(score.getScore()));
		text.setPosition(10, 10);

		int sc;
		sc = score.getScore();
		// отрисовываем все объекты
		window.draw(bksprite);
		window.draw(p.sprite);
		window.draw(text);
		window.draw(jet.sprite);

		for (int pl = 0; pl < 20; pl++) {
			window.draw(plat[pl]->sprite);
		}
		for (int i = 0; i < g.getSize(); i++) {
			window.draw(g.getItem(i)->sprite);
		}

		window.draw(broke.sprite);
		window.draw(movePlat.sprite);
		window.draw(en.sprite);
		window.display();

		p.isDeadP();
		if (p.isDeadP() == 1) {
			menuEnd(window, sc);
			return true;
		}
	}
}

void gameRunning()
{
	if (StartGame()) { gameRunning(); }////если startGame() == true, то вызываем заново ф-цию isGameRunning, которая в свою очередь опять вызывает startGame()
}

int main()
{

	gameRunning();
	return 0;
}