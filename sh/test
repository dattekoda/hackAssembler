#!/bin/bash

cc -Wall -Wextra -Werror -o strchr ft_strchr.c
cc -Wall -Wextra -Werror -o strdup ft_memmove.c ft_strlen.c ft_strdup.c
cc -Wall -Wextra -Werror -o strjoin ft_memmove.c ft_strlen.c ft_strjoin.c
cc -Wall -Wextra -Werror -o strndup ft_memmove.c ft_strlen.c ft_strndup.c

runStrcharTest() {
	output=$(./strchr "$1" "$2")
	if [ "$output" != "$3" ]; then
		echo "$1, $2: $3 expected, but got $output"
		exit 1
	fi
	echo "$1, $2 => $output"
}

runStrdupTest() {
	output=$(./strdup "$1")
	if [ "$output" != "$2" ]; then
		echo "$1: $2 expected, but got $output"
		exit 1
	fi
	echo "$1 => $output"
}

runStrjoinTest() {
	output=$(./strjoin "$1" "$2")
	if [ "$output" != "$3" ]; then
		echo "$1, $2: $3 expected, but got $output"
		exit 1
	fi
	echo "$1, $2 => $output"
}

runStrndupTest() {
	output=$(./strndup "$1" "$2")
	if [ "$output" != "$3" ]; then
		echo "$1, $2: $3 expected, but got $output"
		exit 1
	fi
	echo "$1, $2 => $output"
}

echo '=== strchr test ==='
runStrcharTest a gohanoishi anoishi
runStrcharTest ' ' "ohayo gozaimasu" " gozaimasu"
runStrcharTest 1 hello "(null)"

echo '=== strdup test ==='
runStrdupTest "gohan" "gohan"

echo '=== strjoin test ==='
runStrjoinTest "gohan" "gohan" "gohangohan"
runStrjoinTest "abc" "123" "abc123"

echo '=== strdup test ==='
runStrndupTest "gohantabetai" "3" goh
runStrndupTest "gohantabetai" "12" gohantabetai
runStrndupTest "gohantabetai" "15" gohantabetai

echo OK
