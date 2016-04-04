#include "stdafx.h"
#include "sciter-x.h"
#include "sciter-x-behavior.h"
#include "sciter-x-graphics.hpp"
#include <time.h>   

namespace sciter
{
/*
BEHAVIOR: native-clock
  - draws content layer using sciter-x-graphics.hpp primitives.

SAMPLE:
   See: samples/behaviors/native-drawing.htm
*/

struct native_clock: public event_handler
{
    // ctor
    native_clock() {}
    virtual ~native_clock() {}

    virtual bool subscription( HELEMENT he, UINT& event_groups )
    {
      event_groups = HANDLE_DRAW   // it does drawing
                   | HANDLE_TIMER; // and handles timer
      return true;
    }

    virtual void attached  (HELEMENT he ) 
    {
      dom::element(he).start_timer(1000);
    }
    virtual void detached  (HELEMENT he ) { delete this; }


    virtual bool handle_timer  (HELEMENT he,TIMER_PARAMS& params )
    {
      dom::element(he).refresh(); // refresh element's area
      return true; // keep ticking
    }

    virtual bool handle_draw   (HELEMENT he, DRAW_PARAMS& params ) 
    {
      if( params.cmd != DRAW_CONTENT ) return false; // drawing only content layer

      const float PI = 3.141592653f;

      float w = float(params.area.right - params.area.left);
      float h = float(params.area.bottom - params.area.top);

      float scale = w < h? w / 300.0f: h / 300.0f;

      time_t rawtime;
      time (&rawtime);
      struct tm timeinfo = *localtime (&rawtime);

      sciter::graphics gfx(params.gfx);
      gfx.state_save();

      gfx.translate(params.area.left + w / 2.0f,params.area.top + h / 2.0f);
      gfx.scale(scale,scale);    
      gfx.rotate(-PI/2);
      gfx.line_color(0);
      gfx.line_width(8.f);
      gfx.line_cap(SCITER_LINE_CAP_ROUND);
       
      // Hour marks
      gfx.state_save();
        gfx.line_color(gcolor(0x32,0x5F,0xA2));
        for (int i = 0; i < 12; ++i) {
          gfx.rotate(PI/6);
          gfx.line(137.f,0,144.f,0);
        }
      gfx.state_restore();

      // Minute marks
      gfx.state_save();
        gfx.line_width(3.f);
        gfx.line_color(gcolor(0xA5,0x2A, 0x2A));
        for (int i = 0; i < 60; ++i) {
          if ( i % 5 != 0)
            gfx.line(143,0,146,0);
          gfx.rotate(PI/30.f);
        }
      gfx.state_restore();

      int sec = timeinfo.tm_sec;
      int min = timeinfo.tm_min;
      int hr  = timeinfo.tm_hour;
      hr = hr >= 12 ? hr - 12 : hr;
  
      // draw Hours
      gfx.state_save();
        gfx.rotate( hr*(PI/6) + (PI/360)*min + (PI/21600)*sec );
        gfx.line_width(14);
        gfx.line_color(gcolor(0x32,0x5F,0xA2));
        gfx.line(-20,0,70,0);
        gfx.state_restore();

        // draw Minutes
        gfx.state_save();
        gfx.rotate( (PI/30)*min + (PI/1800)*sec );
        gfx.line_width(10);
        gfx.line_color(gcolor(0x32,0x5F,0xA2));
        gfx.line(-28,0,100,0);
      gfx.state_restore();
      
      // draw seconds
      gfx.state_save();
        gfx.rotate(sec * PI/30);
        gfx.line_color(gcolor(0xD4,0,0));
        gfx.fill_color(gcolor(0xD4,0,0));
        gfx.line_width(6);
        gfx.line(-30,0,83,0);
        gfx.ellipse(0,0,10,10);
    
        gfx.fill_color(0);
        gfx.ellipse(95,0,10,10);
      gfx.state_restore();
    
      gfx.state_restore();

      return false;
    
    }

};

struct native_clock_factory: public behavior_factory {

  native_clock_factory(): behavior_factory("native-clock") { }

  // the only behavior_factory method:
  virtual event_handler* create(HELEMENT he) {
    return new native_clock();
  }

};

// instantiating and attaching it to the global list
native_clock_factory native_clock_factory_instance;


}
