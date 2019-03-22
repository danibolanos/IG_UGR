// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

using namespace std;

#define MODO_INMEDIATO_DRAW_ELEMENTS 1  // glBegin/glVertex/glEnd (0), glDrawElements (1)

// *****************************************************************************
// funciones auxiliares
GLuint VBO_Crear (GLuint tipo, GLuint tamanio, GLvoid *puntero)
{
  assert (tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER);
  GLuint id_vbo;
  glGenBuffers (1, &id_vbo);
  glBindBuffer (tipo,id_vbo);
  glBufferData (tipo, tamanio, puntero, GL_STATIC_DRAW);
  glBindBuffer (tipo, 0);
  return id_vbo;
}

// -----------------------------------------------------------------------------

Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f>& vertices)
{
  assert(vertices.size() > 0);

  Tupla3f maximo = vertices[0],
          minimo = vertices[0]; // puntos diagonales opuestos

  for (auto tabla_vertices : vertices) {
    maximo = {max(tabla_vertices(X), maximo(X)), max(tabla_vertices(Y), maximo(Y)), max(tabla_vertices(Z), maximo(Z))};
    minimo = {min(tabla_vertices(X), minimo(X)), min(tabla_vertices(Y), minimo(Y)), min(tabla_vertices(Z), minimo(Z))};
  }

  return (maximo + minimo) / 2.0;
}

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");
   vbo_creado = false;
   normales_creadas = false;
   usar_texturas = false;
   id_vbo_caras = id_vbo_vertices = id_vbo_col_ver = id_vbo_nor_ver = id_vbo_tex = 0;
   tam_caras = tam_vertices = 0;
   num_vertices = num_caras = 0;
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
   vbo_creado = false;
   normales_creadas = false;
   usar_texturas = false;
   id_vbo_caras = id_vbo_vertices = id_vbo_col_ver = id_vbo_nor_ver = id_vbo_tex = 0;
   tam_caras = tam_vertices = 0;
   num_vertices = num_caras = 0;
}

// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
  // Inicializo a 0 la tabla de normales
  for (auto i: tabla_vertices){
    nor_vertices.push_back({0.0, 0.0, 0.0});
  }

  // Primer bucle -> normales de las caras
  for (auto cara: tabla_caras){
    Tupla3f a, b, n;
    int p, q, r;
    p = cara(0);
    q = cara(1);
    r = cara(2);

    // Calcular normales de las caras ((v1-v0)x(v2-vo))||(v1-v0)x(v2-v0)||
    a = tabla_vertices[q] - tabla_vertices[p];
    b = tabla_vertices[r] - tabla_vertices[p];

    //std::cout << n[0] << ' ' << n[1] << ' ' << n[2]<< std::endl;
    n = a.cross(b);
    if (n.lengthSq() > 0.0)
      n = n.normalized();  // n=axb/|axb|

    nor_caras.push_back(n);  // Guardo la normal en la tabla de caras

    // Aprovecho el bucle para calcular normales de los vértices
    nor_vertices[p] = (nor_vertices[p] + n);
    nor_vertices[q] = (nor_vertices[q] + n);
    nor_vertices[r] = (nor_vertices[r] + n);
  }

  for (auto &nor_vertice : nor_vertices)
    if (nor_vertice.lengthSq() > 0.0)
      nor_vertice = nor_vertice.normalized();

  normales_creadas = true;
}

// -----------------------------------------------------------------------------
// Creación de los VBOs utilizando la función auxiliar

void MallaInd::crearVBOs()
{
  tam_vertices = sizeof(float)*3L*tabla_vertices.size();
  tam_caras = sizeof(unsigned)*3L*tabla_caras.size();
  tam_texturas = sizeof(float)*2L*num_vertices;

  // creando el VBO vértices
  if (id_vbo_vertices == 0){
    id_vbo_vertices = VBO_Crear(GL_ARRAY_BUFFER, tam_vertices, tabla_vertices.data());
  }

  // creando el VBO caras
  if (id_vbo_caras == 0){
    id_vbo_caras = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, tam_caras, tabla_caras.data());
  }

  // crear VBO con los colores de los vértices
  if ( color_vertices.size() > 0 )
    id_vbo_col_ver = VBO_Crear( GL_ARRAY_BUFFER, tam_vertices, color_vertices.data() );

  // crear VBO con las normales de los vertices
  if (nor_vertices.size() > 0)
    id_vbo_nor_ver = VBO_Crear (GL_ARRAY_BUFFER, tam_vertices, nor_vertices.data());

  // crear VBO con las coordenadas de textura
  if (tabla_texturas.size() > 0)
    id_vbo_tex = VBO_Crear (GL_ARRAY_BUFFER, tam_texturas, tabla_texturas.data());

  vbo_creado = true;
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_Plano(ContextoVis & cv)
{
  glBegin(GL_TRIANGLES);

  for (unsigned i = 0; i < tabla_caras.size(); i++) {
    glNormal3fv(nor_caras[i]);
    for (unsigned j = 0; j < 3; j++) {
      unsigned iv = tabla_caras[i](j);
      if (tabla_texturas.size() > 0) glTexCoord2fv(tabla_texturas[iv]);
      if (color_vertices.size() > 0) glColor3fv(color_vertices[iv]);
      glVertex3fv(tabla_vertices[iv]);
    }
  }

  glEnd();
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
#if MODO_INMEDIATO_DRAW_ELEMENTS == 0

  glBegin(GL_TRIANGLES);

  for (unsigned i = 0; i < tabla_caras.size(); i++) {
    for (unsigned j = 0; j < 3; j++) {
      unsigned iv = tabla_caras[i](j);

      if (usar_texturas) {
        glNormal3fv(normales_vertices[iv]);
        if (tabla_texturas.size() > 0) glTexCoord2fv(tabla_texturas[iv]);
      }

      if (color_vertices.size() > 0 && ! cv.modoSeleccionFBO)
        glColor3fv(color_vertices[iv]);

      glVertex3fv(tabla_vertices[iv]);
    }
  }

  glEnd();

#else

  if (usar_texturas) {
    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_FLOAT, 0, nor_vertices.data() );

    if (tabla_texturas.size() > 0) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, tabla_texturas.data());
    }
  }

  if (color_vertices.size() > 0 && ! cv.modoSeleccionFBO) {
    glColorPointer(3, GL_FLOAT, 0, color_vertices.data());
    glEnableClientState( GL_COLOR_ARRAY );
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, tabla_vertices.data());
  // Multiplicamos por 3 ya que en cada posición hay una 3-upla con 3 índices
  glDrawElements(GL_TRIANGLES, 3L * tabla_caras.size(), GL_UNSIGNED_INT, tabla_caras.data());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

#endif
}

// ----------------------------------------------------------------------------

void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
  if (!vbo_creado){
    crearVBOs();
  }

  // Colores o normales
  if (usar_texturas) {
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_nor_ver);
    glNormalPointer(GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);

    if (tabla_texturas.size() > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, id_vbo_tex);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }

  if (color_vertices.size() > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_col_ver);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
  }

  // Vértices
  glBindBuffer(GL_ARRAY_BUFFER, id_vbo_vertices);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Caras
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_caras);
  glDrawElements(GL_TRIANGLES, 3L * tabla_caras.size(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   // Texturas
   usar_texturas = cv.modoVis == modoIluminacionPlano || cv.modoVis == modoIluminacionSuave;

   if (!usar_texturas) {
     glDisable(GL_LIGHTING);
     glDisable(GL_TEXTURE_2D);
   }
   else if (!normales_creadas)
     calcular_normales();

   // Modo selección P5
   if (cv.modoSeleccionFBO) {
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     glShadeModel(GL_FLAT);
     visualizarDE_MI(cv);
     return;
   }

   GLenum modo;
   switch(cv.modoVis) {
     case modoSolido: modo = GL_FILL; glShadeModel(GL_SMOOTH); break;
     case modoPuntos: modo = GL_POINT; break;
     case modoIluminacionPlano: modo = GL_FILL; glShadeModel(GL_FLAT); break;
     case modoIluminacionSuave: modo = GL_FILL; glShadeModel(GL_SMOOTH); break;
     case modoAlambre: default: modo = GL_LINE; break;
   }

   glPolygonMode(GL_FRONT_AND_BACK, modo);

   // Visualizar
   if (cv.usarVBOs)
     visualizarDE_VBOs(cv);
   else if (cv.modoVis == modoIluminacionPlano)
     visualizarDE_MI_Plano(cv);
   else
     visualizarDE_MI(cv);
}

// -----------------------------------------------------------------------------

void MallaInd::fijarColorNodo(const Tupla3f& color) {
  color_vertices.clear();
  for (unsigned i = 0; i < tabla_vertices.size(); i++)
    color_vertices.push_back(color);
}

// -----------------------------------------------------------------------------

void MallaInd::calcularCentroOC() {
  if (!centro_calculado) {
    ponerCentroOC(calcularCentroCajaEnglobante(tabla_vertices));
    centro_calculado = true;
  }
}

// *****************************************************************************

// *****************************************************************************

Cubo::Cubo()
:  MallaInd( "malla cubo" )
{
	const float l = 2.0;
	tam_vertices = 8;
	tam_caras = 12;

	Tupla3f a( -l/2 , -l/2, -l/2 );
	Tupla3f b( l/2, -l/2, -l/2 );
	Tupla3f c( -l/2, -l/2, l/2 );
	Tupla3f d( l/2, -l/2, l/2 );
	Tupla3f a1( -l/2 , l/2, -l/2 );
	Tupla3f b1( l/2, l/2, -l/2 );
	Tupla3f c1( -l/2, l/2, l/2 );
	Tupla3f d1( l/2, l/2, l/2 );

	tabla_vertices.push_back(a);//0
	tabla_vertices.push_back(b);//1
	tabla_vertices.push_back(c);//2
	tabla_vertices.push_back(d);//3
	tabla_vertices.push_back(a1);//4
	tabla_vertices.push_back(b1);//5
	tabla_vertices.push_back(c1);//6
	tabla_vertices.push_back(d1);//7

	Tupla3i t1(0, 1, 3);
	Tupla3i t2(0, 2, 3);
	Tupla3i t3(3, 1, 5);
	Tupla3i t4(3, 7, 5);
	Tupla3i t5(2, 3, 6);
	Tupla3i t6(3, 7, 6);
	Tupla3i t7(0, 2, 4);
	Tupla3i t8(2, 6, 4);
	Tupla3i t9(0, 1, 5);
	Tupla3i t10(0, 4, 5);
	Tupla3i t11(7, 6, 4);
	Tupla3i t12(7, 4, 5);

	tabla_caras.push_back(t1);
	tabla_caras.push_back(t2);
	tabla_caras.push_back(t3);
	tabla_caras.push_back(t4);
	tabla_caras.push_back(t5);
	tabla_caras.push_back(t6);
	tabla_caras.push_back(t7);
	tabla_caras.push_back(t8);
	tabla_caras.push_back(t9);
	tabla_caras.push_back(t10);
	tabla_caras.push_back(t11);
	tabla_caras.push_back(t12);

}
// *****************************************************************************

Tetraedro::Tetraedro()
:  MallaInd( "malla tetraedro")
{
	const float x = 2.0;
	tam_vertices = 4;
	tam_caras = 4;

	Tupla3f a( x*cos(0) , -x/2, x*sin(0) );
	Tupla3f b( x*cos(2*M_PI/3), -x/2, x*sin(2*M_PI/3) );
	Tupla3f c( x*cos(4*M_PI/3), -x/2, x*sin(4*M_PI/3) );
	Tupla3f d( 0.0, x/2, 0.0 );

	tabla_vertices.push_back(a);//0
	tabla_vertices.push_back(b);//1
	tabla_vertices.push_back(c);//2
	tabla_vertices.push_back(d);//3

	Tupla3i t1(0, 1, 2);
	Tupla3i t2(1, 2, 3);
	Tupla3i t3(0, 2, 3);
	Tupla3i t4(0, 1, 3);

	tabla_caras.push_back(t1);
	tabla_caras.push_back(t2);
	tabla_caras.push_back(t3);
	tabla_caras.push_back(t4);
}
