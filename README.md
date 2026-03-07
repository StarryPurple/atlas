# Atlas

Adaptive Template Library for Academical Studies, or *ATLAS*,
is a C++ header-only library designed mainly for the purpose of SJTU ACM Class data structure lecture teaching,
~~which serves more like a wrapper for the STLite project~~.

This project repository is supposed to be included as git submodules for two projects (of ACM Class): Swirl and TicketSystem.
No further usages (like, tools for other projects) is designed or expected by the author.

# What does this project contain?

The main stuff are divided into four base directories:
- `common/`, which includes exceptions, some basic debug utils, and some configurations.
- `memory/`, which contains smart pointers from STL `<memory>`. Currently, they're just typedefs.
- `container/`, which contains re-implemented in-memory data containers.
- `concurrency/`, which contains some concurrency primitives with *limited interfaces* and some thread-safe utils (like `channel`).

And, there's a `test/` directory which includes some GoogleTest files. You can write Google tests to test your implementations.

## What does the `Adaptive` word stand for?

In `CMakeLists.txt`, a macro `ATLAS_ENABLE_CONCURRENCY` which is on by default is defined.
If this option is tuned off, the concurrency primitives in `concurrency/` will fall to a single-thread version,
which might be an optimization in time and space (thread-safe primitives are large) at the cost of thread safety.

I also added that macro `ONLINE_JUDGE` will also enable this optimization.
(But it seems that ACMOJ does not active it when you submit through Git.)

This information is also recorded in `common/config.hpp`.
Read the files if you want to explore the mechanics more.

Submitting to ACMOJ with this option on won't affect the correctness.

## What should I do in this project?

Just fill the `container/*.cpp` with what you coded in STLite, with the exceptions switched to Atlas version defined in `common/exception.hpp`.

If you found certain containers needed / unneeded in implementing Bplustree or Ticket System, feel free to add / remove files.
Reload CMakeFiles.txt (through `cmake` command or something else) if you met some problems that emerge from file adjustment.

You can write some tests to confirm that you implemented the data structures right.

## What else can I do for time optimization / concurrency bonus?

If you'd like to have a try on the concurrency bonus of the further works, we recommend you:
- Complete `atlas::channel`. That shall be a piece of cake for the simplest implementation.
- Re-design `atlas::shared_ptr`. Its STL version guarantees limited thread-safety on reference counting.
If you're sure you do not need `weak_ptr` or this limited thread-safety, you can try to rewrite it.

Feel free to add more things you need.

You can some tests to confirm that you implemented the data structures right.

## Postscript

I once came up with different names:

- ATL (Alternative Template Library)
- ALTAR (Adaptive Template Library with Algorithms & Resources)
- STLite Replica

...Naming things was inspiration-demanding.