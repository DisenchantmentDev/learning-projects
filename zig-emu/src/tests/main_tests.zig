const std = @import("std");

// Import test files - this ensures all test blocks in these files are included
// These imports are intentionally unused; they're imported for their test blocks
comptime {
    _ = @import("bitflags_tests.zig");
    _ = @import("cpu_tests.zig");
}

// Add a test in main_tests.zig to verify tests are running
test "main_tests: verify test execution" {
    try std.testing.expect(true);
}
