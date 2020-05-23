#include <iostream>

#include <liboscar/StaticOsmCompleter.h>
#include <liboscar/routing/support/Edge2CellIds.h>

void help() {
    std::cerr << "pgr -f <oscar files> -s <lat> <lon> -t <lat> <lon>" << std::endl;
}

int main(int argc, char ** argv) {
    liboscar::Static::OsmCompleter cmp;
    sserialize::spatial::GeoPoint src, tgt;
    for(int i (1); i < argc; ++i){
        std::string token(argv[i]);
        if (token == "-f" && i+1 < argc) {
            cmp.setAllFilesFromPrefix(std::string(argv[i+1]));
            ++i;
        }
        else if (token == "-s" && i+2 < argc) {
            src.lat() = ::atof(argv[i+1]);
            src.lon() = ::atof(argv[i+2]);
            i += 2;
        }
        else if (token == "-t" && i+2 < argc) {
            tgt.lat() = ::atof(argv[i+1]);
            tgt.lon() = ::atof(argv[i+2]);
            i += 2;
        }
        else {
            help();
            std::cerr << "Unkown command " << token << std::endl;
            return -1;
        }
    }
    try {
        cmp.energize();
    }
    catch (std::exception const & e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    liboscar::routing::support::Edge2CellIds e2c(cmp.store());

    auto cells = e2c(src, tgt);

    for(auto x : cells) {
        std::cout << x << '\n';
    }
    std::cout << std::flush;

    return 0;
}