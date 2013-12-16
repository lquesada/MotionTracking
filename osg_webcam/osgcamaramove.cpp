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

ref_ptr<MatrixTransform> mNavTrans;

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
   mNavTrans = new MatrixTransform();


   geode->addDrawable (geometry.get());

   ref_ptr<Group> root = new Group();
   mNavTrans->addChild(geode.get());
   root->addChild(mNavTrans.get());

   return root;
}

void timer() {
  Matrix mat = mNavTrans->getMatrix();
  mat.makeRotate(FACTOR_MOV*(actua_movx+actua_movy),0,1,1);
  mNavTrans->setMatrix(mat);
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

