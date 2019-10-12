#ifndef __SPEC_HH
#define __SPEC_HH

#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <exception>

#define _() []()
#define $() [=]()

namespace spec {
    class TestException: public std::exception {
    };

    class Runnable {
    public:
        Runnable(std::string d): description(d)
        {
        }

        virtual ~Runnable() {
        }

        virtual void run() = 0;

    protected:
        std::string description;
    };

    class Test: public Runnable {
    public:
        Test(std::string d, std::function<void (void)> t): Runnable(d), test(t)
        {
        }

        virtual void run();

    private:
        std::function<void (void)> test;
    };

    class Suite: public Runnable {
    public:
        Suite(Suite *p, std::string d): Runnable(d), previous(p)
        {
        }

        virtual ~Suite();

        virtual void run();

        void addBeforeAll(std::function<void (void)> f);

        void addBeforeEach(std::function<void (void)> f);

        void add(Runnable *r);

        void addAfterAll(std::function<void (void)> f);

        void addAfterEach(std::function<void (void)> f);

        Suite *getPrevious();

    private:
        Suite *previous;

        std::vector<std::function<void (void)>> beforeAll;

        std::vector<std::function<void (void)>> beforeEach;

        std::vector<Runnable*> tests;

        std::vector<std::function<void (void)>> afterAll;

        std::vector<std::function<void (void)>> afterEach;
    };

    class TestContext {
    public:
        void addBeforeAll(std::function<void (void)> f);

        void addBeforeEach(std::function<void (void)> f);

        void addSuite(std::string description, std::function<void (void)> suite);

        void addTest(std::string description, std::function<void (void)> test);

        void addAfterAll(std::function<void (void)> f);

        void addAfterEach(std::function<void (void)> f);

    private:
        Suite *current = nullptr;
    };

    extern TestContext testContext;

    inline void beforeAll(std::function<void (void)> before)
    {
        testContext.addBeforeAll(before);
    }

    inline void beforeEach(std::function<void (void)> before)
    {
        testContext.addBeforeEach(before);
    }

    inline int describe(std::string description, std::function<void (void)> suite)
    {
        testContext.addSuite(description, suite);

        return 0;
    }

    inline void it(std::string description, std::function<void (void)> test)
    {
        testContext.addTest(description, test);
    }

    inline void afterAll(std::function<void (void)> after)
    {
        testContext.addAfterAll(after);
    }

    inline void afterEach(std::function<void (void)> after)
    {
        testContext.addAfterEach(after);
    }
}

#endif // __SPEC_HH
