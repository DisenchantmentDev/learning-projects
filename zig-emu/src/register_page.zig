const std = @import("std");
const bitflags = @import("bitflags.zig");

const r8 = enum { A, B, C, D, E, F, H, L };

pub const r16 = enum { AF, BC, DE, HL, SP };

pub const RegisterPage = struct {
    a: u8,
    flags: bitflags,
    b: u8,
    c: u8,
    d: u8,
    e: u8,
    h: u8,
    l: u8,
    sp: u16,
    pc: u16,

    pub fn init() RegisterPage {
        return RegisterPage{
            .a = 0,
            .flags = bitflags.bitflags.init(),
            .b = 0,
            .c = 0,
            .d = 0,
            .e = 0,
            .h = 0,
            .l = 0,
            .sp = 0,
            .pc = 0,
        };
    }

    pub fn read16(self: RegisterPage, reg: r16) u16 {
        return switch (reg) {
            r16.SP => self.sp,
            r16.AF => blk: {
                break :blk ((@as(u16, self.a)) << 8) | self.flags.bitflags.bits();
            },
            r16.BC => blk: {
                break :blk ((@as(u16, self.a)) << 8) | @as(u16, self.c);
            },
            r16.DE => blk: {
                break :blk ((@as(u16, self.d)) << 8) | @as(u16, self.e);
            },
            r16.HL => blk: {
                break :blk ((@as(u16, self.h)) << 8) | @as(u16, self.l);
            },
        };
    }

    pub fn write16(self: *RegisterPage, reg: r16, val: u16) void {
        switch (reg) {
            r16.SP => self.sp = val,
            r16.AF => {
                self.a = @as(u8, val >> 8);
                self.f = bitflags.get_flag_from(@truncate(val));
            },
            r16.BC => {
                self.b = @as(u8, val >> 8);
                self.c = @truncate(val);
            },
            r16.DE => {
                self.d = @as(u8, val >> 8);
                self.c = @truncate(val);
            },
            r16.HL => {
                self.h = @as(u8, val >> 8);
                self.l = @truncate(val);
            },
        }
    }

    pub fn read8(self: RegisterPage, reg: r8) u8 {
        return switch (reg) {
            r8.A => self.a,
            r8.B => self.b,
            r8.C => self.c,
            r8.D => self.d,
            r8.E => self.e,
            r8.F => self.f.bits(),
            r8.H => self.h,
            r8.L => self.l,
        };
    }

    pub fn write8(self: *RegisterPage, reg: r8, val: u8) void {
        switch (reg) {
            r8.A => self.a = val,
            r8.B => self.b = val,
            r8.C => self.c = val,
            r8.D => self.d = val,
            r8.E => self.e = val,
            r8.F => self.f.set_from_bits(val),
            r8.H => self.h = val,
            r8.L => self.l = val,
        }
    }

    pub fn check_z(self: RegisterPage) bool {
        return self.flags.bitflags.check(bitflags.flags.Z);
    }

    pub fn check_n(self: RegisterPage) bool {
        return self.flags.bitflags.check(bitflags.flags.N);
    }

    pub fn check_h(self: RegisterPage) bool {
        return self.flags.bitflags.check(bitflags.flags.H);
    }

    pub fn check_c(self: RegisterPage) bool {
        return self.flags.bitflags.check(bitflags.flags.C);
    }

    pub fn set_z(self: *RegisterPage, z: bool) void {
        self.flags.set(bitflags.flags.Z, z);
    }

    pub fn set_n(self: *RegisterPage, n: bool) void {
        self.flags.set(bitflags.flags.N, n);
    }

    pub fn set_h(self: *RegisterPage, h: bool) void {
        self.flags.set(bitflags.flags.H, h);
    }

    pub fn set_c(self: *RegisterPage, c: bool) void {
        self.flags.set(bitflags.flags.C, c);
    }
};
