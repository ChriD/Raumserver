
#include <raumserver/raumserver.h>
#include <raumkernel/rlutil/rlutil.h>


int main()
{
    Raumserver::Raumserver raumServerObject;

    // init the raumserver
    // this will do all the stuff we only would have to create some gui to view the status
    raumServerObject.init();

    // wait for escape to quit    
    while (rlutil::getkey() != rlutil::KEY_ESCAPE)
    {
        // enldless loop until escape is pressed
    }

    return 0;
}





