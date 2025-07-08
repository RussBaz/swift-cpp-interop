import cxxLib

public final class Inspector {
    let t: TestClass

    public init() {
        print("Inspector initialised")
        t = TestClass.swift_create("meow world!")
    }

    public func message() -> String {
        String(t.message)
    }
}
