#ifndef CARREGA
#define CARREGA

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

class CarregarArquivo
{
public:
    vector<vector<GLfloat> > vertices; //guarda os vertices
    vector<vector<GLfloat> > normais;   //guarda as normais
    vector<vector<GLfloat> > texturas;  //guarda as texturas
    vector<vector<vector<GLint> > > faces; //guarda as faces
    void Carregar(char in[]){

        ifstream arquivo(in);
        if(!arquivo)
            cout << "Erro ao ler o arquivo";
        char lido;
        arquivo.get(lido); // pega o primeiro char


        while(arquivo)
        {

            if(lido == 'v') // olha se � um v,vn ou vt
            {
                arquivo.get(lido); // usado para decidir se � v,vn ou vt
                float aux ;
                GLfloat aux1 ;
                vector<GLfloat> vauxf;// ser� colocado na lista
                if(lido == 'n') // olha se � normal
                {
                    arquivo >>  aux; //pega o x
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o y
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o z

                    aux1 =  aux;
                    vauxf.push_back(aux1);
                    normais.push_back(vauxf); //coloca na sua lista

                }
                if(lido == 't') // olha se � textura
                {
                    arquivo >>  aux; //pega o x
                    aux1 =  aux;
                    vauxf.push_back(aux1);
                    arquivo >>  aux; //pega o y
                    aux1 =  aux;
                    vauxf.push_back(aux1);
                    texturas.push_back(vauxf); //coloca na sua lista

                }
                if(lido == ' ') // olha se � vertice
                {
                    arquivo >>  aux; //pega o x
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o y
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o z

                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    vertices.push_back(vauxf);//coloca na sua lista

                }
            }
            if(lido == 'f')
            {
                vector<vector<GLint> > face; // a face pode ter qualquer numero de vertices...
                arquivo.get(lido);
                if(lido == ' ')
                {
                    bool barras = true;

                    string t;
                    getline(arquivo,t);

                    unsigned int found = t.find("//");

                    if (found!=string::npos) //vertices e normal
                    {
                        barras = false;
                        vector<GLint> vert_normal;
                        GLint valor_inteiro ;
                        int valor = 0;
                        int numero = 10;

                        for(unsigned int i = 0 ; i < t.size(); i++)
                        {
                            char ch = t[i];
                            if(ch >= '0' && ch <= '9')
                            {
                                valor = valor*numero;
                                valor = valor + ((int)ch - (int)'0');//constroi numero

                                if(i== t.size() -1)// chegou no final da linha
                                {
                                    valor_inteiro  = valor -1;
                                    vert_normal.push_back(valor_inteiro );

                                    valor = 0;

                                    face.push_back(vert_normal);
                                    vector<GLint> aux4;
                                    vert_normal = aux4; //limpar

                                }


                            }
                            else if(ch == '/')
                            {
                                valor_inteiro  = valor -1;
                                vert_normal.push_back(valor_inteiro );
                                i++;
                                vert_normal.push_back(-1); //textura n�o existe
                                valor = 0;


                            }
                            else
                            {
                                valor_inteiro  = valor -1;
                                vert_normal.push_back(valor_inteiro );

                                valor = 0;
                                face.push_back(vert_normal);
                                vector<GLint> aux4;
                                vert_normal = aux4;

                            }




                        }

                        faces.push_back(face);


                    }
                    if(barras) //n�o tem normal
                    {

                        found = t.find("/"); //

                        if (found!=string::npos)//tem textura
                        {

                            vector<GLint> vert_textura;
                            GLint valor_inteiro ;
                            int valor = 0;
                            int numero = 10;


                            for(unsigned int i = 0 ; i < t.size(); i++)
                            {

                                char ch = t[i];

                                if(ch >= '0' && ch <= '9')
                                {
                                    valor = valor*numero;
                                    valor = valor + ((int)ch - (int)'0');

                                    if(i== t.size() -1)
                                    {
                                        valor_inteiro = valor -1;
                                        vert_textura.push_back(valor_inteiro);
                                        vert_textura.push_back(-1);//normal
                                        valor = 0;

                                        face.push_back(vert_textura);
                                        vector<GLint> aux4;
                                        vert_textura = aux4;

                                    }


                                }
                                else if(ch == '/')
                                {
                                    valor_inteiro = valor -1;
                                    vert_textura.push_back(valor_inteiro); //vertice



                                    valor = 0;


                                }
                                else
                                {
                                    valor_inteiro = valor -1;
                                    vert_textura.push_back(valor_inteiro);
                                    vert_textura.push_back(-1);//normal

                                    valor = 0;

                                    face.push_back(vert_textura);
                                    vector<GLint> aux4;
                                    vert_textura = aux4;

                                }




                            }

                            faces.push_back(face);


                        }
                        else // s� tem vertice
                        {
                            vector<GLint> vertice_resto;
                            GLint valor_inteiro ;
                            int valor = 0;
                            int numero = 10;

                            for(unsigned int i = 0 ; i < t.size(); i++)
                            {

                                char ch = t[i];

                                if(ch >= '0' && ch <= '9')
                                {
                                    valor = valor*numero;
                                    valor = valor + ((int)ch - (int)'0');

                                    if(i== t.size() -1)
                                    {
                                        valor_inteiro = valor -1;
                                        vertice_resto.push_back(valor_inteiro);//vertice
                                        vertice_resto.push_back(-1);//textura
                                        vertice_resto.push_back(-1);//normal
                                        valor = 0;

                                        face.push_back(vertice_resto);

                                        vector<GLint> aux4;
                                        vertice_resto = aux4;

                                    }


                                }
                                else
                                {
                                    valor_inteiro = valor -1;

                                    vertice_resto.push_back(valor_inteiro);//vertice
                                    vertice_resto.push_back(-1);//textura
                                    vertice_resto.push_back(-1);//normal
                                    valor = 0;

                                    face.push_back(vertice_resto);
                                    vector<GLint> aux4;
                                    vertice_resto = aux4;

                                }




                            }

                            faces.push_back(face);






                        }

                    }


                }




            }


            arquivo.get(lido);

        }

    }
};

#endif // CARREGA
