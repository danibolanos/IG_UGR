
#include "latapeones.h"
using namespace std;

// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase LataPeones.

VariasLatasPeones::VariasLatasPeones(){

  ponerNombre("VariasLatasPeones");
  agregar(new Lata("../recursos/imgs/lata-coke.jpg", "Lata de Coca-Cola", 1));
  agregar(MAT_Traslacion(4.0, 0.0, 0.0));
  agregar(new Lata("../recursos/imgs/lata-pepsi.jpg", "Lata de Pepsi", 2));
  agregar(MAT_Traslacion(4.0, 0.0, 0.0));
  agregar(new Lata("../recursos/imgs/window-icon.jpg", "Lata de la UGR", 3));

  agregar(MAT_Traslacion(-8.0, 0.0, 0.0));
  agregar(MAT_Escalado(0.9, 0.9, 0.9));
  agregar(MAT_Traslacion(0.0, 1.4, 4.0));

  // Peón Madera
  NodoGrafoEscena *nodo_peon1 = new NodoGrafoEscena();
  Textura *textura_peon1 = new Textura("../recursos/imgs/text-madera.jpg");
  Material *material_peon1 = new Material(textura_peon1, 0.3, 0.7, 1.0, 40.0);
  MallaRevol *peon1 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  peon1->ponerNombre("Peón de madera");
  peon1->ponerIdentificador(123);
  nodo_peon1->agregar(material_peon1);
  nodo_peon1->agregar(peon1);

  // Peón Blanco
  NodoGrafoEscena *nodo_peon2 = new NodoGrafoEscena();
  Material *material_peon2 = new Material(0.3, 1.0, 0.0, 0.0);
  MallaRevol *peon2 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  peon2->ponerColor({0.8, 0.8, 0.8});
  peon2->ponerNombre("Peón blanco");
  peon2->ponerIdentificador(124);
  nodo_peon2->agregar(material_peon2);
  nodo_peon2->agregar(peon2);

  // Peón Negro
  NodoGrafoEscena *nodo_peon3 = new NodoGrafoEscena();
  Material *material_peon3 = new Material(0.5, 0.1, 0.9, 10.0);
  MallaRevol *peon3 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  peon3->ponerColor({0.1, 0.1, 0.1});
  peon3->ponerNombre("Peón negro");
  peon3->ponerIdentificador(125);
  nodo_peon3->agregar(material_peon3);
  nodo_peon3->agregar(peon3);

  agregar(nodo_peon1);
  agregar(MAT_Traslacion(3.0, 0.0, 0.0));
  agregar(nodo_peon2);
  agregar(MAT_Traslacion(3.0, 0.0, 0.0));
  agregar(nodo_peon3);
  entradas.back().objeto->ponerIdentificador(121211);
}

LataPeones::LataPeones(){
  ponerNombre("LataPeones");
  agregar(new Lata("../recursos/imgs/lata-coke.jpg", "Lata de Coca-Cola", 1));

  agregar(MAT_Escalado(0.9, 0.9, 0.9));
  agregar(MAT_Traslacion(0.0, 1.4, 4.0));

  // Peón Madera
  NodoGrafoEscena *nodo_peon1 = new NodoGrafoEscena();
  Textura *textura_peon1 = new Textura("../recursos/imgs/text-madera.jpg");
  Material *material_peon1 = new Material(textura_peon1, 0.3, 0.7, 1.0, 40.0);
  MallaRevol *peon1 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  peon1->ponerNombre("Peón de madera");
  peon1->ponerIdentificador(123);
  nodo_peon1->agregar(material_peon1);
  nodo_peon1->agregar(peon1);

  // Peón Blanco
  NodoGrafoEscena *nodo_peon2 = new NodoGrafoEscena();
  Material *material_peon2 = new Material(0.3, 1.0, 0.0, 0.0);
  MallaRevol *peon2 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  peon2->ponerColor({0.8, 0.8, 0.8});
  peon2->ponerNombre("Peón blanco");
  peon2->ponerIdentificador(124);
  nodo_peon2->agregar(material_peon2);
  nodo_peon2->agregar(peon2);

  // Peón Negro
  NodoGrafoEscena *nodo_peon3 = new NodoGrafoEscena();
  Material *material_peon3 = new Material(0.5, 0.1, 0.9, 10.0);
  MallaRevol *peon3 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  peon3->ponerColor({0.1, 0.1, 0.1});
  peon3->ponerNombre("Peón negro");
  peon3->ponerIdentificador(125);
  nodo_peon3->agregar(material_peon3);
  nodo_peon3->agregar(peon3);

  agregar(nodo_peon1);
  agregar(MAT_Traslacion(3.0, 0.0, 0.0));
  agregar(nodo_peon2);
  agregar(MAT_Traslacion(3.0, 0.0, 0.0));
  agregar(nodo_peon3);
  entradas.back().objeto->ponerIdentificador(82371);
}


Lata::Lata(const std::string & textureJPG, const std::string nombre, const int id){
  ponerNombre(nombre);
  ponerIdentificador(id);

  agregar(MAT_Escalado(4.0, 4.0, 4.0));

  // Tapa superior
  NodoGrafoEscena *nodo_psup = new NodoGrafoEscena();
  Material *material_psup = new Material(0.4, 0.4, 0.8, 25.0);
  MallaRevol *psup = new MallaRevolPLY("../recursos/plys/lata-psup.ply", 50);
  psup->ponerColor({0.7, 0.7, 0.7});
  nodo_psup->agregar(material_psup);
  nodo_psup->agregar(psup);

  // Perfil parte central
  NodoGrafoEscena *nodo_pcue = new NodoGrafoEscena();
  Textura *textura_pcue = new Textura(textureJPG);
  Material *material_pcue = new Material(textura_pcue, 0.6, 0.6, 0.9, 50.0);
  MallaRevol *pcue = new MallaRevolPLY("../recursos/plys/lata-pcue.ply", 50);
  nodo_pcue->agregar(material_pcue);
  nodo_pcue->agregar(pcue);

  // Base inferior
  NodoGrafoEscena *nodo_binf = new NodoGrafoEscena();
  Material *material_binf = new Material(0.4, 0.4, 0.8, 25.0);
  MallaRevol *binf = new MallaRevolPLY("../recursos/plys/lata-pinf.ply", 50);
  binf->ponerColor({0.7, 0.7, 0.7});
  nodo_binf->agregar(material_binf);
  nodo_binf->agregar(binf);

  // Agregamos las tres partes que forman la lata
  agregar(nodo_psup);
  agregar(nodo_binf);
  agregar(nodo_pcue);
}
