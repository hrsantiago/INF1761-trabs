/* 
 * File:   OpenGLMA.h
 * Author: jcoelho
 *
 * Created on June 7, 2013, 2:32 PM
 */

#ifndef OPENGLMatrxManager_H
#define OPENGLMatrxManager_H

#include <stack>

#include "Math4f.h"

class OpenGLMatrixManager
{
public:

    /**
     * Construtor default.
     */
    OpenGLMatrixManager( );

    /**
     * Destrutor.
     */
    ~OpenGLMatrixManager( );

    /**
     * Empilha a matriz corrente e coloca uma identidade como corrente.
     */
    void push( );

    /**
     * Retira a matriz corrente do topo.
     */
    void pop( );

    /**
     * Carrega a matriz identidade na matriz corrente.
     */
    void loadIdentity( );

    /**
     * Carrega a matriz m no lugar da matriz corrente.
     * @param m - matriz passada por parametro.
     */
    void loadMatrix( const float* m );

    /**
     * Multiplica a matriz corrente pela matriz m.
     * @param m - matriz passada por parametro.
     */
    void multMatrix( const float* m );

    /**
     * Aplica a operacao de translacao na matriz corrente.
     * @param x - coordenada x para o novo ponto de translacao.
     * @param y - coordenada y para o novo ponto de translacao.
     * @param z - coordenada z para o novo ponto de translacao.
     */
    void translate( float x, float y, float z );

    /**
     * Aplica uma esacala nas tres direcoes na matriz corrente.
     * @param x - valor de escala aplicado na direcao x.
     * @param y - valor de escala aplicado na direcao y.
     * @param z - valor de escala aplicado na direcao z.
     */
    void scale( float x, float y, float z );

    /**
     * Aplica a operacao de rotacao de um angulo em torno do vetor
     * (x, y, z).
     * @param a - angulo de rotacao.
     * @param (x, y, z) - vetor de rotacao.
     */
    void rotate( float a, float x, float y, float z );

    /**
     * Define as configuracoes da camera. Equivalente ao gluLookAt.
     * @param (eyeX, eyeY, eyeZ) - posicao do observador.
     * @param (centerX, centerY, centerY) - ponto de referencia para onde o
     * observador esta olhando.
     * @param (upX, upY, upZ) - vetor normal a "cabeca" do usuario.
     */
    void lookAt( float eyeX, float eyeY, float eyeZ,
                 float centerX, float centerY, float centerZ,
                 float upX, float upY, float upZ );

    /**
     * Descreve uma matriz de pespectiva para produzir a uma projecao em pespectiva.
     * Equivalente ao glFrustum.
     * @param (xMin, yMin) - ponto minimo.
     * @param (xMax, yMin) - ponto maximo.
     * @param zNear - distancia minima para o observador.
     * @param zFar - distancia maxima para o observador.
     */
    void frustum( float xMin, float xMax, float yMin, float yMax, float zNear, float zFar );

    /**
     * Matriz ortografica a ser multiplicada com a matriz corrente.
     * Equivalente ao glFrustum.
     * @param (left, right) - ponto minimo.
     * @param (bottom, top) - ponto maximo.
     * @param nearVal - distancia minima para o observador.
     * @param farVal - distancia maxima para o observador.
     */
    void ortho( float left, float right, float bottom, float top, float nearVal, float farVal );
    /**
     * Defina a pespectiva.
     * @param fovY - angulo de abertura do eixo y.
     * @param aspect - razao de aspecto. w/h.
     * @param znear - distancia minima pro observador.
     * @param zfar - distancia maxima pro observador.
     */
    void perspective( float fovY, float aspect, float zNear, float zFar );

    /**
     * Retorna a matrix corrente.
     * @param m - variavel onde sera armazenada a matriz corrente.
     */
    void getMatrix( float* m );

    /**
     * Obtem a matrix inversa transposta.
     * @param mit - matriz inversa transposta.
     */
    void getMatrixInverseTransposed( float* mit );

    /**
     * Calcula a matriz inversa e a retorna.
     * @return - matriz inversa da matriz corrente.
     */
    Math4f getMatrixInverse( );

    /**
     * Retorna a matrix em float *.
     * @return 
     */
    operator float*( )
    {
        return ( float* ) _currentMatrix;
    }

    /**
     * Retorna a matrix em float *.
     * @return 
     */
    operator const float*( ) const
    {
        return ( const float* ) _currentMatrix;
    }

    float* getMatrix( )
    {
        return _currentMatrix.getMatrix( );
    }
private:
    /**
     * Pilha de matrizes
     */
    std::stack<Math4f> _matrixStack;

    /**
     * Matriz corrente
     */
    Math4f _currentMatrix;

private:
    /**
     * Realiza o produto vetorial entre dois vetores.
     * @param x1, y1, z1 - vetor 1.
     * @param x2, y2, z2 - vetor 1.
     * @param [out] - x, y, z - vetor de saida.
     */
    void computeCrossProduct( float x1, float y1, float z1,
                              float x2, float y2, float z2,
                              float &x, float &y, float &z );
};

#endif /* OPENGLMatrxManager_H */
