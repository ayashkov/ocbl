#ifndef __SPEC_HH
#define __SPEC_HH

#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <exception>

#define _() []()
#define $() [&]()

namespace spec {
    class SpecException: public std::exception {
    };

    class Testable {
    public:
        Testable(std::string d, std::function<void (void)> f):
            description(d), function(f)
        {
        }

        virtual ~Testable()
        {
        }

        virtual void test() = 0;

    protected:
        const std::string description;

        const std::function<void (void)> function;
    };

    class Test: public Testable {
    public:
        Test(std::string d, std::function<void (void)> t): Testable(d, t)
        {
        }

        virtual void test();
    };

    class Excluded: public Testable {
    public:
        Excluded(std::string d, std::function<void (void)> t): Testable(d, t)
        {
        }

        virtual void test()
        {
        }
    };

    class Suite: public Testable {
    public:
        Suite(std::string d, std::function<void (void)> s): Testable(d, s)
        {
        }

        virtual ~Suite();

        virtual void test();

        void addBeforeAll(std::function<void (void)> f);

        void addBeforeEach(std::function<void (void)> f);

        void processChild(Testable *t);

        void addAfterAll(std::function<void (void)> f);

        void addAfterEach(std::function<void (void)> f);

    private:
        bool discovery;

        int current = 0;

        int total = 0;

        std::vector<std::function<void (void)>> beforeAll;

        std::vector<std::function<void (void)>> beforeEach;

        std::vector<std::function<void (void)>> afterAll;

        std::vector<std::function<void (void)>> afterEach;

        void executeTest(Testable *t);
    };

    class Context {
    public:
        Context();

        ~Context();

        void beforeAll(std::function<void (void)> before);

        void beforeEach(std::function<void (void)> before);

        void *describe(std::string description,
            std::function<void (void)> suite);

        void it(std::string description, std::function<void (void)> test);

        void xit(std::string description, std::function<void (void)> test);

        void afterAll(std::function<void (void)> after);

        void afterEach(std::function<void (void)> after);

        Suite *updateCurrent(Suite *next);

    private:
        Suite *current = nullptr;

        void ensureNested(std::string name);
    };

    extern Context context;

    inline void beforeAll(std::function<void (void)> before)
    {
        context.beforeAll(before);
    }

    inline void beforeEach(std::function<void (void)> before)
    {
        context.beforeEach(before);
    }

    inline void *describe(std::string description,
        std::function<void (void)> suite)
    {
        return context.describe(description, suite);
    }

    inline void it(std::string description, std::function<void (void)> test)
    {
        context.it(description, test);
    }

    inline void xit(std::string description, std::function<void (void)> test)
    {
        context.xit(description, test);
    }

    inline void afterAll(std::function<void (void)> after)
    {
        context.afterAll(after);
    }

    inline void afterEach(std::function<void (void)> after)
    {
        context.afterEach(after);
    }
}

#endif // __SPEC_HH
