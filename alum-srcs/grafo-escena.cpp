// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"
#include "MallaRevol.hpp"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
  assert( pObjeto != NULL );
  tipo   = TipoEntNGE::objeto ;
  objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
  tipo    = TipoEntNGE::transformacion ;
  matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
  *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
  assert( pMaterial != NULL );
  tipo     = TipoEntNGE::material ;
  material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
  /**  no fnciona debido a que se hacen copias (duplicados) de punteros
  if ( tipo == TipoEntNGE::transformacion )
  {
    assert( matriz != NULL );
    delete matriz ;
    matriz = NULL ;
  }
  * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  cv.pilaMateriales.push(); // Se guarda el material activo

  for (unsigned i = 0; i < entradas.size(); i++)
    switch (entradas[i].tipo) {
      case TipoEntNGE::objeto:
        if (cv.modoSeleccionFBO && leerIdentificador() >= 0) {
          std::cout << "Fijando color con identificador "<< leerIdentificador() << std::endl;
          FijarColorIdent(leerIdentificador());
        }
        entradas[i].objeto->visualizarGL (cv);
        break;
      case TipoEntNGE::transformacion:
        glMatrixMode (GL_MODELVIEW);
        glMultMatrixf (*(entradas[i].matriz));
        break;
      case TipoEntNGE::material:
        //if(cv.modoVis == modoIluminacionPlano || cv.modoVis == modoIluminacionSuave)
        if(!cv.modoSeleccionFBO)
          cv.pilaMateriales.activarMaterial(entradas[i].material);
        break;
      default:
        cout << "Tipo desconocido" << endl;
    }

  cv.pilaMateriales.pop();  // Restauramos el material activo y lo activamos
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
  ponerNombre("Nodo escena vacío");
}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
  for(auto e : entradas){
    if(e.tipo == TipoEntNGE::objeto)
      e.objeto->fijarColorNodo(nuevo_color);
  }
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
  entradas.push_back(entrada);
  return entradas.size() - 1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
  return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
  return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
  return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
  if (indice >= 0 && indice < entradas.size())
    if (entradas[indice].tipo == TipoEntNGE::transformacion && entradas[indice].matriz != nullptr)
      return entradas[indice].matriz;

  cout << "ERROR en práctica 3:no hay un puntero válido para el índice " << indice << endl;
  exit(EXIT_FAILURE);
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
  // calcular y guardar el centro del nodo en coordenadas de objeto (recursivamente)
  // (si el centro ya ha sido calculado, no volver a hacerlo)
  Matriz4f mm = MAT_Ident();
  vector<Tupla3f> centros;

  if(!centro_calculado){
    for (auto &entrada : entradas) {
      if (entrada.tipo == TipoEntNGE::objeto) {
        entrada.objeto->calcularCentroOC();
        centros.push_back(mm * entrada.objeto->leerCentroOC());
      }
      else if (entrada.tipo == TipoEntNGE::transformacion)
        mm = mm * (*entrada.matriz);
    }
  }

  ponerCentroOC(calcularCentroCajaEnglobante(centros));
  centro_calculado = true;
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
  const int         ident_busc, // identificador a buscar
  const Matriz4f &  mmodelado,  // matriz de modelado
  Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
  Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
  // buscar un sub-objeto con un identificador
  assert( 0 < ident_busc );

  if (identificador > 0 && !centro_calculado)  // no calcula los centros de cosas no seleccionables
    calcularCentroOC();

  if (identificador == ident_busc) {
    centro_wc = mmodelado * leerCentroOC();
    *objeto = this;
    return true;
  }

  else {
    bool found = false;
    Matriz4f nueva_mmodelado = mmodelado;
    for (unsigned i = 0; i < entradas.size() && !found; ++i) {
      if (entradas[i].tipo == TipoEntNGE::transformacion)
        nueva_mmodelado = nueva_mmodelado * (*entradas[i].matriz);
      else if (entradas[i].tipo == TipoEntNGE::objeto)
        found = entradas[i].objeto->buscarObjeto(ident_busc, nueva_mmodelado, objeto, centro_wc);
    }
    return found;
  }
}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************


// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros()
{
  return parametros.size();
}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned i )
{
  return (i >= 0 && i < parametros.size())?(&parametros[i]):nullptr;
}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
  for (auto &param : parametros)
    param.siguiente_cuadro();
}

// *****************************************************************************
// Partes Brazo Mecánico
// *****************************************************************************

Base::Base(std::vector<Parametro> &p)
{
	agregar(MAT_Escalado(3.0,0.2,3.0));
	agregar(new Cubo);
	// DarkSlateGrey
	fijarColorNodo({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/3.0,1.0/0.2,1.0/3.0));
	agregar(MAT_Traslacion(0,0.2,0));
	agregar(MAT_Escalado(2.7,0.1,2.7));
	agregar(new Cubo);
	// SteelBlue
	fijarColorNodo({0.137255 , 0.419608 , 0.556863});
	agregar(MAT_Escalado(1.0/2.7,1.0/0.1,1.0/2.7));
	agregar(MAT_Traslacion(0,0.1,0));
	agregar(MAT_Escalado(2.5,0.1,2.5));
	agregar(new Cilindro(2, 50, true, true, false));
	// DarkSlateGrey
	fijarColorNodo({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/2.5,1.0/0.1,1.0/2.5));
	agregar(MAT_Traslacion(-1.0,1.0,0));
	agregar(MAT_Escalado(1.0,2.0,1.0));
	agregar(new Cilindro(2, 50, true, true, false));
	// FireBrick
	fijarColorNodo({0.556863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/1.0,1.0/2.0,1.0/1.0));
	agregar(MAT_Traslacion(1.7,-0.2,0));
	agregar(MAT_Escalado(0.7,0.8,0.7));
	agregar(new Cubo);
	// FireBrick + dark
	fijarColorNodo({0.656863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/0.7,1.0/0.8,1.0/0.7));
	agregar(MAT_Traslacion(0.8,0,0));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(MAT_Escalado(0.7,0.2,0.6));
	Matriz4f m = MAT_Ident();
	auto ptr_p0 = leerPtrMatriz(agregar(m));
	Parametro p0("rotación de generador", ptr_p0,
		          [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
               true, 0, 180, 0.09);
	p.push_back(p0);
	agregar(new Cilindro(2, 50, true, true, false));
	// LightSteelBlue
	fijarColorNodo({0.560784 , 0.560784 , 0.737255});
	agregar(MAT_Escalado(1.0/0.7,1.0/0.2,1.0/0.6));
	agregar(MAT_Traslacion(0,-0.3,-0.2));
	agregar(MAT_Escalado(0.05,0.2,0.05));
	agregar(new Cubo);
	agregar(MAT_Escalado(1.0/0.05,1.0/0.2,1.0/0.05));
	agregar(MAT_Traslacion(0,0,0.4));
	agregar(MAT_Escalado(0.05,0.2,0.05));
	agregar(new Cubo);
	// Bronce
	fijarColorNodo({0.65 , 0.49 , 0.24});
}

Apoyo::Apoyo()
{
	agregar(MAT_Escalado(0.9,0.1,0.9));
	agregar(new Cilindro(2, 50, true, true, false));
	// FireBrick ++ dark
	fijarColorNodo({0.856863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/0.9,1.0/0.1,1.0/0.9));
	agregar(MAT_Traslacion(0,0.75,0));
	agregar(MAT_Escalado(0.7,0.7,0.7));
	agregar(new Cubo);
	// FireBrick + dark
	fijarColorNodo({0.656863 , 0.137255 , 0.137255});
}

Infinito::Infinito(){
	agregar(MAT_Rotacion(180, 0, 1, 0));
	agregar(MAT_Traslacion(0,0.1,0));
	agregar(MAT_Escalado(0.9,0.1,0.4));
	agregar(new Cubo);
	// SteelBlue
	fijarColorNodo({0.137255 , 0.419608 , 0.556863});
	agregar(MAT_Escalado(1.0/0.9,1.0/0.1,1.0/0.4));
	agregar(MAT_Traslacion(-1.5,0,0));
	agregar(MAT_Escalado(1.0,0.2,1.0));
	agregar(new CilindroTornillos);
	agregar(MAT_Escalado(1.0,1.0/0.2,1.0));
	agregar(MAT_Traslacion(3.0,0,0));
	agregar(MAT_Escalado(1.0,0.2,1.0));
	agregar(MAT_Rotacion(180, 0, 1, 0));
	agregar(new CilindroTornillos);
}

CilindroTornillos::CilindroTornillos(){
	agregar(new Cilindro(2, 50, true, true, false));
	// SteelBlue
	fijarColorNodo({0.137255 , 0.419608 , 0.556863});
	agregar(MAT_Traslacion(-0.5,0.6,0.6));
	agregar(MAT_Escalado(0.1,0.4,0.1));
	agregar(new Cilindro(2, 50, true, true, false));
	// DarkSlateGrey
	fijarColorNodo({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/0.1,1.0/0.1,1.0/0.1));
	agregar(MAT_Traslacion(0,0,-1.2));
	agregar(MAT_Escalado(0.1,0.1,0.1));
	agregar(new Cilindro(2, 50, true, true, false));
	// DarkSlateGrey
	fijarColorNodo({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/0.1,1.0/0.1,1.0/0.1));
	agregar(MAT_Traslacion(0.5,0,0.6));
	agregar(MAT_Escalado(0.5,0.5,0.5));
	agregar(new Cono(2, 50, true, true, false));
	// GreenForest
	fijarColorNodo({0.137255 , 0.556863 , 0.137255});
}

Brazo::Brazo(){
	agregar(MAT_Traslacion(0,0,-2.2));
	agregar(MAT_Escalado(0.7,0.7,0.7));
	agregar(new Cubo);
	// FireBrick + dark
	fijarColorNodo({0.656863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/0.7,1.0/0.7,1.0/0.7));
	agregar(MAT_Traslacion(-0.7,0,1.0));
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(MAT_Escalado(0.8,1.0,0.8));
	agregar(new Infinito);
	agregar(MAT_Traslacion(0,-1.4,0));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(new Infinito);
}

Giro::Giro(){
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Escalado(0.7,1.0,0.7));
	agregar(new Esfera(10, 50, true, true, false));
	// GreenForest
	fijarColorNodo({0.137255 , 0.556863 , 0.137255});
}

Maquinaria::Maquinaria(std::vector<Parametro> &p){
  Matriz4f m = MAT_Ident();
  auto ptr_p1 = leerPtrMatriz(agregar(m));
  Parametro p1("movimiento de maquinaria", ptr_p1,
               [=](float v) {return MAT_Traslacion(0, v, 0);},
               true, 0, 0.05, 0.2);
  p.push_back(p1);
  agregar(MAT_Rotacion(90, 0, 1, 0));
  agregar(MAT_Traslacion(-0.5,0,0));
  agregar(MAT_Escalado(0.3,0.3,0.3));
  agregar(new MallaRevol("../plys/peon.ply", 50, true, true, false));
  // Gold
  fijarColorNodo({0.8 , 0.498039 , 0.196078});
  agregar(MAT_Traslacion(1.7,0,0));
  agregar(MAT_Escalado(0.7,0.7,0.7));
  agregar(new MallaRevol("../plys/peon.ply", 50, true, true, false));
  // Gold + dark
  fijarColorNodo({0.9 , 0.598039 , 0.196078});
  agregar(MAT_Traslacion(1.7,0,0));
  agregar(MAT_Escalado(0.6,0.6,0.6));
  agregar(new MallaRevol("../plys/peon.ply", 50, true, true, false));
  // Gold ++ dark
  fijarColorNodo({0.9 , 0.698039 , 0.196078});
}

Pinza::Pinza(){
	agregar(MAT_Rotacion(-20, 0, 1, 0));
	agregar(MAT_Traslacion(1.0,0,-0.8));
	agregar(MAT_Escalado(0.7,0.4,0.08));
	agregar(new Cubo);
	agregar(MAT_Escalado(1.0/0.7,1.0/0.4,1.0/0.08));
	agregar(MAT_Traslacion(0.6,0,0.7));
	agregar(MAT_Rotacion(89, 0, 1, 0));
	agregar(MAT_Escalado(0.7,0.4,0.08));
	agregar(new Cubo);
	// Bronce
	fijarColorNodo({0.65 , 0.49 , 0.24});
}

GiroPinza::GiroPinza(){
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Traslacion(-0.2,-1.0,0));
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Escalado(0.8,0.8,0.8));
	agregar(new Pinza);
}



// *****************************************************************************
// Nodo raíz del modelo
// *****************************************************************************
BrazoMecanico::BrazoMecanico()
{
  ponerNombre("brazo mecánico");
  
  Matriz4f m = MAT_Ident();
  agregar(MAT_Traslacion(0.8,2.0,-0.15));  
  agregar(new Maquinaria(parametros));
  agregar(MAT_Traslacion(-0.8,-2.0,0.15));
  agregar(new Base(parametros));
  agregar(MAT_Rotacion(180, 0, 1, 0));
  agregar(MAT_Traslacion(1.0,2.35,0));
  auto ptr_p2 = leerPtrMatriz(agregar(m));
  Parametro p2("rotación de apoyo", ptr_p2,
               [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
               false, 0, 5, 0);
  agregar(new Apoyo);
  agregar(MAT_Traslacion(1.0,-2.35,0));
  agregar(MAT_Traslacion(-1.0,3.1,0));
  auto ptr_p3 = leerPtrMatriz(agregar(m));
  Parametro p3("movimiento de articulación 1", ptr_p3,
               [=](float v) {return MAT_Rotacion(v, 1, 0, 0);},
               true, 45, -45, 0.01);
  agregar(new Brazo);
  agregar(MAT_Traslacion(0,0,-2.4));
  auto ptr_p4 = leerPtrMatriz(agregar(m));
  Parametro p4("movimiento de articulación 2", ptr_p4,
               [=](float v) {return MAT_Rotacion(v, 1, 0, 0);},
               true, -15, 15, 0.02);
  agregar(new Brazo);
  agregar(MAT_Traslacion(1.0,-3.1,2.5));
  agregar(MAT_Traslacion(-1.0,3.1,-5.0));
  auto ptr_p5 = leerPtrMatriz(agregar(m));
  Parametro p5("rotación de giro", ptr_p5,
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, 0, 90, 0.01);
  agregar(new Giro);
  auto ptr_p6 = leerPtrMatriz(agregar(m));
  Parametro p6("abrir/cerrar pinza 1", ptr_p6,
               [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
               true, 10, 15, 0.05);
  agregar(new GiroPinza);
  auto ptr_p7 = leerPtrMatriz(agregar(m));
  Parametro p7("abrir/cerrar pinza 2", ptr_p7,
               [=](float v) {return MAT_Rotacion(-2*v, 0, 1, 0);},
               true, 10, 15, 0.05);
  agregar(MAT_Rotacion(180, 0, 0, 1));
  agregar(new GiroPinza);
  parametros.push_back(p2);
  parametros.push_back(p3);
  parametros.push_back(p4);
  parametros.push_back(p5);
  parametros.push_back(p6);
  parametros.push_back(p7);

}

//-----------------------------------------------------------------------------
// Clases para la escena P4

Lata::Lata(){
  ponerIdentificador(1);
  ponerNombre("Lata");
  agregar(new TapaArriba());
  agregar(new Cuerpo());
  agregar(new TapaAbajo());
}

Lata::Cuerpo::Cuerpo(){
  ponerIdentificador(-1);
  ponerNombre("Cuerpo lata");
  agregar(new MaterialLata());
  agregar(new MallaRevol("../plys/lata-pcue.ply", 100, false ,false, true));
}

Lata::TapaArriba::TapaArriba(){
  ponerIdentificador(-1);
  ponerNombre("Tapa arriba lata");
  agregar(new MaterialTapasLata());
  agregar(new MallaRevol("../plys/lata-psup.ply", 100, true, false, true));
}

Lata::TapaAbajo::TapaAbajo(){
  ponerIdentificador(-1);
  ponerNombre("Tapa abajo lata");
  agregar(new MaterialTapasLata());
  agregar(new MallaRevol("../plys/lata-pinf.ply", 100, true, false, true));
}

PeonMadera::PeonMadera(){
  ponerIdentificador(2);
  ponerNombre("Peon madera");
  agregar(new MaterialPeonMadera());
  agregar(new MallaRevol("../plys/peon.ply", 100, true, false, true));
}

PeonBlanco::PeonBlanco(){
  ponerIdentificador(3);
  ponerNombre("Peon blanco");
  agregar(new MaterialPeonBlanco());
  agregar(new MallaRevol("../plys/peon.ply", 100, true, false, true));
}

PeonNegro::PeonNegro(){
  ponerIdentificador(4);
  ponerNombre("Peon negro");
  agregar(new MaterialPeonNegro());
  agregar(new MallaRevol("../plys/peon.ply", 100, true, false, true));
}

EscenaP4::EscenaP4(){
  agregar(new Lata());
  agregar(MAT_Traslacion(0, 0.25, 1));
  agregar(MAT_Escalado((float) 1/5,(float) 1/5,(float) 1/5));
  agregar(new PeonMadera);
  agregar(MAT_Traslacion(3, 0, 0));
  agregar(new PeonBlanco);
  agregar(MAT_Traslacion(3, 0, 0));
  agregar(new PeonNegro);
}
