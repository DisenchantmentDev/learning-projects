const std = @import("std");
const zig_emu = @import("zig_emu");
const bf = zig_emu.bitflags;

test "bf set flag test 1" {
    var f = bf.bitflags.init();
    f.set(bf.flags.C, true);
    try std.testing.expectEqual(0b00010000, f.bits());
}

test "bf set flag test 2" {
    var f = bf.bitflags.init();
    f.set(bf.flags.Z, true);
    try std.testing.expectEqual(0b10000000, f.bits());
}

test "bf set flag test 3" {
    var f = bf.bitflags.init();
    f.set(bf.flags.Z, true);
    f.set(bf.flags.C, true);
    try std.testing.expectEqual(0b10010000, f.bits());
}

test "bf set flag test 4" {
    var f = bf.bitflags.init();
    f.set(bf.flags.Z, true);
    f.set(bf.flags.C, true);
    f.set(bf.flags.Z, false);
    try std.testing.expectEqual(0b00010000, f.bits());
}
