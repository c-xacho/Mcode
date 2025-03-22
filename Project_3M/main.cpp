#include "project.h"
bool window = false;
bool InRect(RECT& rect,int x,int y) {
	return (rect.left <= x && x <= rect.right && rect.top <= y && y <= rect.bottom);
}
bool RectCrashRect(RECT& o1, RECT& o2){
	RECT CrashZone;
	CrashZone.left = o1.left - (o2.right - o2.left);
	CrashZone.right = o1.right;
	CrashZone.top = o1.top - (o2.bottom - o2.top);
	CrashZone.bottom = o1.bottom;
	return (CrashZone.left < o2.left && o2.left <= CrashZone.right && CrashZone.top <= o2.top && o2.top <= CrashZone.bottom);
}
void Menu() {
	//创建窗口
	if (!window) {
		window = true;
		initgraph(width, height, EW_NOMINIMIZE );
	}
	setbkcolor(WHITE);
	cleardevice();
	//设置字体
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy((width - textwidth(_T("Title"))) / 2, height / 5, _T("Title"));
	settextstyle(50, 0, _T("黑体"));
	RECT tplay, texit;
	tplay.left = width / 2 - textwidth(_T("Play")) / 2;
	tplay.right = tplay.left + textwidth(_T("Play"));
	tplay.top = height / 2;
	tplay.bottom = tplay.top + textheight(_T("Play"));
	texit.left = width / 2 - textwidth(_T("Exit")) / 2;
	texit.right = texit.left + textwidth(_T("Exit"));
	texit.top = height / 1.6;
	texit.bottom = texit.top + textheight(_T("Exit"));
	outtextxy(tplay.left,tplay.top, _T("Play"));
	outtextxy(texit.left, texit.top, _T("Exit"));
	//获取鼠标信息
	flushmessage();
	while (true) {
		ExMessage meg;
		getmessage(&meg, EX_MOUSE);
		//PLAY
		if (meg.lbutton) {
			if (InRect(tplay,meg.x,meg.y)) {
				return;
			}
			//EXIT
			else if (InRect(texit, meg.x, meg.y)) {
				exit(0);
			}
		}
	}
}
void Failed_Menu(unsigned long long& x) {
	cleardevice();
	settextcolor(RED);
	wchar_t buffer[100];
	swprintf(buffer, 100, L"击杀数%llu", x);
	settextstyle(60, 0, _T("黑体"));
	outtextxy((width - textwidth(buffer)) / 2, height / 4, buffer);
	settextstyle(50, 0, _T("黑体"));
	settextcolor(BLACK);
	RECT again, texit;
	again.left = width / 2 - textwidth(_T("Menu")) / 2;
	again.right = again.left + textwidth(_T("Menu"));
	again.top = height / 2;
	again.bottom = again.top + textheight(_T("Menu"));
	texit.left = width / 2 - textwidth(_T("Exit")) / 2;
	texit.right = texit.left + textwidth(_T("Exit"));
	texit.top = height / 1.6;
	texit.bottom = texit.top + textheight(_T("Exit"));
	outtextxy(again.left, again.top, _T("Menu"));
	outtextxy(texit.left, texit.top, _T("Exit"));
	//获取鼠标信息
	while (true) {
		ExMessage meg;
		getmessage(&meg, EX_MOUSE);
		//PLAY
		if (meg.lbutton) {
			if (InRect(again, meg.x, meg.y)) {
				return;
			}
			//EXIT
			else if (InRect(texit, meg.x, meg.y)) {
				exit(0);
			}
		}
	}
}
bool AddEnemy(vector<Enemy*>& Es, IMAGE& EnemyImg) {
	Enemy* e = new Enemy(EnemyImg, abs(rand() % (width - EnemyImg.getwidth())));
	bool flag = true;
	while (flag) {
		flag = false;
		e = new Enemy(EnemyImg, abs(rand() % (width - EnemyImg.getwidth())));
		for (auto& it : Es) {
			if (RectCrashRect(e->getRECT(), it->getRECT())) {
				delete e;
				flag = true;
				break;
			}
		}
	}
	Es.push_back(e);
	return true;
}
bool Play() {
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = true;
	IMAGE EnemyImg, PlayerImg, BulletImg, BackgroundImg, GiftImg;
	loadimage(&EnemyImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\enemy1.png"));
	loadimage(&PlayerImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\life.png"));
	loadimage(&BulletImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\bullet1.png"));
	loadimage(&BackgroundImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\bk2.png",600));
	loadimage(&GiftImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\bullet_supply.png"));
	
	Background background(BackgroundImg);
	Player Player(PlayerImg);
	vector<Enemy*> Es;
	for (int i = 0; i < 4; i++) {
		AddEnemy(Es, EnemyImg);
	}
	vector<Bullet*> Bullets;
	vector<Gift*> Gifts;
	unsigned long long BulletTime = 0;
	unsigned long long GiftTime = 0;
	unsigned long long kill = 0;
	int level = 30;
	bool isGiftGenerated = false;
	//游戏主循环
	while (is_play) {
		BulletTime++;
		GiftTime++;
		if (BulletTime >= level) {
			BulletTime = 0;
			Bullet* b = new Bullet(BulletImg, Player.getRECT());
			Bullets.push_back(b);
		}
		//按时间生成礼物
		/*if (level > 1 && GiftTime == 300) {
			GiftTime = 0;
			if (level - Gifts.size() > 0){
				Gift* g = new Gift(GiftImg, abs(rand() % (width - GiftImg.getwidth())));
				Gifts.push_back(g);
			}
		}*/
		//按击杀数生成礼物
		if ((kill != 0 && level > 1) && kill % 10 == 0) {
			if (!isGiftGenerated) {
				// 生成礼物
				if (level - Gifts.size() > 0) {//保证吃完当前存在礼物数不会超过最高级
					printf("生成Gift！\n");
					Gift* g = new Gift(GiftImg, abs(rand() % (width - GiftImg.getwidth())));
					Gifts.push_back(g);
					isGiftGenerated = true;  // 设置标志变量
				}
			}
		}
		else {
			isGiftGenerated = false;  // 重置标志变量
		}
		BeginBatchDraw();
		background.Show();
		flushmessage();
		Sleep(2);
		Player.Control();
		Player.Show();
		for (auto bullet = Bullets.begin(); bullet != Bullets.end(); bullet++) {
			(*bullet)->Show();
		}
		for (auto gift = Gifts.begin(); gift != Gifts.end(); gift++) {
			(*gift)->Show();
		}
		auto it = Es.begin();
		while (it != Es.end()) {
			//如果敌机移动到屏幕外，移除
			if (RectCrashRect(Player.getRECT(), (*it)->getRECT())) {
					is_play = false;
			}
			for (auto bullet = Bullets.begin(); bullet != Bullets.end(); bullet++) {
				if (RectCrashRect((*it)->getRECT(), (*bullet)->getRECT())) {
					kill++;
					printf("kill:%d", kill);
					printf("\n");
					delete (*it);
					Es.erase(it);
					it = Es.begin();
					delete (*bullet);
					Bullets.erase(bullet);
					break;
				}
			}
			for (auto gift = Gifts.begin(); gift != Gifts.end(); gift++) {
				if (RectCrashRect(Player.getRECT(), (*gift)->getRECT())) {
					if (level >= 10) {
						level -= 5;
					}
					else if (level > 1) {
						level -= 1;
					}
					delete (*gift);
					gift = Gifts.erase(gift);
					break;
				}
			}
			if (!(*it)->Show()) {
				delete (*it);
				Es.erase(it);
				it = Es.begin();
			}else
			it++;
		}
		//如果当前敌机不足设定数，添加
		for (int i = 0; i < 4 - Es.size(); i++) {
			AddEnemy(Es, EnemyImg);
		}
		EndBatchDraw();	
	}
	Failed_Menu(kill);
	return is_play;
}
int main()
{
	bool is_live = 1;
	while (1) {
		Menu();
		Play();
		flushmessage();
	}
	return 0;
}
