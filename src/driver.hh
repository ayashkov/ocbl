#ifndef __DRIVER_HH
#define __DRIVER_HH

#include <string>
#include <iostream>
#include <fstream>

namespace parser {
    /// Forward declarations of classes
    class Parser;

    class Scanner;

    class location;

    class Driver {
    public:
        Driver();

        ~Driver();

        int parse();

        int parseFile(std::string &path);

        void reset();

    private:
        Scanner *scanner;

        Parser *parser_;

        location *location_;

        int error_;

        friend class Scanner;

        friend class Parser;
    };
}

#endif /* __DRIVER_HH */
