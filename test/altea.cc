#include "altea.hh"

using namespace std;

namespace altea {
    Context context;

    void Test::test()
    {
        function();
    }

    Suite::~Suite()
    {
        for (auto test : testables)
            delete test;

        testables.clear();
    }

    int Suite::describe(string description, std::function<void (void)> suite)
    {
        add([=] {
            return new Suite(description, suite);
        });

        return 0;
    }

    void Suite::it(string description, std::function<void (void)> test)
    {
        add([=] {
            return new Test(description, test);
        });
    }

    void Suite::test()
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

    void Suite::run()
    {
        for (auto t : testables) {
            cout << t->description << endl;
            t->test();
        }
    }

    void Suite::checkAndRun()
    {
        if (testables.size() < discovered)
            return;

        run();
    }

    Context::Context(): top()
    {
        current = &top;
    }

    Context::~Context()
    {
    }
}

int main()
{
    altea::context.getCurrent()->run();

    return 0;
}
