/* 
 * File:   main.cpp
 * Author: jcoelho
 *
 * Created on October 22, 2016, 11:24 AM
 */

using namespace std;
#include "GraphicsShader.h"
#include "OpenGLMatrixManager.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>


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
GraphicsShader* shader;



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
    shader = new GraphicsShader( );

    std::string vertexShader = readFile( "shaders/example.vert" );
    shader->setVertexProgram( vertexShader.c_str( ), vertexShader.size( ) );

    std::string fragmentShader = readFile( "shaders/example.frag" );
    shader->setFragmentProgram( fragmentShader.c_str( ), fragmentShader.size( ) );
}



static void errorCallback( int error, const char* description )
{
    fprintf( stderr, "Error: %s\n", description );
}



static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose( window, GLFW_TRUE );
}



static void resizeCallback( GLFWwindow* window, int w, int h )
{
    //Define o viewport.
    glViewport( 0, 0, w, h );

    projectionMatrix.loadIdentity( );
    projectionMatrix.ortho( -10, 10, -10, 10, -1, 1 );
}



int main( void )
{
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

    //Enquanto a janela nao precisar ser fechada. Executa o loop.
    while (!glfwWindowShouldClose( window ))
    {
        //Limpa a janela.
        glClear( GL_COLOR_BUFFER_BIT );
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
        projectionMatrix.ortho( -10, 10, -10, 10, -1, 1 );

        float coords[4 * 2] = { 0, 0, 1, 0, 1, 1, 0, 1 };
        unsigned int triangleMesh[6] = { 0, 1, 2, 0, 2, 3 };

        //Aplica uma transformacao de escala.
        modelViewMatrix.push( );
        modelViewMatrix.scale( 5, 5, 0 );

        //compila o shader se este nao tiver sido compilado ainda
        if (!shader->isAllocated( ))
            shader->compileShader( );

        //Carrega o programa na placa.
        shader->load( );

        unsigned int glShader = shader->getShaderIndex( );

        //Transfere os vertices para a placa.
        int vertexParam = glGetAttribLocation( glShader, "vtx" );
        glVertexAttribPointer( vertexParam, 2, GL_FLOAT, GL_FALSE, 0, coords );
        glEnableVertexAttribArray( vertexParam );

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
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, triangleMesh );

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
