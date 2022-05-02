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

        int describe(std::string description,
            std::function<void (void)> suite);

        void it(std::string description, std::function<void (void)> test);

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

        inline Suite *getCurrent()
        {
            return current;
        }

        inline Suite *updateCurrent(Suite *next)
        {
            Suite *prev = current;

            current = next;

            return prev;
        }

    private:
        Suite top;

        Suite *current;
    };

    extern Context context;

    inline int describe(std::string description,
        std::function<void (void)> suite)
    {
        return context.getCurrent()->describe(description, suite);
    }

    inline void it(std::string description,
        std::function<void (void)> test)
    {
        context.getCurrent()->it(description, test);
    }
}

#endif // __ALTEA_HH
