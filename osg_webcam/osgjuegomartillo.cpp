#include "webcamframework.h"
#include <stdlib.h>

double COMPONENTE_NUEVA = 0.7;
double COMPONENTE_ANTERIOR = 0.3;
double DUREZA_ACTUA = 0.3;
double UMBRAL = 20.0;
int RESOLUTION_W = 64;
int RESOLUTION_H = 48;
int MOVEMENT_EXPLORE = 3;
int MSPERFRAME = 20000;
int RESOLUTION_W_SHOW = 256;
int RESOLUTION_H_SHOW = 256;

#define N_BICHOS 2
#define FRAMES_BICHOS 150
#define PASO_BICHO 0.01*SPEEDUP
#define SPEEDMAX 1.3

#define UMBRAL_MOV 1.2
#define CORRECCION_IZQ 2.0
#define CORRECCION_DER 2.0
#define CORRECCION_ABA 4.0
#define SPEEDUP 3
#define MOVE_1 (30/SPEEDUP)
#define MOVE_2 (60/SPEEDUP)
#define MOVE_3 (60/SPEEDUP)

ref_ptr<Node> WebcamScreen() {
  ref_ptr<Geode> geode;
  geode = new Geode();
  ref_ptr<Geometry> geometry (new Geometry());

  ref_ptr<Vec3Array> vertices (new Vec3Array());

  vertices->push_back (Vec3 ( 0.0, 0.0, 0.0));
  vertices->push_back (Vec3 ( 0.0, 0.0, 2.0));
  vertices->push_back (Vec3 ( 2.0, 0.0, 2.0));
  vertices->push_back (Vec3 ( 2.0, 0.0, 0.0));

  geometry->setVertexArray (vertices.get());

   ref_ptr<Vec3Array> normals (new Vec3Array());
   normals->push_back (Vec3 (0.0f, -1.0f, 0.0f));
   geometry->setNormalArray (normals.get());
   geometry->setNormalBinding (Geometry::BIND_OVERALL);

  ref_ptr<StateSet> stateSet (geode->getOrCreateStateSet());
  stateSet->setTextureAttributeAndModes (0, texture.get(),StateAttribute::ON);
  ref_ptr<Vec2Array> texCoords (new Vec2Array());

  texCoords->push_back (Vec2 (0.0, 0.0));
  texCoords->push_back (Vec2 (0.0, 1.0));
  texCoords->push_back (Vec2 (1.0, 1.0));
  texCoords->push_back (Vec2 (1.0, 0.0));

  geometry->setTexCoordArray (0,texCoords.get());

  geometry->addPrimitiveSet(new DrawArrays (PrimitiveSet::QUADS,0,vertices->size()));

  geode->addDrawable (geometry.get());

  ref_ptr<MatrixTransform> mTall = new MatrixTransform;
  Matrix m;
  m.makeTranslate(-1.0,0,-0.6);
  mTall->setMatrix(m);
  mTall->addChild(geode);

  ref_ptr<Group> screen = new Group();

  screen->addChild(mTall);

  return screen;
}

ref_ptr<Node> Martillo() {
  Matrix m,aux;
  ref_ptr<Group> martillo = new Group;

  ref_ptr<Geode> mango = new Geode;
  mango->addDrawable(new ShapeDrawable(new Cylinder(Vec3(0.0, 0.0, 0.0),0.05,1)));
  ref_ptr<Image> image (osgDB::readImageFile("texturas/madera.png"));
  ref_ptr<Texture2D> text (new Texture2D);
  ref_ptr<StateSet> stateSet (mango->getOrCreateStateSet());
  stateSet->setTextureAttributeAndModes(0, text.get(),StateAttribute::ON);
  text->setImage (image.get());

  ref_ptr<Geode> golpeador = new Geode;
  ref_ptr<Cylinder> cylinder = new Cylinder(Vec3(0.0, 0.0, 0.0),0.10,0.4);
  golpeador->addDrawable(new ShapeDrawable(cylinder));
  ref_ptr<Image> image2 (osgDB::readImageFile("texturas/metal-oscuro.png"));
  ref_ptr<Texture2D> text2 (new Texture2D);
  ref_ptr<StateSet> stateSet2 (golpeador->getOrCreateStateSet());
  stateSet2->setTextureAttributeAndModes(0, text2.get(),StateAttribute::ON);
  text2->setImage (image2.get());
  ref_ptr<MatrixTransform> martilloT = new MatrixTransform;
  m.makeIdentity();

  aux.makeRotate(PI/2,1,0,0);
  m.postMult(aux);
  aux.makeTranslate(0,0,+0.38);
  m.postMult(aux);

  martilloT->setMatrix(m);
  martilloT->addChild(golpeador);
  ref_ptr<MatrixTransform> mTall = new MatrixTransform;
  m.makeTranslate(0,0,0.5);
  mTall->setMatrix(m);
  mTall->addChild(mango);
  mTall->addChild(martilloT);
  martillo->addChild(mTall);
  return martillo;
}

ref_ptr<Node> Bicho() {
  ref_ptr<Texture2D> textbicho = 0;
  ref_ptr<Geode> bichillo = new Geode;
  bichillo->addDrawable(new ShapeDrawable(new Sphere (Vec3(0.0,0.0,0.0),0.05)));
  ref_ptr<Image> image (osgDB::readImageFile("texturas/pelo.png"));
  textbicho = new Texture2D;
  textbicho->setImage (image.get());
  ref_ptr<StateSet> stateSet (bichillo->getOrCreateStateSet());
  stateSet->setTextureAttributeAndModes(0, textbicho.get(),StateAttribute::ON);

  ref_ptr<MatrixTransform> mTall = new MatrixTransform;
  Matrix m;
  m.makeIdentity();
  mTall->setMatrix(m);
  mTall->addChild(bichillo);

  ref_ptr<Group> bicho = new Group;
  bicho->addChild(mTall);
  return bicho;
}

ref_ptr<Node> Pared() {
  ref_ptr<Geode> geode = new Geode();
  geode->addDrawable(new ShapeDrawable(new Box(Vec3(0.0,0.101,0.30),2,0.2,2.2)));
  ref_ptr<Image> image (osgDB::readImageFile("texturas/asfalto2.png"));
  ref_ptr<Texture2D> text = new Texture2D;
  text->setImage (image.get());
  ref_ptr<StateSet> stateSet (geode->getOrCreateStateSet());
  stateSet->setTextureAttributeAndModes(0, text.get(),StateAttribute::ON);
  return geode;
}

ref_ptr<Node> Suelo() {
  ref_ptr<Group> grupo = new Group();
  ref_ptr<Geode> geode = new Geode();
  geode->addDrawable(new ShapeDrawable(new Box(Vec3(0.0,-2.0,-0.65),2,4,0.1)));
  ref_ptr<Image> image (osgDB::readImageFile("texturas/asfalto.png"));
  ref_ptr<Texture2D> text = new Texture2D;
  text->setImage (image.get());
  ref_ptr<StateSet> stateSet (geode->getOrCreateStateSet());
  stateSet->setTextureAttributeAndModes(0, text.get(),StateAttribute::ON);
  ref_ptr<Geode> geode2 = new Geode();
  geode2->addDrawable(new ShapeDrawable(new Box(Vec3(0.0,-2.0,-0.725),2,4,0.15)));
  ref_ptr<Image> image2 (osgDB::readImageFile("texturas/asfalto2.png"));
  ref_ptr<Texture2D> text2 = new Texture2D;
  text2->setImage (image2.get());
  ref_ptr<StateSet> stateSet2 (geode2->getOrCreateStateSet());
  stateSet2->setTextureAttributeAndModes(0, text2.get(),StateAttribute::ON);
  grupo->addChild(geode);
  grupo->addChild(geode2);
  return grupo;
}

ref_ptr<Node> Luz() {
  ref_ptr<Group> grupo = new Group();
  ref_ptr<Light> light = new Light();

  light->setAmbient(osg::Vec4d(0.5, 0.5, 0.5, 1.0));
  ref_ptr<LightSource> lightsource = new LightSource();
  lightsource->setLight(light);

  grupo->addChild(lightsource);
  return grupo;
}

ref_ptr<MatrixTransform> martilloT = new MatrixTransform;


struct bichostr {
  double x;
  double y;
  double speedy;
  char activo;
  ref_ptr<MatrixTransform> transf;
};
int cuentabichos = 50;

bichostr bichos[N_BICHOS];

ref_ptr<Node> CreateGeometry ()
{
  Matrix m,aux;
  ref_ptr<Group> root = new Group();

  ref_ptr<Node> martillo = Martillo();
  int i;
  for (i = 0;i < N_BICHOS;i++) {
    bichos[i].x = 0;
    bichos[i].y = 0;
    bichos[i].speedy = 0;
    bichos[i].activo = 0;
    bichos[i].transf = new MatrixTransform;
    m.makeTranslate(0,0.08,0);
    bichos[i].transf->setMatrix(m);
    bichos[i].transf->addChild(Bicho());
  }
  ref_ptr<Node> bicho = Bicho();

  m.makeTranslate(0,-0.5,-0.3);
  martilloT->setMatrix(m);
  martilloT->addChild(martillo);

  ref_ptr<MatrixTransform> luzT = new MatrixTransform;
  m.makeTranslate(0,-1.8,0.0);
  luzT->addChild(Luz());
  luzT->setMatrix(m);

  root->addChild(WebcamScreen());
  root->addChild(martilloT);
  root->addChild(Pared());
  root->addChild(Suelo());
  root->addChild(luzT);
  for (i = 0;i < N_BICHOS;i++)
    root->addChild(bichos[i].transf);

  ref_ptr<MatrixTransform> rootT = new MatrixTransform;
  m.makeRotate(0.33,1,0,0);
  aux.makeRotate(-0.32,0,0,1);
  m.postMult(aux);
  rootT->setMatrix(m);
  
  rootT->addChild(root);
  return rootT;
}

char moviendo = 0;
int imov;
double multx,multz;
char keyleft = 0;
char keyright = 0;

void timer() {
  double izq,der,cantidad,angx,angz;
  int i;
  Matrix m,aux;
  cuentabichos--;
  if (cuentabichos <= 0) {
    for (i = 0;i < N_BICHOS;i++) {
      if (bichos[i].activo == 0) {
        bichos[i].x = (rand()%2?-1.:1.)*0.74;
        bichos[i].y = -3.8;
        bichos[i].speedy = PASO_BICHO*(1+(((double)1-SPEEDMAX)*(double)rand()/(double)RAND_MAX));
        m.makeTranslate(bichos[i].x,bichos[i].y,-0.55);
        bichos[i].transf->setMatrix(m);
        bichos[i].activo = 1;
        cuentabichos = FRAMES_BICHOS;
        i = N_BICHOS;
      }
    }
  }
  for (i = 0;i < N_BICHOS;i++) {
    bichos[i].y += bichos[i].speedy;
    m.makeTranslate(bichos[i].x,bichos[i].y,-0.55);
    bichos[i].transf->setMatrix(m);
    if (bichos[i].y >= 0) {
      bichos[i].activo = 0;
      bichos[i].x = 0;
      bichos[i].y = 0;
      m.makeTranslate(0,0.08,0);
      bichos[i].transf->setMatrix(m);
      //TODO PERDEMOS
    }
  }
  if (keyleft && !keyright) {
    if (moviendo == 0) {
      moviendo = 1;
      imov = 0;
      multx = -PI/3;
      multz = PI/2;
    }
  }
  if (keyright && !keyleft) {
    if (moviendo == 0) {
      moviendo = 1;
      imov = 0;
      multx = PI/3;
      multz = PI/2;
    }
  }
  switch (moviendo) {
    case 0:
      int gana;
      izq = -actua_movx*CORRECCION_IZQ;
      der = actua_movx*CORRECCION_DER;
      if (izq < 0)
        izq = 0;
      if (der < 0)
        der = 0;
      if (izq > der) {
        cantidad = izq-der;
        gana = 1;
        if (cantidad > UMBRAL_MOV) {
          moviendo = 1;
          imov = 0;
          multx = -PI/3;
          multz = PI/2;
        }
      }
      if (der > izq) {
        cantidad = der-izq;
        gana = 2;
        if (cantidad > UMBRAL_MOV)
          moviendo = 1;
          imov = 0;
          multx = PI/3;
          multz = PI/2;
      }
      break;
    case 1:
      if (imov<=MOVE_1) { 
        imov++;
        angx = multx*((double)imov)/(double)MOVE_1;
        angz = multz*((double)imov)/(double)MOVE_1;
        if (angz > 1.40) {
          for (i = 0;i < N_BICHOS;i++) {
            if (bichos[i].activo) {
              double dist = bichos[i].y+0.93;
              if (dist < 0)
                dist *= -1.0;
              if (dist < 0.20) {
                bichos[i].activo = 0;
                bichos[i].x = 0;
                bichos[i].y = 0;
                m.makeTranslate(0,0.08,0);
                bichos[i].transf->setMatrix(m);
                //TODO GANAMOS
              }
            }
          }
        }
      }
      else {
        angx = multx;
        angz = multz;
        moviendo++;
        imov = 0;
      }
      m.makeRotate(angz,1,0,0);
      aux.makeRotate(angx,0,0,1);
      m.postMult(aux);
      aux.makeTranslate(0,-0.5,-0.3);
      m.postMult(aux);
      martilloT->setMatrix(m);
      break;
    case 2:
      if (imov<=MOVE_2) { 
        imov++;
        angx = multx*((double)(MOVE_2-imov))/(double)MOVE_2;
        angz = multz*((double)(MOVE_2-imov))/(double)MOVE_2;
      }
      else {
        angx = 0;
        angz = 0;
        moviendo++;
        imov = 0;
      }
      m.makeRotate(angz,1,0,0);
      aux.makeRotate(angx,0,0,1);
      m.postMult(aux);
      aux.makeTranslate(0,-0.5,-0.3);
      m.postMult(aux);
      martilloT->setMatrix(m);
      break;
    case 3:
      if (imov<=MOVE_3) { 
        imov++;
        if (keyleft || keyright)
          imov=MOVE_3+1;
        angx = 0;
        angz = 0;
      }
      else {
        angx = 0;
        angz = 0;
        moviendo++;
        imov = 0;
      }
      m.makeRotate(angz,1,0,0);
      aux.makeRotate(angx,0,0,1);
      m.postMult(aux);
      aux.makeTranslate(0,-0.5,-0.3);
      m.postMult(aux);
      martilloT->setMatrix(m);
      break;
    default:
      moviendo = 0;
      imov = 0;
      multx = 0;
      multz = 0;
      angx = 0;
      angz = 0;
      m.makeRotate(angz,1,0,0);
      aux.makeRotate(angx,0,0,1);
      m.postMult(aux);
      aux.makeTranslate(0,-0.5,-0.3);
      m.postMult(aux);
      martilloT->setMatrix(m);
      break;
  }

}

class myKeyboardEventHandler : public osgGA::GUIEventHandler  {
  public:
  virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);
  virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };
};

bool myKeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa) {
  switch(ea.getEventType()) {
    case(osgGA::GUIEventAdapter::KEYDOWN):
      switch(ea.getKey()) {
        case 'a':
        case 0xFF51:
          keyleft = 1;
          return false;
        break;
        case 'd':
        case 0xFF53:
          keyright = 1;
          return false;
        break;
        default:
          return false;
      } 
    break;
    case(osgGA::GUIEventAdapter::KEYUP):
      switch(ea.getKey()) {
        case 'q':
          exit(0);
          return false;
        case 'a':
        case 0xFF51:
          keyleft = 0;
          return false;
        break;
        case 'd':
        case 0xFF53:
          keyright = 0;
          return false;
        break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
}

void init_viewer(osgViewer::Viewer &viewer) {
 srand (time (NULL));
 myKeyboardEventHandler* keh = new myKeyboardEventHandler();
 viewer.addEventHandler(keh); 
}

Matrix CTransform() {
  Matrix m,aux;
  m.makeTranslate(0.4,0.7,-9);
  aux.makeRotate(0.15,0,1,0);
  m.preMult(aux);
  aux.makeRotate(-1.6,1,0,0);
  m.preMult(aux);
  return m;
}
