contract B {
   constructor() { fail(); }

   function fail() pure internal { assert(false); }
}

contract A {
    function fail() pure public { assert(false); }
}
// ====
// SMTContract: A
// SMTEngine: chc
// ----
// Warning 6328: (145-158): CHC: Assertion violation happens here.\nCounterexample:\n\n\nTransaction trace:\nA.constructor()\nA.fail()
