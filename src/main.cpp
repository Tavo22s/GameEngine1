#include "Engine/System.hpp"
#include "Engine/Utils.hpp"

int main(int argc, char** argv)
{
    std::string path = Utils::to_string<char*> (argv[0]);
    int idx = path.find_last_of('\\') + 1;
    path = Utils::Ex(path, 0, idx);

    Utils::Path = path;

    System *sys = new System(1280, 720);
    if(!sys)
        return -1;

    if(sys->Init())
    {
        sys->Loop();
        sys->Shutdown();
    }

    delete sys;

    return 0;
}