#include "altea.hh"

using namespace std;

namespace altea {
    Context context;

    void Test::run()
    {
        function();
    }

    Suite::~Suite()
    {
        for (auto test : testables)
            delete test;

        testables.clear();
    }

    void Suite::run()
    {
        Suite *prev = context.updateCurrent(this);

        discovery = true;
        function();
        discovery = false;
        function();
        context.updateCurrent(prev);
    }

    void Suite::add(std::function<Testable* (void)> gen)
    {
        if (discovery)
            ++discovered;
        else {
            testables.push_back(gen());
            checkAndRun();
        }
    }

    void Suite::checkAndRun()
    {
        if (testables.size() < discovered)
            return;

        for (auto t : testables) {
            cout << t->description << endl;
            t->run();
        }
    }

    Context::Context(): top("top", nullptr)
    {
        currentSuite = &top;
    }

    Context::~Context()
    {
    }

    int Context::describe(string description, function<void (void)> suite)
    {
        currentSuite->add([=] {
            return new Suite(description, suite);
        });

        return 0;
    }

    void Context::it(string description, function<void (void)> test)
    {
        currentSuite->add([=] {
            return new Test(description, test);
        });
    }

    Suite *Context::updateCurrent(Suite *next)
    {
        Suite *prev = currentSuite;

        currentSuite = next;

        return prev;
    }
}

int main()
{
    return 0;
}
