contract C layout at 2**256 - 2 {
    uint x;
    bool b;
}
// ----
// Warning 3495: (21-31): There are 1 slots before the end of the contract storage when this specified base layout is used.
// TypeError 5015: (21-31): Contract extends past the end of storage when this base slot value is specified.
