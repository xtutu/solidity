contract C {
    uint8 x;
    int8 y;
    bytes2 b;
    // Note the return types are larger than the state variable ones
    function f(uint _x, int _y, bytes3 _b) public returns (uint16, int32, bytes32) {
        assembly {
            sstore(x.slot, _x)
            sstore(y.slot, _y)
            sstore(b.slot, _b)
        }
        return (x, y, b);
    }
}
// ----
// f(uint256,int256,bytes3): 256, -129, "abc" -> 0, 0, 0x00
