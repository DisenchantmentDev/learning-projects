const std = @import("std");

const registers = struct { a: u8, b: u8, c: u8, d: u8, e: u8, h: u8, l: u8, flags: u8, sp: u16, pc: u16 };
