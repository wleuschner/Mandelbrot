#include"shader.h"

char* fragment=
"uniform vec2 offset;"
"uniform float zoom;"\
"uniform int max_iter;"\
"varying vec2 vPos;"\
"void main()"\
"{"\
"    float imag=(vPos.y)*zoom+offset.y;"\
"    float real=(vPos.x)*zoom+offset.x;"\
"    int i;"\
"    for(i=0;i<max_iter;i++)"\
"    {"\
"        float re=real*real-imag*imag;"\
"        float im=2.0f*real*imag;"\
"        imag = im + vPos.y*zoom+offset.y;"\
"        real = re + vPos.x*zoom+offset.x;"\
"        if((real*real+imag*imag)>4.0f)"\
"            break;"\
"    }"\
"        gl_FragColor = vec4(0.0,1.0/(log( log( float(i) ) / log(2)) ) ,0.0,1.0);"\
"}";

char* vertex=
"varying vec2 vPos;"\
"void main()"\
"{"\
"    vPos = vec2(gl_Vertex);"\
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"\
"}";
