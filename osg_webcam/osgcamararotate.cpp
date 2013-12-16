#include "webcamframework.h"

double COMPONENTE_NUEVA = 0.5;
double COMPONENTE_ANTERIOR = 0.5;
double DUREZA_ACTUA = 0.1;
double UMBRAL = 20.0;
int RESOLUTION_W = 64;
int RESOLUTION_H = 48;
int MOVEMENT_EXPLORE = 3;
int MSPERFRAME = 20000;
int RESOLUTION_W_SHOW = 256;
int RESOLUTION_H_SHOW = 256;

#define FACTOR_MOV 2

ref_ptr<MatrixTransform> mR1;
ref_ptr<MatrixTransform> mR2;
ref_ptr<MatrixTransform> mT1;
ref_ptr<MatrixTransform> mT2;

ref_ptr<Node> CreateGeometry ()
{
   ref_ptr<Geode> geode;
   geode = new Geode();
   ref_ptr<Geometry> geometry (new Geometry());

   ref_ptr<Vec3Array> vertices (new Vec3Array());

   vertices->push_back (Vec3 ( 0.0, 0.0, 0.0));
   vertices->push_back (Vec3 ( 0.0, 0.0, 1.0));
   vertices->push_back (Vec3 ( 1.0, 0.0, 1.0));
   vertices->push_back (Vec3 ( 1.0, 0.0, 0.0));

   geometry->setVertexArray (vertices.get());

   ref_ptr<Vec3Array> normals (new Vec3Array());
   normals->push_back (Vec3 (0.0f, -1.0f, 0.0f));
   geometry->setNormalArray (normals.get());
   geometry->setNormalBinding (Geometry::BIND_OVERALL);

   ref_ptr<StateSet> stateSet (geode->getOrCreateStateSet());
   stateSet->setTextureAttributeAndModes (0, texture.get(),
                                          StateAttribute::ON);

   ref_ptr<Vec2Array> texCoords (new Vec2Array());

   texCoords->push_back (Vec2 (0.0, 0.0));
   texCoords->push_back (Vec2 (0.0, 1.0));
   texCoords->push_back (Vec2 (1.0, 1.0));
   texCoords->push_back (Vec2 (1.0, 0.0));

   geometry->setTexCoordArray (0,                // Texture unit (for multi-texturing?)
                               texCoords.get()); // The coordinates vector
   // Binding is always per vertex, I guess. Anything different than this does
   // not make sense for me.


   // Back to the usual: setup a primitive set and add the geometry to the geode.
   geometry->addPrimitiveSet(
      new DrawArrays (PrimitiveSet::QUADS, // how to render?
                           0,                            // index of first vertex
                           vertices->size()));           // how many vertices?
   mR1 = new MatrixTransform();
   mR2 = new MatrixTransform();
   mT1 = new MatrixTransform();
   mT2 = new MatrixTransform();


   geode->addDrawable (geometry.get());

   ref_ptr<Group> root = new Group();

   mT2->addChild(geode.get());
   mR1->addChild(mT2.get());
   mR2->addChild(mR1.get());
   mT1->addChild(mR2.get());
   root->addChild(mT1.get());

   return root;
}

void timer() {
  Matrix mat;
  mat.makeRotate(FACTOR_MOV*actua_movx,0,0,1);
  mR1->setMatrix(mat);
  mat.makeRotate(-FACTOR_MOV*actua_movy,1,0,0);
  mR2->setMatrix(mat);
  mat.makeTranslate(0.5,0.0,0.5);
  mT1->setMatrix(mat);
  mat.makeTranslate(-0.5,0.0,-0.5);
  mT2->setMatrix(mat);
}

void init_viewer(osgViewer::Viewer &viewer) {
}

Matrix CTransform() {
  Matrix m,aux;
  m.makeTranslate(-0.5,-0.5,-3);
  aux.makeRotate(0.15,0,1,0);
  m.preMult(aux);
  aux.makeRotate(-1.6,1,0,0);
  m.preMult(aux);
  return m;
}

