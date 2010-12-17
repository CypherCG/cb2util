#!/bin/sh
# based on Git's test-lib.sh by Junio C Hamano
# see https://github.com/git/git/blob/master/t/test-lib.sh

if [ "$1" = "-v" -o "$1" = "--verbose" ]; then
    verbose=t
fi

exec 5>&1
if [ "$verbose" = "t" ]; then
    exec 4>&2 3>&1
else
    exec 4>/dev/null 3>/dev/null
fi

test_count=0

test_run() {
    test_cleanup=:
    eval >&3 2>&4 "$1"
    eval_ret=$?
    eval >&3 2>&4 "$test_cleanup"
    return $eval_ret
}

test_expect_success() {
    test_count=$((test_count + 1))

    echo >&3 "expecting success: $2"
    if test_run "$2"; then
        echo "ok $test_count - $1"
    else
        echo "not ok $test_count - $1"
    fi
    echo >&3 ""
}

test_cmp() {
    diff -u "$@"
}

test_done() {
    echo "1..$test_count"
}
