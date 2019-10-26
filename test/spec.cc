#include "spec.hh"

namespace spec {
    Context context;

    void Test::test()
    {
        function();
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

    void Suite::addBeforeAll(std::function<void (void)> f)
    {
        if (discovery)
            beforeAll.push_back(f);
    }

    void Suite::addBeforeEach(std::function<void (void)> f)
    {
        if (discovery)
            beforeEach.push_back(f);
    }

    void Suite::addAfterAll(std::function<void (void)> f)
    {
        if (discovery)
            afterAll.push_back(f);
    }

    void Suite::addAfterEach(std::function<void (void)> f)
    {
        if (discovery)
            afterEach.push_back(f);
    }

    void Suite::processChild(Testable *t)
    {
        if (discovery)
            ++total;
        else
            executeTest(t);
    }

    void Suite::executeTest(Testable *t)
    {
        if (current == 0)
            for (auto before : beforeAll)
                before();

        for (auto before : beforeEach)
            before();

        try {
            t->test();
        } catch (const SpecException &ex) {
            // XXX: what now?
        } catch (...) {
            // XXX: and here?
        }

        for (auto after : afterEach)
            after();

        ++current;

        if (current == total)
            for (auto after : afterAll)
                after();
    }

    Suite::~Suite()
    {
    }

    Context::Context()
    {
    }

    Context::~Context()
    {
    }

    void *Context::describe(std::string description,
        std::function<void (void)> suite)
    {
        Suite described(description, suite);

        if (current == nullptr)
            described.test();
        else
            current->processChild(&described);

        return nullptr;
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

        Test described(description, test);

        current->processChild(&described);
    }

    void Context::xit(std::string description,
        std::function<void (void)> test)
    {
        ensureNested("xit");

        Excluded described(description, test);

        current->processChild(&described);
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

    Suite *Context::updateCurrent(Suite *next)
    {
        Suite *prev = current;

        current = next;

        return prev;
    }

    void Context::ensureNested(std::string name)
    {
        if (current == nullptr)
            throw SpecException();
    }
}

int main()
{
}
