// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

//#include <set>   // std::set
#include "ig-aux.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   Tupla3f a, b, m;
   for(int i=0; i<triangulos.size(); i++){
     a = vertices[triangulos[i][1]]-vertices[triangulos[i][0]];
     b = vertices[triangulos[i][2]]-vertices[triangulos[i][0]];
     m = a.cross(b);    // Producto vectorial
     if(m[0] != 0 || m[1] != 0 || m[2] != 0)
       m = m.normalized();
     nor_tri.push_back(m);
   }
}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'

   calcularNormalesTriangulos();

   for(int i=0; i<vertices.size(); i++)
     nor_ver.push_back({0.0,0.0,0.0});

   for(int i=0; i<triangulos.size(); i++)
     for(int j=0; j<3; j++)
       nor_ver[triangulos[i][j]] = nor_ver[triangulos[i][j]]+nor_tri[i];

   for(int i=0; i<nor_ver.size(); i++)
     if(nor_ver[i][0] != 0 || nor_ver[i][1] != 0 || nor_ver[i][2] != 0)
       nor_ver[i]=nor_ver[i].normalized();
}


//----------------------------------------------------------------------------
// comprueba que los valores de 'tipo' y 'tabla' son valores legales.

void Comprobar( GLuint tipo, GLuint tabla )
{
   assert( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER );
   if ( tipo == GL_ARRAY_BUFFER )
      assert(  tabla == GL_VERTEX_ARRAY || tabla == GL_COLOR_ARRAY ||
               tabla == GL_NORMAL_ARRAY || tabla == GL_TEXTURE_COORD_ARRAY ) ;
}
//----------------------------------------------------------------------------
// crea un VBO, devuelve:
//  ** 0 si 'num_elems' es 0, o
//  ** el identificador de VBO, si num_elems > 0
//
// deja activado el VBO 0

GLuint CrearVBO( GLuint tipo, GLuint tabla, GLuint cuenta, GLvoid * puntero )
{
   Comprobar( tipo, tabla );
   // Crear un VBO para esta tabla y enviar datos a la GPU, devolver id de VBO
   // Si 'cuenta' es 0, la tabla está vacía: devuelve 0 y no hace nada.
   if ( cuenta == 0 ) // si la tabla está vacía, no hacemos nada (id=0)
     return 0 ;
   // calcular: tamaño de cada elemento en bytes (teb), y de toda la tabla (ttb)
   GLuint teb = (tipo == GL_ELEMENT_ARRAY_BUFFER) ? sizeof( unsigned int ):
          (tabla == GL_TEXTURE_COORD_ARRAY ) ? sizeof( Tupla2f ) :
          sizeof( Tupla3f ) ;
   unsigned long ttb = (unsigned long)teb * (unsigned long) cuenta ;
   GLuint id_vbo = 0; // identificador de VBO resultante.
   glGenBuffers( 1, &id_vbo );  // crear un nuevo identificador de VBO
   glBindBuffer( tipo, id_vbo ); // activar el nuevo VBO
   glBufferData( tipo, ttb, puntero, GL_STATIC_DRAW ); // transf. RAM->GPU
   glBindBuffer( tipo, 0 ); // no dejar activado el VBO
   return id_vbo;
}

//----------------------------------------------------------------------------
// Especifica localización y estructura de una tabla

void LocForTabla( GLuint tipo_tabla, GLuint tabla, GLuint id_vbo,
                  GLvoid * p )
{
   // especificar localización y formato de la tabla
   glBindBuffer( tipo_tabla, id_vbo ); // acti. VBO (id_vbo==0 si está en RAM)
   if ( tipo_tabla == GL_ARRAY_BUFFER ) // especificar estructura y puntero:
     switch( tabla ){
       case GL_VERTEX_ARRAY:
         glVertexPointer ( 3, GL_FLOAT, 0, p );
         break;
       case GL_TEXTURE_COORD_ARRAY:
         glTexCoordPointer( 2, GL_FLOAT, 0, p );
         break;
       case GL_COLOR_ARRAY:
         glColorPointer( 3, GL_FLOAT, 0, p );
         break;
       case GL_NORMAL_ARRAY:
         glNormalPointer ( GL_FLOAT, 0, p );
         break;
       default:
         assert( false );
         break ; // error
     }
}


//----------------------------------------------------------------------------
// habilita o deshabilita una tabla, si la habilita, especifica localización
// y estructura

void RegistrarTabla( GLuint tipo_tabla, GLuint tabla, GLsizei cuenta,
                     GLuint id_vbo, GLvoid * puntero_offset )
{
   // usar 'glBindBuffer', 'glEnableClientState', 'glDisableClientState' y 'FijarPunteroTabla'
   // (al llamar a 'FijarPunteroTabla', se asume que el puntero es el offset nulo)
   if ( cuenta == 0 && tipo_tabla == GL_ARRAY_BUFFER )
     glDisableClientState( tabla ); // deshabilitar uso de una tabla vacía
   else{
     LocForTabla( tipo_tabla, tabla, id_vbo, puntero_offset );
     if ( tipo_tabla == GL_ARRAY_BUFFER )
       glEnableClientState( tabla ); // habilitar uso de la tabla
   }
}
// -----------------------------------------------------------------------------
// crea todos los VBOs

void MallaInd::crearVBOs()
{
   if ( vbos_creados )
      return ;

   CError();
   // Crear los VBOs de las 5 tablas posibles (llamar a 'CrearVBO')
   // almacenar los identificadores en las variables de instancia 'id_vbo_...'
   id_vbo_col = CrearVBO( GL_ARRAY_BUFFER, GL_COLOR_ARRAY, col_ver.size(), col_ver.data() ) ;
   id_vbo_nor = CrearVBO( GL_ARRAY_BUFFER, GL_NORMAL_ARRAY, nor_ver.size(), nor_ver.data()) ;
   id_vbo_cct = CrearVBO( GL_ARRAY_BUFFER, GL_TEXTURE_COORD_ARRAY, cc_tt_ver.size(), cc_tt_ver.data()) ;
   id_vbo_ver = CrearVBO( GL_ARRAY_BUFFER, GL_VERTEX_ARRAY, vertices.size(), vertices.data()) ;
   id_vbo_tri = CrearVBO( GL_ELEMENT_ARRAY_BUFFER, 0, 3*triangulos.size(), triangulos.data()) ;

   CError();

   vbos_creados = true ;
}

// -----------------------------------------------------------------------------
// Registrar tablas en modo inmediato

void MallaInd::registrarTablas_MI()
{
   CError();
   // registrar las 4 tablas en modo inmediato (llamar a 'RegistrarTabla')
   RegistrarTabla( GL_ARRAY_BUFFER, GL_VERTEX_ARRAY, vertices.size(), 0, vertices.data() );
   RegistrarTabla( GL_ARRAY_BUFFER, GL_COLOR_ARRAY, col_ver.size(), 0, col_ver.data() );
   RegistrarTabla( GL_ARRAY_BUFFER, GL_NORMAL_ARRAY, nor_ver.size(), 0, nor_ver.data() );
   RegistrarTabla( GL_ARRAY_BUFFER, GL_TEXTURE_COORD_ARRAY, cc_tt_ver.size(), 0, cc_tt_ver.data() );

   CError();
}

//--------------------------------------------------------------------------------
// Registrar tablas en modo diferido

void MallaInd::registrarTablas_MD()
{
   CError();
   // crear VBOs y registrar las 5 tablas en modo diferido (llamar a 'RegistrarTabla')
   crearVBOs(); // crear los VBOs
   RegistrarTabla( GL_ARRAY_BUFFER, GL_VERTEX_ARRAY, vertices.size(), id_vbo_ver, 0 );
   RegistrarTabla( GL_ELEMENT_ARRAY_BUFFER, 0, 3*triangulos.size(), id_vbo_tri, 0 );
   RegistrarTabla( GL_ARRAY_BUFFER, GL_COLOR_ARRAY, col_ver.size(), id_vbo_col, 0 );
   RegistrarTabla( GL_ARRAY_BUFFER, GL_NORMAL_ARRAY, nor_ver.size(), id_vbo_nor, 0 );
   RegistrarTabla( GL_ARRAY_BUFFER, GL_TEXTURE_COORD_ARRAY, cc_tt_ver.size(), id_vbo_cct, 0 );

   CError();
}

// -----------------------------------------------------------------------------
// crear los dos VAOS: 'id_vao_attr' e 'id_vao_geom' , solo si no estaban creados

void MallaInd::crearActivarVAO_MD( )
{
   CError();
   // Crear (si no lo estaba) el VAO del M.D., y activarlo.
   if ( id_vao_md == 0 ){ // Si el VAO no estaba ya creado:
     glGenVertexArrays( 1, &id_vao_md ); // crear un VAO nuevo (no activado)
     glBindVertexArray( id_vao_md ); // activar el VAO
     registrarTablas_MD(); // registrar loc. y formato tablas
   }
   else
     glBindVertexArray( id_vao_md ); // Activar el VAO (ya estaba creado)

   CError();
}



// -----------------------------------------------------------------------------

void MallaInd::visualizarGL_MI_DE( ContextoVis & cv )
{
   CError();
   using namespace std ;
   // es necesario asegurarnos antes de que el VAO para M.I. está creado
   // (usando crearActivarVAO_MI), y luego llamar a 'glDrawElements')
   // al final, dejar activado el VAO por defecto
   glBindVertexArray( 0 ); // activar el VAO 0 (VAO por defecto)
   registrarTablas_MI() ; // registrar loc. y formato de tablas.
   glDrawElements( GL_TRIANGLES, 3*triangulos.size(), GL_UNSIGNED_INT, triangulos.data() );

   CError();

}
// ----------------------------------------------------------------------------

void MallaInd::visualizarGL_MD_VAO( ContextoVis & cv )
{
   // visualizar en modo diferido (glDrawElements)
   // es necesario asegurarnos antes de que el VAO para M.D. está creado
   // (usando crearActivarVAO_MD), y luego llamar a 'glDrawElements')
   // al final, dejar activado el VAO por defecto
   crearActivarVAO_MD(); // asegurarnos de que el VAO de MD está creado y activo
   // visualizar la secuencia:
   glDrawElements( GL_TRIANGLES, 3*triangulos.size(), GL_UNSIGNED_INT, 0 );
   glBindVertexArray( 0 ); // dejar activado el VAO 0 (VAO por defecto)
}

// -----------------------------------------------------------------------------
// visualizar con begin/end (modo inmediato), enviando normales de triángulos y
// cc.tt. (adecuado para modo de sombreado plano)

void MallaInd::visualizarGL_MI_BE( ContextoVis & cv )
{
   // Visualizacion en modo inmediato usando begin/end
   // usar 'glBegin/glVertex/glEnd'
   const unsigned long nv = triangulos.size(); // núm.verts
   glBegin( GL_TRIANGLES );
   for( unsigned long i = 0 ; i < nv ; i++ ){
     for( unsigned long j = 0 ; j < 3 ; j++ ){
       const unsigned long iv = triangulos[i](j);
       if ( col_ver.size() > 0 )
         glColor3fv( col_ver[iv] );
       if ( nor_ver.size() > 0 )
         glNormal3fv( nor_ver[iv] );
       if ( cc_tt_ver.size() > 0 )
         glTexCoord2fv( cc_tt_ver[iv] );
       glVertex3fv( vertices[iv] );
     }
   }
   glEnd();
}
// ----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// visualizar modo aristas extra

/*void MallaInd::visualizarGL_modoAristas( ContextoVis & cv )
{
  if(varistas.size()==0){
    for(Tupla3i tri: triangulos){
      for(int ivt : {0,1,2}){
        varistas.push_back(vertices[tri(ivt)]);
        varistas.push_back(vertices[tri((ivt+1)%3)]);
      }
    }
  }
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, varistas.data());
  glDrawArrays(GL_LINES, 0, varistas.size());
  glDisableClientState(GL_VERTEX_ARRAY);
}*/
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------


void MallaInd::visualizarGL( ContextoVis & cv )
{

   using namespace std ;
   assert( cv.cauce_act != nullptr );

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   if(cv.visualizando_normales){
     visualizarNormales();
     return;
   }

   // guardar el color previamente fijado
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   //   visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end                   : usar 'visualizarGL_MI_BE'
   //   ** inmediato con un VAO (glDrawElements) : usar 'visualizarGL_MI_VAO'
   //   ** diferido con un VAO  (glDrawElements) : usar 'visualizarGL_MD_VAO'
   if(cv.modo_envio == ModosEnvio::inmediato_begin_end)
     visualizarGL_MI_BE( cv );
   if(cv.modo_envio == ModosEnvio::inmediato_drawelements)
     visualizarGL_MI_DE( cv );
   if(cv.modo_envio == ModosEnvio::diferido_vao)
     visualizarGL_MD_VAO( cv );
//   if(cv.modo_envio == ModosEnvio::modoAristas)
//     visualizarGL_modoAristas( cv );

   // restaurar el color previamente fijado
   glColor4fv( color_previo );
}

// *****************************************************************************

void MallaInd::visualizarNormales()
{
   using namespace std ;
   CError();
   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }
   std::vector<Tupla3f> segmentos ;
   for( unsigned i = 0 ; i < vertices.size() ; i++ )
   {  segmentos.push_back( vertices[i] );
      segmentos.push_back( vertices[i]+ 0.35f*(nor_ver[i]) );
   }
   CError();
   glVertexPointer( 3, GL_FLOAT, 0, segmentos.data() );
   glDrawArrays( GL_LINES,  0, segmentos.size() );
   CError();
}

// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
  ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // llamar a 'calcularNormales' para el cálculo de normales

	LeerPLY(nombre_arch, vertices, triangulos);

  // calcular la tabla de normales
  calcularNormales();
}

// ****************************************************************************
// Modos de visualización colores

// Modo multicolor
void MallaInd::modoMulticolor(){
  for(unsigned int i=0; i < vertices.size(); i++){
    Tupla3f color = {0.0, 0.0, 0.0};
    for(int j=0; j < 3; j++){
      if(vertices[i](j) == +1.0)
        color(j) = 1.0;
    }
    col_ver.push_back(color);
  }
}

// Modo Ajedrez
void MallaInd::modoAjedrez(){
  for(unsigned int i=0; i < vertices.size(); i++){
    if(i%2 == 0)
      col_ver.push_back({1.0,0.0,0.0});
    else
      col_ver.push_back({0.0,1.0,0.0});
  }
}

// Modo Random
void MallaInd::modoRandom(){
  srand(time(NULL));
  double num;

  for(unsigned int i=0; i < vertices.size(); i++){
    Tupla3f color = {0.0, 0.0, 0.0};
    for(int j=0; j < 3; j++){
      num = (rand()%256)/255.0;
      color(j) = num;
    }
    col_ver.push_back(color);
  }
}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{
  vertices =
     {  { -1.0, -1.0, -1.0 }, // 0
        { -1.0, -1.0, +1.0 }, // 1
        { -1.0, +1.0, -1.0 }, // 2
        { -1.0, +1.0, +1.0 }, // 3
        { +1.0, -1.0, -1.0 }, // 4
        { +1.0, -1.0, +1.0 }, // 5
        { +1.0, +1.0, -1.0 }, // 6
        { +1.0, +1.0, +1.0 }, // 7
     } ;

  triangulos =
     {  {0,1,3}, {0,3,2}, // X-
        {4,7,5}, {4,6,7}, // X+ (+4)

        {0,5,1}, {0,4,5}, // Y-
        {2,3,7}, {2,7,6}, // Y+ (+2)

        {0,6,4}, {0,2,6}, // Z-
        {1,5,7}, {1,7,3}  // Z+ (+1)
     } ;
  calcularNormales();
}
// ****************************************************************************
// Clase 'Tetraedro

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro 4 vértices" )
{
  ponerColor({1.0,0.0,0.0}); // ponemos color rojo en el tetraedro

  vertices =
     {  { 2.0*cos(0) , -1.0, 2.0*sin(0) }, // 0
        { 2.0*cos(2*M_PI/3), -1.0, 2.0*sin(2*M_PI/3) }, // 1
        { 2.0*cos(4*M_PI/3), -1.0, 2.0*sin(4*M_PI/3) }, // 2
        { 0.0, 1.0, 0.0 }, // 3
     } ;

  triangulos =
     {  {0,1,2}, {1,3,2},
        {0,2,3}, {0,1,3}
     } ;
  calcularNormales();
}
// ****************************************************************************
// Clase 'CuboColores

CuboColores::CuboColores()
:  MallaInd( "cubo de colores" )
{
  vertices =
     {  { -1.0, -1.0, -1.0 }, // 0
        { -1.0, -1.0, +1.0 }, // 1
        { -1.0, +1.0, -1.0 }, // 2
        { -1.0, +1.0, +1.0 }, // 3
        { +1.0, -1.0, -1.0 }, // 4
        { +1.0, -1.0, +1.0 }, // 5
        { +1.0, +1.0, -1.0 }, // 6
        { +1.0, +1.0, +1.0 }, // 7
     } ;

  triangulos =
     {  {0,1,3}, {0,3,2}, // X-
        {4,7,5}, {4,6,7}, // X+ (+4)

        {0,5,1}, {0,4,5}, // Y-
        {2,3,7}, {2,7,6}, // Y+ (+2)

        {0,6,4}, {0,2,6}, // Z-
        {1,5,7}, {1,7,3}  // Z+ (+1)
     } ;
  // Implementa modo multicolor al cubo especificado
  modoMulticolor();
  calcularNormales();
}
// ****************************************************************************

Cubo24::Cubo24() : MallaInd("Cubo de 24 vértices")
{
  vertices =
  //cara abajo
  { { -1.0, -1.0, -1.0 }, //0
  { 1.0, -1.0, -1.0 },    //1
  { 1.0, -1.0, 1.0 },    //2
  { -1.0, -1.0, 1.0 },    //3
  //cara arriba
  { -1.0, 1.0, -1.0 },   //4
  { 1.0, 1.0, -1.0 },   //5
  { 1.0, 1.0, 1.0 },  //6
  { -1.0, 1.0, 1.0 },   //7
  //cara detrás
  { -1.0, -1.0, -1.0 }, //8
  { 1.0, -1.0, -1.0 },  //9
  { 1.0, 1.0, -1.0 },   //10
  { -1.0, 1.0, -1.0 },  //11
  //cara delante
  { -1.0, -1.0, 1.0 }, //12
  { 1.0, -1.0, 1.0 },  //13
  { 1.0, 1.0, 1.0 },  //14
  { -1.0, 1.0, 1.0 }, //15
  //cara izquierda
  { -1.0, -1.0, -1.0 }, //16
  { -1.0, -1.0, 1.0 },  //17
  { -1.0, 1.0, 1.0 },   //18
  { -1.0, 1.0, -1.0 },  //19
  //cara derecha
  { 1.0, -1.0, -1.0 }, //20
  { 1.0, -1.0, 1.0 },  //21
  { 1.0, 1.0, 1.0 },   //22
  { 1.0, 1.0, -1.0 }  //23
  } ;

  triangulos =
  { {1,3,0}, {1,2,3}, //cara abajo
  {5,4,7}, {5,7,6}, //cara arriba
  {9,8,11}, {9,11,10}, //cara detrás
  {12,13,15}, {13,14,15}, //cara delante
  {17,19,16}, {17,18,19}, //cara izquierda
  {21,20,23}, {21,23,22} //cara derecha
  } ;

  cc_tt_ver=
  {{0,0}, {0,1}, {1,1}, {1,0}, //cara abajo
  {0,0}, {0,1}, {1,1}, {1,0}, //cara arriba
  {0,1}, {1,1}, {1,0}, {0,0}, //cara detrás
  {0,1}, {1,1}, {1,0}, {0,0}, //cara delante
  {0,1}, {1,1}, {1,0}, {0,0}, //cara izquierda
  {0,1}, {1,1}, {1,0}, {0,0} //cara derecha
  };

  calcularNormales();
}
