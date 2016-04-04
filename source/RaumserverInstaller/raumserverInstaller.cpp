
#include "raumserverInstallerView.h"
#include "resources.cpp" // packed /res/ folder

//http://www.terrainformatica.com/2012/11/sciter-ui-application-architecture/


int uimain(std::function<int()> run)
{
    SciterSetOption(NULL, SCITER_SET_DEBUG_MODE, TRUE);

    // bind resources[] (defined in "resources.cpp") with the archive
    sciter::archive::instance().open(aux::elements_of(resources));

    frame *pwin = new frame();

    // note: this:://app URL schema is dedicated to the sciter::archive content associated with the application
    pwin->load(WSTR("this://app/installer.htm"));

    pwin->expand();

    return run();
}



