// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"

using namespace std ;

static unsigned objetoActivo3 = 0 ;
static constexpr int numObjetos3 = 1 ;
static NodoGrafoEscenaParam* objetos3[numObjetos3] = { nullptr } ;
static bool p3_animaciones = false ;
static unsigned p3_grado_libertad_activo = 0 ;
static ColFuentesLuz * cf3 = nullptr ;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;

   cf3 = new ColeccionFuentesP4;
   objetos3[0] = new BrazoMecanico();

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto (true si se ha procesado tecla)

   switch ( toupper( tecla ) )
   {
      case 'O' :
         // COMPLETAR: práctica 3: activar siguiente objeto de la práctica
         //           (solo en el caso de que se incluyan varios objetos en la práctica)
      objetoActivo3 = (objetoActivo3+1) % numObjetos3 ;
   	cout << "práctica 3: nuevo objeto activo es: " << objetoActivo3 ;
   	
		if ( objetos3[objetoActivo3] != nullptr )
      	cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl ;
   	else
      	cout << " (objeto no creado)" << endl ;
		res = true;

         break ;

      case 'A' :
         // COMPLETAR: práctica 3: activar o desactivar animaciones
   	p3_animaciones = !p3_animaciones;
	   if (p3_animaciones)
	      FijarFuncDesocupado(FGE_Desocupado);
     
		cout << "práctica 3: animaciones (activadas/desactivadas)" << endl;
		res = true;

         break ;

      case 'G' :
         // COMPLETAR: práctica 3: activar siguiente parámetro
     	if ( objetos3[objetoActivo3] != nullptr ){
      	p3_grado_libertad_activo = (p3_grado_libertad_activo + 1) % objetos3[objetoActivo3]->numParametros();
      
		cout << "prática 3: nuevo parámetro activado " << objetos3[objetoActivo3]->leerPtrParametro(p3_grado_libertad_activo)->leer_descripcion() << endl;
      res = true;
		}

         break ;

      case 'R' :
         // COMPLETAR: práctica 3: reset de animaciones
      p3_animaciones = false;
		if (objetos3[objetoActivo3] != nullptr){
      	for (unsigned i = 0; i < objetos3[objetoActivo3]->numParametros(); i++)
         	objetos3[objetoActivo3]->leerPtrParametro(i)->reset();
		}

    	cout << "práctica 3: animaciones reseteadas" << endl;
		res = true;       

         break ;

      case '>' :
         // COMPLETAR: práctica 3: acelerar o incrementar parámetro
      if (objetos3[objetoActivo3] != nullptr) {
      	auto param_activo = objetos3[objetoActivo3]->leerPtrParametro(p3_grado_libertad_activo);

      	if (p3_animaciones){
      	   param_activo->acelerar();
            cout << "práctica 3: el parámetro activo es " << p3_grado_libertad_activo << ", con velocidad " << param_activo->leer_velocidad_actual() << endl;
         }

         else {
           param_activo->incrementar();
           cout << "práctica 3: el parámetro activo es " << p3_grado_libertad_activo << ", con valor " << param_activo->leer_valor_actual() << endl;
         }
		}
		res = true;

         break ;

      case '<' :
         // COMPLETAR: práctica 3: decelerar o decrementar parámetro
      if (objetos3[objetoActivo3] != nullptr) {
      	auto param_activo = objetos3[objetoActivo3]->leerPtrParametro(p3_grado_libertad_activo);

         if (p3_animaciones) {
         	param_activo->decelerar();
            cout << "práctica 3: el parámetro activo es " << p3_grado_libertad_activo << ", con velocidad " << param_activo->leer_velocidad_actual() << endl;
         }

         else {
           param_activo->decrementar();
           cout << "práctica 3: el parámetro activo es " << p3_grado_libertad_activo << ", con valor " << param_activo->leer_valor_actual() << endl;
         }
		}
		res = true;

         break ;

      default :

         break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
  cf3->activar(cv.modoVis);
  if (objetos3[objetoActivo3] != nullptr)
    objetos3[objetoActivo3]->visualizarGL(cv);
  glDisable(GL_LIGHTING);
}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
  if (!p3_animaciones)
    return false;

  if (objetos3[objetoActivo3] != nullptr)
    objetos3[objetoActivo3]->siguienteCuadro();

  redibujar_ventana = true;

  return true;
}
