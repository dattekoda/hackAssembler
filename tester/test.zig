const   std = @import("std");

const testing = std.testing;
const c = @cImport({
	@cInclude("info.h");
});

test {
	try checkAInstruction(32, "0000000000100000");
	try checkAInstruction(0, "0000000000000000");
	try checkAInstruction(127, "0000000001111111");
}

test {
	try checkComp("0", "0101010");
	try checkComp("1", "0111111");
	try checkComp("-1", "0111010");
	try checkComp("D", "0001100");
	try checkComp("0", "0101010");
	try checkComp("1", "0111111");
	try checkComp("-1", "0111010");
	try checkComp("D", "0001100");
	try checkComp("A", "0110000");
	try checkComp("!D", "0001101");
	try checkComp("!A", "0110001");
	try checkComp("-D", "0001111");
	try checkComp("-A", "0110011");
	try checkComp("D+1", "0011111");
	try checkComp("A+1", "0110111");
	try checkComp("D-1", "0001110");
	try checkComp("A-1", "0110010");
	try checkComp("D+A", "0000010");
	try checkComp("D-A", "0010011");
	try checkComp("A-D", "0000111");
	try checkComp("D&A", "0000000");
	try checkComp("D|A", "0010101");
}

test {
	// --- C-Instruction: comp (a == 1) ---
	try checkComp("M", "1110000");
	try checkComp("!M", "1110001");
	try checkComp("-M", "1110011");
	try checkComp("M+1", "1110111");
	try checkComp("M-1", "1110010");
	try checkComp("D+M", "1000010");
	try checkComp("D-M", "1010011");
	try checkComp("M-D", "1000111");
	try checkComp("D&M", "1000000");
	try checkComp("D|M", "1010101");
}

fn checkAInstruction(label_symbol: u16, expected: []const u8) !void {
	var info = c.t_info{
		.i = c.A_INSTRUCTION,
		.labelSymbol = label_symbol,
		.comp = null,
		.dest = null,
		.jump = null,
	};

	var buf = [1]u8{'A'} ** 16;
	c.setHackLine(&buf, &info);
	try testing.expectEqualSlices(u8, expected, buf[0..16]);
}

fn checkCInstruction(line: [:0]const u8, expected: []const u8) !void {
	var info : c.t_info = undefined;

	var mutable_line: [256]u8 = undefined;
	if (line.len >= mutable_line.len)
		return error.StreamTooLong;
	@memcpy(mutable_line[0..line.len], line);
	mutable_line[line.len] = 0;

	try testing.expectEqual(@as(c_int, 0), c.setInfo(&info, @ptrCast(&mutable_line)));

	var buf: [16]u8 = @splat(0);
	c.setHackLine(&buf, &info);
	try testing.expectEqualSlices(u8, expected, buf[0..16]);
}

fn checkComp(line: [:0]const u8, comptime expected: []const u8) !void {
	const _expected = "111" ++ expected ++ "000000";
	try checkCInstruction(line, _expected);
}
