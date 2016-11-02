/* 
 * File:   main.cpp
 * Author: jcoelho
 *
 * Created on October 22, 2016, 11:24 AM
 */


// Reference: http://www.learnopengles.com/tag/per-vertex-lighting/

using namespace std;
#include "GraphicsShader.h"
#include "OpenGLMatrixManager.h"
#include "Vec3.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>

Vec3<float> eye(15, -430, 265); // apple eye
//Vec3<float> eye(0, 0, 50);
Vec3<float> center(0, 0, 0);
Vec3<float> up(0, 1, 0);


/**
 * Matriz de projecao OpenGL
 */
OpenGLMatrixManager projectionMatrix;

/**
 * Matriz de MODELVIEW OpenGL
 */
OpenGLMatrixManager modelViewMatrix;

/**
 * A shader pointer.
 */
GraphicsShader* vertexLightShader;
GraphicsShader* fragmentLightShader;
GraphicsShader* shader = nullptr;

float g_Scale = 1;


std::string readFile( const char* name )
{
    std::ifstream in( name );
    std::string shader;

    if (in.fail( ))
    {
        return "";
    }

    char a;
    while (in.get( a ) && a != EOF)
    {
        shader += a;
    }
    shader += '\0';
    return shader;
}



void initializeCanvas( )
{
    glClearColor( 1.0, 1.0, 1.0, 1.0 );

    //Aloca shader.
    vertexLightShader = new GraphicsShader( );

    std::string vertexShader1 = readFile( "shaders/vertexlight.vert" );
    vertexLightShader->setVertexProgram( vertexShader1.c_str( ), vertexShader1.size( ) );

    std::string fragmentShader1 = readFile( "shaders/vertexlight.frag" );
    vertexLightShader->setFragmentProgram( fragmentShader1.c_str( ), fragmentShader1.size( ) );


    fragmentLightShader = new GraphicsShader( );

    std::string vertexShader2 = readFile( "shaders/fragmentlight.vert" );
    fragmentLightShader->setVertexProgram( vertexShader2.c_str( ), vertexShader2.size( ) );

    std::string fragmentShader2 = readFile( "shaders/fragmentlight.frag" );
    fragmentLightShader->setFragmentProgram( fragmentShader2.c_str( ), fragmentShader2.size( ) );

    shader = vertexLightShader;
}



static void errorCallback( int error, const char* description )
{
    fprintf( stderr, "Error: %s\n", description );
}



static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose( window, GLFW_TRUE );

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        g_Scale /= 1.5;

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        g_Scale *= 1.5;

    if (key == GLFW_KEY_A)
        eye.addX(-5);
    if (key == GLFW_KEY_D)
        eye.addX(5);
    if (key == GLFW_KEY_W)
        eye.addY(-5);
    if (key == GLFW_KEY_S)
        eye.addY(5);

    if (key == GLFW_KEY_Q)
        eye.addZ(-5);
    if (key == GLFW_KEY_E)
        eye.addZ(5);

    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        if(shader == vertexLightShader)
            shader = fragmentLightShader;
        else
            shader = vertexLightShader;
    }


    printf("%.2f %.2f %.2f\n", eye.getX(), eye.getY(), eye.getZ());
}



static void resizeCallback( GLFWwindow* window, int w, int h )
{
    //Define o viewport.
    glViewport( 0, 0, w, h );

    //projectionMatrix.loadIdentity( );
    //projectionMatrix.ortho( -10, 10, -10, 10, -1, 1 );
}

bool loadModel(const std::string& filename, std::vector<Vec3<float>>& vertices, std::vector<unsigned int>& triangles)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if(!fp)
        return false;
    fscanf(fp, "OFF");

    int numVertices = 0;
    int numTriangles = 0;
    fscanf(fp, "%d %d 0", &numVertices, &numTriangles);
    vertices.resize(numVertices);
    triangles.resize(3 * numTriangles);

    for(int i = 0; i < numVertices; ++i) {
        float x, y, z;
        fscanf(fp, "%f %f %f", &x, &y, &z);
        vertices[i] = Vec3<float>(x, y, z);
    }
    for(int i = 0; i < numTriangles; ++i)
        fscanf(fp, "%*d %d %d %d", &triangles[3*i], &triangles[3*i+1], &triangles[3*i+2]);

    fclose(fp);
    return true;
}



int main( void )
{
    setvbuf(stdout, NULL, _IONBF, 0);

    std::vector<Vec3<float>> vertices;
    std::vector<unsigned int> triangles;
    //if(!loadModel("models/bimba.off", vertices, triangles))
    if(!loadModel("models/apple.off", vertices, triangles))
    //if(!loadModel("models/singleTriangle.off", vertices, triangles))
        return -1;

    std::vector<Vec3<float>> verticesNormal(vertices.size());

    for(int i = 0; i < triangles.size() / 3; ++i) {
        int vertex0 = triangles[3*i+0];
        int vertex1 = triangles[3*i+1];
        int vertex2 = triangles[3*i+2];
        Vec3<float> v0 = vertices[vertex0];
        Vec3<float> v1 = vertices[vertex1];
        Vec3<float> v2 = vertices[vertex2];
        Vec3<float> normal = Vec3<float>::crossProduct(v1 - v0, v2 - v0);
        normal.normalise();

        verticesNormal[vertex0] += normal;
        verticesNormal[vertex1] += normal;
        verticesNormal[vertex2] += normal;
    }

    for(int i = 0; i < verticesNormal.size(); ++i) {
        verticesNormal[i].normalise();
    }


    //Define uma callback de erro.
    glfwSetErrorCallback( errorCallback );

    //Inicializa a glew.
    if (!glfwInit( ))
    {
        exit( EXIT_FAILURE );
    }

    //Requere a versao do contexto opengl da janela a ser criada.
    GLFWwindow *window = glfwCreateWindow( 600, 600, "Simple example", NULL, NULL );
    if (!window)
    {
        glfwTerminate( );
        exit( EXIT_FAILURE );
    }

    //Define uma callbck para o teclado.
    glfwSetKeyCallback( window, keyCallback );

    glfwSetFramebufferSizeCallback( window, resizeCallback );

    //Define o contexto corrente.
    glfwMakeContextCurrent( window );

    //Inicializa o canvas.
    initializeCanvas( );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Enquanto a janela nao precisar ser fechada. Executa o loop.
    while (!glfwWindowShouldClose( window ))
    {
        Vec3<float> light = eye;
        //Limpa a janela.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLenum err = glGetError( );

        if (err != GL_NO_ERROR)
        {
            printf( "Error : %s\n", gluErrorString( err ) );
        }

        int w, h;
        glfwGetFramebufferSize( window, &w, &h );

        //Define o viewport.
        glViewport( 0, 0, w, h );

        projectionMatrix.loadIdentity( );
        projectionMatrix.perspective(60, w / (float)h, 1, 10000);

        //Aplica uma transformacao de escala.
        modelViewMatrix.push( );
        modelViewMatrix.lookAt(eye.getX(), eye.getY(), eye.getZ(), center.getX(), center.getY(), center.getZ(), up.getX(), up.getY(), up.getZ());
        modelViewMatrix.scale( g_Scale, g_Scale, 1 );

        //compila o shader se este nao tiver sido compilado ainda
        if (!shader->isAllocated( ))
            shader->compileShader( );

        //Carrega o programa na placa.
        shader->load( );

        unsigned int glShader = shader->getShaderIndex( );

        //Transfere os vertices para a placa.
        int vertexParam = glGetAttribLocation( glShader, "vtx" );
        glVertexAttribPointer( vertexParam, 3, GL_FLOAT, GL_FALSE, 0, vertices.data() );
        glEnableVertexAttribArray( vertexParam );

        // Transfere as normais para a placa.
        int normalParam = glGetAttribLocation( glShader, "normal" );
        glVertexAttribPointer( normalParam, 3, GL_FLOAT, GL_FALSE, 0, verticesNormal.data() );
        glEnableVertexAttribArray( normalParam );

        // Transfere light para a placa.
        int lightParam = glGetUniformLocation( glShader, "light" );
        glUniform4f( lightParam, light.getX(), light.getY(), light.getZ(), 1);

        // Transfere eye para a placa.
        int eyeParam = glGetUniformLocation( glShader, "eye" );
        glUniform4f( eyeParam, eye.getX(), eye.getY(), eye.getZ(), 1);

        //Obtem a modelview projection (mvp)
        {
            projectionMatrix.push( );

            //Multiplica a modelview pela projection.
            projectionMatrix.multMatrix( ( float* ) modelViewMatrix );

            //Transfere a matriz para a placa.
            int matrixParam = glGetUniformLocation( glShader, "mvp" );
            glUniformMatrix4fv( matrixParam, 1, GL_FALSE, ( float* ) projectionMatrix );

            projectionMatrix.pop( );
        }

        //Desempilha a matriz que foi empilhada para fazer a transformacao de escala.
        modelViewMatrix.pop( );

        //Desenha os elementos.
        glDrawElements( GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, triangles.data() );

        //Descarrega o programa da placa.
        shader->unload( );

        //Realiza a troca dos buffers.
        glfwSwapBuffers( window );

        //Renderiza continuamente.
        glfwPollEvents( );
    }

    //Destroi a janela.
    glfwDestroyWindow( window );

    //Finaliza a biblioteca.
    glfwTerminate( );


    exit( EXIT_SUCCESS );
}
