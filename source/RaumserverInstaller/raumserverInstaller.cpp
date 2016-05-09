
#include "raumserverInstallerView.h"
#include "resources.cpp" // packed /res/ folder

//http://www.terrainformatica.com/2012/11/sciter-ui-application-architecture/


int uimain(std::function<int()> run)
{
    SciterSetOption(NULL, SCITER_SET_DEBUG_MODE, TRUE);

    // bind resources[] (defined in "resources.cpp") with the archive
    sciter::archive::instance().open(aux::elements_of(resources));

    ApplicationWindow *appWindow = new ApplicationWindow();

    appWindow->init();
    // note: this:://app URL schema is dedicated to the sciter::archive content associated with the application
    appWindow->load(WSTR("this://app/installer.htm"));
    //appWindow->load(WSTR("res/installer.htm"));
    appWindow->expand();

    return run();
}



