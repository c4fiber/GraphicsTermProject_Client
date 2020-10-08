#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include <iostream>
#include <string>
#include "Client.hpp"

using namespace std;

class Vector2d {
public:
	double x = 0.0, y = 0.0;
};

constexpr auto PORT = 45678;
constexpr auto SERVER_IP = "114.29.136.181";

class Game {
public:
	Game() : client(SERVER_IP, PORT) {
	}

	void run() {
		try {
			client.init();
		}
		catch (const char *error) {
			cout << error << endl;
			exit(1);
		}

		while (client.checkConnect()) {
			string msg;
			cin >> msg;
			if (msg == "end")
				break;
			Sleep(1000);
		}

		client.close();
		exit(2);
	}

	void onRecv(const char *buff, const int &len) {
		Vector2d pos = *(Vector2d *)(buff);
		teapotPos.x = pos.x;
		teapotPos.y = pos.y;
	}

	void keyboard(unsigned char key) {
		switch (key) {
		case 'w':
			teapotPos.y += 0.01;
			break;
		case 'a':
			teapotPos.x -= 0.01;
			break;
		case 's':
			teapotPos.y -= 0.01;
			break;
		case 'd':
			teapotPos.x += 0.01;
			break;
		default:
			break;
		}

		if (client.checkConnect())
			client.send((const char *)&teapotPos, sizeof(teapotPos));
	}

	Client client;
	Vector2d teapotPos;
};

Game game;

GLvoid Screen(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(0, 0, 300, 300);
	
	glBegin(GL_POLYGON);
	{
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glLoadIdentity();
	glTranslated(game.teapotPos.x, game.teapotPos.y, 0);
	glutSolidTeapot(0.5);

	//glFlush();
	glutSwapBuffers();
}
GLvoid Keyboard(unsigned char key, int x, int y) {
	game.keyboard(key);
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

#ifdef _DEBUG
int main(int argc, char **argv)
#else
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
#endif
{
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("����");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(Screen);
	glutTimerFunc(0, timer, 0);

	game.client.setOnRecv([](const char *buff, const int &len) {
		game.onRecv(buff, len);
	});
	thread gameThread(&Game::run, &game);

	glutMainLoop();
	return 0;
}