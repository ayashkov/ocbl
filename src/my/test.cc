#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <exception>

#define _() []()
#define $() [=]()

using namespace std;

class TestException: public exception {
};

class Runnable {
public:
    Runnable(string d): description(d)
    {
    }

    virtual ~Runnable() {
    }

    virtual void run() = 0;

protected:
    string description;
};

class Test: public Runnable {
public:
    Test(string d, function<void (void)> t): Runnable(d), test(t)
    {
    }

    virtual void run();

private:
    function<void (void)> test;
};

class Suite: public Runnable {
public:
    Suite(Suite *p, string d): Runnable(d), previous(p)
    {
    }

    virtual ~Suite();

    virtual void run();

    void addBeforeAll(function<void (void)> f);

    void addBeforeEach(function<void (void)> f);

    void add(Runnable *r);

    void addAfterAll(function<void (void)> f);

    void addAfterEach(function<void (void)> f);

    Suite *getPrevious();

private:
    Suite *previous;

    vector<function<void (void)>> beforeAll;

    vector<function<void (void)>> beforeEach;

    vector<Runnable*> tests;

    vector<function<void (void)>> afterAll;

    vector<function<void (void)>> afterEach;
};

class TestContext {
public:
    void addBeforeAll(function<void (void)> f);

    void addBeforeEach(function<void (void)> f);

    void addSuite(string description, function<void (void)> suite);

    void addTest(string description, function<void (void)> test);

    void addAfterAll(function<void (void)> f);

    void addAfterEach(function<void (void)> f);

private:
    Suite *current = nullptr;
};

TestContext testContext;

void Test::run()
{
    cout << description << '\n';
    test();
}

void Suite::addBeforeAll(function<void (void)> f)
{
    beforeAll.push_back(f);
}

void Suite::addBeforeEach(function<void (void)> f)
{
    beforeEach.push_back(f);
}

void Suite::add(Runnable *r)
{
    tests.push_back(r);
}

void Suite::addAfterAll(function<void (void)> f)
{
    afterAll.push_back(f);
}

void Suite::addAfterEach(function<void (void)> f)
{
    afterEach.push_back(f);
}

Suite *Suite::getPrevious()
{
    return previous;
}

void Suite::run()
{
    cout << description << '\n';

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

void TestContext::addBeforeAll(function<void (void)> before)
{
    if (current == nullptr)
        throw TestException();

    current->addBeforeAll(before);
}

void TestContext::addBeforeEach(function<void (void)> before)
{
    if (current == nullptr)
        throw TestException();

    current->addBeforeEach(before);
}

void TestContext::addSuite(string description, function<void (void)> suite)
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

void TestContext::addTest(string description, function<void (void)> test)
{
    if (current == nullptr)
        throw TestException();

    current->add(new Test(description, test));
}

void TestContext::addAfterAll(function<void (void)> after)
{
    if (current == nullptr)
        throw TestException();

    current->addAfterAll(after);
}

void TestContext::addAfterEach(function<void (void)> after)
{
    if (current == nullptr)
        throw TestException();

    current->addAfterEach(after);
}

inline void beforeAll(function<void (void)> before)
{
    testContext.addBeforeAll(before);
}

inline void beforeEach(function<void (void)> before)
{
    testContext.addBeforeEach(before);
}

inline int describe(string description, function<void (void)> suite)
{
    testContext.addSuite(description, suite);

    return 0;
}

inline void it(string description, function<void (void)> test)
{
    testContext.addTest(description, test);
}

inline void afterAll(function<void (void)> after)
{
    testContext.addAfterAll(after);
}

inline void afterEach(function<void (void)> after)
{
    testContext.addAfterEach(after);
}

static int o = 21;

static int s1 = describe("Test for test", _() {
    int *v = new int(1);

    beforeEach($() {
        *v += 2;
    });

    afterAll($() {
        delete v;
    });

    describe("nested 1", $() {
        it("is inside nested 1", $() {
            cout << "v = " << *v << '\n';
            cout << "o = " << o << '\n';
        });

        it("is also inside nested 1", $() {
            cout << "v = " << *v << '\n';
            cout << "o = " << o << '\n';
        });
    });

    describe("nested 2", $() {
        it("is inside nested 2", $() {
            cout << "v = " << *v << '\n';
            cout << "o = " << o << '\n';
        });

        it("is also inside nested 2", $() {
            cout << "v = " << *v << '\n';
            cout << "o = " << o << '\n';
        });
    });

    it("Should work", $() {
        cout << "v = " << *v << '\n';
        cout << "o = " << o << '\n';
    });
});

int main()
{
}
