/**
  * Monte Carlo Localization Test
  * Written by Eric Bakan
  */
#include <math.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <stdint.h>
#include <stdio.h>
#include "map.h"

//keyboard smoothers
Map m_map;
int m_mousex;
int m_mousey;
unsigned int m_width;
unsigned int m_height;

void swap(uint16_t* x, uint16_t* y)
{
    if(*x!=*y) {
	*x=*x^*y;
	*y=*x^*y;
	*x=*x^*y;
    }
}

//convert a value from 0-dim to -1 to 1
float convertpx(int px, int dim)
{
    return (px*2.0)/dim-1;
}

void bresenham_int(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Map* map)
{
    uint8_t steep=abs(y1-y0)>abs(x1-x0);
    if(steep) {
	swap(&x0,&y0);
	swap(&x1,&y1);
    }
    if(x0>x1) {
	swap(&x0,&x1);
	swap(&y0,&y1);
    }
    uint16_t deltax=x1-x0;
    uint16_t deltay=abs(y1-y0);
    int16_t error=deltax/2;
    uint16_t y=y0;
    int8_t ystep=y0<y1?1:-1;
    for(uint16_t x=x0;x<=x1;x++) {
	if(steep)
	    setVal(y,x,1,map);
	else
	    setVal(x,y,1,map);
	error-=deltay;
	if(error<0) {
	    y+=ystep;
	    error+=deltax;
	}
    }
}

void bresenham_float(float x0, float y0, float x1, float y1, Map* map)
{
    if(x1>1)
	x1=1;
    if(x1<-1)
	x1=-1;
    if(y1>1)
	y1=1;
    if(y1<-1)
	y1=-1;
    uint16_t x0val=(x0+1)/2*map->width;
    uint16_t y0val=(y0+1)/2*map->height;
    uint16_t x1val=(x1+1)/2*map->width;
    uint16_t y1val=(y1+1)/2*map->height;
    bresenham_int(x0val,y0val,x1val,y1val,map);
}

void glutKeyboardCallback(unsigned char key, int x, int y)
{
    switch(key) {
	case 'q':
	    exit(0);
	    break;
    }
}

void glutKeyboardUpCallback(unsigned char key, int x, int y)
{
}

void glutSpecialKeyboardCallback(int key, int x, int y)
{
    switch(key) {
	case GLUT_KEY_UP:
	    if(m_map.height<(1<<15) && m_map.width<(1<<15)) {
		free(m_map.data);
		initMap(&m_map,m_map.height*2+1,m_map.width*2+1);
	    }
	    break;
	case GLUT_KEY_DOWN:
	    if(m_map.height>(1) && m_map.width>(1)) {
		free(m_map.data);
		initMap(&m_map,m_map.height/2,m_map.width/2);
	    }
	    break;
    }
}

void glutSpecialKeyboardUpCallback(int key, int x, int y)
{
}

void glutReshapeCallback(int w, int h)
{
    glViewport(0,0,w,h);
    m_width=w;
    m_height=h;
}

void glutMouseFuncCallback(int button, int state, int x, int y)
{
}

void glutMotionFuncCallback(int x,int y)
{
    m_mousex=x;
    m_mousey=y;
}

void glutDisplayCallback(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float mousex_scaled=convertpx(m_mousex,m_width);
    float mousey_scaled=convertpx(m_mousey,m_height);


    
    glColor3f(0.f,1.f,0.f);

    //draw grid
    glLineWidth(1);
    drawGrid(&m_map);

    //draw map
    clearMap(&m_map);
    bresenham_float(0,0,mousex_scaled,mousey_scaled,&m_map);
    //printMap(&m_map);
    printf("\n");
    printf("x: %d y: %d\n",m_map.width,m_map.height);
    drawMap(&m_map);

    //draw mouse vector
    glColor3f(1.f,0.f,0.f);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(mousex_scaled,-mousey_scaled);
    glEnd();

    glutSwapBuffers();
}

void glutIdleCallback()
{
    glutPostRedisplay();
}

//basic structs
int main(int argc, char** argv)
{
    m_width=640;
    m_height=640;
    uint16_t mapwidth=63;
    uint16_t mapheight=63;
    initMap(&m_map,mapwidth,mapheight);
    char title[]="Bresehnam Line Algorithm";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(m_width,m_height);
    glutCreateWindow(title);

    glClearColor(0.0,0.0,0.0,1.0);

    glutKeyboardFunc(glutKeyboardCallback);
    glutKeyboardUpFunc(glutKeyboardUpCallback);
    glutSpecialFunc(glutSpecialKeyboardCallback);
    glutSpecialUpFunc(glutSpecialKeyboardUpCallback);
    glutReshapeFunc(glutReshapeCallback);
    glutIdleFunc(glutIdleCallback);
    glutMouseFunc(glutMouseFuncCallback);
    glutPassiveMotionFunc(glutMotionFuncCallback);
    glutMotionFunc(glutMotionFuncCallback);
    glutDisplayFunc(glutDisplayCallback);

    glutMainLoop();


    return 0;
}

