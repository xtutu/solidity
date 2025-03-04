contract A layout at (2**256 + 1) * 2  - 2**256 - 3 {}
contract B layout at (2**2 - 2**3) * (2**5 - 2**8) {}
// ----
// Warning 3495: (21-51): There are 0 slots before the end of the contract storage when this specified base layout is used.
