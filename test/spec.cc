#include "spec.hh"

namespace spec {
    TestContext testContext;

    void Test::run()
    {
        std::cout << description << '\n';
        test();
    }

    void Suite::addBeforeAll(std::function<void (void)> f)
    {
        beforeAll.push_back(f);
    }

    void Suite::addBeforeEach(std::function<void (void)> f)
    {
        beforeEach.push_back(f);
    }

    void Suite::add(Runnable *r)
    {
        tests.push_back(r);
    }

    void Suite::addAfterAll(std::function<void (void)> f)
    {
        afterAll.push_back(f);
    }

    void Suite::addAfterEach(std::function<void (void)> f)
    {
        afterEach.push_back(f);
    }

    Suite *Suite::getPrevious()
    {
        return previous;
    }

    void Suite::run()
    {
        std::cout << description << '\n';

        for (auto b : beforeAll)
            b();

        for (auto t : tests) {
            for (auto b : beforeEach)
                b();

            t->run();

            for (auto a : afterEach)
                a();
        }

        for (auto a : afterAll)
            a();
    }

    Suite::~Suite()
    {
        for (auto t : tests)
            delete t;
    }

    void TestContext::addBeforeAll(std::function<void (void)> before)
    {
        if (current == nullptr)
            throw TestException();

        current->addBeforeAll(before);
    }

    void TestContext::addBeforeEach(std::function<void (void)> before)
    {
        if (current == nullptr)
            throw TestException();

        current->addBeforeEach(before);
    }

    void TestContext::addSuite(std::string description,
        std::function<void (void)> suite)
    {
        Suite *s = new Suite(current, description);

        if (current != nullptr)
            current->add(s);

        current = s;
        suite();
        current = current->getPrevious();

        if (current == nullptr) {
            s->run();
            delete s;
        }
    }

    void TestContext::addTest(std::string description,
        std::function<void (void)> test)
    {
        if (current == nullptr)
            throw TestException();

        current->add(new Test(description, test));
    }

    void TestContext::addAfterAll(std::function<void (void)> after)
    {
        if (current == nullptr)
            throw TestException();

        current->addAfterAll(after);
    }

    void TestContext::addAfterEach(std::function<void (void)> after)
    {
        if (current == nullptr)
            throw TestException();

        current->addAfterEach(after);
    }
}

int main()
{
}
