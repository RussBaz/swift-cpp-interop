@testable import Lib
import Testing

@Test func example() async throws {
    let inspector = Inspector()
    #expect(inspector.message() == "meow world!")
}
