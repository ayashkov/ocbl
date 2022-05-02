#ifndef __ALTEA_HH
#define __ALTEA_HH

#include <string>
#include <functional>
#include <vector>
#include <exception>
#include <iostream>

namespace altea {
    class Testable {
    public:
        const std::string description;

        const std::function<void (void)> function;

        Testable(std::string d, std::function<void (void)> f):
            description(d), function(f)
        {
        }

        virtual ~Testable()
        {
        }

        virtual void run() = 0;
    };

    class Test: public Testable {
    public:
        Test(std::string d, std::function<void (void)> t): Testable(d, t)
        {
        }

        void run();
    };

    class Suite: public Testable {
    public:
        Suite(std::string d, std::function<void (void)> s): Testable(d, s)
        {
        }

        virtual ~Suite();

        void run();

        void add(std::function<Testable* (void)> gen);

    private:
        bool discovery = false;

        int discovered = 0;

        std::vector<Testable*> testables;

        void checkAndRun();
    };

    class Context {
    public:
        Context();

        ~Context();

        int describe(std::string description,
            std::function<void (void)> suite);

        void it(std::string description, std::function<void (void)> test);

        Suite *updateCurrent(Suite *next);

    private:
        Suite top;

        Suite *currentSuite;
    };

    extern Context context;

    inline int describe(std::string description,
        std::function<void (void)> suite)
    {
        return context.describe(description, suite);
    }

    inline void it(std::string description,
        std::function<void (void)> test)
    {
        context.it(description, test);
    }
}

#endif // __ALTEA_HH
