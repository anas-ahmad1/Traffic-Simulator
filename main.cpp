#include <iostream>
#include"mygraphics.h"
#include"myconsole.h"
#include<windows.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <sstream>
using namespace std;

void triDown(int x,int y);
void triUp(int x,int y);
void triRight(int x,int y);
void triLeft(int x,int y);
void drawArrows();

class trafficsignal			//Class for traffic signals for each road on intersection
{
	char state;
	int signalNo;
	int timer;
	int x1,x2,y1,y2;
public:
	trafficsignal()
	{
		state=0;
		signalNo=0;
		timer=0;
		x1=x2=y1=y2=0;
	}
	trafficsignal(char s,int n,int xx1,int yy1,int xx2,int yy2,int t)
	{
		state=s;
		signalNo=n;
		x1=xx1;
		x2=xx2;
		y1=yy1;
		y2=yy2;
		timer=t;
	}
	void drawsignal()
	{
		COLORREF black = RGB(0,0,0);
		COLORREF red = RGB(215,0,0);
		COLORREF green = RGB(0,150,0);
		COLORREF yellow = RGB (255,255,0);
		myRect(x1,y1,x2,y2,black,black);
		if(state=='r')
			myEllipse(x1+2,y1+2,x2-2,y2-2,red,red);
		else if(state=='g')
			myEllipse(x1+2,y1+2,x2-2,y2-2,green,green);
		else if(state=='y')
			myEllipse(x1+2,y1+2,x2-2,y2-2,yellow,yellow);
	}
	char getState()
	{
		return state;
	}
	void changeState(char s)
	{
		state=s;
	}
	int getTime()
	{
		return timer;
	}
	void setTime(int t)
	{
		if(t!=0)
			timer=timer+t;
		else
			timer=0;
	}
};

class road:public trafficsignal			//Class to make road objects which inherit traffic signals for each road
{
	int roadnum;
	int x1,x2,y1,y2;
	int mid;
	char type;
	trafficsignal ts;		//Composition of traffic signal's object in road class
public:
	road()
	{
		roadnum=0;
		x1=x2=y1=y2=0;
		mid=0;
	}
	road(char t,int r,int xx1,int yy1,int xx2,int yy2,char s,int n,int tx1,int ty1,int tx2,int ty2,int timer):ts(s,n,tx1,ty1,tx2,ty2,timer)
	{
		roadnum=r;
		x1=xx1;
		x2=xx2;
		y1=yy1;
		y2=yy2;
		type=t;
		mid=0;
		drawroad();
		ts.drawsignal();
	}
	void getroad(int& r,int& xx1,int& xx2,int& yy1,int& yy2);
	void drawroad()
	{
		COLORREF yellow = RGB (255,255,0);
		COLORREF grey = RGB(50,50,50);
		myRect(x1,y1,x2,y2,grey,grey);
		if(type=='h')
		{
			mid=(y1+y2)/2;
			myLine(x1,mid,x2,mid,yellow);
		}
		else
		{
			mid=(x1+x2)/2;
			myLine(mid,y1,mid,y2,yellow);
		}
	}
	void changeSignal(char s)
	{
		ts.changeState(s);
		ts.drawsignal();
	}
	char getLight()
	{
		return ts.getState();
	}
	int getTimer()
	{
		return ts.getTime();
	}
	void setTimer(int t)
	{
		ts.setTime(t);
	}
};

class drawboard
{
public:
	virtual void drawcar()=0;
};

class car:public drawboard
{
	int x;
	int y;
	int colour;
	char direction;
public:
	car()
	{
		x=y=colour=0;
	}
	void setcar(int xx,int yy,int c,char d)
	{
		x=xx;
		y=yy;
		colour=c;
		direction=d;
	}
	void setcarCord(int xx,int yy)
	{
		x=x+xx;
		y=y+yy;
	}
	void getcarCord(int& xx,int& yy)
	{
		xx=x;
		yy=y;
	}
	char getdirection()
	{
		return direction;
	}
	int getcolor()
	{
		return colour;
	}
	void changeDirection(char d)
	{
		direction=d;
	}

	void drawcar() override 		//This function draws a car on specified position
	{
		COLORREF black = RGB(00,00,00);
		COLORREF color = RGB(255,178,102);
	
		if(this->colour==1)
			color = RGB(255,0,0);		//red car
		else if(this->colour==2)
			color = RGB (255,255,0);	//yellow car
		else if(this->colour==3)
			color = RGB(0,0,150);		//blue car
		else if(this->colour==4)
			color = RGB(0,200,0);		//green car
		else 
			color = RGB(255,178,102);	//peach car


		myRect(x-10,y-5,x+10,y+5,color,color);		//Draws upper half of car
		myRect(x-20,y,x+20,y+10,color,color);		//Draws lower half of car
		myEllipse(x-11,y+8,x-4,y+15,black,black);	//Draws left tyre of car
		myEllipse(x+4,y+8,x+11,y+15,black,black);	//Draws right tyre of car
	 }
	void removeCar()
	{
		COLORREF grey = RGB(50,50,50);
		myRect(x-20,y-5,x+20,y+15,grey,grey);
	}
};

class intersection
{
	int x1,x2,y1,y2;
public:
	intersection()
	{
		x1=x2=y1=y2=0;
	}
	intersection(int xx1,int yy1,int xx2,int yy2)
	{
		x1=xx1;
		x2=xx2;
		y1=yy1;
		y2=yy2;
		drawintersection();
	}
	void drawintersection()
	{
		COLORREF grey = RGB(50,50,50);
		myRect(x1,y1,x2,y2,grey,grey);
	}
};

bool hitting(car cars[],int n)			//Function to check coordinates of all cars to avoid hitting
{
	int x,y,thisx,thisy;
	bool hitting=false;
	char dir=cars[n].getdirection();
	char d;
	cars[n].getcarCord(thisx,thisy);
	for(int i=0;i<6;i++)
	{
		if(i!=n)
		{
			cars[i].getcarCord(x,y);
			d=cars[i].getdirection();
			if(dir=='r' && d=='r' && thisx+5>x-50 && thisx+5<x+20)
				hitting=true;
			else if(dir=='l' && d=='l' && thisx-5<x+50 && thisx-5>x-20)
				hitting=true;
			else if(dir=='d' && d=='d' && thisy+5>y-40 && thisy+5<y+10)
				hitting=true;
			else if(dir=='u' && d=='u' && thisy-5<y+40 && thisy>y-20)
				hitting=true;
		}
	}
	return hitting;
}

bool change()
{
	if(rand()%2==1)
		return true;
	else 
		return false;
}

int main()
{
	char direction,num[3],key;			//All variables to be used
	bool complete=false,starting=true;
	int xx,yy,t,turn=1,counter;
	double time,second;
	clock_t tStart = clock();			//Store time before starting main loop
	clock_t secondStart=clock();		//Store time to check for each second

	COLORREF grey = RGB(128,128,128);			//Colors to be used in traffic simulator graphics
	COLORREF white = RGB(255,255,255);
	COLORREF black = RGB(00,00,00);
	COLORREF text = RGB(64,128,128);
	COLORREF darkgrey = RGB(50,50,50);
	COLORREF green = RGB(0,200,0);
	COLORREF peach = RGB(255,178,102);
	COLORREF yellow = RGB (255,255,0);
	COLORREF red = RGB(255,0,0);
	COLORREF red2=RGB(180,0,0);
	COLORREF grey2 = RGB(100,100,100);
	COLORREF lgreen=RGB(200,150,34);

	myDrawTextWithFont(270,50,150,"TRAFFIC",black,white);			//Game Title
	myDrawTextWithFont(200,220,150,"SIMULATOR",black,white);
	myDrawTextWithFont(370,450,30,"Press any key to continue",white,black);
	key=getch();
	myDrawTextWithFont(270,50,150,"TRAFFIC",black,black);		//Removing title
	myDrawTextWithFont(200,220,150,"SIMULATOR",black,black);

	myDrawTextWithFont(170,30,90,"TRAFFIC SIMULATOR",black,white);	
	/*myDrawTextWithFont(30,170,40,"Murtaza Ahmed Khan: 20L-0954",white,black);
	myDrawTextWithFont(30,270,40,"Muhammad Usman Hameed: 20L-1198",white,black);
	myDrawTextWithFont(30,370,40,"Abdul Raheem Jalil: 21L-5287",white,black);*/
	key=getch();
	myDrawTextWithFont(370,450,30,"Press any key to continue",black,black);

	myRect(5,5,765,455,lgreen,lgreen);			//Background Color

	road r1('h',1,5,160,300,280,'g',1,270,290,290,310,0);				//Road 1
	road r2('h',2,470,160,765,280,'r',2,480,130,500,150,10);			//Road 2
	road r3('v',3,300,5,470,160,'r',3,270,130,290,150,20);				//Road 3
	road r4('v',4,300,280,470,455,'r',4,480,290,500,310,30);			//Road 4
	intersection i(300,160,470,280);									//Main Intersection

	car cars[6];
	cars[0].setcar(345,15,4,'d');		//Creating cars (Coordinates,Color,Direction)
	cars[1].setcar(430,435,2,'u');	
	cars[2].setcar(225,185,2,'l');	
	cars[3].setcar(430,105,1,'u');	
	cars[4].setcar(735,185,3,'l');
	cars[5].setcar(35,245,1,'r');	

	for(int i=0;i<6;i++)			//Loop to draw cars
	{
		cars[i].drawcar();
	}

	drawArrows();					//Drawing arrows to show direction of car movement

	while(complete==false)				//Main Loop
	{
		myDrawTextWithFont(780,20,30,"Press 'q' to Exit !",white,black);
		while(kbhit()==false)			//Loop continues until a key is pressed
		{
			time=(double)(clock()-tStart)/CLOCKS_PER_SEC;			//Calculating time passed after setting starting time to 0
			second=(double)(clock()-secondStart)/CLOCKS_PER_SEC;
			counter=r1.getTimer();									//Getting timer for traffic signal 1 to print it
			num1(counter,num);
			if(counter==0)
				myDrawTextWithFont(240,290,20,num,green,black);
			else
				myDrawTextWithFont(240,290,20,num,red,black);

			counter=r2.getTimer();									//Getting timer for traffic signal 2 to print it
			num1(counter,num);
			if(counter==0)
				myDrawTextWithFont(520,130,20,num,green,black);
			else
				myDrawTextWithFont(520,130,20,num,red,black);

			counter=r3.getTimer();									//Getting timer for traffic signal 3 to print it
			num1(counter,num);
			if(counter==0)
				myDrawTextWithFont(240,130,20,num,green,black);
			else
				myDrawTextWithFont(240,130,20,num,red,black);

			counter=r4.getTimer();									//Getting timer for traffic signal 4 to print it
			num1(counter,num);
			if(counter==0)
				myDrawTextWithFont(520,290,20,num,green,black);
			else
				myDrawTextWithFont(520,290,20,num,red,black);

			t=time;
			if(starting==true)			//Just for the starting of program
				{
					turn++;
					starting=false;
				}
			if(time>9.9 && time<10.1)			//Check whether 10 seconds have been passed or not to change signal lights
			{
				if(turn==1)
				{
					r1.setTimer(0);
					r1.changeSignal('y');
					Sleep(500);
					r1.changeSignal('g');
					r4.changeSignal('r');
					r4.setTimer(30);
				}
				else if(turn==2)
				{
					r2.setTimer(0);
					r1.changeSignal('r');
					r2.changeSignal('y');
					Sleep(500);
					r2.changeSignal('g');
					r1.setTimer(30);
				}
				else if(turn==3)
				{
					r3.setTimer(0);
					r2.changeSignal('r');
					r3.changeSignal('y');
					Sleep(500);
					r3.changeSignal('g');
					r2.setTimer(30);
				}
				else if(turn==4)
				{
					r4.setTimer(0);
					r3.changeSignal('r');
					r4.changeSignal('y');
					Sleep(500);
					r4.changeSignal('g');
					r3.setTimer(30);
				}
				turn++;
				if(turn>4)
					turn=turn-4;
				tStart = clock();
			}
			if(second>=1)					//If 1 second is passed then timer is decremented
			{
				if(turn==2)
				{
					r2.setTimer(-1);
					r3.setTimer(-1);
					r4.setTimer(-1);
					secondStart = clock();
				}
				else if(turn==3)
				{
					r3.setTimer(-1);
					r4.setTimer(-1);
					r1.setTimer(-1);
					secondStart = clock();
				}
				else if(turn==4)
				{
					r4.setTimer(-1);
					r1.setTimer(-1);
					r2.setTimer(-1);
					secondStart = clock();
				}
				else if(turn==1)
				{
					r1.setTimer(-1);
					r2.setTimer(-1);
					r3.setTimer(-1);
					secondStart = clock();
				}
			}

			for(int i=0;i<6;i++)				//Loop for movement of all cars
			{
				direction=cars[i].getdirection();
				if(direction=='r' && hitting(cars,i)==false)		//Cars moving right
				{
					cars[i].getcarCord(xx,yy);
					if(xx>260 && xx<300 && yy==245 && r1.getLight()=='r')		//Condition to stop car at signal
					{
					}
					else
					{
						if(xx>740)
						{
							cars[i].removeCar();
							cars[i].setcarCord(-720,0);
						}
						cars[i].removeCar();
						cars[i].setcarCord(5,0);
						cars[i].drawcar();

						cars[i].getcarCord(xx,yy);
						if(xx==345 && change()==true)
							cars[i].changeDirection('d');
						cars[i].getcarCord(xx,yy);
						if(xx==430 && change()==true)
							cars[i].changeDirection('u');
					}
				}
				else if(direction=='l' && hitting(cars,i)==false)		//Cars moving left
				{
					cars[i].getcarCord(xx,yy);
					if(xx<510 && xx>480 && yy==185 && r2.getLight()=='r')		//Condition to stop car at signal
					{
					}
					else
					{
						if(xx<30)
						{
							cars[i].removeCar();
							cars[i].setcarCord(+720,0);
						}
						cars[i].removeCar();
						cars[i].setcarCord(-5,0);
						cars[i].drawcar();
				
						cars[i].getcarCord(xx,yy);
						if(xx==345 && change()==true)
							cars[i].changeDirection('d');
						cars[i].getcarCord(xx,yy);
						if(xx==430 && change()==true)
							cars[i].changeDirection('u');
					}
				}
				else if(direction=='u' && hitting(cars,i)==false)		//Cars moving up
				{
					cars[i].getcarCord(xx,yy);
					if(xx==430 && yy>290 && yy<310 && r4.getLight()=='r')		//Condition to stop car at signal
					{
					}
					else
					{
						if(yy<15)
						{
							cars[i].removeCar();
							cars[i].setcarCord(0,430);
						}
						cars[i].removeCar();
						cars[i].setcarCord(0,-5);
						cars[i].drawcar();

						cars[i].getcarCord(xx,yy);
						if(yy==245 && change()==true)
							cars[i].changeDirection('r');
						cars[i].getcarCord(xx,yy);
						if(yy==185 && change()==true)
							cars[i].changeDirection('l');
					}
				}
				else if(direction=='d' && hitting(cars,i)==false)		//Cars moving down
				{
					cars[i].getcarCord(xx,yy);
					if(xx==345 &&  yy>130 && yy<150 && r3.getLight()=='r')		//Condition to stop car at signal
					{
					}
					else
					{
						if(yy>430)
						{
							cars[i].removeCar();
							cars[i].setcarCord(0,-415);
						}
						cars[i].removeCar();
						cars[i].setcarCord(0,5);
						cars[i].drawcar();

						cars[i].getcarCord(xx,yy);
						if(yy==245 && change()==true)
							cars[i].changeDirection('r');
						cars[i].getcarCord(xx,yy);
						if(yy==185 && change()==true)
							cars[i].changeDirection('l');
					}
				}
				Sleep(10);		//To slow down car speed
			}
		}
		key=getch();
		if(key=='q')
		{
			complete=true;
		}
	}
}


void triDown(int x,int y)
{
	int size=6;
	COLORREF white = RGB(255,255,255);
	for(int i=-size;i<size;i++)
	{
		myLine(x,y+5,x+i,y,white);
	}
}

void triUp(int x,int y)
{
	int size=6;
	COLORREF white = RGB(255,255,255);
	for(int i=-size;i<size;i++)
	{
		myLine(x,y-5,x+i,y,white);
	}
}

void triRight(int x,int y)
{
	int size=6;
	COLORREF white = RGB(255,255,255);
	for(int i=-size;i<size;i++)
	{
		myLine(x,y,x-5,y+i,white);
	}
}

void triLeft(int x,int y)
{
	int size=6;
	COLORREF white = RGB(255,255,255);
	for(int i=-size;i<size;i++)
	{
		myLine(x,y,x+5,y+i,white);
	}
}

void drawArrows()
{
	COLORREF white = RGB(255,255,255);
	myRect(110,295,170,300,white,white);		//right going arrows
	triRight(175,297);						
	myRect(610,295,670,300,white,white);
	triRight(675,297);						

	myRect(110,145,170,150,white,white);		//left going arrows
	triLeft(105,147);
	myRect(610,145,670,150,white,white);
	triLeft(605,147);

	myRect(280,40,285,90,white,white);			//down going arrows
	triDown(282,90);
	myRect(280,360,285,410,white,white);
	triDown(282,410);

	myRect(485,40,490,90,white,white);			//up going arrows
	triUp(487,40);
	myRect(485,360,490,410,white,white);
	triUp(487,360);
}