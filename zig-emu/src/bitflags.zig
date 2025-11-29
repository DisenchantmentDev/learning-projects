const std = @import("std");

const flags = enum(u8) {
    Z = 0b10000000,
    N = 0b01000000,
    H = 0b00100000,
    C = 0b00010000,
    EMPTY = 0b00000000,
};

pub fn get_flag_from(val: u8) flags {
    return switch (val) {
        0b10000000 => flags.Z,
        0b01000000 => flags.N,
        0b00100000 => flags.H,
        0b00010000 => flags.C,
        0b00000000 => flags.EMPTY,
    };
}

pub const bitflags = struct {
    B: u8,

    pub fn init() bitflags {
        return bitflags{
            .B = @intFromEnum(flags.EMPTY),
        };
    }

    pub fn set(self: *bitflags, other: flags, value: bool) void {
        if (value) {
            self.B = self.B | @intFromEnum(other);
        } else {
            self.B = self.B & ~@intFromEnum(other);
        }
    }

    pub fn check(self: bitflags, other: flags) bool {
        const o = @intFromEnum(other);
        return self.B & o == o;
    }

    pub fn bits(self: bitflags) u8 {
        return self.B;
    }

    pub fn set_from_bits(self: *bitflags, val: u8) void {
        self.B = val;
    }
};

test "bitflags_test_1" {
    var f = bitflags.init();
    f.set(flags.C, true);
    try std.testing.expectEqual(0b00010000, f.bits());
}

test "bitflags_test_2" {
    var f = bitflags.init();
    f.set(flags.Z, true);
    try std.testing.expectEqual(0b10000000, f.bits());
}
