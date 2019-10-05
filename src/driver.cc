#include "driver.hh"
#include "parser.hh"
#include "scanner.hh"

namespace parser {
    Driver::Driver(): scanner(new Scanner()), parser_(new Parser(*this)),
        location_(new location())
    {
    }

    Driver::~Driver()
    {
        delete parser_;
        delete scanner;
        delete location_;
    }

    void Driver::reset()
    {
        delete location_;
        location_ = new location();
    }

    int Driver::parse()
    {
        scanner->switch_streams(&std::cin, &std::cerr);

        return parser_->parse();
    }

    int Driver::parseFile(std::string &path)
    {
        std::ifstream s(path.c_str(), std::ifstream::in);
        scanner->switch_streams(&s, &std::cerr);

        parser_->parse();

        s.close();

        return 0;
    }
}

int main()
{
    parser::Driver driver;

    return driver.parse();
}
