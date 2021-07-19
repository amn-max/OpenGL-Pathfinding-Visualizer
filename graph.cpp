#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;
const int n = 40;

int INF = 9999;
#define SOURCE 1024
#define DEST 2024
#define WALL 3024
#define EMPTY 4024
#define CHECKED 5024
#define FOUND 6024
#define NOTFOUND 7024
#define PATH 8024

float screenWidth = 750;
float screenHeight = 750;
int clickCount = 0;
int prevX, prevY;
int isMouseLeftDown = 0;
int isMouseRightDown = 0;
int startEndNodes = 0;
double hVDistance = 1.0;
double dDistance = 1.4;
const int TIME_SECS = 1000;
const int SCREEN_FPS = 60;
bool canAcceptInput = true;
int flag = 0;
bool isOnStartPage = true;

class SPoint {
public:
	int x;
	int y;
	SPoint(void) {

	}

	SPoint(int x, int y) {
		this->x = x;
		this->y = y;
	}

};


class Node {
public:
	int gridx;
	int gridy;
	double distance;
	int type;
	bool visited;
	bool blocked;
	SPoint parent = SPoint(NULL, NULL);
	Node(void) {

	}

	Node(int x, int y) {
		this->gridx = x;
		this->gridy = y;
	}

	Node(int x, int y, int type, bool visited, int distance = INF) {
		this->gridx = x;
		this->gridy = y;
		this->type = type;
		this->distance = distance;
		this->visited = visited;
	}

	int getType() {
		return this->type;
	}

	int getGridPositionX() {
		return this->gridx;
	}
	int getGridPositionY() {
		return this->gridy;
	}

	int getDistance() {
		return this->distance;
	}

	bool isEqual(Node s, Node d) {
		if (s.getGridPositionX() == d.getGridPositionX() && s.getGridPositionY() == d.getGridPositionY()) {
			return true;
		}
		return false;
	}
};



bool operator<(const Node& n1, const Node& n2) {
	return n1.distance > n2.distance;
}


vector<Node> nodes;
priority_queue<Node> nQueue;
Node mapn[n][n];
Node *start;
Node *endn;
vector<SPoint> path;
bool gridHasSource() {
	for (int i = 0; i < n; i++) {
		int type = nodes.at(i).getType();
		if (type == SOURCE) {
			return true;
			break;
		}
	}
	return false;
}

bool gridHasDest() {
	for (int i = 0; i < n; i++) {
		int type = nodes.at(i).getType();
		if (type == DEST) {
			return true;
			break;
		}
	}
	return false;
}

void clicked(int i, int j, int type) {

	if (type != EMPTY) {
		float dif = 0.1;
		if (type == SOURCE)
		{
			glColor3f(0.258, 0.031, 0.388);
			dif = -0.1;
		}
		else if (type == DEST) {
			glColor3f(0.772, 0.447, 1);
			dif = -0.1;
		}
		else if (type == WALL) {
			glColor3f(0.047, 0.207, 0.278);
			dif = 0.05;
		}
		else if (type == CHECKED) {
			glColor3f(0.250, 0.807, 0.890);
			dif = 0.08;
		}
		else if (type == PATH) {
			glColor3f(1, 0.996, 0.415);
			dif = 0.0;
		}

		glBegin(GL_POLYGON);
		glVertex2f(i + dif, j + dif);
		glVertex2f(i + dif, j - dif + 1);
		glVertex2f(i - dif + 1, j - dif + 1);
		glVertex2f(i - dif + 1, j + dif);
		glEnd();

	}

}

void reDrawPoints() {
	for (int i = 0; i < nodes.size(); i++) {
		int x = nodes[i].getGridPositionX();
		int y = nodes[i].getGridPositionY();
		//i == 0 ? clicked(x, y, i + 1) : clicked(x, y, i + 1);
		clicked(x, y, nodes[i].getType());
	}
}

bool hasNodePlaced(Node n) {
	int type = n.getType();
	if (type == PATH) {
		return true;
	}
	if (type == SOURCE || type == DEST || type == WALL || type == CHECKED) {
		return true;
	}
	return false;
}

void addPoints(int x, int y) {
	int posx = x / (screenWidth / n);
	int posy = y / (screenHeight / n);

	for (int i = 0; i < nodes.size(); i++) {
		int x = nodes.at(i).getGridPositionX();
		int y = nodes.at(i).getGridPositionY();

		if ((x == posx && y == posy) && !hasNodePlaced(nodes.at(i))) {
			if (startEndNodes == 0) {
				Node node = Node(posx, posy, SOURCE, true, 0);
				nodes.at(i) = node;
				startEndNodes = 1;
			}
			else if (startEndNodes == 1) {
				Node node = Node(posx, posy, DEST, false, INF);
				nodes.at(i) = node;
				startEndNodes = 2;
			}
			else if (startEndNodes >= 2) {
				Node node = Node(posx, posy, WALL, false, INF);
				nodes.at(i) = node;
				startEndNodes++;
			}
			break;
		}
	}

}

void removePoints(int x, int y) {
	int posx = x / (screenWidth / n);
	int posy = y / (screenHeight / n);
	for (int i = 2; i < nodes.size(); i++) {
		int x = nodes.at(i).getGridPositionX();
		int y = nodes.at(i).getGridPositionY();
		if (x == posx && y == posy && hasNodePlaced(nodes.at(i))) {
			Node node = Node(posx, posy, EMPTY, false, INF);
			nodes.at(i) = node;
			startEndNodes--;
			break;
		}
	}
}

void handleMouse(int button, int state, int x, int y) {
	if (canAcceptInput) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			isMouseLeftDown = 0;
			////cout << "Mouse Left Clicked at : x=" << x / 50 + 1 << " y=" << y / 50 + 1 << endl;
			addPoints(x, y);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			isMouseLeftDown = 1;
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
			isMouseRightDown = 0;
			removePoints(x, y);
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			isMouseRightDown = 1;
		}
		glutPostRedisplay();
	}
}

Node getStartNode() {
	for (int i = 0; i < n; i++) {
		if (nodes.at(i).getType() == SOURCE) {
			return nodes.at(i);
		}
	}
}

Node getEndNode() {
	for (int i = 0; i < n; i++) {
		if (nodes.at(i).getType() == DEST) {
			return nodes.at(i);
		}
	}
}

void makeReady() {
	//for testing
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int x = nodes.at(j * n + i).getGridPositionX();
			int y = nodes.at(j * n + i).getGridPositionY();
			////cout << "(" << x << "," << y << ")" << "  ";
		}
		////cout << endl;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int type = nodes.at(j * n + i).getType();
			if (type == SOURCE) {
				////cout << " " << "S" << " ";
				start = &nodes.at(j * n + i);
			}
			else if (type == DEST) {
				////cout << " " << "D" << " ";
				endn = &nodes.at(j * n + i);
			}
			else if (type == WALL) {
				////cout << " " << "W" << " ";
			}
			else if (type == EMPTY) {
				////cout << " " << "I" << " ";
			}
		}
		////cout << endl;
	}

	//Convert to 2d array
	memset(mapn, 0, sizeof(mapn[0][0]) * n * n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//int x = nodes.at(j * n + i).getGridPositionX();
			//int y = nodes.at(j * n + i).getGridPositionY();
			mapn[j][i] = nodes.at(j * n + i);

		}
	}

	////cout << "Distmap is:" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			try {

				////cout << "(" << mapn[i][j].getGridPositionX() << "," << mapn[i][j].getGridPositionY() << ")" << "  ";
			}
			catch (const out_of_range& e) {

			}
		}
		////cout << endl;
	}


	////cout << "distance" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			try {

				////cout << mapn[i][j].distance << "  ";
			}
			catch (const out_of_range& e) {

			}
		}
		////cout << endl;
	}
	nQueue.push(*start);
	////cout << "start node" << start.getGridPositionX() << "," << start.getGridPositionY() << endl;
}

void showPath(int value) {

	//cout << " Path is : " << endl;
	if (mapn[endn->getGridPositionX()][endn->getGridPositionY()].type != INF) {
		Node current = mapn[endn->getGridPositionX()][endn->getGridPositionY()];

		while (current.type != SOURCE && current.parent.x != NULL && current.parent.y != NULL) {
			//cout << "Current node at end : " << current.parent.x << "," << current.parent.y << endl;
			//path.push_back(current.parent);
			current = mapn[current.parent.x][current.parent.y];
			//nodes.at(current.getGridPositionX()* n + current.getGridPositionY()).type = PATH;
			if (current.type != start->type) {
				nodes.at(current.getGridPositionX() * n + current.getGridPositionY()).type = PATH;
			}
		}
		glutPostRedisplay();
	}



}
void drive(int value) {
	if (value > 0 && !nQueue.empty()) {
		Node current; 
		if (!nQueue.empty()) {
			current = nQueue.top();
		}
		/////cout <<"Size Queue: "<<nQueue.size() << endl;/
		//nQueue.pop();
		Node t;
		/*////cout << "End Node" << endn.getGridPositionX() << "," << endn.getGridPositionY() << endl;
		////cout << "Current Node" << current.getGridPositionX() << "," << current.getGridPositionY() << endl;*/
		if (current.type == DEST) {
			////cout << "End Found" << endl;
			mapn[endn->getGridPositionX()][endn->getGridPositionY()].distance = FOUND;
			flag = 1;
		}
		//top
		if (current.getGridPositionX() - 1 >= 0) {
			t = mapn[current.getGridPositionX() - 1][current.getGridPositionY()];
			//top top
			if (!t.visited && t.getType() != WALL && t.distance > current.distance + hVDistance) {
				/*////cout << "Top Top temp is:" << t.getType() << endl;
				////cout << "Top Top type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
				t.distance = current.distance + hVDistance;
				mapn[current.getGridPositionX() - 1][current.getGridPositionY()].distance = current.distance + hVDistance;
				t.parent.x = current.getGridPositionX();
				t.parent.y = current.getGridPositionY();
				mapn[current.getGridPositionX() - 1][current.getGridPositionY()].parent.x = current.getGridPositionX();
				mapn[current.getGridPositionX() - 1][current.getGridPositionY()].parent.y = current.getGridPositionY();
				//mapn[current.getGridPositionX() - 1][current.getGridPositionY()].visited = true;
				nQueue.push(t);
			}

			//top left
			if (current.getGridPositionY() - 1 > 0) {
				t = mapn[current.getGridPositionX() - 1][current.getGridPositionY() - 1];
				if (!t.visited && t.getType() != WALL && t.distance > current.distance + dDistance) {
					/*////cout << "Top left temp is:" << t.getType() << endl;
					////cout << "Top left type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
					t.distance = current.distance + dDistance;
					mapn[current.getGridPositionX() - 1][current.getGridPositionY() - 1].distance = current.distance + dDistance;
					t.parent.x = current.getGridPositionX();
					t.parent.y = current.getGridPositionY();
					mapn[current.getGridPositionX() - 1][current.getGridPositionY() - 1].parent.x = current.getGridPositionX();
					mapn[current.getGridPositionX() - 1][current.getGridPositionY() - 1].parent.y = current.getGridPositionY();
					//mapn[current.getGridPositionX() - 1][current.getGridPositionY() - 1].visited = true;
					nQueue.push(t);
				}
			}

			//top right
			if (current.getGridPositionY() + 1 < n) {
				t = mapn[current.getGridPositionX() - 1][current.getGridPositionY() + 1];
				if (!t.visited && t.getType() != WALL && t.distance > current.distance + dDistance) {
					/*////cout << "Top right temp is:" << t.getType() << endl;
					////cout << "Top right type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
					t.distance = current.distance + dDistance;
					mapn[current.getGridPositionX() - 1][current.getGridPositionY() + 1].distance = current.distance + dDistance;
					t.parent.x = current.getGridPositionX();
					t.parent.y = current.getGridPositionY();
					mapn[current.getGridPositionX() - 1][current.getGridPositionY() + 1].parent.x = current.getGridPositionX();
					mapn[current.getGridPositionX() - 1][current.getGridPositionY() + 1].parent.y = current.getGridPositionY();
					//mapn[current.getGridPositionX() - 1][current.getGridPositionY() + 1].visited = true;
					nQueue.push(t);
				}
			}
		}
		//left
		if (current.getGridPositionY() - 1 > 0) {
			t = mapn[current.getGridPositionX()][current.getGridPositionY() - 1];
			if (!t.visited && t.getType() != WALL && t.distance > current.distance + hVDistance) {
				/*////cout << "left temp is:" << t.getType() << endl;
				////cout << "left type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
				t.distance = current.distance + hVDistance;
				mapn[current.getGridPositionX()][current.getGridPositionY() - 1].distance = current.distance + hVDistance;
				t.parent.x = current.getGridPositionX();
				t.parent.y = current.getGridPositionY();
				mapn[current.getGridPositionX()][current.getGridPositionY() - 1].parent.x = current.getGridPositionX();
				mapn[current.getGridPositionX()][current.getGridPositionY() - 1].parent.y = current.getGridPositionY();
				//mapn[current.getGridPositionX()][current.getGridPositionY() - 1].visited = true;
				nQueue.push(t);
			}
		}

		//right
		if (current.getGridPositionY() + 1 < n) {
			t = mapn[current.getGridPositionX()][current.getGridPositionY() + 1];
			if (!t.visited && t.getType() != WALL && t.distance > current.distance + hVDistance) {
				/*////cout << "right temp is:" << t.getType() << endl;
				////cout << "right type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
				t.distance = current.distance + hVDistance;
				mapn[current.getGridPositionX()][current.getGridPositionY() + 1].distance = current.distance + hVDistance;
				t.parent.x = current.getGridPositionX();
				t.parent.y = current.getGridPositionY();
				mapn[current.getGridPositionX()][current.getGridPositionY() + 1].parent.x = current.getGridPositionX();
				mapn[current.getGridPositionX()][current.getGridPositionY() + 1].parent.y = current.getGridPositionY();
				//mapn[current.getGridPositionX()][current.getGridPositionY() + 1].visited = true;
				nQueue.push(t);
			}
		}

		//down
		if (current.getGridPositionX() + 1 < n) {
			//down down
			t = mapn[current.getGridPositionX() + 1][current.getGridPositionY()];
			if (!t.visited && t.getType() != WALL && t.distance > current.distance + hVDistance) {
				/*////cout << "down temp is:" << t.getType() << endl;
				////cout << "down type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
				t.distance = current.distance + hVDistance;
				mapn[current.getGridPositionX() + 1][current.getGridPositionY()].distance = current.distance + hVDistance;
				t.parent.x = current.getGridPositionX();
				t.parent.y = current.getGridPositionY();
				mapn[current.getGridPositionX() + 1][current.getGridPositionY()].parent.x = current.getGridPositionX();
				mapn[current.getGridPositionX() + 1][current.getGridPositionY()].parent.y = current.getGridPositionY();
				//mapn[current.getGridPositionX() + 1][current.getGridPositionY()].visited = true;
				nQueue.push(t);
			}

			//down left
			if (current.getGridPositionY() - 1 >= 0) {
				t = mapn[current.getGridPositionX() + 1][current.getGridPositionY() - 1];
				if (!t.visited && t.getType() != WALL && t.distance > current.distance + dDistance) {
					/*////cout << "down left temp is:" << t.getType() << endl;
					////cout << "down left type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
					t.distance = current.distance + dDistance;
					mapn[current.getGridPositionX() + 1][current.getGridPositionY() - 1].distance = current.distance + dDistance;
					t.parent.x = current.getGridPositionX();
					t.parent.y = current.getGridPositionY();
					mapn[current.getGridPositionX() + 1][current.getGridPositionY() - 1].parent.x = current.getGridPositionX();
					mapn[current.getGridPositionX() + 1][current.getGridPositionY() - 1].parent.y = current.getGridPositionY();
					//mapn[current.getGridPositionX() + 1][current.getGridPositionY() - 1].visited = true;
					nQueue.push(t);
				}
			}

			//down right
			if (current.getGridPositionY() + 1 < n) {
				t = mapn[current.getGridPositionX() + 1][current.getGridPositionY() + 1];
				if (!t.visited && t.getType() != WALL && t.distance > current.distance + dDistance) {
					/*////cout << "down right temp is:" << t.getType() << endl;
					////cout << "down right type pos:" << t.getGridPositionX() << "," << t.getGridPositionY() << endl;*/
					t.distance = current.distance + dDistance;
					mapn[current.getGridPositionX() + 1][current.getGridPositionY() + 1].distance = current.distance + dDistance;
					t.parent.x = current.getGridPositionX();
					t.parent.y = current.getGridPositionY();
					mapn[current.getGridPositionX() + 1][current.getGridPositionY() + 1].parent.x = current.getGridPositionX();
					mapn[current.getGridPositionX() + 1][current.getGridPositionY() + 1].parent.y = current.getGridPositionY();
					//mapn[current.getGridPositionX() + 1][current.getGridPositionY() + 1].visited = true;
					nQueue.push(t);
				}
			}
		}
		current.visited = true;

		if (current.visited && !hasNodePlaced(current)) {
			nodes.at(current.getGridPositionX() * n + current.getGridPositionY()).type = CHECKED;
		}
		glutPostRedisplay();
		//mapn[current.getGridPositionX()][current.getGridPositionY()].visited = true;
		////cout << endl;
		//for (int i = 0; i < n; i++) {
		//	for (int j = 0; j < n; j++) {
		//		try {

		//			if (mapn[i][j].distance != INF && !hasNodePlaced(mapn[i][j])) {
		//				//nodes.at(j * n + i).type = CHECKED;
		//			}
		//			//cout << mapn[i][j].distance << "\t";
		//		}
		//		catch (const out_of_range& e) {

		//		}
		//	}
		//	//cout << endl;
		//}


	}
	int prevSize = nQueue.size();
	if (!nQueue.empty()) {
		nQueue.pop();
	}
	else {
		//cout << "Destination Not Found" << endl;
		canAcceptInput = true;
	}
	int currSize = nQueue.size();
	if (prevSize - currSize && flag == 0) {
		glutTimerFunc(TIME_SECS / SCREEN_FPS, drive, nQueue.size());
	}
	else if (flag == 1) {
		showPath(1);
		canAcceptInput = true;
	}

}

void findPath() {
	canAcceptInput = false;
	makeReady();
	drive(nQueue.size());
	//while (nQueue.size() > 0) {

	//}

	////cout << "Path is:" << endl;
	//if (path.size() > 0) {
	//	for (int i = 0; i < path.size(); i++) {
	//		////cout << "(" << path.at(i).getGridPositionX() << "," << path.at(i).getGridPositionY() << ")" << "  ";
	//	}
	//}
}

void drawString(float x, float y, float z, char* string) {
	glRasterPos3f(x, y, z);

	for (char* c = string; *c != '\0'; c++) {
		glColor3f(1, 1, 1);
		
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Updates the position
	}
}

float findBitmapMidPoint(const char* str) {
	return ((n / 2) - (strlen(str) / 2) * 8.76 / 31);
}

void display(void) {
	////cout << "Node Size is : " << nodes.size() << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (isOnStartPage) {
		//Starting page
		glClearColor(0.117, 0.423, 0.713, 1.0);
		char collegeName[64] = "BLDEA's V P Dr PG Halakatti College of Engineering & Technology";
		char dept[31] = "Department of Computer Science";
		char proj[21] = "OpengGL Mini Project";
		char title[23] = "Pathfinding Visualizer";
		char instrc[43] = "Instructions";
		char text1[21] = "Press Enter To Start";
		char text2[28] = "Press 'r' or 'R' to Restart";
		char text3[38] = "Press 's' or 'S' to Start Visualizing";
		char text4[64] = "Press Left Mouse to place Nodes and Right Mouse to remove Nodes";
		char text5[35] = "Press <ESC KEY> to quit the window";
		char text6[13] = "Created By:";
		char text7[23] = "Ayush Naik, Arjun Naik";
		char text8[23] = "2BL18CS012, 2BL18CS010";
		drawString(findBitmapMidPoint(collegeName), 3, 0.0, collegeName);
		drawString(findBitmapMidPoint(dept), 6, 0.0, dept);
		drawString(findBitmapMidPoint(proj), 8, 0.0, proj);
		drawString(findBitmapMidPoint(title), 13, 0.0, title);
		drawString(findBitmapMidPoint(instrc), 16, 0.0, instrc);
		drawString(findBitmapMidPoint(text1), 18, 0.0, text1);
		drawString(findBitmapMidPoint(text2), 19, 0.0, text2);
		drawString(findBitmapMidPoint(text3), 20, 0.0, text3);
		drawString(findBitmapMidPoint(text4), 21, 0.0, text4);
		drawString(findBitmapMidPoint(text5), 22, 0.0, text5);
		drawString(n-10, n-5, 0.0, text6);
		drawString(n - 10, n - 3, 0.0, text7);
		drawString(n - 10, n - 2, 0.0, text8);
	}
	else {
		//gird page
		
		glClearColor(0.964, 0.964, 0.964, 1.0);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				glLineWidth(1);
				glColor3f(0.686, 0.847, 0.972);
				glBegin(GL_LINE_LOOP);
				glVertex2f(i, j);
				glVertex2f(i, j + 1);
				glVertex2f(i + 1, j + 1);
				glVertex2f(i + 1, j);
				glEnd();
			}
		}
		reDrawPoints();
		
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

void onMouseMovement(int x, int y) {
	if (canAcceptInput) {
		if (isMouseLeftDown == 1 && nodes.size() > 1) {
			addPoints(x, y);
			glutPostRedisplay();
		}
		if (isMouseRightDown == 1 && nodes.size() > 1) {
			removePoints(x, y);
			glutPostRedisplay();
		}
	}
}

void resize(int w, int h) {
	screenWidth = w;
	screenHeight = h;
	int ratio = 1.0f * w / h;
	glViewport(0, 0, w, h);
	//glClearColor(1.0, 1.0, 1.0, 1.0);         // black background
	glMatrixMode(GL_PROJECTION);              // setup viewing projection
	glLoadIdentity();        // start with identity matrix
	glOrtho(0.0, n, n, 0.0, -1.0, 1.0);   // setup a 10x10x2 viewing world
}

void reset() {
	flag = 0;
	memset(mapn, NULL, sizeof(mapn[0][0])*n*n);
	while (!nQueue.empty()) {
		nQueue.pop();
	}
	path.clear();
	vector<SPoint>().swap(path);
	path.shrink_to_fit();
	//cout << "size and capacity" << nodes.size() << " " << nodes.capacity() << endl;
	nodes.clear();
	vector<Node>().swap(nodes);
	//cout << "size and capacity" << nodes.size() << " " << nodes.capacity() << endl;
	nodes.shrink_to_fit();
	//cout << "size and capacity" << nodes.size() << " " << nodes.capacity() << endl;
	//delete &start;
	//delete &endn;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Node node = Node(i, j, EMPTY, false, INF);
			nodes.push_back(node);
		}
	}
	startEndNodes = 0;
	glutPostRedisplay();
}

void onKeyPress(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case 115:
	case 'S':
		if (canAcceptInput) {
			findPath(); //starts algorithm
		}
		break;
	case 114:
	case 'R':
		reset(); //clears all points
		break;
	case 13:
		isOnStartPage = false;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Node node = Node(i, j, EMPTY, false, INF);
			nodes.push_back(node);
		}
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("square");
	glutFullScreen();
	//glutInitWindowSize(screenWidth, screenHeight);
	
	//glClearColor(0.964, 0.964, 0.964, 1.0);
	glMatrixMode(GL_PROJECTION);              // setup viewing projection
	glLoadIdentity();        // start with identity matrix
	glOrtho(0.0, n, n, 0.0, -1.0, 1.0);   // setup a 10x10x2 viewing world
	/*glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);*/
	glutDisplayFunc(display);
	glutMouseFunc(handleMouse);
	glutReshapeFunc(resize);
	glutMotionFunc(onMouseMovement);
	glutKeyboardFunc(onKeyPress);
	glutMainLoop();
	return 0;
}