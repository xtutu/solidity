contract C layout at 2**256 - 1 {
    uint transient x;
    uint transient y;
    uint transient z;
}
// ====
// EVMVersion: >=cancun
// ----
// Warning 3495: (21-31): There are 0 slots before the end of the contract storage when this specified base layout is used.
