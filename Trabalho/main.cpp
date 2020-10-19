#include <iostream>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "CarregarArquivo.h"

#define PI 3.14159265

using namespace std;

class Eixos{
    public:
    float x,y,z;
    void inicializa(){
        x=0.0;
        y=0.0;
        z=0.0;
    }
};

class Entradas{
public:
    float velocidade;
    int rotacao;
    int delayCamera;

    void inicializa(){
        velocidade=0.0;
        rotacao = 0.0;
        delayCamera=0;

    }
};

Entradas input;
Eixos camera;

void teclado(unsigned char tecla, int x, int y){

    switch(tecla){
    case 'w':
        if(input.velocidade==0.0){
            input.velocidade=1;
        }else{
            input.velocidade=0.0;
        }
        break;
    case 's':
        if(input.velocidade==0.0){
            input.velocidade=-1;
        }else{
            input.velocidade=0.0;
        }
        break;
    case 'a':
        input.rotacao = (input.rotacao + 5) % 360;
        input.delayCamera=20;

        break;
    case 'd':
        input.rotacao = (input.rotacao - 5) % 360;
        input.delayCamera=-20;

        break;
    }

    glutPostRedisplay();
}

class Object3d{
protected:
    CarregarArquivo modelo;
public:
    Eixos posicao;

    void inicializa(char nomeArq[]){
        modelo.Carregar(nomeArq);

        glutKeyboardFunc(teclado);
        desenha();
    }

    void desenha(){

        for (unsigned int j = 0; j < (modelo.faces).size(); ++j ){
            glBegin ( GL_POLYGON );

            for (unsigned int i = 0; i < (modelo.faces[j]).size() ; ++i ){
                GLfloat vert[3] = {(modelo.vertices[modelo.faces[j][i][0]][0]),(modelo.vertices[modelo.faces[j][i][0]][1]),(modelo.vertices[modelo.faces[j][i][0]][2])};
                glVertex3fv ( vert );
            }

            glEnd( );
        }
    }
};

class Carro : public Object3d{
public:
    Object3d rodas[4];

    void desenha(){
        glPushMatrix();

        glTranslatef(posicao.x,0.0,posicao.z);
        if(input.velocidade!=0){
            glRotatef(input.rotacao,0,1,0);
        }
        desenhaRodas();
        glRotatef(90,1,0,0);
        glColor3f(1.0, 0.0, 0.0);
        Object3d::desenha();

        glPopMatrix();
    }

    void inicializa(char nomeArqCarro[],char nomeArqRoda[]){
        Object3d::inicializa(nomeArqCarro);
        for(int i=0;i<4;++i){
            rodas[i].inicializa(nomeArqRoda);
        }
    }

    void desenhaRodas(){
        glColor3f(0.0, 0.0, 0.0);

        glPushMatrix();
        glTranslatef(-1.6,0.0,0.0);
        giraRodas();
        glRotatef(5,0,input.delayCamera,0);
        glTranslatef(1.6,0.0,0.0);
        rodas[0].desenha();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.6,0.0,0.0);
        giraRodas();
        glRotatef(30,0,input.delayCamera,0);
        glTranslatef(-1.6,0.0,0.0);
        rodas[1].desenha();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0,0.0,-2.5);
        giraRodas();
        rodas[2].desenha();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-3.2,0.0,-2.5);
        giraRodas();
        rodas[3].desenha();
        glPopMatrix();

    }
    void giraRodas(){
        glRotatef(posicao.x,1,0,0);
        glRotatef(posicao.z,1,0,0);
    }

    void anda(){
        posicao.x = posicao.x + input.velocidade*sin(input.rotacao*PI/180);
        posicao.z = posicao.z + input.velocidade*cos(input.rotacao*PI/180);
        if(input.velocidade!=0){
            Sleep(30);
            camera.x = posicao.x + 13.0*sin((input.rotacao+180-input.delayCamera)*PI/180);
            camera.z = posicao.z + 13.0*cos((input.rotacao+180-input.delayCamera)*PI/180);
            if(input.delayCamera>0){
                input.delayCamera-=1;
            }else if(input.delayCamera<0){
                input.delayCamera+=1;
            }
        }
        glutPostRedisplay();

    }
};

Carro carro;

class Piso{
public:
    GLubyte imgBit[700][700][3];
    GLuint textura_id;
    void inicializa(){
        try{
            ifstream arq("pista.bmp" ,ios::binary);
            char c;
            if(!arq)
                cout << "Erro ao abrir";

            for(int i = 0; i < 54 ; i++)
                c = arq.get();
            for(int i = 0; i < 700 ; i++)
                for(int j = 0; j < 700 ; j++)
                {
                    c = arq.get();
                    imgBit[i][j][2] = c;
                    c =  arq.get();
                    imgBit[i][j][1] = c ;
                    c =  arq.get();
                    imgBit[i][j][0] = c;
                }

            arq.close();
            arq.clear();
        }catch(...){
            cout << "Erro ao ler imagem" << endl;
        }

        glGenTextures(1,&textura_id);
        // Associa a textura aos comandos seguintes
        glBindTexture(GL_TEXTURE_2D, textura_id);
        // Envia a textura para uso pela OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 700, 700, 0, GL_RGB,GL_UNSIGNED_BYTE, imgBit);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    void desenha(){
        glPushMatrix();

        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, textura_id);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        glTranslatef(0.0,-1.0,0.0);

        glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0);
        glNormal3f(0,-1,0);
        glTexCoord2f(1,1);
        glVertex3f(100.0,0.0,100.0);
        glTexCoord2f(0,1);
        glVertex3f(-100.0,0.0,100.0);
        glTexCoord2f(0,0);
        glVertex3f(-100.0,0.0,-100.0);
        glTexCoord2f(1,0);
        glVertex3f(100.0,0.0,-100.0);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
};
Piso pista;

void inicializa(){
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    carro.inicializa("carro.obj","roda.obj");
    input.inicializa();
    camera.inicializa();
    pista.inicializa();
    camera.z=10;
}

void alteraJanela(int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    if(h == 0) h = 1;

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(65, (GLfloat) w/(GLfloat) h, 0.5, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void desenha(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    carro.anda();
    gluLookAt(camera.x,10,camera.z, carro.posicao.x,0,carro.posicao.z, 0,1,0);
    //gluLookAt(0,20,0, carro.posicao.x,0,carro.posicao.z, 1,1,1);
    carro.desenha();
    pista.desenha();
    glutSwapBuffers();

}

void criaJanela(){
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition ((1280 - 800)/2, (800 - 800)/2);
    glutCreateWindow ("prototipo carrinho");
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    criaJanela();
    inicializa();

    glutDisplayFunc(desenha);
    glutReshapeFunc(alteraJanela);
    glutMainLoop();
    return 0;
}

















