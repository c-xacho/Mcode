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
		setbkcolor(WHITE);
		cleardevice();
	}
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
void Failed_Menu() {
	cleardevice();

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
	IMAGE EnemyImg, PlayerImg, BulletImg, BackgroundImg;
	loadimage(&EnemyImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\enemy1.png"));
	loadimage(&PlayerImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\life.png"));
	loadimage(&BulletImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\bullet1.png"));
	loadimage(&BackgroundImg, _T("B:\\MyRepositories\\PlaneFight_BC\\images\\bk2.png",600));
	
	Background background(BackgroundImg);
	Player Player(PlayerImg);
	vector<Enemy*> Es;
	for (int i = 0; i < 4; i++) {
		AddEnemy(Es, EnemyImg);
	}
	vector<Bullet*> Bullets;
	int BulletTime = 0;
	//游戏主循环
	while (is_play) {
		BulletTime++;
		if (BulletTime == 1) {
			BulletTime = 0;
			Bullet* b = new Bullet(BulletImg, Player.getRECT());
			Bullets.push_back(b);
		}
		BeginBatchDraw();
		background.Show();
		flushmessage();
		Sleep(2);
		Player.Control();
		Player.Show();
		auto it = Es.begin();
		for (auto& bullet : Bullets) {
			bullet->Show();
		}
		while (it != Es.end()) {
			//如果敌机移动到屏幕外，移除
			if (RectCrashRect(Player.getRECT(), (*it)->getRECT())) {
				is_play = false;
			}
			for (auto bullet = Bullets.begin(); bullet != Bullets.end(); bullet++) {
				if (RectCrashRect((*it)->getRECT(), (*bullet)->getRECT())) {
					delete (*it);
					Es.erase(it);
					it = Es.begin();
					delete (*bullet);
					Bullets.erase(bullet);
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
	return is_play;
}
int main()
{
	bool is_live = 1;
	while (1) {
		Menu();
		Play();
		Failed_Menu();
	}
	return 0;
}
