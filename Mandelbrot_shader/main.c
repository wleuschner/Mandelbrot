#include<GL/glew.h>
#include<GL/glut.h>
#include"shader.h"

unsigned int program=0;
unsigned int shader1=0;
unsigned int shader2=0;
unsigned int iterationLocation=0;
unsigned int ofsLocation;
unsigned int zoomLocation;
int iterations=4096;
float xofs=0.0f;
float yofs=0.0f;
float zoom=1.0f;

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    program=glCreateProgramObjectARB();
    shader2=glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    shader1=glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    glShaderSourceARB(shader2,1,&vertex,0);
    glShaderSourceARB(shader1,1,&fragment,0);
    glCompileShader(shader2);
    glCompileShader(shader1);
    glAttachObjectARB(program,shader2);
    glAttachObjectARB(program,shader1);
    glLinkProgramARB(program);
    glUseProgramObjectARB(program);
    iterationLocation=glGetUniformLocationARB(program,"max_iter");
    glUniform1iARB(iterationLocation,iterations); 
    ofsLocation=glGetUniformLocationARB(program,"offset");
    glUniform2fARB(ofsLocation,xofs,yofs); 
    zoomLocation=glGetUniformLocationARB(program,"zoom");
    glUniform1fARB(zoomLocation,zoom); 
}

void keyboardFunc(unsigned char key, int x,int y)
{
    switch(key)
    {
        case '+':
            iterations++;
            glUniform1iARB(iterationLocation,iterations); 
            break;
        case '-':
            iterations--;
            glUniform1iARB(iterationLocation,iterations);
            break;
        case 'w':
            yofs+=0.1/zoom;
            glUniform2fARB(ofsLocation,xofs,yofs); 
            break;
        case 'a':
            xofs-=0.1/zoom;
            glUniform2fARB(ofsLocation,xofs,yofs); 
            break;
        case 's':
            yofs-=0.1/zoom;
            glUniform2fARB(ofsLocation,xofs,yofs); 
            break;
        case 'd':
            xofs+=0.1/zoom;
            glUniform2fARB(ofsLocation,xofs,yofs); 
            break;
        case 'u':
            zoom*=2;
            glUniform1fARB(zoomLocation,1/zoom); 
            break;
        case 'j':
            zoom/=2;
            glUniform1fARB(zoomLocation,1/zoom); 
    }
    glutPostRedisplay();
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(1,0,0);
    glVertex2f(-2,-1);
    glVertex2f(1,-1);
    glVertex2f(1,1);
    glVertex2f(-2,1);
    glEnd();
    glutSwapBuffers();
}

int main(int argc,char**argv)
{
    glutInit(&argc,argv);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutCreateWindow("Mandelbrot");
    glewInit();
    init();
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardFunc);
    glutMainLoop();
    return 0;
}
