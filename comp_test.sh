#!/bin/bash
make
./HackAssembler testcase/comp.asm

for i in {1..18}; do
	./HackAssembler testcase/"$i".asm
	if ! diff -q testcase/"$i".hack testcase/"$i"_expect 2> /dev/null; then
		sed -n '1p' testcase/"$i".asm
	fi
done

