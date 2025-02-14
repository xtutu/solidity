==== Source: A.sol ====
contract A { function a() pure public { assert(false); } }
==== Source: B.sol ====
import "A.sol";
contract B { function b() pure public { assert(false); } }
// ====
// SMTContract: B.sol:B
// SMTEngine: chc
// ----
// Warning 6328: (B.sol:56-69): CHC: Assertion violation happens here.\nCounterexample:\n\n\nTransaction trace:\nB.constructor()\nB.b()
