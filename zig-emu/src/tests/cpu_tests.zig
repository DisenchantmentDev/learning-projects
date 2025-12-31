const std = @import("std");
const zig_emu = @import("zig_emu");
const cpu = zig_emu.cpu;
const rpage = zig_emu.rpage;

test "cpu nop" {
    const nop = cpu.instructions[0];
    nop.execute();
    try std.testing.expectEqual(rpage.RegisterPage.init(), cpu.registers);
}
