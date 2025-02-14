contract B {
   function fail() pure public { assert(false); }
}

contract A {
    function fail() pure public { assert(false); }
}
// ====
// SMTContract: A
// SMTEngine: chc
// ----
// Warning 6328: (113-126): CHC: Assertion violation happens here.\nCounterexample:\n\n\nTransaction trace:\nA.constructor()\nA.fail()
