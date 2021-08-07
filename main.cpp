#include"Header.h"
#include"Model.h"
#include "LoadTexture.h"
#include "Dice.h"

#define WIDTH 1280
#define HEIGHT 720

#define INTERVAL 15


using namespace std;


float x1=.245;
float x2=.34;
float y=.64;
float y2=.77;

int mousex,mousey;

int snakeHead[6]={99,92,87,62,54,17};
int snakeTail[6]={79,73,36,18,34,7};
int ladderTop[6]={91,84,67,42,38,31};
int ladderButtom[6]={71,44,51,21,2,9};
int snaketrigger[3]={40,69,83};
int laddertrigger[3]={25,58,75};
bool snakeOn=true;
bool ladderInv=false;

bool finish=false;

int numberOfPlayer=4;

float cord[101][3];
int playerPos[4]={0,0,0,0};
int sendPos[4]={0,0,0,0};

void givecoord();

bool nextrun=false;
bool nextTurn = true;
bool statusCheck = false;

int valueFromDice;
int playerTurn=0;


Model board;
Model player1;
Model player2;
Model player3;
Model player4;
Model snakeAll;
Model ladderUp;
Model text[4];

float resizedW;
float resizedH;


void init();
void display();
void reshape(int, int);
void timer(int);
void dice_drag(int, int, int, int);
void game(int);
void movePlayer(int);
void cutEatSlide();
bool ladderTriggerCheck();
bool snakeTriggerCheck();
void sendPosTOAnimate();


Dice dice;
int b;
int a= GLUT_UP;
float angle = 0.0;
int roll_output = 0;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

    int POS_X=(glutGet(GLUT_SCREEN_WIDTH)-WIDTH)>>1;
    int POS_Y=(glutGet(GLUT_SCREEN_HEIGHT)-HEIGHT)>>1;

    glutInitWindowPosition(POS_X,POS_Y);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("SNAKE AND LADDER GAME");
    init();

    dice.imgTex();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, timer, 0);
    glutMouseFunc(dice_drag);
    glutMainLoop();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    givecoord();
    for(int i=0;i<101;i++){
        cout<<cord[i][0]<<" "<<cord[i][1]<<" "<<cord[i][2]<<endl;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[]={-1,10,100,1};
    glLightfv(GL_LIGHT0,GL_POSITION,light_pos);

    board.load("../../Models/board.obj");
    player1.load("../../Models/player1.obj");
    player2.load("../../Models/player2.obj");
    player3.load("../../Models/player3.obj");
    player4.load("../../Models/player4.obj");
    snakeAll.load("../../Models/snakeAll.obj");
    ladderUp.load("../../Models/ladderUpMode.obj");
    text[0].load("../../Models/text1.obj");
    text[1].load("../../Models/text2.obj");
    text[2].load("../../Models/text3.obj");
    text[3].load("../../Models/text4.obj");
    glEnable(GL_DEPTH_TEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();

    glPointSize(10.0);

    glTranslatef(-5.5, -3, -15);
    glRotatef(10, -1, 1, 0);

    if (b == GLUT_LEFT_BUTTON && a == GLUT_DOWN  && mousex>x1*resizedW && mousex<x2*resizedW  && mousey>y*resizedH && mousey<y2*resizedH)
    {
        glRotatef(angle + 30, 1, 20, 1);
        glRotatef(angle-26.235879, 15, -1, 6);
        dice.roll(roll_output);
    }

    dice.updateCoord(roll_output);

    dice.draw();
    glDisable(GL_TEXTURE_2D);

    glLoadIdentity();

    glTranslatef(-2,-5.5,-15);
    glRotatef(30,1,0,0);
    board.draw();
    player1.draw();
    player2.draw();
    player3.draw();
    player4.draw();
    if(snakeOn)
        snakeAll.draw();
    ladderUp.draw();
    glLoadIdentity();


    if(!finish && nextrun){

        game(roll_output);
        sendPosTOAnimate();
        player1.load("../../Models/player1.obj");
        player2.load("../../Models/player2.obj");
        player3.load("../../Models/player3.obj");
        player4.load("../../Models/player4.obj");
    }

    glLoadIdentity();
    glTranslatef(-6,2,-15);
    switch(playerTurn){
    case 0:
        text[0].draw();
        break;
    case 1:
        text[1].draw();
        break;
    case 2:
        text[2].draw();
        break;
    case 3:
        text[3].draw();
        break;
    }

    glutSwapBuffers();
    glFlush();
}

void reshape(int w, int h)
{
    resizedW=w;
    resizedH=h;
    const float ar = (float) w / (float) h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-ar, ar, -1,1, 2, 150.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/10, timer, 0);

    if(angle == 360)
        angle = 0.0;

    angle += rand() % 10 + 20;
}

void dice_drag(int button, int action, int xMouse, int yMouse)
{
    b = button;
    a = action;
    mousex=xMouse;
    mousey=yMouse;


    if(button == GLUT_LEFT_BUTTON && action == GLUT_UP  && xMouse>x1*resizedW && xMouse<x2*resizedW  && yMouse>y*resizedH && yMouse<y2*resizedH )
    {
        std::cout<<std::endl<<"Roll Output: "<<roll_output<<std::endl;
        cout<<xMouse<<" "<<yMouse<<endl;
         nextrun=true;
    }
}

void givecoord(){

    float x=0;
    float y=0;
    float z=0;

    int xadder=1;
    cord[0][0]=0;
    cord[0][1]=0;
    cord[0][2]=0;
    x+=xadder;

    for(int i=1;i<101;i=i){
        cord[i][0]=x;
        cord[i][1]=y;
        cord[i][2]=z;
        i++;
        if(i%10==1)
        {
            xadder*=-1;
            y+=1;
            z-=1;
        }
        else
            x+=xadder;
    }

}

void game(int n){

    if(!statusCheck)
    {

        if(nextTurn)
        {
            valueFromDice=n;

            //cout<<playerTurn+1<<" rolls ::"<<valueFromDice;

            if(playerPos[playerTurn]+valueFromDice<=100)
                playerPos[playerTurn]+=valueFromDice;

            //cout<<"  Now its pos::"<<playerPos[playerTurn]<<endl;

            nextTurn=false;
        }
       else
            movePlayer(playerTurn);
    }
    else
    {
        cutEatSlide();
        nextTurn=true;
        statusCheck=false;
        for(int i =0;i<numberOfPlayer;i++)
        {
            if(playerPos[i]==100)
                finish=true;
        }
    }
}

void movePlayer(int n)
{
    if(playerPos[n]==sendPos[n])
    {
        statusCheck=true;
    }
    else if(playerPos[n]>sendPos[n])
    {
        sendPos[n]+=1;
    }
}



void sendPosTOAnimate()
{
  player1.sendOffset(cord[sendPos[0]][0],cord[sendPos[0]][1],cord[sendPos[0]][2]);
  player2.sendOffset(cord[sendPos[1]][0],cord[sendPos[1]][1],cord[sendPos[1]][2]);
  player3.sendOffset(cord[sendPos[2]][0],cord[sendPos[2]][1],cord[sendPos[2]][2]);
  player4.sendOffset(cord[sendPos[3]][0],cord[sendPos[3]][1],cord[sendPos[3]][2]);
}

void cutEatSlide()
{

    ladderInv=ladderTriggerCheck();
    snakeOn=snakeTriggerCheck();

    for(int j=0;j<6;j++)
    {
        if(playerPos[playerTurn]==snakeHead[j] && snakeOn)
        {
            playerPos[playerTurn]=snakeTail[j];
            //cout<<"\tsnake:"<<playerPos[playerTurn]<<endl;
        }
        if(playerPos[playerTurn]==ladderButtom[j] && ladderInv==false)
        {
            playerPos[playerTurn]=ladderTop[j];
            //cout<<"\tladder:"<<playerPos[playerTurn]<<endl;
        }

        if(ladderInv)
        {
            for(int k=0;k<numberOfPlayer;k++)
            {
                if(playerPos[k]==ladderTop[j])
                {
                    playerPos[k]=ladderButtom[j];
                    //cout<<"\t"<<k+1<<" Dropped from ladder: "<<playerPos[k]<<endl;
                }
            }
        }
    }

    for(int j=0;j<numberOfPlayer;j++)
    {
        if(j!=playerTurn&&playerPos[playerTurn]==playerPos[j])
        {
            playerPos[j]=0;
            //cout<<"\t\t cut:"<<j+1<<"new pos::"<<playerPos[j]<<endl;
        }

    }

    for(int i=0;i<numberOfPlayer;i++)
    {
        sendPos[i]=playerPos[i];
    }

    playerTurn++;
    if(playerTurn==4)
        playerTurn=0;

    nextrun=false;
}

bool ladderTriggerCheck()
{
    for(int i=0;i<numberOfPlayer;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(playerPos[i]==laddertrigger[j])
                return true;
        }
    }

    return false;
}

bool snakeTriggerCheck()
{

    for(int i=0;i<numberOfPlayer;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(playerPos[i]==snaketrigger[j])
                return false;
        }
    }
    return true;
}
