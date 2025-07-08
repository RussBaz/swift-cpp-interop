# Swift (6.1) and C++(20) interop example with SWIFT_SHARED_REFERENCE and std::shared_ptr

I am trying to use std::shared_ptr in the background, and yes, be careful. This is still a work in progress.

Just run `swift test` from the root of this repository to test on your machine.

NOTE: Swift might not rebuild the C++ headers if there are no changes in the Swift code, regardless of changes to the C++ side. I am not sure if this is intended or a bug, but I had an issue with this.
