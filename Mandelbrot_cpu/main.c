#include<SDL/SDL.h>
#include<math.h>
#include<pthread.h>
#include<stdlib.h>
#include"fixed.h"
#define ITERATIONS 512
SDL_Surface* screen;
int numth=5;
pthread_t threads[5];

typedef struct _AREA
{
    FIXED dx;
    FIXED dy;
    int x;
    int y;
    int width;
    int height;
    int* pixels;
    FIXED xofs;
    FIXED yofs;
    FIXED zoom;
} AREA;


void* mandelbrot(void* a)
{
    AREA *ar=a;

    int *pixels=ar->pixels;
    int width=ar->width;
    int height=ar->height;
    FIXED xofs=ar->xofs;
    FIXED yofs=ar->yofs;
    FIXED zoom=ar->zoom;

    FIXED dist;
    FIXED real;
    FIXED imag;
    FIXED dx=ar->dx;
    FIXED dy=ar->dy;
    FIXED fx=fp_fromInt(-2LL)+dx*ar->x;
    FIXED fy=fp_fromInt(-1LL)+dy*ar->y;

    int x,y,i;
    int*p = pixels+ar->y*width+ar->x;
    for(y=0;y<height;y++)
    {
        fy+=dy;
        for(x=0;x<width;x++)
        {
            fx+=dx;
            real=fp_mul(fx,zoom)+xofs;
            imag=fp_mul(fy,zoom)+yofs;
            dist=0LL;
            for(i=0;i<ITERATIONS && dist<fp_fromInt(4);i++)
            {
                FIXED realt=fp_mul(real,real)-fp_mul(imag,imag)+fp_mul(fx,zoom)+xofs;
                FIXED imagt=2LL*fp_mul(real,imag)+fp_mul(fy,zoom)+yofs;
                real=realt;
                imag=imagt;
                dist=fp_mul(real,real)+fp_mul(imag,imag);
            }
            /*if(i<ITERATIONS)
            {
                *p=0xFFFF0000;
            }
            else
            {
                *p=0xFF00FF00;
            }*/
            *p=0xFF000000|(unsigned int)(i);
            p++;

        }
        fx=fp_fromInt(-2LL);
    }
    return NULL;
}

void* calculate(int* pixels,int width,int height,FIXED xofs,FIXED yofs,FIXED zoom)
{
    int i=0;
    int hofs=height/numth;
    AREA *a=(AREA*)malloc(sizeof(AREA)*numth);
    FIXED dx=fp_fromInt(3LL)/width;
    FIXED dy=fp_fromInt(2LL)/height;
    for(i=0;i<numth;i++)
    {
        a[i].pixels=pixels;
        a[i].dx=dx;
        a[i].dy=dy;
        a[i].x=0;
        a[i].y=i*hofs;
        a[i].width=width;
        a[i].height=hofs;
        a[i].xofs=xofs;
        a[i].yofs=yofs;
        a[i].zoom=zoom;
        pthread_create(&threads[i],NULL,mandelbrot,&a[i]);
    }
    for(i=0;i<numth;i++)
    {
        pthread_join(threads[i],0);
    }

    free(a);
    return NULL;
    /*FIXED dist;
    FIXED real;
    FIXED imag;
    FIXED dx=fp_fromInt(3LL)/width;
    FIXED dy=fp_fromInt(2LL)/height;
    FIXED fx=fp_fromInt(-2LL);
    FIXED fy=fp_fromInt(-1LL);

    int x,y,i;
    int*p = pixels;
    for(y=0;y<height;y++)
    {
        fy+=dy;
        for(x=0;x<width;x++)
        {
            fx+=dx;
            real=fp_mul(fx,zoom)+xofs;
            imag=fp_mul(fy,zoom)+yofs;
            dist=0LL;
            for(i=0;i<ITERATIONS && dist<fp_fromInt(4);i++)
            {
                FIXED realt=fp_mul(real,real)-fp_mul(imag,imag)+fp_mul(fx,zoom)+xofs;
                FIXED imagt=2LL*fp_mul(real,imag)+fp_mul(fy,zoom)+yofs;
                real=realt;
                imag=imagt;
                dist=fp_mul(real,real)+fp_mul(imag,imag);
            }
            /*if(i<ITERATIONS)
            {
                *p=0xFFFF0000;
            }
            else
            {
                *p=0xFF00FF00;
            }
            *p=0xFF000000|(i);
            p++;

        }
        fx=fp_fromInt(-2LL);
    }
    return NULL;*/
}

int main(int argc,char**argv)
{
    int run=1;
    float dx=0.0f;
    float dy=0.0f;
    float zoom=1.0f;
    SDL_Event event;
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        return -1;
    }
    if((screen=SDL_SetVideoMode(800,600,32,SDL_DOUBLEBUF))==NULL)
    {
        return -1;
    }

    SDL_Surface* img=SDL_CreateRGBSurface(0,800,600,32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
    int* pixels=img->pixels;
    int width=img->w;
    int height=img->h;
    calculate(pixels,width,height,fp_fromFloat(dx),fp_fromFloat(dy),fp_fromFloat(zoom));
    while(run)
    {
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    run=0;
                    break;
                }
                case SDL_KEYUP:
                {
                    SDL_LockSurface(img);
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                        {
                            dx-=0.1*(1/zoom);
                            calculate(pixels,width,height,fp_fromFloat(dx),fp_fromFloat(dy),fp_fromFloat(1/zoom));
                            break;
                        }
                        case SDLK_RIGHT:
                        {
                            dx+=0.1*(1/zoom);
                            calculate(pixels,width,height,fp_fromFloat(dx),fp_fromFloat(dy),fp_fromFloat(1/zoom));
                            break;
                        }
                        case SDLK_UP:
                        {
                            dy-=0.1*(1/zoom);
                            calculate(pixels,width,height,fp_fromFloat(dx),fp_fromFloat(dy),fp_fromFloat(1/zoom));
                            break;
                        }
                        case SDLK_DOWN:
                        {
                            dy+=0.1*(1/zoom);
                            calculate(pixels,width,height,fp_fromFloat(dx),fp_fromFloat(dy),fp_fromFloat(1/zoom));
                            break;
                        }
                        case SDLK_PLUS:
                        {
                            zoom+=1;
                            calculate(pixels,width,height,fp_fromFloat(dx),fp_fromFloat(dy),fp_fromFloat(1/zoom));
                            break;
                        }
                        case SDLK_MINUS:
                        {
                            zoom-=1;
                            calculate(pixels,width,height,fp_fromFloat(dx),fp_fromFloat(dy),fp_fromFloat(1/zoom));
                            break;
                        }
                        case SDLK_ESCAPE:
                        {
                            run=0;
                            break;
                        }
                    }
                    SDL_UnlockSurface(img);
                }
            }    
        }
        SDL_BlitSurface(img,0,screen,0);
        SDL_Flip(screen);
        //run=0;
    }
    return 0;
}
