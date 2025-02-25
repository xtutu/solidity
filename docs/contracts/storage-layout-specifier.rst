.. index:: ! storage layout specifier, ! layout at

.. _storage-layout-specifier:

************************
Storage Layout Specifier
************************

Contracts can define an arbitrary base slot for its own storage.
The contract's state variables will be stored from the specified slot
instead of the default slot zero.
This can be done by using ``layout at <base-slot-expression>`` in the header of
a contract definition, either after or before the inheritance specifier.
It can be given at most once.
The ``base-slot-expression`` must be an expression consisting of number literals
that can be evaluated at compile time and yield in a rational value in the range of ``uint256``.
As a general rule, any expression that can be used to specify the size of an array
may also be used to indicate a slot.
The layout of a contract without an explicit specification is identical to one with ``layout at 0``.
The storage layout cannot be specified for abstract contracts, interfaces and libraries.
The identifiers ``layout`` and ``at`` are not reserved keywords of the Solidity language, but
it is strongly recommended to avoid using them since that may change in the future.
In the following example, contract ``C`` explicitly specifies its storage base slot which
results in its state variables ``x`` and ``flag`` being located at slots ``42`` and ``43``, respectively.

.. code-block:: solidity

    // SPDX-License-Identifier: GPL-3.0
    pragma solidity >=0.8.29 <0.9.0;
    contract C layout at 42 {
        uint x;
        bool flag;
    }


The ``base-slot-expression`` determines the starting storage location for the
inheritance hierarchy as a whole.
Locations of all storage variables are shifted by its value and their locations
relative to each other remain unaffected.
In the next example, contract ``C`` inherits from contracts ``A`` and ``B``
and also specifies a custom storage base slot. So, inherited state variables,
starting from the top level contract ``A`` will be stored from slot ``42``.
Thus, state variable ``y``, for instance, will be at slot ``43``, while
state variable ``flag`` will be at slot ``45``.

.. code-block:: solidity

    // SPDX-License-Identifier: GPL-3.0
    pragma solidity >=0.8.29 <0.9.0;
    contract A {
        uint x;
    }

    contract B {
        address payable y;
        mapping (address => bool) public map;
    }

    contract C is A, B layout at 0x1234 {
        bool flag;
    }