#include "Axis.h"
#include <stdio.h>
Axis::Axis() {
  x = 0.0;
  y = 0.0;
  z = 0.0;

  speedx = 0.0;
  speedy = 0.0;
  speedz = 0.0;

}

void Axis::update(const Output &output) {
  //Actualizar velocidad.
  speedx += output.movementscreen.y;
  speedy -= output.movementscreen.x;
  speedz += output.movementscreen.z;

  //Actualizar movimiento.

  double factx = 5.3;
  double facty = 5.3;
  double factz = -9;
  double factxopo = 10;
  double factyopo = 10;
  double factzopo = 10;
  if (speedx*x < 0)
    x += speedx*factxopo*factx;  
  else
    x += speedx*factx;  
  if (speedy*y < 0)
    y += speedy*factyopo*facty;  
  else
    y += speedy*facty;  
  if (speedz*z < 0)
    z += speedz*factzopo*factz;  
  else
    z += speedz*factz;  

  //Frenada.
  speedx *= 0.9; 
  speedy *= 0.9; 
  speedz *= 0.8; 

  //Retorno al eje.
  x *= 0.5; 
  y *= 0.5; 
  z *= 0.9; 
}
