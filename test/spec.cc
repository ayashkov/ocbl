#include <iostream>
#include "spec.hh"

using namespace std;

namespace spec {
    Context context;

    void Test::test()
    {
        cout << ">>> " << description << '\n';
        function();
    }

    void Suite::discover()
    {
        cout << "<<< " << description << '\n';

        Suite *prev = context.updateCurrent(this);

        function();

        context.updateCurrent(prev);
    }

    void Suite::test()
    {
        cout << ">>> " << description << '\n';

        Suite *prev = context.updateCurrent(this);

        function();

        context.updateCurrent(prev);
    }

    void Suite::addBeforeAll(std::function<void (void)> f)
    {
        if (context.isDiscovery())
            beforeAll.push_back(f);
    }

    void Suite::addBeforeEach(std::function<void (void)> f)
    {
        if (context.isDiscovery())
            beforeEach.push_back(f);
    }

    void Suite::addAfterAll(std::function<void (void)> f)
    {
        if (context.isDiscovery())
            afterAll.push_back(f);
    }

    void Suite::addAfterEach(std::function<void (void)> f)
    {
        if (context.isDiscovery())
            afterEach.push_back(f);
    }

    void Suite::runTests()
    {
        for (auto before : beforeAll)
            before();

        for (auto child : children) {
            for (auto before : beforeEach)
                before();

            try {
                child->test();
            } catch (const SpecException &ex) {
                // XXX: what now?
            } catch (...) {
                // XXX: and here?
            }

            for (auto after : afterEach)
                after();
        }

        for (auto after : afterAll)
            after();
    }

    void Suite::processChild(Testable *t)
    {
        if (context.isDiscovery()) {
            children.push_back(t);
            t->discover();
        } else {
            ++current;

            if (current < children.size())
                return;

            runTests();
        }
    }

    Suite::~Suite()
    {
    }

    Context::Context(): top("top", nullptr)
    {
        current = &top;
    }

    Context::~Context()
    {
    }

    void *Context::describe(std::string description,
        std::function<void (void)> suite)
    {
        Suite *s = new Suite(description, suite);

        current->processChild(s);

        return s;
    }

    void Context::beforeAll(std::function<void (void)> before)
    {
        ensureNested("beforeAll");
        current->addBeforeAll(before);
    }

    void Context::beforeEach(std::function<void (void)> before)
    {
        ensureNested("beforeEach");
        current->addBeforeEach(before);
    }

    void Context::it(std::string description,
        std::function<void (void)> test)
    {
        ensureNested("it");
        current->processChild(new Test(description, test));
    }

    void Context::xit(std::string description,
        std::function<void (void)> test)
    {
        ensureNested("xit");
        current->processChild(new Excluded(description, test));
    }

    void Context::afterAll(std::function<void (void)> after)
    {
        ensureNested("afterAll");
        current->addAfterAll(after);
    }

    void Context::afterEach(std::function<void (void)> after)
    {
        ensureNested("afterEach");
        current->addAfterEach(after);
    }

    void Context::runTests()
    {
        discovery = false;
        top.runTests();
    }

    Suite *Context::updateCurrent(Suite *next)
    {
        Suite *prev = current;

        current = next;

        return prev;
    }

    void Context::ensureNested(std::string name)
    {
        if (current == &top)
            throw SpecException();
    }
}

int main()
{
    using namespace spec;

    context.runTests();
}
