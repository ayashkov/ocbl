#include "spec.hh"

using namespace std;
using namespace spec;

static int o = 21;

static int s = describe("Test for test", _() {
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
