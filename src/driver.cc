#include "driver.hh"
#include "parser.hh"
#include "scanner.hh"

namespace parser {
    Driver::Driver(): scanner(new Scanner()), parser(new Parser(*this)),
        loc(new location())
    {
    }

    Driver::~Driver()
    {
        delete parser;
        delete scanner;
        delete loc;
    }

    void Driver::reset()
    {
        delete loc;
        loc = new location();
    }

    int Driver::parse()
    {
        scanner->switch_streams(&std::cin, &std::cerr);

        return parser->parse();
    }

    int Driver::parseFile(std::string &path)
    {
        std::ifstream s(path.c_str(), std::ifstream::in);

        scanner->switch_streams(&s, &std::cerr);
        parser->parse();
        s.close();

        return 0;
    }
}

int main()
{
    parser::Driver driver;

    return driver.parse();
}
