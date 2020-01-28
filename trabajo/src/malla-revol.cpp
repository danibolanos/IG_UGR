// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "lector-ply.h"
#include "matrices-tr.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************

// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
  int m = perfil.size();
  // Crear tablas de normales y texturas
  std::vector<Tupla3f> nor_arist, nor_ver_perfil;
  Tupla3f aux;

  for(int j=0; j<m-1; j++){
    aux = perfil[j+1]-perfil[j];
    aux={aux[1], -aux[0], 0.0};
    if(aux[0]!=0 || aux[1]!=0 || aux[2]!=0)
      aux = aux.normalized();
    nor_arist.push_back(aux);
  }
  nor_ver_perfil.push_back(nor_arist.front());

  for(int j=1; j<m-1; j++){
    aux = nor_arist[j-1]+nor_arist[j];
    if(aux[0]!=0 || aux[1]!=0 || aux[2]!=0)
      aux = aux.normalized();
    nor_ver_perfil.push_back(aux);
  }
  nor_ver_perfil.push_back(nor_arist.back());

  // Cálculo de las coordenadas de textura
  vector<float> d, t;
  float dividendo, divisor, tmp;
  dividendo = 0; divisor = 0;
  for(int i=0; i<m-1; i++){
    tmp = sqrt(pow(perfil[i][0]-perfil[i+1][0], 2) + pow(perfil[i][1]-perfil[i+1][1], 2));
    d.push_back(tmp);
    divisor += tmp;
  }
  for(int j=0; j<m; j++){
    t.push_back(dividendo/divisor);
    dividendo += d[j];
  }

  // COMPLETAR: Práctica 2: completar: creación de la malla....

  int n = num_copias;
  float alpha = 360.0/(n-1);
  Matriz4f matriz;

  // Partimos de la tabla de vértices vacía.
  vertices.clear();

  // Para cada i desde 0 hasta n − 1
  for(int i=0; i <= n-1; i++){
    // Para cada j desde 0 hasta m − 1
		for(int j=0; j <= m-1; j++){
      // Sea q = vértice rotación pj con ángulo 2πi/(n − 1) radianes
			matriz = MAT_Rotacion(alpha*(i), 0, 1, 0);
			Tupla3f p = {perfil[j][0], perfil[j][1], perfil[j][2]};
			Tupla3f q = matriz*p;
      // Añadir q a la tabla de vértices
			vertices.push_back({q[0], q[1], q[2]});
      // Añadir normal
      Tupla3f n = {nor_ver_perfil[j][0], nor_ver_perfil[j][1], nor_ver_perfil[j][2]};
      Tupla3f nq = matriz*n;
      nor_ver.push_back({nq[0], nq[1], nq[2]});
      // Añadir coordenadas de textura
	    cc_tt_ver.push_back({float((i*1.0)/(num_copias-1)), 1-t[j]});
		}
	}

  // Partimos de la tabla de triángulos vacía
  triangulos.clear();

  // Para cada i desde 0 hasta n − 2
  for(int i=0; i <= n-2; i++){
    // Para cada j desde 0 hasta m − 2
    for(int j=0; j <= m-2; j++){
      // Sea k = im + j
      int k1 = i*m + j;
      int k2 = (i+1)*m + j;
      int k3 = (i+1)*m + j + 1;
      int k4 = i*m + j + 1;
      // Añadir triángulo formado por los índices k, k + m y k + m + 1
      triangulos.push_back({k1,k2,k3});
      // Añadir triángulo formado por los índices k, k + m + 1 y k + 1
      triangulos.push_back({k1,k3,k4});
    }
  }
}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'

   LeerVerticesPLY(nombre_arch, vertices);

   inicializar(vertices, nperfiles);
}

Cilindro::Cilindro(const int num_verts_per,
                   const unsigned nperfiles)
{
  const float radio = 1.0;
  const float altura = 1.0;
  vector<Tupla3f> perfil_malla;

  // Construir perfil de n_vértices
  for(int i=0; i < num_verts_per; i++)
    perfil_malla.push_back({radio, altura*((float)i/(num_verts_per-1)), 0.0});

  inicializar(perfil_malla, nperfiles);
}

Cono::Cono(const int num_verts_per,
           const unsigned nperfiles)
{
  const float radio = 1.0;
  const float altura = 1.0;
  vector<Tupla3f> perfil_malla;

  // Construir perfil de n_vértices
  for (int i = 0; i < num_verts_per; i++) {
    float x = (1-(float) i / (num_verts_per-1))*radio;
    perfil_malla.push_back({x, (-altura/radio)*x+altura, 0.0});
  }

  inicializar(perfil_malla, nperfiles);
}

Esfera::Esfera(const int num_verts_per,
                   const unsigned nperfiles)
{
  const float radio = 1.0;
  Matriz4f matriz;
  vector<Tupla3f> perfil_malla;

  perfil_malla.push_back({0.0, -radio, 0.0});

  float angbase = 180.0/(num_verts_per-1);
  for(int i=0; i < num_verts_per; i++){
    matriz = MAT_Rotacion( angbase*(i), 0, 0, 1 );
    Tupla3f p = {perfil_malla[0][0], perfil_malla[0][1], perfil_malla[0][2]};
    Tupla3f q = matriz*p;
    perfil_malla.push_back({q[0], q[1], q[2]});
  }

  inicializar(perfil_malla, nperfiles);
}

// -----------------------------------------------------------------------------
// EXTRA: Toroide Regular n > 20 aproxima a Toro

ToroideRegular::ToroideRegular(const int n,
                   const unsigned nperfiles)
{
  const float radio = 0.5;
  const float dist = radio*2;
  const int num_verts_per = n+1;
  Matriz4f matrizR, matrizT;
  vector<Tupla3f> perfil_malla;

  perfil_malla.push_back({radio, 0.0, 0.0});

  float angbase = 360.0/(num_verts_per-1);
  for(int i=0; i < num_verts_per; i++){
    matrizR = MAT_Rotacion( angbase*(i), 0, 0, 1 );
    matrizT = MAT_Traslacion(dist,0.0,0.0);
    Tupla3f p = {perfil_malla[0][0], perfil_malla[0][1], perfil_malla[0][2]};
    Tupla3f q = matrizR*p;
    q = matrizT*q;
    perfil_malla.push_back({q[0], q[1], q[2]});
  }

  inicializar(perfil_malla, nperfiles);
}

CilindroTapas::CilindroTapas(const int num_verts_per,
                   const unsigned nperfiles)
{
  const float radio = 1.0;
  const float altura = 1.0;
  vector<Tupla3f> perfil_malla;

  perfil_malla.push_back({radio, altura/2.0, 0.0});
  perfil_malla.push_back({radio, -altura/2.0, 0.0});

  inicializar(perfil_malla, nperfiles);

  // Crear tapas
  int n = nperfiles;
  int m = perfil_malla.size();
  Tupla3f ver_inf = vertices[0];
  Tupla3f ver_sup = vertices[m-1];
  // Añadimos dos vertices más para crear las tapas del Cilindro
  vertices.push_back({0.0, ver_inf[1], 0.0});
  vertices.push_back({0.0, ver_sup[1], 0.0});
  int num_ver = vertices.size();
  for(int i=0; i < n-1; i++){
    int t1 = num_ver-2;
    int t2 = i*m;
    int t3 = (i+1)*m;
    int t4 = num_ver-1 ;
    int t5 = ((i+1)*m)-1;
    int t6 = ((i+2)*m)-1;
    triangulos.push_back({t1,t2,t3});
    triangulos.push_back({t4,t5,t6});
  }
}
