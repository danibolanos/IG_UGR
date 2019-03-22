// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"
#include "CamaraInter.hpp"

using namespace std ;

// declarar variables de la práctica 5 (static) (escena, viewport,
// el vector de camaras, y las variables que sirven para gestionar el modo arrastrar)

// Desplazamiento de la cámara actual en el eje Z
static const float INCR = 5.0;
// viewport actual (se recalcula al inicializar y al fijar las matrices)
static Viewport viewport;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;
// número de cámaras
static constexpr int numCamaras = 4;
// vector de cámaras con vista como mínimo de alzado, planta y perfil
static CamaraInteractiva * camaras[numCamaras] = {nullptr, nullptr, nullptr, nullptr};
// cámara activa
static int camaraActiva = 0;
// posiciones anteriores del ratón
static int xant, yant;
// grafo de escena
static constexpr int numObjetos5 = 1;
static NodoGrafoEscena * objetos5[numObjetos5] = {nullptr};
static ColFuentesLuz * cf5 = nullptr;

// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
  cout << "Creando objetos de la práctica 5 .... " << flush ;

  cf5 = new ColeccionFuentesP4;
  objetos5[0] = new EscenaP4;   // uso los objetos de la p4: lata y peones
  viewport = Viewport(0, 0, vp_ancho, vp_alto);
  float ratio = (float) vp_alto / vp_ancho;
  camaras[0] = new CamaraInteractiva (true, ratio, 0, 0, {0,0,0}, true, 80.0, 1.8);  // Alzado
  camaras[1] = new CamaraInteractiva (true, ratio, 90, 0, {0,0,0}, true, 80.0, 2.0); // Perfil
  camaras[2] = new CamaraInteractiva (true, ratio, 0, 90, {0,0,0}, true, 80.0, 4.0); // Planta
  camaras[3] = new CamaraInteractiva (true, ratio, 0, 0, {0,0,0}, false, 80.0, 3.2); // Alzado ortográfica

  cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
  // actualizar viewport, actualizar y activar la camara actual (en base a las dimensiones del viewport)
  viewport = Viewport(0, 0, vp_ancho, vp_alto);
  viewport.fijarViewport();

  camaras[camaraActiva]->ratio_yx_vp = viewport.ratio_yx;
  camaras[camaraActiva]->calcularViewfrustum(); // recalcular m_proy
  camaras[camaraActiva]->activar();
}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{
  // activar las fuentes de luz y visualizar la escena (se supone que la camara actual ya está activada)
  cf5->activar(cv.modoVis);
  if (objetos5[0] != nullptr)
    objetos5[0]->visualizarGL(cv);
  glDisable(GL_LIGHTING);
}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{
  bool result = true ;
  bool examinar ;

  switch ( toupper( tecla ) )
  {
    case 'C': // activar siguiente camara
      camaraActiva = (camaraActiva + 1) % numCamaras;
      cout << "práctica 5: nueva cámara activa es " << camaraActiva << endl;
      break ;

    case 'V': // conmutar la cámara actual entre modo examinar y el modo primera persona
      examinar = camaras[camaraActiva]->examinar;
      if (examinar) camaras[camaraActiva]->modoPrimeraPersona();
      else camaras[camaraActiva]->modoExaminar();
      cout << "práctica 5: modo de la cámara " << camaraActiva
           << " cambiado a " << (examinar ? "primera persona" : "examinar") << endl;
      break ;

    case '+': // desplazamiento en Z de la cámara actual (positivo) (desplaZ)
      camaras[camaraActiva]->desplaZ(INCR);
      cout << "práctica 5: acercando respecto al eje Z" << endl;
      break;

    case '-': // desplazamiento en Z de la cámara actual (negativo) (desplaZ)
      camaras[camaraActiva]->desplaZ(-INCR);
      cout << "práctica 5: alejando respecto al eje Z" << endl;
      break;

    default:
      result = false ;
    break ;
  }

  return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{
  bool result = true ;

  switch ( tecla )
  {
    case GLFW_KEY_LEFT: // desplazamiento/rotacion hacia la izquierda (moverHV)
      camaras[camaraActiva]->moverHV(-INCR, 0);
      cout << "práctica 5: desplazando hacia la izquierda" << endl;
      break;

    case GLFW_KEY_RIGHT:// desplazamiento/rotación hacia la derecha (moverHV)
      camaras[camaraActiva]->moverHV(INCR, 0);
      cout << "práctica 5: desplazando hacia la derecha" << endl;
      break;

    case GLFW_KEY_UP:    // desplazamiento/rotación hacia arriba (moverHV)
      camaras[camaraActiva]->moverHV(0, INCR);
      cout << "práctica 5: desplazando hacia arriba" << endl;
      break;

    case GLFW_KEY_DOWN:   // desplazamiento/rotación hacia abajo (moverHV)
      camaras[camaraActiva]->moverHV(0, -INCR);
      cout << "práctica 5: desplazando hacia abajo" << endl;
      break;

    default:
      result = false ;
      break ;
  }

  return result ;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y )
{
  // visualizar escena en modo selección y leer el color del pixel en (x,y)

  // 1. crear un 'contextovis' apropiado
  ContextoVis cv;

  // 2. visualizar en modo selección (sobre el backbuffer)
  cv.modoSeleccionFBO = true;
  glClearColor(0, 0, 0, 1);                           // color de fondo
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpiar pantalla
  objetos5[0]->visualizarGL(cv);

  // 3. leer el color del pixel, si es 0 no se hace nada
  unsigned ident = LeerIdentEnPixel(x, y);

  if (ident == 0) {
    cout << "práctica 5: no se ha seleccionado ningún objeto" << endl;
    return;
  }

  // 4. buscar el objeto en el grafo de escena e informar del mismo
  Tupla3f centro_wc(0.0, 0.0, 0.0);
  Matriz4f m = MAT_Ident();
  Objeto3D * objeto = nullptr;

  if (objetos5[0]->buscarObjeto(ident, m, &objeto, centro_wc)) {
    camaras[camaraActiva]->modoExaminar(centro_wc);
    cout << "práctica 5: seleccionado objeto " << objeto->leerNombre()
         << " con centro " << centro_wc << endl;
  }
  else
    cout << "práctica 5: no se ha encontrado el objeto seleccionado" << endl;
}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
  // activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
  modo_arrastrar = true;
  xant = x;
  yant = y;
}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
  // desactivar bool del modo arrastrar
  modo_arrastrar = false;
}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
  // calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)
  camaras[camaraActiva]->moverHV(x - xant, y - yant);
   xant = x;
   yant = y;
}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
  //cout << "P5_FGE_ClickRaton" << endl ;
  if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
    P5_ClickIzquierdo( x, viewport.alto-y );
  else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
    P5_InicioModoArrastrar( x, viewport.alto-y );
  else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
    P5_FinModoArrastrar();
  else
    return false ;

  return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
  if ( modo_arrastrar )
  {
    P5_RatonArrastradoHasta( x, viewport.alto-y );
    return true ;
  }
  else
    return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction ) // direction es +-1
{
  // acercar/alejar la camara (desplaZ)
  camaras[camaraActiva]->desplaZ(direction * INCR);
  return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
  // fijar color actual de OpenGL usando 'ident' (glColor3ub)
  if (ident >= 0) {
    const unsigned char
      byteR = ident % 0x100U,               // rojo = byte menos significativo
      byteG = (ident / 0x100U) % 0x100U,    // verde = byte intermedio
      byteB = (ident / 0x10000U) % 0x100U;  // azul = byte más significativo

    glColor3ub(byteR, byteG, byteB);        // cambio de color en OpenGL
  }
}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
  // leer el identificador codificado en el color del pixel (x,y)
  unsigned char bytes[3];
  // leer los 3 bytes del frame-buffer
  glReadPixels(xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) bytes);
  // reconstruir el indentificador y devolverlo:
  return bytes[0] + (0x100U * bytes[1]) + (0x10000U * bytes[2]) ;
}
//---------------
