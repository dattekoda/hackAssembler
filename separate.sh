#!/bin/bash

for file in ft_*.c; do
	[ -e "$file" ] || continue

	test_file="${file%.c}Test.c"
	if grep -q '#include <stdio.h>' "$file"; then
		awk '
		BEGIN { found = 0 }
		/#include <stdio.h>/ { found = 1 }
		{
			if (found == 0) {
				print > "src.tmp"
			} else {
				print > "test.tmp"
			}
		}' "$file"

		mv src.tmp "$file"
		mv test.tmp "$test_file"

		echo "Separated: $file -> $file & $test_file"
	fi
done
