#include <stdio.h>
#include <string.h>
#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <random>
#include <GLUT/glut.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <set>
#include <SDL.h>
#include <SDL_mixer.h>

void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void Key(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();
std::vector<std::string> songsPaths;

float rotationAngle = 0.0f;
float rotationAnglestar = 0.0f;
int rotationAnglecircle=0;
float rotationAngleGoal = 0.0f;
int WindowSizeX=1000;
int WindowSizeY=600;
int y =550;
int PlayerX = (WindowSizeX+30)/2;
int PlayerY = (WindowSizeY+40)/2;
int ballY = 0;
int x =20 ,rectlen=100;
int TimerValue=80;
float radius=25;
bool GameOver = false, youwin=false, EndGameDisplayed = false;
bool rot = false;
bool rot2 = false;
bool isMovingUp=true, isMovingDown=false, isMovingLeft=false,  isMovingRight=false;
int score=0;
int speed=20;
bool powerup=false;
bool powerupClover=false;
int seconds=10;
int scoreFactor = 1;
bool pointsShown = true;

int starCoordinates[3][2];
int rockCoordinates[10][2];
int collectablesCoordinates[10][2];
int cloverCoordinates[2][2];
int pointsCoordinates[30][2];

float outerRadius = 15;
float innerRadius = 7.5;
float scale = 3;
float currentScale = 5;

void border(){
    glColor3f(0, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(2, 2);
    glVertex2f(WindowSizeX-2, 2);
    glVertex2f(WindowSizeX-2, WindowSizeY-2);
    glVertex2f(2, WindowSizeY-2);
    glEnd();
}

void DrawGoal(float x, float y) {
    
    float centerGX = x + 50;
    float centerGY = y + 50;
    
    glPushMatrix();
    glTranslatef(centerGX, centerGY, 0.0f);
    glRotatef(rotationAngleGoal, 0.0f, 0.0f, 1.0f);
    glTranslatef(-centerGX, -centerGY, 0.0f);
    
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + 100.0f, y);
    glVertex2f(x + 100.0f, y + 50.0f);
    glVertex2f(x, y + 50.0f);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(x, y + 10.0f);
    glVertex2f(x, y + 40.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x + 100.0f, y + 10.0f);
    glVertex2f(x + 100.0f, y + 40.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x, y + 10.0f);
    glVertex2f(x + 50.0f, y + 40.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x + 100.0f, y + 10.0f);
    glVertex2f(x + 50.0f, y + 40.0f);
    glEnd();
    
    glPopMatrix();
}

void drawCircle1(float radius, float offsetX, float offsetY) {
    glPushMatrix();
    glTranslatef(offsetX, offsetY, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);

    for (int angle = 0; angle <= 360; angle += 5) {
        float rad = angle * 3.14159f / 180.0f;
        float x = radius * cos(rad);
        float y = radius * sin(rad);
        glVertex2f(x, y);
    }

    glEnd();
    
    glColor3f(0.5, 0.5, 0.5);

    glBegin(GL_POINTS);
    for (int angle = 0; angle < 360; angle += 72) {
        float rad = angle * 3.14159f / 180.0f;
        float x = radius * cos(rad);
        float y = radius * sin(rad);
        glVertex2f(x, y);
    }
    glEnd();
    
    glPopMatrix();
    
    rotationAnglecircle += 3;
}


bool isDistanceValid(int x, int y, const std::set<std::pair<int, int>>& coordinatesSet)
{
    for (const auto& coordinate : coordinatesSet)
    {
        int dx = std::abs(x - coordinate.first);
        int dy = std::abs(y - coordinate.second);
        if (dx < 70 && dy < 70)
        {
            return false;
        }
    }
    return true;
}
void generateRandomCoordinates()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    std::set<std::pair<int, int>> coordinatesSet;
    
    coordinatesSet.insert({500, 300});

    for (int i = 0; i < 3; i++)
    {
        int x, y;
        do {
            x = rand() % 851 + 50;
            y = rand() % 480 + 60;
        } while (!coordinatesSet.empty() && !isDistanceValid(x, y, coordinatesSet));

        starCoordinates[i][0] = x;
        starCoordinates[i][1] = y;
        coordinatesSet.insert({x, y});
    }
    
    for (int i = 0; i < 2; i++)
    {
        int x, y;
        do {
            x = rand() % 851 + 50;
            y = rand() % 480 + 60;
        } while (!coordinatesSet.empty() && !isDistanceValid(x, y, coordinatesSet));

        cloverCoordinates[i][0] = x;
        cloverCoordinates[i][1] = y;
        coordinatesSet.insert({x, y});
    }

    for (int i = 0; i < 10; i++)
    {
        int x, y;
        do {
            x = rand() % 851 + 50;
            y = rand() % 480 + 60;
        } while (!coordinatesSet.empty() && !isDistanceValid(x, y, coordinatesSet));

        rockCoordinates[i][0] = x;
        rockCoordinates[i][1] = y;
        coordinatesSet.insert({x, y});
    }
    
    for (int i = 0; i < 30; i++)
    {
        int x, y;
        x = rand() % 851 + 50;
        y = rand() % 480 + 60;

        pointsCoordinates[i][0] = x;
        pointsCoordinates[i][1] = y;
    }

    for (int i = 0; i < 10; i++)
    {
        int x, y;
        do {
            x = rand() % 851 + 50;
            y = rand() % 480 + 60;
        } while (!coordinatesSet.empty() && !isDistanceValid(x, y, coordinatesSet));

        collectablesCoordinates[i][0] = x;
        collectablesCoordinates[i][1] = y;
        coordinatesSet.insert({x, y});
    }
}

void Key(unsigned char key, int x, int y) {
    if (key=='w') {
        if (PlayerY+2*radius<WindowSizeY) {
            PlayerY+=speed;
            isMovingUp=true;
            isMovingDown=false;
            isMovingLeft=false;
            isMovingRight=false;
        } else {
            Mix_Chunk* sound = Mix_LoadWAV(songsPaths.at(1).c_str());
            if (!sound) {
                std::cerr << "error" << std::endl;
            }

            Mix_PlayChannel(2, sound, 0);
            Mix_Volume(2, 100);
            PlayerY-=50;
            rectlen -= 20;
            if (rectlen<=0) {
                GameOver=true;
            }
        }
    }
    if (key=='s') {
        if (PlayerY>10+radius) {
            PlayerY-=speed;
            isMovingUp=false;
            isMovingDown=true;
            isMovingLeft=false;
            isMovingRight=false;
        } else {
            Mix_Chunk* sound = Mix_LoadWAV(songsPaths.at(1).c_str());
            if (!sound) {
                std::cerr << "error" << std::endl;
            }

            Mix_PlayChannel(2, sound, 0);
            Mix_Volume(2, 100);
            PlayerY+=50;
            rectlen -= 20;
            if (rectlen<=0) {
                GameOver=true;
            }
        }
    }
    if (key=='a') {
        if (PlayerX-2*radius>10) {
            PlayerX-=speed;
            isMovingUp=false;
            isMovingDown=false;
            isMovingLeft=true;
            isMovingRight=false;
        } else {
            Mix_Chunk* sound = Mix_LoadWAV(songsPaths.at(1).c_str());
            if (!sound) {
                std::cerr << "error" << std::endl;
            }

            Mix_PlayChannel(2, sound, 0);
            Mix_Volume(2, 100);
            PlayerX+=50;
            rectlen -= 20;
            if (rectlen<=0) {
                GameOver=true;
            }
        }
    }
    if (key=='d') {
        if (PlayerX + (2 * radius) < WindowSizeX - 10) {
            PlayerX+=speed;
            isMovingUp=false;
            isMovingDown=false;
            isMovingLeft=false;
            isMovingRight=true;
        } else {
            Mix_Chunk* sound = Mix_LoadWAV(songsPaths.at(1).c_str());
            if (!sound) {
                std::cerr << "error" << std::endl;
            }

            Mix_PlayChannel(2, sound, 0);
            Mix_Volume(2, 100);
            PlayerX-=50;
            rectlen -= 20;
            if (rectlen<=0) {
                GameOver=true;
            }
        }
    }
  glutPostRedisplay();
}

void KeyUp(unsigned char key, int x, int y) {
  glutPostRedisplay();
}

void drawText(float x, float y, const std::string& text)
{
    glRasterPos2f(x, y);

    for (const char& c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void drawRocket(float centerX, float centerY, float width, float height)
{
    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(centerX - width / 2.0f, centerY - height / 2.0f);
    glVertex2f(centerX + width / 2.0f, centerY - height / 2.0f);
    glVertex2f(centerX + width / 2.0f, centerY + height / 2.0f);
    glVertex2f(centerX - width / 2.0f, centerY + height / 2.0f);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(centerX - width / 2.0f, centerY - height / 2.0f);
    glVertex2f(centerX + width / 2.0f, centerY - height / 2.0f);
    glVertex2f(centerX + width / 2.0f, centerY + height / 2.0f);
    glVertex2f(centerX - width / 2.0f, centerY + height / 2.0f);
    glEnd();

     
    glColor3f(0.2f, 0.2f, 0.8f);

    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(centerX - width / 4.0f, centerY);
    glVertex2f(centerX + width / 4.0f, centerY);
    glEnd();

    glColor3f(1.0f, 0.5f, 0.0f);

    glBegin(GL_TRIANGLES);
    if (isMovingUp)
    {
        glVertex2f(centerX - width / 2.0f, centerY + height / 2.0f);
        glVertex2f(centerX + width / 2.0f, centerY + height / 2.0f);
        glVertex2f(centerX, centerY + height);
    }
    else if (isMovingDown)
    {
        glVertex2f(centerX - width / 2.0f, centerY - height / 2.0f);
        glVertex2f(centerX + width / 2.0f, centerY - height / 2.0f);
        glVertex2f(centerX, centerY - height);
    }
    else if (isMovingLeft)
    {
        glVertex2f(centerX - width / 2.0f, centerY - height / 2.0f);
        glVertex2f(centerX - width / 2.0f, centerY + height / 2.0f);
        glVertex2f(centerX - width, centerY);
    }
    else if (isMovingRight)
    {
        glVertex2f(centerX + width / 2.0f, centerY - height / 2.0f);
        glVertex2f(centerX + width / 2.0f, centerY + height / 2.0f);
        glVertex2f(centerX + width, centerY);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    float textX = centerX - (width + 20) / 4.0f;
    float textY = centerY - (height + 20) / 4.0f;


    std::string text = "Player";
    glRasterPos2f(textX, textY);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}



void drawRock(float centerX, float centerY, float size, float rotationAngle)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

    float radius = size / 2.0f;
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);

    const int numSegments = 100;
    const float angleIncrement = 2.0f * 3.1415926f / numSegments;

    for (int i = 0; i <= numSegments; i++)
    {
        float angle = i * angleIncrement;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(x, y);
    }

    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-radius, 0.0f);
    glVertex2f(radius, 0.0f);
    glVertex2f(radius, -radius);
    glVertex2f(-radius, -radius);
    glEnd();

    glPopMatrix();
}

void drawPoint(float xCenter, float yCenter) {
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(2.0);
    
    glBegin(GL_POINTS);
    glVertex2f(xCenter, yCenter);
    glEnd();
}

void drawStar(float centerX, float centerY, float outerRadius, float innerRadius, int numPoints, float rotationAngle)
{
    const float angleIncrement = 2.0f * 3.1415926f / numPoints;
    const float startAngle = -3.1415926f / 2.0f + rotationAngle;
    int loops = 100;
    
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < loops; i++) {
        float angle = i * 2 * 3.14159265 / loops;
        float x = centerX + outerRadius * cos(angle);
        float y = centerY + outerRadius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (int i = 0; i <= numPoints; i++)
    {
        float angle = startAngle + i * angleIncrement;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float x = centerX + radius * cosf(angle);
        float y = centerY + radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();

    for (int i = 0; i < 3; i++)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY);
        for (int j = 0; j <= numPoints / 2; j++)
        {
            float angle = startAngle + (i * 2 + j) * angleIncrement;
            float radius = innerRadius;
            float x = centerX + radius * cosf(angle);
            float y = centerY + radius * sinf(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < numPoints; i++) {
        float angle = i * 2 * 3.14159265 / numPoints;
        float x = centerX + outerRadius/4 * cos(angle);
        float y = centerY + outerRadius/4 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(0.0f, 1.0f, 1.0f);
}

void drawClover(float x, float y) {
    
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(rotationAngleGoal, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0.0f);
    
    // Draw the stem
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0); // Green color
    glVertex2f(x, y - 30);
    glVertex2f(x, y);
    glEnd();

    // Draw the petals
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // Red color
    glVertex2f(x - 12, y);
    glVertex2f(x, y + 12);
    glVertex2f(x + 12, y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0); // Green color
    glVertex2f(x, y + 12);
    glVertex2f(x + 12, y);
    glVertex2f(x, y - 12);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0); // Blue color
    glVertex2f(x + 12, y);
    glVertex2f(x, y - 12);
    glVertex2f(x - 12, y);
    glEnd();

    // Draw the center
    glPointSize(3.0);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 0.0); // Yellow color
    glVertex2f(x, y);
    glEnd();
    
    glPopMatrix();
}

void drawRect(int x, int y, int w, int h) {
  glBegin(GL_POLYGON);
  glVertex2f(x, y);
  glVertex2f(x + w, y);
  glVertex2f(x + w, y + h);
  glVertex2f(x, y + h);
  glEnd();
}

void Timer(int value) {
    
    rotationAnglestar++;
    
    if(!rot2){
        rotationAngleGoal += 5;
        if (rotationAngleGoal>10) {
            rot2=true;
        }
    }
    
    if (rot2){
        rotationAngleGoal -= 5;
        if (rotationAngleGoal<-10) {
            rot2=false;
        }
    }
    
    if(!rot){
        rotationAngle += 10;
        if (rotationAngle>=25) {
            rot=true;
        }
    }
    
    if (rot){
        rotationAngle-=10;
        if (rotationAngle<=-25) {
            rot=false;
        }
    }
    
    if (!GameOver) {
        TimerValue--;
        if (TimerValue % 2 == 0) {
            pointsShown = !pointsShown;
        }
        if (TimerValue <= 0) {
            GameOver = true;
        }
    }
    if (powerup) {
        if (seconds>0) {
            speed=30;
            seconds--;
        } else {
            speed=20;
            seconds=10;
            powerup=false;
        }
    }
    
    if (powerupClover) {
        if (seconds>0) {
            scoreFactor = 2;
            seconds--;
        } else {
            scoreFactor = 1;
            seconds=10;
            powerupClover=false;
        }
    }
    
    glutTimerFunc(1000, Timer, 0);
    glutPostRedisplay();
}

void drawCircle(int x, int y, float r ) {
  glPushMatrix();
  glTranslatef(x, y, 0);
  GLUquadric *quadObj = gluNewQuadric();
  gluDisk(quadObj, 0, r, 50, 50);
  glPopMatrix();
}

void print(int x, int y, char* string)
{
    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

bool isColliding(float x1, float y1, float x2, float y2, float radius) {
    float distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance <= radius;
}

void checkCollisions() {
    if (PlayerX < 0 || PlayerX > WindowSizeX-10 || PlayerY < 0 || PlayerY > WindowSizeY-10) {
        
    }

    for (int i = 0; i < 10; i++) {
        if (isColliding(PlayerX, PlayerY, rockCoordinates[i][0], rockCoordinates[i][1], radius+10)) {
            Mix_Chunk* sound = Mix_LoadWAV(songsPaths.at(1).c_str());
            if (!sound) {
                std::cerr << "error" << std::endl;
            }

            Mix_PlayChannel(2, sound, 0);
            Mix_Volume(2, 100);
            if (isMovingRight) {
                PlayerX-=50;
            }
            if (isMovingLeft) {
                PlayerX+=50;
            }
            if (isMovingUp) {
                PlayerY-=50;
            }
            if (isMovingDown) {
                PlayerY+=50;
            }
            if (rectlen<=20) {
                GameOver=true;
            }
            rectlen -= 20;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (isColliding(PlayerX, PlayerY, collectablesCoordinates[i][0], collectablesCoordinates[i][1], radius+10)) {
            Mix_Chunk* sound = Mix_LoadWAV(songsPaths.at(2).c_str());
            if (!sound) {
                std::cerr << "error" << std::endl;
            }

            Mix_PlayChannel(2, sound, 0);
            Mix_Volume(2, 100);
            collectablesCoordinates[i][0] = -100;
            collectablesCoordinates[i][1] = -100;
            score = score + (10 * scoreFactor);
        }
    }
    for (int i = 0; i < 3; i++) {
        if (isColliding(PlayerX, PlayerY, starCoordinates[i][0], starCoordinates[i][1], radius+7)) {
            starCoordinates[i][0] = -100;
            starCoordinates[i][1] = -100;
            powerup=true;
        
        }
    }
    for (int i = 0; i < 2; i++) {
        if (isColliding(PlayerX, PlayerY, cloverCoordinates[i][0], cloverCoordinates[i][1], radius+7)) {
            cloverCoordinates[i][0] = -100;
            cloverCoordinates[i][1] = -100;
            powerupClover=true;
        }
    }
    if (isColliding(PlayerX, PlayerY, 10, 10, radius+120) && score>=100) {
        Mix_Chunk* sound = Mix_LoadWAV(songsPaths.at(3).c_str());
        if (!sound) {
            std::cerr << "error" << std::endl;
        }

        Mix_PlayChannel(2, sound, 0);
        Mix_Volume(2, 100);
            youwin=true;
    }
}
void Display1(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
    glPushMatrix();
    glColor3f(1, 0, 0);
    char* p7s[20];
    sprintf((char*)p7s, "GAME LOSE");
    print(WindowSizeX/2, WindowSizeY/2, (char*)p7s);
    glFlush();
}
void Display2(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
    glPushMatrix();
    glColor3f(1, 0, 0);
    char* p6s[20];
    sprintf((char*)p6s, "GAME WIN");
    print(WindowSizeX/2, WindowSizeY/2, (char*)p6s);
    glFlush();
}


void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    checkCollisions();
    static float animationTime = 0.0f;
        animationTime += 0.01f;

    glPushMatrix();
    
    glColor3f(0, 0, 0);
    char* p0s[20];
    sprintf((char*)p0s, "Score: %d", score);
    print(x+160, y, (char*)p0s);
    
    
    glColor3f(0, 0, 0);
    char* p1s[20];
    sprintf((char*)p1s, "HP: " );
    print(x, y, (char*)p1s);
    if (pointsShown) {
        for (int i = 0; i < 30; i++) {
            drawPoint(pointsCoordinates[i][0], pointsCoordinates[i][1]);
        }
    }
    glColor3f(0, 1, 0);
    drawRect(x+50, y-4, rectlen, 20);
    for (int i = 0; i < 2; i++) {
        drawClover(cloverCoordinates[i][0], cloverCoordinates[i][1]);
    }
    glColor3f(0.0f, 1.0f, 1.0f);
    for (int i = 0; i < 3; i++) {
        drawStar(starCoordinates[i][0], starCoordinates[i][1], 15, 7.5, 10,rotationAnglestar);
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 10; i++) {
        drawRock(rockCoordinates[i][0], rockCoordinates[i][1], 30 ,rotationAngle);
    }
    for (int i = 0; i<10; i++) {
        glColor3f(1.0f, 1.0f, 0.0f);
        drawCircle1(outerRadius,collectablesCoordinates[i][0],collectablesCoordinates[i][1]);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle1(innerRadius, collectablesCoordinates[i][0], collectablesCoordinates[i][1]);
    }

    
    glColor3f(0, 0, 0);
    char* p2s[20];
    sprintf((char*)p2s, "Timer: %d ", TimerValue );
    print(x+300, y, (char*)p2s);
    glColor3f(0.2, 0.2, 0.2);
    drawRocket(PlayerX, PlayerY, 50, 30);
    if (youwin && !EndGameDisplayed) {
        
        Mix_HaltMusic();
        Mix_Music* audio = Mix_LoadMUS(songsPaths.at(5).c_str());
        if (!audio) {
            std::cerr << "error loading music" << std::endl;
        }
        
        Mix_VolumeMusic(20);
        if (Mix_PlayMusic(audio, -1) == -1) {
            std::cerr << "error playing music" << std::endl;
        }
        
        int currentWindow = glutGetWindow();
        glutDestroyWindow(currentWindow);
        int endGameWindow = glutCreateWindow("End Game");
        glutDisplayFunc(Display2);
        gluOrtho2D(0.0, WindowSizeX, 0.0, WindowSizeY);
        EndGameDisplayed = true;
        }
    if (GameOver && !EndGameDisplayed) {
        Mix_HaltMusic();
        Mix_Music* audio = Mix_LoadMUS(songsPaths.at(6).c_str());
        if (!audio) {
            std::cerr << "error loading music" << std::endl;
        }
        
        Mix_VolumeMusic(20);
        if (Mix_PlayMusic(audio, -1) == -1) {
            std::cerr << "error playing music" << std::endl;
        }
        int currentWindow = glutGetWindow();
        glutDestroyWindow(currentWindow);
        int endGameWindow = glutCreateWindow("End Game");
        glutDisplayFunc(Display1);
        gluOrtho2D(0.0, WindowSizeX, 0.0, WindowSizeY);
        EndGameDisplayed = true;
        }
    if (score>=100) {
        glColor3f(0, 0, 0);
        DrawGoal(10,10);
    }
    border();
    glFlush();
}



int main(int argc, char** argr) {
    glutInit(&argc, argr);
    glutInitWindowSize(WindowSizeX, WindowSizeY);
    glutCreateWindow("Game");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "error initializing sdl" << std::endl;
                return 1;
            }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "error in mixer" << std::endl;
            return 1;
        }
    
    std::vector<std::string> songs = {
        "/music/interstellar.mp3",
        "/music/deepvoice.mp3",
        "/music/dbz.mp3",
        "/music/oppai.mp3",
        "/music/steam.mp3",
        "/music/whitebeard.mp3",
        "/music/directed.mp3",
    };

    for (int i = 0; i < songs.size(); i++) {
        std::string path = __FILE__ + songs.at(i);
        std::string substring = "P07-52-1085.cpp/";
        size_t found = path.find(substring);
        if (found != std::string::npos) {
            path.erase(found, substring.length());
        }
       songsPaths.push_back(path);
    }
    Mix_Music* backgroundMusic = Mix_LoadMUS(songsPaths.at(0).c_str());
        if (!backgroundMusic) {
            std::cerr << "error" << std::endl;
        }

        Mix_VolumeMusic(18);
        if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
            std::cerr << "error" << std::endl;
        }

    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    generateRandomCoordinates();
    glutKeyboardFunc(Key);
    glutKeyboardUpFunc(KeyUp);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(0.0, WindowSizeX, 0.0, WindowSizeY);
    glutMainLoop();
    Mix_CloseAudio();
        SDL_Quit();
    return 0;
}
