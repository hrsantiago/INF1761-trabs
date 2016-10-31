
#ifndef MATH4F_H
#define	MATH4F_H
#include <cstring>

class Math4f
{
public:
    /**
     * Construtor default
     */
    Math4f( );

    /**
     * Construtor que recebe uma matriz por parametro
     * @param m - matriz inicial
     */
    Math4f( const float* m );

    /**
     * Carrega a matriz identidade na matriz corrente
     */
    void loadIdentity( );

    /**
     * Carrega uma nova matriz no lugar da matriz corrente
     * @param m - nova matriz a ser carregada no lugar da corrente
     */
    void setMatrix( const float *m );

    /**
     * destrutor
     */
    virtual ~Math4f( );

    /**
     * Acessa a posicao (x, y) da matriz e retorna uma referencia para
     * o valor.
     * @param x - linha da matriz
     * @param y - coluna da matriz
     * @return - referencia para o valor da posicao (x, y)
     */
    float& at( int x, int y );


    /**
     * Calcula a inversa da matriz corrente
     * @return - retorna a matriz inversa
     */
    Math4f inverse( );

    /**
     * Calcula a matriz transposta
     * @return retorna a matriz transposta
     */
    Math4f transposed( );


    /**
     * Operador de multiplicação entre matrizes
     * @return retorna o resultado da multiplicacao das matrizes
     */
    Math4f operator *(Math4f rhs);

    /**
     * Multiplica a matriz por um escalar
     * @param val - valor escalar
     * @return  - retorna a matrz resultante
     */
    Math4f operator*( float val );

    /**
     * Retorna a matrix em float *
     * @return 
     */
    void operator =( const Math4f& m)
    {
        memcpy( _matrix, m._matrix, 16 * sizeof ( float ) );
    }

    /**
     * Retorna a matrix em float *
     * @return 
     */
    operator float*()
    {
        return ( float* ) _matrix;
    }

    /**
     * Retorna a matrix em float *
     * @return 
     */
    operator const float*() const
    {
        return ( const float* ) _matrix;
    }

    float* getMatrix( )
    {
        return _matrix;
    }
private:

    /**
     * Calcula o determinante da matriz corrente
     * @return - retorna o valor do determinante
     */
    float determinant( );

    /**
     * vetor coluna representando uma matriz 4 x 4
     */
    float _matrix[16];
};

#endif	/* MATH4F_H */

