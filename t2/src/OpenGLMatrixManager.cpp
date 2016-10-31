#include "OpenGLMatrixManager.h"
#include <cstring>
#include <cmath>



void OpenGLMatrixManager::computeCrossProduct( float x1, float y1, float z1,
                                               float x2, float y2, float z2,
                                               float &x, float &y, float &z )
{
    x = y1 * z2 - z1 * y2;
    y = z1 * x2 - x1 * z2;
    z = x1 * y2 - y1 * x2;
}



OpenGLMatrixManager::OpenGLMatrixManager( )
{
}



OpenGLMatrixManager::~OpenGLMatrixManager( )
{
}



void OpenGLMatrixManager::push( )
{
    //Empilha uma copia da matriz corrente.
    _matrixStack.push( _currentMatrix );

}



void OpenGLMatrixManager::pop( )
{
    //Torna a matriz do topo da pilha como matriz corrente.
    if (!_matrixStack.empty( ))
    {
        _currentMatrix = _matrixStack.top( );
        _matrixStack.pop( );
    }
}



void OpenGLMatrixManager::loadIdentity( )
{
    //Carrega a matriz identidade.
    _currentMatrix.loadIdentity( );
}



void OpenGLMatrixManager::loadMatrix( const float* m )
{
    _currentMatrix.setMatrix( m );
}



void OpenGLMatrixManager::multMatrix( const float* m )
{
    _currentMatrix = _currentMatrix * Math4f( m );
}



void OpenGLMatrixManager::translate( float x, float y, float z )
{
    //Constroi a matriz de translacao.
    Math4f translateMatrix;
    translateMatrix.at( 3, 0 ) = x;
    translateMatrix.at( 3, 1 ) = y;
    translateMatrix.at( 3, 2 ) = z;
    translateMatrix.at( 3, 3 ) = 1;

    //Multiplica pela matriz corrente.
    _currentMatrix = _currentMatrix * translateMatrix;
}



void OpenGLMatrixManager::scale( float x, float y, float z )
{
    //Constroi a matriz de escala.
    Math4f scaleMatrix;
    scaleMatrix.at( 0, 0 ) = x;
    scaleMatrix.at( 1, 1 ) = y;
    scaleMatrix.at( 2, 2 ) = z;
    scaleMatrix.at( 3, 3 ) = 1;

    //Multiplica pela matriz corrente.
    _currentMatrix = _currentMatrix * scaleMatrix;
}



void OpenGLMatrixManager::rotate( float a, float x, float y, float z )
{
    //Fonte: http://www.opengl.org/sdk/docs/man2/xhtml/glRotate.xml

    //Converte o angulo para radianos
    float angle = a * M_PI / 180.0;

    float c = cos( ( float ) angle );
    float s = sin( ( float ) angle );
    float lenght = sqrt( x * x + y * y + z * z );

    //Se o vetor for nulo nao faz nada.
    if (lenght < 10E-5)
        return;

    //Normaliza o vetor passado.
    x /= lenght;
    y /= lenght;
    z /= lenght;

    //Constroi a matriz de rotacao.
    float m[16] = { x * x * ( 1 - c ) + c, y * x * ( 1 - c ) + z * s, x * z * ( 1 - c ) - y * s, 0,
                    x * y * ( 1 - c ) - z * s, y * y * ( 1 - c ) + c, y * z * ( 1 - c ) + x * s, 0,
                    x * z * ( 1 - c ) + y * s, y * z * ( 1 - c ) - x * s, z * z * ( 1 - c ) + c, 0,
                    0, 0, 0, 1 };

    //Multiplica a matriz de rotacao pela matriz corrente.
    _currentMatrix = _currentMatrix * Math4f( m );
}



void OpenGLMatrixManager::lookAt( float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ )
{
    //Fonte: http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml

    //Calcula vetor f que vai da posicao do observador ao ponto de referencia.
    float fX = centerX - eyeX;
    float fY = centerY - eyeY;
    float fZ = centerZ - eyeZ;

    //Normaliza vetor f.
    float lenghtF = sqrt( fX * fX + fY * fY + fZ * fZ );
    fX /= lenghtF;
    fY /= lenghtF;
    fZ /= lenghtF;

    //Normalizando vetor normal a� cabeca do observador.
    float lenghtUp = sqrt( upX * upX + upY * upY + upZ * upZ );
    upX /= lenghtUp;
    upY /= lenghtUp;
    upZ /= lenghtUp;

    //Calculando vetor S.
    float sX = 0.0, sY = 0.0, sZ = 0.0;
    computeCrossProduct( fX, fY, fZ, upX, upY, upZ, sX, sY, sZ );

    float lenghtS = sqrt( sX * sX + sY * sY + sZ * sZ );

    sX /= lenghtS;
    sY /= lenghtS;
    sZ /= lenghtS;

    //Calculando vetor U.
    float uX = 0.0, uY = 0.0, uZ = 0.0;
    computeCrossProduct( sX, sY, sZ, fX, fY, fZ, uX, uY, uZ );

    //Montando a matriz M.
    float m[16] = { ( float ) sX, ( float ) uX, -( float ) fX, 0.0f,
                    ( float ) sY, ( float ) uY, -( float ) fY, 0.0f,
                    ( float ) sZ, ( float ) uZ, -( float ) fZ, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f };

    //Multiplica pela matriz corrente.
    _currentMatrix = _currentMatrix * Math4f( m );

    //Faz uma translacao inversa em relacao a posicao do observador.
    translate( -eyeX, -eyeY, -eyeZ );
}



void OpenGLMatrixManager::frustum( float left, float right, float bottom, float top, float nearVal, float farVal )
{
    //Fonte: http://www.opengl.org/sdk/docs/man2/xhtml/glFrustum.xml

    //Monta a matriz de frustum.
    float f[16] = { 2 * nearVal / ( right - left ), 0, 0, 0,
                    0, 2 * nearVal / ( top - bottom ), 0, 0,
                    ( right + left ) / ( right - left ), ( top + bottom ) / ( top - bottom ), -( farVal + nearVal ) / ( farVal - nearVal ), -1,
                    0, 0, -( 2 * farVal * nearVal ) / ( farVal - nearVal ), 0 };

    //Multiplica pela matriz corrente.
    _currentMatrix = _currentMatrix * Math4f( f );
}



void OpenGLMatrixManager::ortho( float left, float right, float bottom, float top, float nearVal, float farVal )
{

    //Fonte: http://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml

    //Constroi a matriz de projecao ortografica.
    float o[16] = { 2 / ( right - left ), 0, 0, 0,
                    0, 2 / ( top - bottom ), 0, 0,
                    0, 0, -2 / ( farVal - nearVal ), 0,
                    -( right + left ) / ( right - left ), -( top + bottom ) / ( top - bottom ), -( farVal + nearVal ) / ( farVal - nearVal ), 1 };

    //Multiplica pela matriz corrente.
    _currentMatrix = _currentMatrix * Math4f( o );
}



void OpenGLMatrixManager::perspective( float fovY, float aspect, float zNear, float zFar )
{
    //Fonte: http://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml

    //Converte para radianos.
    float angle = fovY * M_PI / 180.0;

    //Calcula a contange.
    float f = 1.0 / tan( angle / 2.0 );

    float p[16] = { ( float ) ( f / aspect ), 0.0f, 0.0f, 0.0f,
                    0.0f, ( float ) f, 0.0f, 0.0f,
                    0.0f, 0.0f, ( zFar + zNear ) / ( zNear - zFar ), -1.0f,
                    0.0f, 0.0f, ( 2.0f * zFar * zNear ) / ( zNear - zFar ), 0.0f };

    //Multiplica pela matriz corrente.
    _currentMatrix = _currentMatrix * Math4f( p );
}



void OpenGLMatrixManager::getMatrix( float* m )
{
    memcpy( m, ( const float * ) _currentMatrix, 16 * sizeof (float ) );
}



void OpenGLMatrixManager::getMatrixInverseTransposed( float* mit )
{
    //Fonte:http://www.tecgraf.puc-rio.br/~celes/inf2610/Home_files/transformacao.pdf
    //Notas de aula do professor Waldemar Celes, pagina 18

    //Calcula a inversa transposta para transformar as normais.
    Math4f matInverseTransposed = _currentMatrix.inverse( );
    matInverseTransposed = matInverseTransposed.transposed( );
    memcpy( mit, ( float* ) matInverseTransposed, sizeof (float ) * 3 );
    memcpy( mit + 3, ( float* ) matInverseTransposed + 4, sizeof (float ) * 3 );
    memcpy( mit + 6, ( float* ) matInverseTransposed + 8, sizeof (float ) * 3 );
}



Math4f OpenGLMatrixManager::getMatrixInverse( )
{
    return _currentMatrix.inverse( );
}


