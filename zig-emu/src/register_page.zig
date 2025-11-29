const std = @import("std");
const bitflags = @import("bitflags.zig");

const r8 = enum { A, B, C, D, E, F, H, L };

const r16 = enum { AF, BC, DE, HL, SP };

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
};
