#include "spec.hh"

using namespace std;
using namespace spec;

static int o = 21;

static auto s = describe("Test for test", _() {
    int v = 1;

    beforeEach($() {
        v += 2;
    });

    afterAll($() {
        v = 0;
    });

    describe("nested 1", $() {
        it("is inside nested 1", $() {
            cout << "v = " << v << '\n';
            cout << "o = " << o << '\n';
        });

        it("is also inside nested 1", $() {
            cout << "v = " << v << '\n';
            cout << "o = " << o << '\n';
        });
    });

    describe("nested 2", $() {
        it("is inside nested 2", $() {
            cout << "v = " << v << '\n';
            cout << "o = " << o << '\n';
        });

        it("is also inside nested 2", $() {
            cout << "v = " << v << '\n';
            cout << "o = " << o << '\n';
        });
    });

    it("Should work", $() {
        cout << "v = " << v << '\n';
        cout << "o = " << o << '\n';
    });

    xit("This one is disabled", $() {
        cout << "DO NOT PRINT ME\n";
    });
});
