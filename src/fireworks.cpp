#include "CImg.h"
using namespace cimg_library;

void* item_fireworks() {
  CImg<unsigned char> img(640,480,1,3,0);
  CImgDisplay disp(img,"[#21] - Fireworks (Click to add/explode rockets)");
  CImgList<unsigned char> colors;
  const unsigned char white[] = { 255,255,255 }, red[] = { 128,0,0 };
  CImgList<float> particles;
  float time = 0, speed = 100.0f;

  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {

    if (disp.button()&1 || !particles.size() || (--time)<0) {
      particles.insert(CImg<>::vector((float)cimg::rand()*img.width(),(float)img.height(),
                                      (float)cimg::rand(-1,1)*4,-6 - (float)cimg::rand()*3,
                                      30 + 60*(float)cimg::rand(),3));
      colors.insert(CImg<unsigned char>::vector(255,255,255));
      time = (float)(cimg::rand()*speed);
    }
    img*=0.992f;

    cimglist_for(particles,l) {
      bool remove_particle = false;
      float &x = particles(l,0), &y = particles(l,1), &vx = particles(l,2), &vy = particles(l,3),
            &t = particles(l,4), &r = particles(l,5);
      const float r2 = (t>0 || t<-42)?r/3:r*(1 - 2*(-(t + 2)/40.0f)/3);
      img.draw_ellipse((int)x,(int)y,r,r2,(float)(std::atan2(vy,vx)*180/cimg::PI),colors[l].data(),0.6f);
      x+=vx; y+=vy; vy+=0.09f; t--;
      if (y>img.height() + 10 || x<0 || x>=img.width() + 10) remove_particle = true;

      if (t<0 && t>=-1) {
        if ((speed*=0.9f)<10) speed=10.0f;
        const unsigned char
          cr = (unsigned char)std::min(50 + 3*(unsigned char)(100*cimg::rand()), 255),
          cg = (unsigned char)std::min(50 + 3*(unsigned char)(100*cimg::rand()), 255),
          cb = (unsigned char)std::min(50 + 3*(unsigned char)(100*cimg::rand()), 255);
        const float di = 10 + (float)cimg::rand()*60, nr = (float)cimg::rand()*30;
        for (float i=0; i<360; i+=di) {
          const float rad = i*(float)cimg::PI/180, c = (float)std::cos(rad), s = (float)std::sin(rad);
          particles.insert(CImg<>::vector(x,y,2*c + vx/1.5f,2*s + vy/1.5f,-2.0f,nr));
          colors.insert(CImg<unsigned char>::vector(cr,cg,cb));
        }
        remove_particle = true;
      } else if (t<-1) { r*=0.95f; if (r<0.5f) remove_particle=true; }
      if (remove_particle) { particles.remove(l); colors.remove(l); l--; }
    }
    if (disp.button()&2) cimglist_for(particles,l) if (particles(l,4)>0) particles(l,4)=0.5f;
    img.draw_text(5,5," %u frames/s ",white,red,0.5f,13,(unsigned int)disp.frames_per_second());
    disp.display(img).wait(25);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
    if (disp.is_resized()) disp.resize(disp,false);
  }
  return 0;
}

int main()
{
    item_fireworks();

    //Vector<3> vec;
}