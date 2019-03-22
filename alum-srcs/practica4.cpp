// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica4.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"

using namespace std ;

static unsigned objetoActivo4 = 0 ;
static constexpr int numObjetos4 = 1 ;
static NodoGrafoEscena * objetos4[numObjetos4] = {nullptr} ;
static ColFuentesLuz * cf4 = nullptr ;
static int angulo_activo ;
static constexpr float INCR = 3.0;


// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
  cout << "Creando objetos de la práctica 4 .... " << flush ;

  cf4 = new ColeccionFuentesP4;
  objetos4[0] = new EscenaP4();

  cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
  bool res = false  ; // true solo cuando se ha procesado alguna tecla
  FuenteDireccional * f_dir;

  switch ( toupper( tecla ) )
  {
    case 'G' :
      angulo_activo = 1 - angulo_activo;
      cout << "práctica 4: ángulo actual cambiado a " << angulo_activo << endl;
      res = true;
      break ;

    case '>' :
      f_dir = (FuenteDireccional *) cf4->ptrFuente(0);
      f_dir->variarAngulo(angulo_activo, INCR);
      cout << "práctica 4: ángulo " << angulo_activo << " cambiado a "
           << (angulo_activo ? f_dir->lati : f_dir->longi) << endl;
      res = true;
      break ;

    case '<' :
      f_dir = (FuenteDireccional *) cf4->ptrFuente(0);
      f_dir->variarAngulo(angulo_activo, -INCR);
      cout << "práctica 4: ángulo " << angulo_activo << " cambiado a "
           << (angulo_activo ? f_dir->lati : f_dir->longi) << endl;
      res = true;
      break ;

    default :
      break ;
  }

  return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
  // requiere activar las fuentes de luz y luego dibujar el grafo de escena
  cf4->activar(cv.modoVis);
  if (objetos4[objetoActivo4] != nullptr)
    objetos4[objetoActivo4]->visualizarGL(cv);
  glDisable(GL_LIGHTING);
}
