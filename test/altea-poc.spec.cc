#include <iostream>
#include "altea.hh"

using namespace std;
using namespace altea;

static auto _ = describe("Top-level context", [] {
    auto var = "variable";

    it("should run one test", [&] {
        cout << "one" << endl;
    });

    it("should run more than one test", [&] {
        cout << "two" << endl;
    });

    it("should have access to variables", [&] {
        cout << var << endl;
    });

    describe("Sub context", [&] {
        auto sub = "sub variable";

        it("should run tests in subcontext", [&] {
            cout << "sub" << endl;
        });

        it("should still have access to variables", [&] {
            cout << var << endl;
            cout << sub << endl;
        });
    });
});
